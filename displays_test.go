/*
 *          Copyright 2021 Vitali Baumtrok.
 * Distributed under the Boost Software License, Version 1.0.
 *      (See accompanying file LICENSE or copy at
 *        http://www.boost.org/LICENSE_1_0.txt)
 */

package displays

import (
	"strconv"
	"testing"
)

func TestAll(t *testing.T) {
	displays := All()
	if len(displays) == 0 {
		t.Error("length zero")
	} else if displays[0].Width-displays[0].X <= 0 {
		t.Error("x:" + strconv.Itoa(displays[0].X) + " width:" + strconv.Itoa(displays[0].Width))
	} else if displays[0].Height-displays[0].Y <= 0 {
		t.Error("y:" + strconv.Itoa(displays[0].Y) + " height:" + strconv.Itoa(displays[0].Height))
	}
}

func TestDefault(t *testing.T) {
	display := Default()
	if display == nil {
		t.Error("nil returned")
	}
}

func TestIndex(t *testing.T) {
	display := Default()
	if display == nil {
		t.Error("nil returned")
	} else if display.Index < 0 {
		t.Error("wrong index")
	}
}
