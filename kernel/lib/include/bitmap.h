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
void bitmap_set(bitmap_item *const items, usize index);
void bitmap_clear(bitmap_item *const items, usize index);
void bitmap_flip(bitmap_item *const items, usize index);
bool bitmap_get_value(const bitmap_item *const items, usize index);

/* Operations for multiple bits */
void bitmap_set_multiple(bitmap_item *const items, const usize begin, const usize cnt);
void bitmap_clear_multiple(bitmap_item *const items, const usize begin, const usize cnt);
void bitmap_flip_multiple(bitmap_item *const items, const usize begin, const usize cnt);

/* Operations for statistics */
usize bitmap_count(const bitmap_item *const items, const usize begin, const usize cnt, const bool value);
bool bitmap_is_empty(const bitmap_item *const items, usize begin, usize cnt);
bool bitmap_is_full(const bitmap_item *const items, usize begin, usize cnt);

#endif //_BITMAP_H