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
  // uint target;
  // int c;
  //
  // iunlock(ip);
  // target = n;
  // while(n > 0){
  //   while(input.r == input.w){
  //     if(myproc()->killed){
  //       ilock(ip);
  //       return -1;
  //     }
  //     sleep(&input.r, &dmem.lock);
  //   }
  //   c = input.buf[input.r++ % INPUT_BUF];
  //   if(c == C('D')){  // EOF
  //     if(n < target){
  //       // Save ^D for next time, to make sure
  //       // caller gets a 0-byte result.
  //       input.r--;
  //     }
  //     break;
  //   }
  //   *dst++ = c;
  //   --n;
  //   if(c == '\n')
  //     break;
  // }
  // ilock(ip);
  //
  // return target - n;
  return 0;
}

int
drvmemwrite(struct inode *ip, char *buf, int n, uint off)
{
  // int i;
  //
  // iunlock(ip);
  // for(i = 0; i < n; i++)
  //   dmemputc(buf[i] & 0xff);
  // ilock(ip);
  //
  return n;
}

void
drvmeminit(void)
{
  devsw[DEVMEM].write = drvmemwrite;
  devsw[DEVMEM].read = drvmemread;
}
