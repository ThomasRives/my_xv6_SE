#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int
main(int argc, char *argv[]) {

	if(argc != 4)
	{
		printf(1, "usage: %s file_name arg2 arg3\n", argv[1]);
		exit();
	}
	int o = atoi(argv[2]);
	int n = atoi(argv[3]);

	int fd;
	char *buf;

	if((buf  = malloc(n)) == 0){
		printf(1, "malloc failed\n");
		exit();
	}

	if((fd = open(argv[1], O_RDONLY)) < 0){
		printf(1, "open failed\n");
		exit();
	}

	if(lseek(fd, o, SEEK_SET) < 0){
		printf(1, "lseek failed\n");
		exit();
	}

	if(read(fd, buf, n) < 0){
		printf(1, "read failed\n");
		exit();
	}

	if(write(1, buf, n) < 0){
		printf(1, "write failed\n");
		exit();
	}
	printf(1,"\n");

	if(close(fd) < 0){
		printf(1, "close failed\n");
		exit();
	}
	free(buf);
	exit();
}
