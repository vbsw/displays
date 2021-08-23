/*
 *          Copyright 2021 Vitali Baumtrok.
 * Distributed under the Boost Software License, Version 1.0.
 *      (See accompanying file LICENSE or copy at
 *        http://www.boost.org/LICENSE_1_0.txt)
 */

// Package displays provides a list of displays with their current resolution.
package displays

import (
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
