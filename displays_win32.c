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
} display_t;

static display_t dspls[MAX_DISPLAYS];
static int dspls_len = 0;

static void display_proc(HMONITOR a, HDC b, LPRECT c, LPARAM d) {
	if (dspls_len < MAX_DISPLAYS) {
		HMONITOR const default_monitor = (HMONITOR)d;
		dspls[dspls_len].x = c->left;
		dspls[dspls_len].y = c->top;
		dspls[dspls_len].width = c->right - c->left;
		dspls[dspls_len].height = c->bottom - c->top;
		dspls[dspls_len].is_default = (a == default_monitor);
		dspls_len++;
	}
}

static void sort_displays() {
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
	HMONITOR const default_display = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
	EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)display_proc, (LPARAM)default_display);
	sort_displays();
	*length = dspls_len;
	*displays = (void*)dspls;
}

void get_values(void *const displays, int index, int *const x, int *const y, int *const width, int *const height, int *const is_default) {
	display_t *dspls = (display_t*)displays;
	*x = dspls[index].x;
	*y = dspls[index].y;
	*width = dspls[index].width;
	*height = dspls[index].height;
	*is_default = dspls[index].is_default;
}

void get_default(int *const x, int *const y, int *const width, int *const height, int *const index, int *const is_default) {
	if (dspls_len <= 0) {
		dspls_len = 0;
		HMONITOR const default_display = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
		EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)display_proc, (LPARAM)default_display);
		sort_displays();
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
		*index = i;
		*is_default = dspls[i].is_default;
	} else {
		*x = 0;
		*y = 0;
		*width = 0;
		*height = 0;
		*index = -1;
		*is_default = 0;
	}
}

#endif