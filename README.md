# svg_with_imlib2
Just a demo of displaying a svg image

This is a simple program to display a svg image in X11
First the image is opened with `librsvg` then rendered on a `cairo surface` then cairo surface data is used to create an `Imlib_Image` which is rendered in the window

Building
--------
```
make
```
**Dependencies**
All these libraries must be installed to build the program successfully
* imlib2
* librsvg
* X11
* cairo


After building just type `make run` to run the program
