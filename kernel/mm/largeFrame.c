#include "largeFrame.h"
#include "assert.h"
#include "memory.h"
#include "math.h"
#include "algorithm.h"

#define SEG_OW_UNDEF ((LF_len_t)(~0u))

static void LF_update(struct LF_manager_t *manager, const LF_idx_t k)
{
	struct segtree_LF *const seg = manager->segtree;
	LF_idx_t l = k*2, r = k*2+1;
	seg[k] = (struct segtree_LF){
		seg[l].left,
		max_u16(
			max_u16(seg[l].mid, seg[r].mid),
			seg[l].right+seg[r].left
		), 
		seg[r].right,
		SEG_OW_UNDEF
	};
	const LF_idx_t len = 1<<(log2i(manager->n_aligned)-log2i(k)-1);
	if(seg[k].left==len) seg[k].left += seg[r].left;
	if(seg[k].right==len) seg[k].right += seg[l].right;
}

static void LF_down(struct LF_manager_t *manager, const LF_idx_t pos, const LF_len_t terminator)
{
	struct segtree_LF *const seg = manager->segtree;
	for(LF_idx_t k=1, width=log2i(manager->n_aligned);
		width && seg[k].overwrite!=terminator;
		k=k<<1|(pos>>--width)&1)
	{
		const LF_len_t overwrite = seg[k].overwrite;
		if(overwrite!=SEG_OW_UNDEF)
		{
			const LF_len_t len = (overwrite<<width)>>1;
			seg[k<<1] = seg[k<<1|1] = (struct segtree_LF){len, len, len, overwrite};
			seg[k].overwrite = SEG_OW_UNDEF;
		}
	}
}

void LF_modify_range(struct LF_manager_t *manager, LF_idx_t begin, LF_idx_t end, const LF_len_t value)
{
	struct segtree_LF *const seg = manager->segtree;
	begin += manager->n_aligned;
	end += manager->n_aligned - 1;

	// pass down the mark to `begin`
	LF_down(manager, begin, value);
	seg[begin] = (struct segtree_LF){value, value, value, SEG_OW_UNDEF};
	// pass down the mark to `end`
	if(begin!=end)
	{
		LF_down(manager, end, value);
		seg[end] = (struct segtree_LF){value, value, value, SEG_OW_UNDEF};
	}

	// update upwards
	for(LF_len_t len=value; (begin^end)>1; begin>>=1,end>>=1, len<<=1)
	{
		if(!(begin&1)) seg[begin^1] = (struct segtree_LF){len, len, len, value};
		if(seg[begin>>1].overwrite==SEG_OW_UNDEF) LF_update(manager, begin>>1);
		if(end&1) seg[end^1] = (struct segtree_LF){len, len, len, value};
		if(seg[end>>1].overwrite==SEG_OW_UNDEF) LF_update(manager, end>>1);
	}
	for(LF_idx_t k=begin>>1; k; k>>=1)
		if(seg[k].overwrite==SEG_OW_UNDEF) LF_update(manager, k);
}

LF_idx_t LF_get_pos_overall(const struct LF_manager_t *manager, LF_len_t target)
{
	KASSERT(target>0);

	const struct segtree_LF *const seg = manager->segtree;
	LF_idx_t begin = 0;
	for(LF_idx_t k=1, len=manager->n_aligned;; len>>=1)
	{
		const struct segtree_LF *const tk=&seg[k];
		const struct segtree_LF *const tl=&seg[k<<1];
		const struct segtree_LF *const tr=&seg[k<<1|1];

		if(tk->left>=target) return begin;
		if(tk->right>=target) return begin+len-tk->right;

		if(tl->mid>=target) k = k<<1;
		else if(tl->right+tr->left>=target) return begin+len/2-tl->right;
		else if(tr->mid>=target) k = k<<1|1, begin += len>>1;
		else return -1;
	}
	return -1;
}

LF_len_t LF_get_max(const struct LF_manager_t *manager, LF_idx_t begin, LF_idx_t end)
{
	const struct segtree_LF *const seg = manager->segtree;
	LF_idx_t l = begin+manager->n_aligned;
	LF_idx_t r = end+manager->n_aligned-1;

	LF_len_t lmans=seg[l].mid, rmans=seg[r].mid;
	LF_len_t lrans=lmans, rlans=rmans;
	
	for(LF_idx_t len=1, mask=manager->n_aligned-1;;
		l>>=1,r>>=1,len<<=1,mask>>=1)
	{
		LF_idx_t end_this = len*(1+(l&mask));
		if(seg[l].overwrite!=SEG_OW_UNDEF)
			lmans = lrans = min_u16(seg[l].mid, end_this-begin);
		LF_idx_t begin_this = len*(r&mask);
		if(seg[r].overwrite!=SEG_OW_UNDEF)
			rmans = rlans = min_u16(seg[r].mid, end-begin_this);

		if((l^r)<=1) break;

		if(!(l&1))
		{
			end_this+=len, l^=1;
			LF_len_t x = min_u16(end_this-begin, max_u16(seg[l].mid, lrans+seg[l].left));
			if(x>lmans) lmans = x;
			if(seg[l].right==len) lrans += seg[l].right;
			else lrans = seg[l].right;
		}
		if(r&1)
		{
			begin_this-=len, r^=1;
			LF_len_t x = min_u16(end-begin_this, max_u16(seg[r].mid, seg[r].right+rlans));
			if(x>rmans) rmans = x;
			if(seg[r].left==len) rlans += seg[r].left;
			else rlans = seg[r].left;
		}
	}
	LF_len_t ans = max_u16(max_u16(lmans, rmans), lrans+rlans);
	for(LF_idx_t k=l>>1; k; k>>=1)
		if(seg[k].overwrite!=SEG_OW_UNDEF) ans = seg[k].mid;
	return min_u16(ans, end-begin);
}

usize LF_init(struct LF_manager_t *manager, void *extra, const LF_idx_t n)
{
	LF_idx_t n_aligned = 1<<log2i(n);
	if(n_aligned<n) n_aligned<<=1;
	KASSERT(n_aligned>=n);

	usize size = n_aligned*sizeof(struct segtree_LF)*2;
	if(extra==NULL) return size;

	KASSERT(manager!=NULL);
	kmemset(extra, 0, size);
	*manager = (struct LF_manager_t){extra, n_aligned};
	return size;
}