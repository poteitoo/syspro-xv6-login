#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// filenameで指定されたファイルを読み込む
// success: 0, failure: -1
int readFileAsText(void *text, const char *filename)
{
    struct stat st;
    int filed = open(filename, O_RDWR);
    if (filed < 0)
    {
        // printf("failed to open userinfo file\n");
        return -1;
    }
    fstat(filed, &st);

    if (read(filed, text, st.size) < 0)
    {
        // printf("failed to read userinfo file\n");
        return -1;
    }
    close(filed);
    return 0;
}

// filenameで指定されたファイルの内容をtextに更新する。ファイルがなければ新しく作る
// success: 0, failure: exit(-1)
int writeTextToFile(const char *text, const char *filename)
{
    int filed = open(filename, O_RDWR);
    if (filed < 0)
    {
        // ファイルが存在しなかったら新しく作る
        filed = open(filename, O_CREATE | O_RDWR);
        // printf("%s was newly created\n", filename);
    }
    if (write(filed, text, strlen(text)) < 0)
    {
        printf("write error\n");
        exit(-1);
    }
    close(filed);
    return 0;
}

// 2つのテキストを受け取り、片方がもう一方に含まれているか検証する
// true: 0, false: -1
int isTextIncluded(const char *text1, const char *text2)
{
    int cou = 0;
    for (int i = 0; i < strlen(text1); i++)
    {
        if (strcmp(&text1[i], &text2[cou]) == 0)
        {
            cou++;
        }
        else
        {
            cou = 0;
        }
    }
    if (cou == strlen(text2))
    {
        return 0;
    }
    return -1;
}

// usernameとpasswordを受け取り、有効か検証する
// true: 0, false: -1
int isValidUser(const char *username, const char *pass)
{
    // ファイルの読み込み
    char _username[1000] = {0}, _pass[1000] = {0};
    if (readFileAsText(_username, "userinfo") < 0)
    {
        // userinfoが存在しなかったら新しく作る
        writeTextToFile("hello world", "userinfo");
        if (readFileAsText(_username, "userinfo") < 0)
            exit(-1);
    }
    if (readFileAsText(_pass, "passinfo") < 0)
    {
        // passinfoが存在しなかったら新しく作る
        writeTextToFile("world", "passinfo");
        if (readFileAsText(_pass, "passinfo") < 0)
            exit(-1);
    }

    // 検証
    // usernameとpassの長さが等しくて、文字が同じ
    if (strlen(username) == strlen(_username) && strlen(pass) == strlen(_pass))
    {
        if (isTextIncluded(username, _username) == 0 && isTextIncluded(pass, _pass) == 0)
        {
            return 0;
        }
    }
    return -1;
}

int main(int argc, char *argv[])
{
    if (isValidUser("hello world", "world") == 0)
    {
        printf("valid user\n");
    }
    else
    {
        printf("invalid user\n");
    }
    exit(0);
}
