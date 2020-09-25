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
	(void)argv;

	int fd = open(argv[1], O_RDONLY);
	lseek(fd, atoi(argv[2]), atoi(argv[3]));
	close(fd);
	exit();
}
