package log

import (
	"io"
	"os"

	"github.com/tysontate/gommap"
)

var (
	offWidth uint64 = 4
	posWidth uint64 = 8
	endWidth = offWidth + posWidth
)

type index struct {
	file *os.File
	mmap gommap.MMap
	size uint64
}
