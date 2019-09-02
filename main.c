
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "device.h"

#define PRINT_ARGS 0
#define DEBUG 0

static char *lockfile;
static int id;
static int button_a = -1;
static int button_b = -1;
static unsigned char *map;
static int button_count;
static int sleep_sec;

void swap_buttons(Display *disp) {
	unsigned char tmp = map[button_a];
	map[button_a] = map[button_b];
	map[button_b] = tmp;
	int ret = set_button_map(disp, id, map, button_count);
	if(ret) {
		fprintf(stderr, "set_button_map failed: %d\n", ret);
	}
}

int parse_arg(int argc, char **argv) {
	int i;
	for(i = 1; i < argc; i++) {
		if(i + 1 < argc) {
			if(!strcmp("--lockfile", argv[i])) {
				lockfile = argv[++i];
			}
			else if(!strcmp("--device-id", argv[i])) {
				id = atoi(argv[++i]);
			}
			else if(!strcmp("--swap", argv[i])) {
				if(sscanf(argv[++i], "%d,%d", &button_a, &button_b) < 2) {
					button_a = -1;
				}
			}
			else if(!strcmp("--sleep", argv[i])) {
				sleep_sec = atoi(argv[++i]);
			}
			else if(!strcmp("--map", argv[i])) {
				char *tmp = malloc(sizeof(char) * strlen(argv[i + 1]));
				memcpy(tmp, argv[i + 1], strlen(argv[i + 1]));
				char *token = strtok(tmp, " ");
				while(token) {
					button_count++;
					token = strtok(0, " ");
				}
				memcpy(tmp, argv[i + 1], strlen(argv[i + 1]));
				map = (unsigned char*)malloc(sizeof(unsigned char*) * button_count);
				token = strtok(tmp, " ");
				int j = 0;
				while(token) {
					map[j++] = atoi(token);
					token = strtok(0, " ");
				}
				free(tmp);
			}
		}
	}

#if PRINT_ARGS
	printf("button_count:\t%d\n", button_count);
#endif

	if(lockfile && id && button_a != -1 && button_count > 0
			&& button_a < button_count && button_b < button_count) {
#if PRINT_ARGS
		printf("lockfile:\t%s\n", lockfile);
		printf("device-id:\t%d\n", id);
		printf("release-key:\t%d\n", release_key);		
		printf("swap:\t\t%d,%d\n", button_a, button_b);
		printf("map:\t\t");
		for(i = 0; i < button_count; i++) {
			printf("%d ", map[i]);
		}
		printf("\n");
#endif
		return 0;
	}

	printf("Usage: %s --lockfile /tmp/xmackey.lock --device-id 11 --swap 0,2 --map 1 2 3 4 5 6 7 8 9 10\n", argv[0]);
	printf("\t--lockfile\tpath to temporary lockfile\n");
	printf("\t--device-id\tX11 mouse device id\n");
	printf("\t--swap\t\tin form of a,b where a and b are the button indices to swap\n");
	printf("\t--map\t\tthe original key map\n");
	return -1;
}

int lock_or_die() {
	FILE *f = fopen(lockfile, "r");
	if(f) {
		fclose(f);
		return -1;
	}
	f = fopen(lockfile, "w");
	fclose(f);
	return 0;
}

void unlock() {
	remove(lockfile);
}

int main(int argc, char **argv) {
	if(parse_arg(argc, argv)) {
		return 1;
	}

	Display *disp = XOpenDisplay(0);
	if(disp == 0) {
		fprintf(stderr, "XOpenDisplay failed\n");
		return -1;
	}

	int real_button_count = get_button_count(disp, id);
	if(real_button_count != button_count) {
		fprintf(stderr, "size of mapping passed to --map does not match device's button count\n");
		return -2;
	}

	if(lock_or_die()) {
		return 2;
	}
	
	// Swap lmb and rmb
	swap_buttons(disp);

	sleep(sleep_sec);

	// Swap back to original mapping
	swap_buttons(disp);

	XUngrabKeyboard(disp, CurrentTime);
	XCloseDisplay(disp);

	free(map);

	unlock();

	return 0;
}
