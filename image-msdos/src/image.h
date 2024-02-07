/**
 * image - disk-send
 * 
 * Sergey Kiselev <skiselev@gmail.com>
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL Version 3.0
 */

#ifndef IMAGE_H
#define IMAGE_H

#define SECTOR_SIZE 512

/**
 * Copy disk to image file - main entrypoint.
 */
void disk_to_image(FILE *output);

#endif /* IMAGE_H */
