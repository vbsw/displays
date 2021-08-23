/*
 *          Copyright 2021 Vitali Baumtrok.
 * Distributed under the Boost Software License, Version 1.0.
 *      (See accompanying file LICENSE or copy at
 *        http://www.boost.org/LICENSE_1_0.txt)
 */

// Package displays provides functions to query displays and their current resolution.
package displays

import (
	"strconv"
	"unsafe"
)

// Display holds metrics of a display.
type Display struct {
	X, Y, Width, Height int
	Default             bool
	InternalData        unsafe.Pointer
}

// Equals returns true, if both displays reference the same display.
func (display *Display) Equals(dspl *Display) bool {
	if dspl != nil {
		return display.InternalData == dspl.InternalData
	}
	return false
}

// String returns properties of display as string.
func (display *Display) String() string {
	str := strconv.Itoa(display.X) + " " + strconv.Itoa(display.Y)
	str += " " + strconv.Itoa(display.Width) + " " + strconv.Itoa(display.Height)
	if display.Default {
		str += " (default)"
	}
	return str
}
