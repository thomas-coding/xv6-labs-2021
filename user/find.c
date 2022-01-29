#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* str)
{
	int fd;
	struct dirent de;
	char buf[512], *p;
	struct stat st;

	if ((fd = open(path, 0)) < 0) { //open file
		fprintf(2, "find can't open %s\n", path);
		return;
	}

	if(fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}

	if(st.type != T_DIR) { //not dir, no need to find
		close(fd);
		return;
	}

	strcpy(buf, path); //copy path to buf, like .
	p = buf+strlen(buf);
	*p++ = '/';

	while (read(fd, &de, sizeof(de)) == sizeof(de)) {
		if (de.inum == 0)
			continue;
		if ((strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0)) //drop . and ..
			continue;
		
		memmove(p, de.name, DIRSIZ); //cope file to buf, like sleep
		p[DIRSIZ] = 0;
		
		if ((strcmp(de.name, str) == 0)) //compare file name and string which we should find
			printf("%s\n", buf);
		
		find(buf, str); //recursion, sub dir
	}
	close(fd);
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(2, "usage: find <path> <name>\n");
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(0);
}
