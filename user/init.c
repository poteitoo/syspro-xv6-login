// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char *argv[] = {"sh", 0};

int main(void)
{
  int pid, wpid, loggedIn;

  if (open("console", O_RDWR) < 0)
  {
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  }
  dup(0); // stdout
  dup(0); // stderr

  for (;;)
  {
    write(1, "init: starting sh\n", 18);
    pid = fork();
    if (pid < 0)
    {
      write(1, "init: fork failed\n", 18);
      exit(1);
    }
    if (pid == 0)
    {
			int count = 0;

			while (count < 5) {
				write(1, "Username: ", 10);
				char *user = (char *)malloc(32);
				read(1, user, 30);
				switch(user[strlen(user) - 1]) {
					case '\n': case '\r':
						user[strlen(user) - 1] = 0;
				}

				write(1, "Password: ", 11);
				char *pass = (char *)malloc(32);
				read(1, pass, 30);
				switch(pass[strlen(pass) - 1]) {
					case '\n': case '\r':
						pass[strlen(pass) - 1] = 0;
				}

				loggedIn = isValidUser(user, pass);
				if(loggedIn == 0) {
					//opens shell for the user
					write(1, "Welcome back ", 13);
					write(1, user, strlen(user));
					write(1, "!\n", 2);
					char *tmp;
					exec("sh", &tmp);
					write(1, "init: exec login failed\n", 24);
					exit(0);
				} else {
					write(1, "User and password do not match, or user does not exist! Try again!\n", 67);
					count++;
				}
			}
			write(1, "Login Failed\n", 1);
			while(1);
      exit(1);
    }

    for (;;)
    {
      // this call to wait() returns if the shell exits,
      // or if a parentless process exits.
      wpid = wait((int *)0);
      if (wpid == pid)
      {
        // the shell exited; restart it.
        break;
      }
      else if (wpid < 0)
      {
        printf("init: wait returned an error\n");
        exit(1);
      }
      else
      {
        // it was a parentless process; do nothing.
      }
    }
  }
}
