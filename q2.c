#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "resource.h"

#define MILLION (1000 * 1000)

/*
 * Programme pour tester le fonctionnement de getrusage.
 *
 * Prend 3 arguments entiers :
 * - nombre d'iterations pour consommer du temps en mode utilisateur
 * - nombre d'iterations pour consommer du temps en mode systeme
 * - nombre de pages a allouer
 *
 * Exemple :
 *   $ q2 1 1 0                      # test cpu user + cpu sys
 *   user=65, sys=123, maxrss=5
 *   $ q2 0 0 3                      # test maxrss
 *   user=0, sys=0, maxrss=8
 *   $ q2 10 5 17                    # la totale
 *   user=752, sys=610, maxrss=22
 */

int cpuuser(void) // passe du temps en mode utilisateur
{
	int s = 0;
	for (int i = 0; i < 100 * MILLION; i++)
		s += i;
	return s;
}

void cpusys(void) // passe du temps en mode systeme
{
	int fd;
	char c;

	fd = open("/README", O_RDONLY);
	if (fd == -1)
	{
		printf(2, "cannot open README\n");
		exit(1);
	}
	while (read(fd, &c, 1) > 0)
		;
	close(fd);
}

void consomme(int npages, int nuser, int nsys)
{
	int i, s;

	if (sbrk(npages * 4096) == 0)
	{
		printf(2, "cannot sbrk %d pages\n", npages);
		return;
	}

	s = 0;
	for (i = 0; i < nuser; i++)
		s += cpuuser();

	for (i = 0; i < nsys; i++)
		cpusys();

	exit(s);
}

int main(int argc, char *argv[])
{
	int npages, nuser, nsys;
	int status;
	struct rusage u;

	if (argc != 4)
	{
		printf(2, "usage: %s nuser nsys npages\n", argv[0]);
		exit(0);
	}

	nuser = atoi(argv[1]);	// nb d'itérations (* 100 million) en mode U
	nsys = atoi(argv[2]);	// nb d'itérations (* 10) en mode S
	npages = atoi(argv[3]); // nb de pages a allouer

	switch (fork())
	{
	case -1:
		printf(2, "cannot fork\n");
		exit(1);
	case 0:
		consomme(npages, nuser, nsys);
		exit(0);
	default:
		if (wait(&status) == -1)
		{
			printf(2, "cannot wait\n");
			exit(1);
		}
		// printf(1, "exit(%d)\n", status);
		if (getrusage(&u) == -1)
			printf(2, "cannot rusage\n");
		printf(1, "user=%d, sys=%d, maxrss=%d\n",
			   u.ru_utime, u.ru_stime, u.ru_maxrss);
	}
	exit(0);
}
