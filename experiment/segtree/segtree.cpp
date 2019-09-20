#include <cstdio>
#include <cstdlib>
#include <algorithm>
#define N 100
#define UNDEF (-1)
using namespace std;
struct segtree{
	int left, mid, right, overwrite;
};
segtree t[N*4];
int n_aligned;

static inline int log2i(const int x)
{
	return sizeof(x)*8-1-__builtin_clz(x);
}

void seg_update(int k)
{
	int l = k*2, r = k*2+1;
	t[k] = (segtree){
		t[l].left, max(max(t[l].mid, t[r].mid),t[l].right+t[r].left), 
		t[r].right, UNDEF
	};
}
/*
void seg_set(int begin, int end)
{
	begin += n_aligned;
	end += n_aligned-1;
	t[begin] = (segtree){1, 1, 1, UNDEF};
	t[end] = (segtree){1, 1, 1, UNDEF};
	for(; begin^end^1; begin>>=1,end>>=1)
	{
		segtree *lch, *rch, *fa=&t[begin>>1];
		if(!(begin&1)) // l is set
		{
			lch = &t[begin], rch = &t[begin^1];
			*rch = (segtree){clen, clen, clen, 1};
		}
		else // r is set
		{
			lch = &t[begin^1], rch = &t[begin];
			if(f->overwrite==1)
				*lch = (segtree){clen, clen, clen, 1};
			else if(f->overwrite==0)
				*lch = (segtree){0, 0, 0, 0};
		}
		seg_update(begin>>1);

		fa = &t[end>>1];
		if(end&1) // r is set
		{
			lch = &t[begin^1], rch = &t[begin];
			*lch = (segtree){clen, clen, clen, 1};
		}
		else // l is set
		{
			lch = &t[begin], rch = &t[begin^1];
			if(f->overwrite==1)
				*rch = (segtree){clen, clen, clen, 1};
			else if(f->overwrite==0)
				*rch = (segtree){0, 0, 0, 0};
		}
		seg_update(end>>1);
	}
	while(auto k=begin; k; k>>=1)
	{

	}
}
*/

void seg_down(const int pos, const int terminator)
{
	for(int k=1, width=log2i(n_aligned);
		width && t[k].overwrite!=terminator;
		k=k<<1|(pos>>--width)&1)
	{
		if(t[k].overwrite==UNDEF) continue;
		if(t[k].overwrite==1)
		{
			const int clen = 1<<width;
			t[k<<1] = t[k<<1|1] = (segtree){clen, clen, clen, 1};
		}
		else t[k<<1] = t[k<<1|1] = (segtree){0, 0, 0, 0};
		t[k].overwrite = UNDEF;
	}
}

void seg_modify_range(int begin, int end, const int value)
{
	// pass down the mark to `begin`
	seg_down(begin, value);
	// pass down the mark to `end`
	seg_down(end, value);

	// update upwards
	begin += n_aligned;
	end += n_aligned - 1;
	t[begin] = (segtree){value, value, value, UNDEF};
	t[end] = (segtree){value, value, value, UNDEF};
	for(; begin^end^1; begin>>=1,end>>=1)
	{
		if(t[begin>>1].overwrite==UNDEF) seg_update(begin>>1);
		if(t[end>>1].overwrite==UNDEF) seg_update(end>>1);
	}
	while(auto k=begin>>1; k; k>>=1)
	{
		if(t[k].overwrite==UNDEF) seg_update(k);
	}
}

int seg_get_max(int begin_, int end_) // get the maximum contiguous area in range [begin, end)
{
	int begin=begin_+n_aligned;
	int end=end_+n_aligned-1;

	int bmans=t[begin].mid, emans=t[end].mid;
	int brans=bmans, elans=emans;
	for(int width=0; begin^end^1; begin>>=1,end>>=1, ++width)
	{
		if(t[begin].overwrite==1)
		{
			int stop = (1<<width)*(1+(begin&((n_aligned>>width)-1)));
			bmans = brans = min(t[begin].mid, stop-begin+1);
		}
		else if(t[begin].overwrite==0) bmans = brans = 0;

		if(!(begin^1))
		{
			bmans = max(bmans, max(t[begin^1].mid, brans+t[begin^1].left));
			if(t[begin^1].mid==1<<width) brans += 1<<width;
		}

		if(t[end].overwrite==1)
		{
			int start = (1<<width)*(end&((n_aligned>>width)-1));
			emans = elans = min(t[end].mid, end-start+1);
		}
		else if(t[end].overwrite==0) emans = elans = 0;

		if(end^1)
		{
			emans = max(emans, max(t[end^1].mid, t[end^1].right+elans));
			if(t[end^1].mid==1<<width) elans += 1<<width;
		}
	}
	int ans = max(max(bmans, emans), brans+elans);
	while(auto k=begin>>1; k; k>>=1)
		if(t[k].overwrite!=UNDEF) ans = t[k].mid;
	return min(ans, end_-begin_+1);
}

int main()
{
	int n;
	scanf("%d\n", &n);
	n_aligned = log2i(n);
	if(n_aligned<n) n_aligned<<=1;
	for(int i=0,x; i<n_aligned; ++i)
	{
		scanf("%d", &x);
		a[i+n_aligned] = (segtree){x, x, x};
	}
	for(int i=n_aligned-1; i; --i)
		seg_update(i);

	int m;
	scanf("%d", &m);
	for(int i=0; i<m; ++i)
	{
		int begin, end, value;
		scanf("%d%d%d", &begin, &end, &value);
		if(value) seg_set(begin, end);
		else seg_clear(begin, end);
	}

	int q;
	scanf("%d", &q);
	for(int i=0; i<q; ++i)
	{
		int begin, end;
		scanf("%d%d", &begin, &end);
		printf("%d\n", seg_get_max(begin, end));
	}
	return 0;
}