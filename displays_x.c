/*
 *          Copyright 2021 Vitali Baumtrok.
 * Distributed under the Boost Software License, Version 1.0.
 *      (See accompanying file LICENSE or copy at
 *        http://www.boost.org/LICENSE_1_0.txt)
 */

#include "displays.h"


#if defined(_DISPLAYS_X)

#include <X11/extensions/Xrender.h>
#include <stdlib.h>

typedef struct {
	int x, y, width, height, is_default;
} display_t;

typedef struct {
	display_t *ptr;
	int length;
} dspl_arr_t;

static void sort_displays(dspl_arr_t *const dspl_arr) {
	const int length = dspl_arr->length;
	if (length > 1) {
		int i, j;
		display_t *const displays = dspl_arr->ptr;
		for (i = 0; i < length - 1; i++) {
			int curr = i;
			for (j = i + 1; j < length; j++)
				if (displays[j].x < displays[curr].x || displays[j].x == displays[curr].x && displays[j].y < displays[curr].y)
					curr = j;
			if (curr != i) {
				const display_t display_tmp = displays[i];
				displays[i] = displays[curr];
				displays[curr] = display_tmp;
			}
		}
	}
}

static void mark_default(dspl_arr_t *const dspl_arr) {
/*
	const int length = dspl_arr->length;
	if (length) {
		if (length == 1) {
			dspl_arr->ptr[0].is_default = 1;
		} else {
			HMONITOR const default_display = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
			display_t *const displays = dspl_arr->ptr;
			int i;
			for (i = 0; i < length; i++) {
				if (displays[i].hndl == default_display) {
					displays[i].is_default = 1;
					break;
				}
			}
		}
	}
*/
}

void get_all(void **const displays, int *const length) {
	dspl_arr_t dspl_arr = { NULL, 0 };
	Display *const dspl = XOpenDisplay(NULL);
	if (dspl) {
		XCloseDisplay(dspl);
	}
/*
	EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)display_proc, (LPARAM)&dspl_arr);
	sort_displays(&dspl_arr);
	mark_default(&dspl_arr);
*/
	*length = dspl_arr.length;
	*displays = (void*)dspl_arr.ptr;
}

void get_values(void *const displays, int index, int *const x, int *const y, int *const width, int *const height, int *const is_default, void **const internal) {
	display_t *dspls = (display_t*)displays;
	*x = dspls[index].x;
	*y = dspls[index].y;
	*width = dspls[index].width;
	*height = dspls[index].height;
	*is_default = dspls[index].is_default;
/*
	*internal = dspls[index].hndl;
*/
}

void get_default(int *const x, int *const y, int *const width, int *const height, int *const is_default, void **const internal) {
/*
	HMONITOR const default_display = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi = { sizeof(mi) };
	if (GetMonitorInfo(default_display, &mi)) {
		*x = mi.rcMonitor.left;
		*y = mi.rcMonitor.top;
		*width = mi.rcMonitor.right - mi.rcMonitor.left;
		*height = mi.rcMonitor.bottom - mi.rcMonitor.top;
		*is_default = 1;
		*internal = default_display;
	} else {
		*x = 0;
		*y = 0;
		*width = 0;
		*height = 0;
		*is_default = 0;
		*internal = NULL;
	}
*/
}

void get_index(int *const index, void *const internal) {
/*
	int index_result = -1;
	display_t displays[MAX_DISPLAYS];
	dspl_arr_t dspl_arr = { displays, 0 };
	EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)display_proc, (LPARAM)&dspl_arr);
	sort_displays(&dspl_arr);
	const int length = dspl_arr.length;
	if (length) {
		HMONITOR const default_display = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
		display_t *const displays = dspl_arr.ptr;
		HMONITOR const display = (HMONITOR)internal;
		int i;
		for (i = 0; i < length; i++) {
			if (displays[i].hndl == default_display) {
				index_result = i;
				if (display == NULL)
					break;
			}
			if (display != NULL && displays[i].hndl == display) {
				index_result = i;
				break;
			}
		}
	}
	*index = index_result;
*/
}

void free_memory(void *const displays) {
	free(displays);
}

#endif
