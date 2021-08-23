/*
 *          Copyright 2021 Vitali Baumtrok.
 * Distributed under the Boost Software License, Version 1.0.
 *      (See accompanying file LICENSE or copy at
 *        http://www.boost.org/LICENSE_1_0.txt)
 */

// Package displays provides a list of displays with their current resolution.
package displays

// #cgo CFLAGS: -D_DISPLAYS_WIN32
// #cgo LDFLAGS: -luser32
// #include "displays.h"
import "C"
import (
	"unsafe"
)

// All returns a list of displays in order from top left to bottom right.
// At failure, nil is returned.
func All() []*Display {
	var displaysC unsafe.Pointer
	var length C.int
	C.get_all(&displaysC, &length)
	if length > 0 {
		displays := make([]*Display, int(length))
		for i := range displays {
			var x, y, width, height, isDefault C.int
			var internalData unsafe.Pointer
			C.get_values(displaysC, C.int(i), &x, &y, &width, &height, &isDefault, &internalData)
			displays[i] = &Display{int(x), int(y), int(width), int(height), bool(isDefault != 0), internalData}
		}
		C.free_memory(displaysC)
		return displays
	}
	return nil
}

// Default returns the default display.
// At failure, nil is returned.
func Default() *Display {
	var x, y, width, height, isDefault C.int
	var internalData unsafe.Pointer
	C.get_default(&x, &y, &width, &height, &isDefault, &internalData)
	if internalData != nil {
		return &Display{int(x), int(y), int(width), int(height), bool(isDefault != 0), internalData}
	}
	return nil
}

// Index returns the index of display in list.
// If display is not in list, the index of default display is returned.
// At failure, -1 is returned.
func (display *Display) Index() int {
	var index C.int
	C.get_index(&index, display.InternalData)
	return int(index)
}
