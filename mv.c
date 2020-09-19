#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char const *argv[])
{
	if(argc != 3){
      printf(2, "Usage: ln old new\n");
      exit();
    }
    if(link(argv[1], argv[2]) < 0){
		printf(2, "mv %s %s: failed\n", argv[1], argv[2]);
		exit();
	}
	if(unlink(argv[1]) < 0)
		printf(2, "unlink %s: failed\n", argv[1]);
 	exit();
}
