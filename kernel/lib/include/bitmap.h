#ifndef _BITMAP_H
#define _BITMAP_H

#include "base.h"

typedef usize bitmap_item;
// We provide a common packaged structure here
struct bitmap
{
	usize cnt; // # of used bits
	bitmap_item *data; // data field
};

// typedef void* (*bitmap_type_func_alloc)(usize cnt_byte);
// typedef void (*bitmap_type_func_free)(void *addr);

/* Operations for a single bit */


/* Operations for multiple bits */

/* Finding set or unset bits. */

#endif //_BITMAP_H