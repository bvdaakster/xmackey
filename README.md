
# xmackey

This small application for X11 briefly swaps the mapping of the left mouse and right mouse button as to immitate a Mac's cmd key being held.
Bind this application to a key, `Super_L` for example, to use that key as the 'right click' key.

## Build

Requirements: make, X11 headers in path.

Run `make` to compile.

## Install

Run `make install` after building.

## Usage
```
Usage: ./xmackey --lockfile /tmp/xmackey.lock --device-id 11 --swap 0,2 --release-key 133 --map 1 2 3 4 5 6 7 8 9 10
	--lockfile	path to temporary lockfile
	--device-id	X11 mouse device id
	--swap		in form of a,b where a and b are the button indices to swap
	--release-key	xmackey exits when this key is released
	--map		the original key map
```

## Example

`xmackey --lockfile /tmp/xmackey.lock --device-id 11 --swap 0,2 --map "1 2 3 4 5 6 7" --sleep 2`

Will swap buttons 1 and 3 in the original mapping (1 2 3 4 5 6 7) of device 11 for 2 seconds.
