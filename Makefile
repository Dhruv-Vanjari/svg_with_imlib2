build: main.c
	cc main.c -o exe `imlib2-config --cflags` `imlib2-config --libs` -lX11 -L/usr/X11/lib `pkg-config --cflags --libs cairo librsvg-2.0`

run: exe
	./exe

clean:
	rm -r exe
