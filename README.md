disk-xfer and image
---------

## disk-xfer
A simple pair of programs to image copy an MS-DOS hard drive to an image file on a remote computer over serial port.

I wrote this because I needed a quick tool to back up a Conner CP-30104H disk from a GRiDCASE 1537, and nothing else was working.

If you find it useful, that's awesome.

to use:

Start tx on source machine:

```
C> tx
```

Start rx on destination machine:

```
$ rx diskimage.img
```

If everything is connected correctly, tx will send the data over the COM1: serial port to the destination machine in the requested image file.

## image

Image utility can be used to create an image of the HDD (the first HDD at this point). The image is written to the file specified by the command line argument.

Usage example:

```
C> image D:\hdd.img
```

This will create an image of the first HDD and write it to D:\hdd.img file. Note that D: needs to have enough space to write the image.
