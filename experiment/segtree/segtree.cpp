#include <cstdio>
#include <cstdlib>
#include <algorithm>
#define N 15000
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
	const int clen = 1<<(log2i(n_aligned)-log2i(k)-1);
	if(t[k].left==clen) t[k].left += t[r].left;
	if(t[k].right==clen) t[k].right += t[l].right;
}

void seg_down(const int pos, const int terminator)
{
	for(int k=1, width=log2i(n_aligned);
		width && t[k].overwrite!=terminator;
		k=k<<1|(pos>>--width)&1)
	{
		if(t[k].overwrite==UNDEF) continue;
		if(t[k].overwrite==1)
		{
			const int clen = (1<<width)>>1;
			t[k<<1] = t[k<<1|1] = (segtree){clen, clen, clen, 1};
		}
		else t[k<<1] = t[k<<1|1] = (segtree){0, 0, 0, 0};
		t[k].overwrite = UNDEF;
	}
}

void seg_modify_range(int begin, int end, const int value)
{
	begin += n_aligned;
	end += n_aligned - 1;

	// pass down the mark to `begin`
	seg_down(begin, value);
	t[begin] = (segtree){value, value, value, UNDEF};
	if(begin==end) goto single;
	// pass down the mark to `end`
	seg_down(end, value);
	t[end] = (segtree){value, value, value, UNDEF};

	// update upwards
	for(; begin^end^1; begin>>=1,end>>=1)
	{
		const int clen = 1<<(log2i(n_aligned)-log2i(begin));
		if(!(begin&1)) t[begin^1]=value?(segtree){clen,clen,clen,1}:(segtree){0,0,0,0};
		if(t[begin>>1].overwrite==UNDEF) seg_update(begin>>1);
		if(end&1) t[end^1]=value?(segtree){clen,clen,clen,1}:(segtree){0,0,0,0};
		if(t[end>>1].overwrite==UNDEF) seg_update(end>>1);
	}
single:
	for(auto k=begin>>1; k; k>>=1)
		if(t[k].overwrite==UNDEF) seg_update(k);
}

int seg_get_max(int begin_, int end_) // get the maximum contiguous area in range [begin, end)
{
	int begin=begin_+n_aligned;
	int end=end_+n_aligned-1;

	int bmans=t[begin].mid, emans=t[end].mid;
	int brans=bmans, elans=emans;
	if(begin==end) goto single;
	for(int width=0; begin^end^1; begin>>=1,end>>=1, ++width)
	{
		const int stop_now = (1<<width)*(1+(begin&((n_aligned>>width)-1)));
		const int stop_ext = (1<<width)*(1+((begin|1)&((n_aligned>>width)-1)));
		// const int clamp_begin = stop - begin_;
		if(t[begin].overwrite==1)
		{
			bmans = brans = min(t[begin].mid, stop_now-begin_);
		}
		else if(t[begin].overwrite==0) bmans = brans = 0;

		if(!(begin&1))
		{
			bmans = max(bmans, max(t[begin^1].mid, brans+t[begin^1].left));
			if(bmans>stop_ext-begin_) bmans = stop_ext-begin_;
			if(t[begin^1].right==1<<width) brans += t[begin^1].right;
			else brans = t[begin^1].right;
			// if(brans==1<<width) brans += t[begin].right;
		}

		const int start_now = (1<<width)*(end&((n_aligned>>width)-1));
		const int start_ext = (1<<width)*(end&~1u&((n_aligned>>width)-1));
		// const int clamp_end = end_ - start;
		if(t[end].overwrite==1)
		{
			emans = elans = min(t[end].mid, end_-start_now);
		}
		else if(t[end].overwrite==0) emans = elans = 0;

		if(end&1)
		{
			emans = max(emans, max(t[end^1].mid, t[end^1].right+elans));
			if(emans>end_-start_ext) emans = end_-start_ext;
			if(t[end^1].left==1<<width) elans += t[end^1].left;
			else elans = t[end^1].left;
			// if(elans==1<<width) elans += t[end].left;
		}
	}
single:
	int width = log2i(n_aligned)-log2i(end);
	int mid = (1<<width)*(end&((n_aligned>>width)-1));
	if(t[begin].overwrite!=UNDEF)
		bmans = brans = min(mid-begin_, t[begin].mid);
	if(t[end].overwrite!=UNDEF)
		elans = emans = min(end_-mid, t[end].mid);
	int ans = max(max(bmans, emans), brans+elans);
	for(auto k=begin>>1; k; k>>=1)
		if(t[k].overwrite!=UNDEF) ans = t[k].mid;
	return min(ans, end_-begin_);
}

void output_segtree()
{
	fprintf(stderr, "---begin---\n");
	for(int width=1,sum=0; width<=n_aligned; sum+=width,width<<=1)
	{
		for(int i=1; i<=width; ++i)
		{
			auto k = &t[i+sum];
			fprintf(stderr, "#[%c](%d,%d,%d)  ",
				k->overwrite!=-1?k->overwrite+'0':'x',
				k->left, k->mid, k->right
			);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "----end----\n");
}

int main()
{
	// freopen("t.in", "r", stdin);
	// freopen("segtree.out", "w", stdout);

	int n;
	scanf("%d\n", &n);
	n_aligned = 1<<log2i(n);
	if(n_aligned<n) n_aligned<<=1;
	for(int i=0,x; i<n; ++i)
	{
		scanf("%d", &x);
		t[i+n_aligned] = (segtree){x, x, x, UNDEF};
	}
	for(int i=n_aligned-1; i; --i)
		seg_update(i);

	output_segtree();

	int m;
	scanf("%d", &m);
	for(int i=0; i<m; ++i)
	{
		int begin, end, value;
		scanf("%d%d%d", &begin, &end, &value);
		seg_modify_range(begin, end, value);
		fprintf(stderr, "--- after #%d modify ---\n", i);
		output_segtree();
	}

	int q;
	scanf("%d", &q);
	for(int i=0; i<q; ++i)
	{
		int begin, end;
		scanf("%d%d", &begin, &end);
		printf("%d\n", seg_get_max(begin, end));
		fprintf(stderr, "--- after #%d query ---\n", i);
		output_segtree();
	}
	return 0;
}