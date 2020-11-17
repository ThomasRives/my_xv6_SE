# Shared memory

## Question 2

```c
struct zone *
zget (int id, int create, int wait, int size)
{
	struct proc* p;
	struct zone * z;
	if((id < 0 || id >= MAXSHM)
		|| (create != 0 && ztab[id].nref != 0) 
		|| (create != 0 && (wait != 0 || size <= 0)) 
		|| size < 0 || size > SHMPAGE
		)
		return 0;

	p = myproc();
	z = &ztable[id];
	acquire(&z->lock);
	if(create != 0)
	{
		z->size = size;
		for(uint i = 0; i < size; i++)
			if((z->pa[i] = kalloc()) == 0)
			{
				for(uint j = 0; j < i; j++)
					kfree(z->pa[j]);
				return 0;
			}

		p->zones[p->nbzones] = z;
		z->nref++;
		p->nbzones++;
		release(&z->lock);
		return z;
	}

	if(wait != 0 && z->nref != 0)
		sleep(z, &z->lock);
	
	z->nref++;
	p->nbzones++;
	release(z, &z->lock);
	return z;
}
```

## Question 3

```c
int
zmap(pde_t *pgdir, struct zone *z, char *va, int rdonly)
{
	int perm = PTE_U;
	if(rdonly == 0)
		perm |= PTE_W;

	for(uint i = 0; i < z->size; i++)
		if(mappages(pgdir, va + i * PGSIZE, PGSIZE, z->pa[i], perm) < 0)
			return -1;
}
```


## Question 4

```js
int
shmem(void)
{
	int id, flags, size;
	struct zone *z;
	struct proc* p;
	if(argint(0, &id) < 0 || argint(1, &flags) <0 || argint(2, &size) < 0)
		return -1;
	
	acquire(&ztab_lock);

	if((z = zget(id, flags & M_CREATE, flags & M_WAIT, size)) == -1)
		return -1

	char *va = 0;
	p = myproc();
	for(uint i = 0; i < p->nzones; i++)
		va += p->zones[i]->size * PGSIZE;

	if(zmap(myproc()->pgdir, z, va + SHMBASE, flags & M_RDONLY))
		return -1;

	release(&ztab_lock);

	return 0;
}
```

## Question 5

```js
void
zcopy(pde_t *npgdir, pde_t *pgdir)
{
	pte_t *pte;
	uint pa;
	uint nb_zones = myproc()->nzones;
	for(uint i = SHMBASE; i < (nb_zones * PGSIZE + SHMBASE); i+= PGSIZE)
	{
		if((pte = walpgdir(pgdir, (void *)i, 0)) == 0)
			panic("???????");

		if(!(*pte & PTE_P))
			panic("!!!!!!!");

		pa = PTE_ADDR(*pte);
		if(mappages(npgdir, SHMEMBASE + i, PGSIZE, pa, PTE_FLAGS(*pte)) < 0)
			panic(".......");
	}
}
```

## Question 6

```js
np->nzones = curproc->nzones;
memmove(np->zones, curproc->zones, np->nzones);

zcopy(np->pgdir, curproc->pgdir);
```

## Question 7

```js
int
deallocuvm(pde_t *pgdir, uint oldsz, uint newsz)
{
	pte_t *pte;
	uint a, pa;
	uint size = 0;
	p = myproc();
	for(uint i = 0; i < p->nzones; i++)
		size += p->zones[i]->size * PGSIZE;

	if(newsz >= oldsz)
		return oldsz;

	a = PGROUNDUP(newsz);
	for(; a  < oldsz; a += PGSIZE){
		pte = walkpgdir(pgdir, (char*)a, 0);
		if(!pte)
			a = PGADDR(PDX(a) + 1, 0, 0) - PGSIZE;
		else if((*pte & PTE_P) != 0){
			if(pte >= SHMBASE && pte < SHMBASE + size)
				*pte = 0;
			else{
				pa = PTE_ADDR(*pte);
				if(pa == 0)
					panic("kfree");
				char *v = P2V(pa);
				kfree(v);
				*pte = 0;
			}
		}
	}
	return newsz;
}
```

## Question 8

```c
void
zfree(struct zone *z)
{
	uint i;
	acquire(&z->lk);
	z->nref--;
	release(&z->lk);
	if(z->nref == 0)
		for(i = 0; i < z->size; i++)
			kfree(z->pa[i]);
}
```

## Question 9

Dans wait:
```c
freevm(p->pgdir);
for(i = 0; i < p->nzones; i++)
	zfree(p->zones[i]);
```
Dans exec, à la fin du cas normal:
```c
freevm(oldpgdir);
for(i = 0; i < p->nzones; i++)
    zfree(p->zones[i]);
return 0;
```