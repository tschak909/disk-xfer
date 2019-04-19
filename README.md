disk-xfer
---------

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

