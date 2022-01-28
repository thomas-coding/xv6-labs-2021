#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int drop[35] = {0};
int index = 2;

int main(int argc, char *argv[])
{
	int ret;

	int r;
	int p[2];

	while (index < 36) {

		if(drop[index] == 1) {//aleady drop, not prime
			index++;
			continue;
		}

		pipe(p);
		if (fork() == 0) { //child
			close(p[1]);
			for (int i = 0; i < 36; i++) {
				ret = read(p[0], &r, 4); //read number
				if (i == 0 && ret != 0) //print first received
					printf("prime %d\n", r);
				if (ret == 0) /* Read all */
				{
					break;
				}
			}
			close(p[0]);
			exit(0);
		} else {
			close(p[0]);
			if ( index == 2)
				printf("prime %d\n", index);
			for (int i = 2; i <36; i++) { //send number to child
				if (i % index == 0) {//not send
					//printf("drop: %d\n", i);
					drop[i] = 1; //drop
					continue;
				}

				if (drop[i] == 0) //not drop yet
					write(p[1], &i, 4);
			}
			close(p[1]);
			wait((int *)0);
		}
		index++;
	}

	exit(0);
}
