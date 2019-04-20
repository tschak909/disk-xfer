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
#include "int13.h"
#include "int14.h"
#include "xmodem-send.h"

int main(int argc, char* argv[])
{
  DiskGeometry geometry;
  if (int13_disk_geometry(&geometry)==0)
    {
      printf("c: %04d - h: %02d - s: %02d\s",geometry.c,geometry.h,geometry.s);
    }
  
  return 0;
}
