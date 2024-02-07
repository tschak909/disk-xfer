/**
 * image - disk-send
 * 
 * Sergey Kiselev <skiselev@gmail.com>
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL Version 3.0
 */

#include <i86.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "int13.h"

short cylinder=0;
unsigned char head=0;
char* buf;
DiskGeometry geometry;

/**
 * Copy disk to image file - main entrypoint.
 */
void disk_to_image(FILE *output)
{
	if (int13_disk_geometry(&geometry) == 1) {
		fprintf(stderr, "Could not retrieve disk geometry for device 0x80. Aborting.\n");
		return;
	}

	/* Note: cylinders and heads values are 0 based, while sectors value is 1 based */
	printf("Disk geometry: C: %4d, H: %2d, S: %2d\n", geometry.c, geometry.h, geometry.s);
  
	/* Read one cylinder/head at the time */
	buf = malloc(geometry.s * SECTOR_SIZE);

	/* Disable buffering on stdout, so that the progress is shown right away */
	setbuf(stdout, NULL);

	while (1) {
		printf("C: %4d, H: %2d\b\b\b\b\b\b\b\b\b\b\b\b\b\b", cylinder, head);
		if (int13_read_sectors(cylinder, head, 1, geometry.s, buf) == 0) {
			if (fwrite(buf, SECTOR_SIZE, geometry.s, output) != geometry.s) {
	      			fprintf(stderr, "\nError: Short write. Destination drive full?\n");
				break;
			}
		} else {
	      		fprintf(stderr, "\nError: Could not read the source drive.\n");
			break;
		}

		/* increment head and cyclinder as needed */
		head++;
		if (head > geometry.h) {
			head=0;
			cylinder++;
			if (cylinder > geometry.c) {
				printf("\nDisk image written successfully\n");
				break;	/* end of the disk */
			}
		}
	}
	free(buf);
}
