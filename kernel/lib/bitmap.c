#include "bitmap.h"
#include "math.h"
#include "arch/x86/atomic.h"

#define BITS_PER_ITEM (sizeof(bitmap_item)*BITS_PER_BYTE)

/*
	Operations for a single bit
*/
void bitmap_set(bitmap_item *const items, usize index)
{
	KASSERT(items!=NULL);
	const usize offset = index/BITS_PER_ITEM;
	const bitmap_item mask = (bitmap_item)1u << (index - offset*BITS_PER_ITEM);

	// An atomic version of items[offset] |= mask
	atomic_or(&items[offset], mask);
}

void bitmap_clear(bitmap_item *const items, usize index)
{
	KASSERT(items!=NULL);
	const usize offset = index/BITS_PER_ITEM;
	const bitmap_item mask = (bitmap_item)1u << (index - offset*BITS_PER_ITEM);

	// An atomic version of items[offset] &= ~mask
	atomic_and(&items[offset], ~mask);
}

void bitmap_flip(bitmap_item *const items, usize index)
{
	KASSERT(items!=NULL);
	const usize offset = index/BITS_PER_ITEM;
	const bitmap_item mask = (bitmap_item)1u << (index - offset*BITS_PER_ITEM);

	// An atomic version of items[offset] &= ~mask
	atomic_xor(&items[offset], mask);
}

bool bitmap_get_value(const bitmap_item *const items, usize index)
{
	KASSERT(items!=NULL);
	const usize offset = index/BITS_PER_ITEM;
	const bitmap_item mask = (bitmap_item)1u << (index - offset*BITS_PER_ITEM);
	return !!(items[offset]&mask);
}

/*
	Operations for multiple bits
*/
// A template for the following implementations
#define bitmap_traverse(_var_item, _var_begin, _var_cnt, _op_mono, _op_other) do{ \
	KASSERT(_var_item!=NULL); \
	usize begin_aligned = div_ceil(_var_begin, BITS_PER_ITEM); \
	const usize end = _var_begin+_var_cnt; \
	usize end_aligned = end/BITS_PER_ITEM; \
	/* Apply operations on monolithic items */ \
	for(usize i=begin_aligned; i<end_aligned; ++i) \
		_op_mono(_var_item, i); \
	/* Apply operations on the rest of monolithic items */ \
	begin_aligned*=BITS_PER_ITEM, end_aligned*=BITS_PER_ITEM; \
	for(usize i=_var_begin; i<begin_aligned; ++i) \
		_op_other(_var_item, i); \
	for(usize i=end_aligned; i<end; ++i) \
		_op_other(_var_item, i); \
}while(0)

void bitmap_set_multiple(bitmap_item *const items, const usize begin, const usize cnt)
{
	#define op_mono(_var_item, i) _var_item[i] = ~(bitmap_item)0u
	#define op_other(_var_item, i) bitmap_set(_var_item, i)
	bitmap_traverse(items, begin, cnt, op_mono, op_other);
	#undef op_other
	#undef op_mono
}

void bitmap_clear_multiple(bitmap_item *const items, const usize begin, const usize cnt)
{
	#define op_mono(_var_item, i) _var_item[i] = (bitmap_item)0u
	#define op_other(_var_item, i) bitmap_clear(_var_item, i)
	bitmap_traverse(items, begin, cnt, op_mono, op_other);
	#undef op_other
	#undef op_mono
}

void bitmap_flip_multiple(bitmap_item *const items, const usize begin, const usize cnt)
{
	#define op_mono(_var_item, i) atomic_xor(&_var_item[i], ~(bitmap_item)0u);
	#define op_other(_var_item, i) bitmap_flip(_var_item, i)
	bitmap_traverse(items, begin, cnt, op_mono, op_other);
	#undef op_other
	#undef op_mono
}

/*
	Operations for statistics
*/
// Return the number of `value`s in range [begin, begin+cnt)
usize bitmap_count(const bitmap_item *const items, const usize begin, const usize cnt, const bool value)
{
	usize cnt_ones = 0u;

	#define op_mono(_var_item, i) cnt_ones += (usize)cnt_bit(_var_item[i]);
	#define op_other(_var_item, i) cnt_ones += (usize)bitmap_get_value(_var_item, i);
	bitmap_traverse(items, begin, cnt, op_mono, op_other);
	#undef op_other
	#undef op_mono

	return value?cnt_ones:(cnt-cnt_ones);
}

// Return whether range [begin, begin+cnt) is full of zeros
bool bitmap_is_empty(const bitmap_item *const items, usize begin, usize cnt)
{
	#define op_mono(_var_item, i) if(_var_item[i] != (bitmap_item)0u) return false
	#define op_other(_var_item, i) if(bitmap_get_value(_var_item, i)) return false
	bitmap_traverse(items, begin, cnt, op_mono, op_other);
	#undef op_other
	#undef op_mono

	return true;
}

// Return whether range [begin, begin+cnt) is full of ones
bool bitmap_is_full(const bitmap_item *const items, usize begin, usize cnt)
{
	#define op_mono(_var_item, i) if(_var_item[i] != ~(bitmap_item)0u) return false
	#define op_other(_var_item, i) if(!bitmap_get_value(_var_item, i)) return false
	bitmap_traverse(items, begin, cnt, op_mono, op_other);
	#undef op_other
	#undef op_mono

	return true;
}