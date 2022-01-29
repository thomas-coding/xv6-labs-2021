#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define INPUT_SIZE 1024
char buf[INPUT_SIZE];
char* param[MAXARG];
int index = 0;

void xarg(char* cmd)
{
	int n;
	char *p = buf;

	memset(buf, 0, INPUT_SIZE);
	while ((n = read(0, p, INPUT_SIZE)) > 0) //Read all input to buf
		p += n;

	param[index] = buf; //First param point to buf start

	for (int i = 0; i < INPUT_SIZE; i++) {

		if (buf[i] == '\n') { //Read line, always exec a line
			buf[i] = 0; //Set 0 for end
			if (fork() == 0) { //Child
				index++;
				param[index] = 0;
				exec(cmd, param);
			} else {
				wait((int *)0);			
				p = buf + i + 1; //Point to next param
				param[index] = p;
			}
		}
	}

}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(2, "usage: xargs <cmd>\n");
		exit(1);
	}

	for(int i = 1; i < argc; i++) {
		param[index] = argv[i];
		index++;
	}

	xarg(argv[1]);
	exit(0);
}
