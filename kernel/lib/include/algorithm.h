#ifndef _ALGORITHM_H
#define _ALGORITHM_H 1

#include "base.h"

bool cmp_default_char(void *x,void *y);
bool cmp_default_short(void *x,void *y);
bool cmp_default_long(void *x,void *y);
bool cmp_default_int(void *x,void *y);
bool cmp_default_unsigned(void *x,void *y);
bool cmp_default_pointer(void *x,void *y);

void kswap(void *x,void *y,u32 size);
void ksort(void *begin,void *end,u32 size_element,bool (*cmp)(void*,void*));

static inline u32 min_u32(const u32 x, const u32 y) { return x<y?x:y; }
static inline u32 max_u32(const u32 x, const u32 y) { return x>y?x:y; }
static inline i32 min_i32(const i32 x, const i32 y) { return x<y?x:y; }
static inline i32 max_i32(const i32 x, const i32 y) { return x>y?x:y; }

static inline u16 min_u16(const u16 x, const u16 y) { return x<y?x:y; }
static inline u16 max_u16(const u16 x, const u16 y) { return x>y?x:y; }
static inline i16 min_i16(const i16 x, const i16 y) { return x<y?x:y; }
static inline i16 max_i16(const i16 x, const i16 y) { return x>y?x:y; }

#endif // _ALGORITHM_H