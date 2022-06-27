#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	int count, loggedIn;
	count = 0;

	while (count < 5) {
		write(1, "Current Username: ", 18);
		char *user = (char *)malloc(32);
		read(1, user, 30);
		switch(user[strlen(user) - 1]) {
			case '\n': case '\r':
				user[strlen(user) - 1] = 0;
		}

		write(1, "Current Password: ", 18);
		char *pass = (char *)malloc(32);
		read(1, pass, 30);
		switch(pass[strlen(pass) - 1]) {
			case '\n': case '\r':
				pass[strlen(pass) - 1] = 0;
		}

		loggedIn = isValidUser(user, pass);
		if(loggedIn == 0) {
			char *newpass = (char *)malloc(32);
			write(1, "New Password: ", 14);
			read(1, newpass, 30);
			switch (newpass[strlen(newpass) - 1]) {
				case '\n': case '\r':
					newpass[strlen(newpass) - 1] = 0;
			}

			// clean file and write text
			unlink("passinfo");
			writeTextToFile(newpass, "passinfo");
			exit(0);
		} else {
			write(1, "User and password do not match, or user does not exist! Try again!\n", 67);
			count++;
		}
	}
	exit(0);
}
