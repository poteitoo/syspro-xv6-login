#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

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
