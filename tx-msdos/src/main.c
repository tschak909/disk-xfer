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
#include "int14.h"
#include "xmodem-send.h"

#define BUFFER_SIZE 512

int main(int argc, char* argv[])
{
  int14_init();
  printf("serial port initialized.\n");
  
  xmodem_send();
    
  return 0;
}
