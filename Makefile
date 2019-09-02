
all: main.c device.c
	gcc -Wall -lXi -lX11 -o xmackey main.c device.c

clean: 
	rm xmackey

install:
	cp xmackey /usr/bin/xmackey

uninstall:
	rm /usr/bin/xmackey

