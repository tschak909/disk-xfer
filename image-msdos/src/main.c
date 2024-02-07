/**
 * image - disk-send
 * 
 * main routines
 *
 * Sergey Kiselev <skiselev@gmail.com>
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL Version 3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main(int argc, char* argv[])
{
	FILE *output;

	if (argc != 2) {
		printf("Usage: %s <output_image_file_name>\n\n", argv[0]);
		exit(1);
	}
	
	if ((output = fopen(argv[1], "wb")) == NULL) {
		printf("%s: Failed to open file '%s' for writing\n\n", argv[0], argv[1]);
		exit(1);
	}

      	disk_to_image(output);

	fclose(output);
    
	return 0;
}
