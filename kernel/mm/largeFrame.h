#ifndef _LARGEFRAME_H
#define _LARGEFRAME_H

#include "base.h"

typedef i16 LF_idx_t;
typedef u16 LF_len_t;

struct segtree_LF
{
	LF_len_t left, mid, right, overwrite;
};
struct LF_manager_t
{
	struct segtree_LF *segtree;
	LF_idx_t n_aligned;
};

// Initialize `manager` and return the size of memory needed accordingly.
// By passing NULL to `extra`, the function returns the size without actual initialization
usize LF_init(struct LF_manager_t *manager, void *extra, const LF_idx_t n);
void LF_modify_range(struct LF_manager_t *manager, LF_idx_t begin, LF_idx_t end, const LF_len_t value);
// Return the starting point of a field which fits the given length: `target`
// If there is no such field, -1 is returned instead
LF_idx_t LF_get_pos_overall(const struct LF_manager_t *manager, LF_len_t target);
// Get the maximum contiguous field in range [begin, end)
LF_len_t LF_get_max(const struct LF_manager_t *manager, LF_idx_t begin, LF_idx_t end);

#endif //_LARGEFRAME_H