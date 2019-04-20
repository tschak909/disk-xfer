/**
 * rx - Receive image from tx
 *
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Released under GPL version 3.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h> // needed for memset

int serial_fd;

/**
 * Main protocol entry point
 */
int xmodem_receive(void)
{
  return 0;
}

/**
 * Initialize serial port
 */
int termio_init(char* serial_filename)
{
  struct termios tio;

  memset(&tio,0,sizeof(tio));
  tio.c_iflag=0;
  tio.c_oflag=0;
  tio.c_cflag=CS8|CREAD|CLOCAL;
  tio.c_lflag=0;
  tio.c_cc[VMIN]=1;
  tio.c_cc[VTIME]=5;
  
  serial_fd=open(serial_filename, O_RDWR|O_NONBLOCK);
  cfsetospeed(&tio,B19200);
  cfsetispeed(&tio,B19200);
  
  tcsetattr(serial_fd,TCSANOW,&tio);
  printf("Serial port initialized.");
}

/**
 * print args if less than two args on cmd line.
 */
void print_args(void)
{
  printf("rx /dev/ttySx destination_image_name.img\n");
}

int main(int argc, char* argv[])
{
  // Display args if not provided.
  if (argc!=3)
    {
      print_args();
      exit(1);
    }
  else
    {
      termio_init(argv[1]);
      return xmodem_receive();
    }
}
