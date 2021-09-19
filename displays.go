/*
 *          Copyright 2021 Vitali Baumtrok.
 * Distributed under the Boost Software License, Version 1.0.
 *      (See accompanying file LICENSE or copy at
 *        http://www.boost.org/LICENSE_1_0.txt)
 */

package displays

import (
	"strconv"
)

// Display holds metrics of a display.
type Display struct {
	X, Y, Width, Height, Index int
	Default                    bool
}

// Equals returns true, if both displays reference the same display.
func (display *Display) Equals(dspl *Display) bool {
	if dspl != nil {
		return display.Index == dspl.Index && display.X == dspl.X && display.Y == dspl.Y && display.Width == dspl.Width && display.Height == dspl.Height
	}
	return false
}

// Valid returns true, if display is available and active.
func (display *Display) Valid() bool {
	dspls := All()
	for _, dspl := range dspls {
		if display.Equals(dspl) {
			return true
		}
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
