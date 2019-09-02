
#ifndef DEVICE_H
#define DEVICE_H

#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>

int get_button_count(Display *disp, int id);

int get_button_map(Display *disp, int id, unsigned char *map, int count);

int set_button_map(Display *disp, int id, unsigned char *map, int count);

#endif
