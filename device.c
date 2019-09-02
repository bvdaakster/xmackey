
#include "device.h"

int get_button_count(Display *disp, int id) {
	int buttonCount = 0;
	int deviceCount;
	XDeviceInfo *devices = XListInputDevices(disp, &deviceCount);
	int i;
	for(i = 0; i < deviceCount; i++) {
		if(devices[i].id == id) {
			XDeviceInfo *device = devices + i;
			XAnyClassPtr classPtr = (XAnyClassPtr)device->inputclassinfo;
			int j;
			for(j = 0; j < device->num_classes; j++) {
				if(classPtr->class == ButtonClass) {
					buttonCount = ((XButtonInfoPtr)classPtr)->num_buttons;
					break;
				}
			}
			break;
		}
	}
	XFreeDeviceList(devices);
	return buttonCount;
}

int get_button_map(Display *disp, int id, unsigned char *map, int count) {
	XDevice *device = XOpenDevice(disp, id);
	if(!device) {
		return -1;		
	}

	int ret = XGetDeviceButtonMapping(disp, device, map, count);
	XCloseDevice(disp, device);
	return ret;
}

int set_button_map(Display *disp, int id, unsigned char *map, int count) {
	XDevice *device = XOpenDevice(disp, id);
	if(!device) {
		return -1;
	}

	int ret = XSetDeviceButtonMapping(disp, device, map, count);
	XCloseDevice(disp, device);
	return ret;
}
