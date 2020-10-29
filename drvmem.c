// dmemole input and output.
// Input is from the keyboard or serial port.
// Output is written to the screen and serial port.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

int
drvmemread(struct inode *ip, char *dst, int n, uint off)
{
  if(n < 0)
    panic("drvmemread n < 0");

  if(ip->minor == 0)
    return 0;
  
  if(ip->minor == 2)
  {
    if ((off < EXTMEM) || ((off + n) > PHYSTOP))
      panic("drvmemread out of mem");

    memmove(dst, (void *)P2V(off), n);
    return n;
  }

  memset(dst, 0, n);
  return n;
}

int
drvmemwrite(struct inode *ip, char *buf, int n, uint off)
{
  return n;
}

void
drvmeminit(void)
{
  devsw[DEVMEM].write = drvmemwrite;
  devsw[DEVMEM].read = drvmemread;
}
