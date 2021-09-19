/*
 *          Copyright 2021 Vitali Baumtrok.
 * Distributed under the Boost Software License, Version 1.0.
 *      (See accompanying file LICENSE or copy at
 *        http://www.boost.org/LICENSE_1_0.txt)
 */

#include "displays.h"


#if defined(_DISPLAYS_X)

#include <X11/extensions/Xrandr.h>
#include <stdlib.h>

typedef struct {
	int x, y, width, height, is_default, index;
} display_t;

static display_t dspls[MAX_DISPLAYS];
static int dspls_len = 0;

static void set_dspls(XRRMonitorInfo *const monitors, const int length) {
	int i;
	for (i = 0; i < length; i++) {
		dspls[i].x = monitors[i].x;
		dspls[i].y = monitors[i].y;
		dspls[i].width = monitors[i].width;
		dspls[i].height = monitors[i].height;
		dspls[i].is_default = monitors[i].primary;
		dspls[i].index = i;
	}
}

static void sort_dspls() {
	int i;
	for (i = 0; i < dspls_len - 1; i++) {
		int j, curr = i;
		for (j = i + 1; j < dspls_len; j++)
			if (dspls[j].x < dspls[curr].x || dspls[j].x == dspls[curr].x && dspls[j].y < dspls[curr].y)
				curr = j;
		if (curr != i) {
			const display_t display_tmp = dspls[i];
			dspls[i] = dspls[curr];
			dspls[curr] = display_tmp;
		}
	}
}

void get_all(void **const displays, int *const length) {
	dspls_len = 0;
	Display *const dspl = XOpenDisplay(NULL);
	if (dspl) {
		const int default_screen_id = DefaultScreen(dspl);
		const Window root = RootWindow(dspl, default_screen_id);
		XRRMonitorInfo *const monitors = XRRGetMonitors(dspl, root, True, &dspls_len);
		if (monitors) {
			set_dspls(monitors, dspls_len);
			sort_dspls();
			free((void*)monitors);
		}
		XCloseDisplay(dspl);
	}
	*length = dspls_len;
	*displays = (void*)dspls;
}

void get_values(void *const displays, int index, int *const x, int *const y, int *const width, int *const height, int *const is_default, void **const internal) {
	display_t *dspls = (display_t*)displays;
	*x = dspls[index].x;
	*y = dspls[index].y;
	*width = dspls[index].width;
	*height = dspls[index].height;
	*is_default = dspls[index].is_default;
	*internal = &dspls[index];
}

void get_default(int *const x, int *const y, int *const width, int *const height, int *const is_default, void **const internal) {
	if (dspls_len <= 0) {
		dspls_len = 0;
		Display *const dspl = XOpenDisplay(NULL);
		if (dspl) {
			const int default_screen_id = DefaultScreen(dspl);
			const Window root = RootWindow(dspl, default_screen_id);
			XRRMonitorInfo *const monitors = XRRGetMonitors(dspl, root, True, &dspls_len);
			if (monitors) {
				set_dspls(monitors, dspls_len);
				sort_dspls();
				free((void*)monitors);
			}
			XCloseDisplay(dspl);
		}
	}
	if (dspls_len) {
		int i;
		for (i = 0; i < dspls_len; i++)
			if (dspls[i].is_default)
				break;
		if (i >= dspls_len)
			i = 0;
		*x = dspls[i].x;
		*y = dspls[i].y;
		*width = dspls[i].width;
		*height = dspls[i].height;
		*is_default = dspls[i].is_default;
		*internal = &dspls[i];
	} else {
		*x = 0;
		*y = 0;
		*width = 0;
		*height = 0;
		*is_default = 0;
		*internal = NULL;
	}
}

void get_index(int *const index, void *const internal) {
	if (dspls_len > 0) {
		display_t *dspl = NULL;
		if (internal)
			dspl = (display_t*)internal;
		if (dspl && dspl->index >= 0 && dspl->index <= dspls_len)
			*index = dspl->index;
		else {
			int i;
			for (i = 0; i < dspls_len; i++) {
				if (dspls[i].is_default) {
					*index = i;
					break;
				}
			}
			if (i >= dspls_len)
				*index = 0;
		}
	} else {
		*index = -1;
	}
}

#endif
