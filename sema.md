# Annale Semaphore

## Question 1
```c
struct sem{
	int cpt;
	int v;
	enum {SEM_UNUSED, SEM_USED, SEM_INTERB} state;
	struct spinlock lock;
};

struct {
	struct sem sema[NSEM];
	struct lock lock;
} stable;

```

## Question 2

```c
void 
seminit(void)
{
	initlock(&sem_table, "sem table");
	for(uint i = 0; i < NSEM; i ++)
		initlock(&stable[i].lock, "sema")
}
```

## Question 3

```c
struct sem *
sem_alloc(int cpt)
{
	if(cpt < 0)
		return 0;

	struct sem *s;
	acquire(&stable.lock);

	for(s = sema; i < &stable.sema[N_SEM]; s++, i++)
		if(s->state == SEM_UNUSED)
		{
			s->cpt = test;
			s->state = SEM_USED;
			release(&stable.lock);
			return s;
		}

	release(&stable.lock);
	return 0;
}
```

## Question 4

Ajouter `FD_SEM` dans l'enum et un nouveau champ: `struct sem* semaphore`.

```c
int
sys_sem_create(void)
{
	int cpt, fd;
	struct file* f;
	struct sem *s;
	if(argint(0,&cpt) < 0)
		return -1;

	if((f = filealloc()) == 0 || (fd = fdalloc(f)) < 0)
		return -1;

	f->type = FD_SEM;
	s = sem_alloc(cpt);
	f->semaphore = s;

	return 0;
}
```

## Question 5

```c
int
sys_sem_p (void)
{
	struct file *f;

	if(argfd(0,0,&f) < 0 || f->type != FD_SEM)
		return -1;

	struct sem *s = f->semaphore;
	acquire(&s->lock);
	s->cpt--;
	while(s->cpt < 0)
		sleep(s, &s->lock);

	release(&s->lock);
	return 0;
}

int
sem_v (int fd)
{
	struct file *f;

	if(argfd(0,0,&f) < 0 || f->type != FD_SEM)
		return -1;

	struct sem *s = f->semaphore;
	acquire(&s->lock);
	s->cpt++;

	if(s->cpt <= 0)
		wakeup(s);
	release(&s->lock);
	return 0;
}
```

## Question 6

Dans `fileclose()`:
```c
else if(ff.type == FD_SEM)
{
	ff.semaphore->state = SEM_UNUSED;
}
```

## Question 7

On se sert du nombre de références au fichier contenant la sémaphore (ref dans la struct file), si `-f.ref = f.semaphore->cpt` alors on débloque.

```c
int
sys_sem_p (void)
{
	struct file *f;

	if(argfd(0,0,&f) < 0 || f->type != FD_SEM)
		return -1;

	struct sem *s = f->semaphore;
	acquire(&s->lock);
	s->cpt--;
	
	if(-f->ref == s->cpt)
	{
		s->state = SEM_INTERB;
		wakeup(s);
	}
	else
		while(s->cpt < 0 && s->state != SEM_INTERB)
			sleep(s, &s->lock);


	if(s->state == SEM_INTERB)
	{
		release(&s->lock);
		return -1;
	}
	else
	{
		release(&s->lock);
		return 0;
	}
}
```