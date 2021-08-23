/*
 *          Copyright 2021 Vitali Baumtrok.
 * Distributed under the Boost Software License, Version 1.0.
 *      (See accompanying file LICENSE or copy at
 *        http://www.boost.org/LICENSE_1_0.txt)
 */

#include "displays.h"


#if defined(_DISPLAYS_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct {
	int x, y, width, height, is_default;
	HMONITOR hndl;
} display_t;

typedef struct {
	display_t *displays;
	int length;
} data_t;

static void display_proc(HMONITOR a, HDC b, LPRECT c, LPARAM d) {
	data_t *data = (data_t*)d;
	if (data->displays == NULL) {
		data->displays = (display_t*)malloc(sizeof(display_t) * MAX_DISPLAYS);
	}
	if (data->displays && data->length < MAX_DISPLAYS) {
		data->displays[data->length].x = c->left;
		data->displays[data->length].y = c->top;
		data->displays[data->length].width = c->right - c->left;
		data->displays[data->length].height = c->bottom - c->top;
		data->displays[data->length].is_default = 0;
		data->displays[data->length].hndl = a;
		data->length++;
	}
}

static void sort_displays(data_t *const data) {
	const int length = data->length;
	if (length > 1) {
		int i, j;
		display_t *const displays = data->displays;
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

static void mark_default(data_t *const data) {
	const int length = data->length;
	if (length) {
		if (length == 1) {
			data->displays[0].is_default = 1;
		} else {
			HMONITOR const default_hndl = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
			display_t *const displays = data->displays;
			int i;
			for (i = 0; i < length; i++) {
				if (displays[i].hndl == default_hndl) {
					displays[i].is_default = 1;
					break;
				}
			}
		}
	}
}

void get_all(void **const displays, int *const length) {
	data_t data = { NULL, 0 };
	EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)display_proc, (LPARAM)&data);
	sort_displays(&data);
	mark_default(&data);
	*length = data.length;
	*displays = (void*)data.displays;
}

void get_values(void *const displays, int index, int *const x, int *const y, int *const width, int *const height, int *const is_default, void **const internal) {
	display_t *displays_d = (display_t*)displays;
	*x = displays_d[index].x;
	*y = displays_d[index].y;
	*width = displays_d[index].width;
	*height = displays_d[index].height;
	*is_default = displays_d[index].is_default;
	*internal = displays_d[index].hndl;
}

void get_default(int *const x, int *const y, int *const width, int *const height, int *const is_default, void **const internal) {
	HMONITOR const default_hndl = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi = { sizeof(mi) };
	if (GetMonitorInfo(default_hndl, &mi)) {
		*x = mi.rcMonitor.left;
		*y = mi.rcMonitor.top;
		*width = mi.rcMonitor.right - mi.rcMonitor.left;
		*height = mi.rcMonitor.bottom - mi.rcMonitor.top;
		*is_default = 1;
		*internal = default_hndl;
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
	int index_result = -1;
	display_t displays[MAX_DISPLAYS];
	data_t data = { displays, 0 };
	EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)display_proc, (LPARAM)&data);
	sort_displays(&data);
	const int length = data.length;
	if (length) {
		HMONITOR const default_hndl = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
		display_t *const displays = data.displays;
		HMONITOR const display = (HMONITOR)internal;
		int i;
		for (i = 0; i < length; i++) {
			if (displays[i].hndl == default_hndl) {
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
}

void free_memory(void *const displays) {
	free(displays);
}

#endif