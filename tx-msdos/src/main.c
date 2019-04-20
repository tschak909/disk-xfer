/**
 * tx - disk-send
 * 
 * main routines
 *
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL Version 3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "int13.h"
#include "int14.h"
#include "xmodem-send.h"

#define BUFFER_SIZE 512

int main(int argc, char* argv[])
{
  DiskGeometry geometry;
  char* buf;
  int i=0;

  buf=malloc(BUFFER_SIZE);
  
  if (int13_disk_geometry(&geometry)==0)
    {
      printf("c: %04d - h: %02d - s: %02d\s",geometry.c,geometry.h,geometry.s);
    }
  else
    return 1;

  if (int13_read_sector(0,0,1,buf)==0)
    {
      for (i=0;i<BUFFER_SIZE;i++)
	{
	  printf("%02x  ",buf[i]);
	}
      printf("\n");
    }

  free(buf);
  
  return 0;
}
