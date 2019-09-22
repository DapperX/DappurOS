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
	const int len = 1<<(log2i(n_aligned)-log2i(k)-1);
	if(t[k].left==len) t[k].left += t[r].left;
	if(t[k].right==len) t[k].right += t[l].right;
}

void seg_down(const int pos, const int terminator)
{
	for(int k=1, width=log2i(n_aligned);
		width && t[k].overwrite!=terminator;
		k=k<<1|(pos>>--width)&1)
	{
		auto &overwrite = t[k].overwrite;
		if(overwrite!=UNDEF)
		{
			const int len = (overwrite<<width)>>1;
			t[k<<1] = t[k<<1|1] = (segtree){len, len, len, overwrite};
			overwrite = UNDEF;
		}
	}
}

void seg_modify_range(int begin, int end, const int value)
{
	begin += n_aligned;
	end += n_aligned - 1;

	// pass down the mark to `begin`
	seg_down(begin, value);
	t[begin] = (segtree){value, value, value, UNDEF};
	// pass down the mark to `end`
	if(begin!=end)
	{
		seg_down(end, value);
		t[end] = (segtree){value, value, value, UNDEF};
	}

	// update upwards
	for(int len=value; (begin^end)>1; begin>>=1,end>>=1, len<<=1)
	{
		if(!(begin&1)) t[begin^1] = (segtree){len, len, len, value};
		if(t[begin>>1].overwrite==UNDEF) seg_update(begin>>1);
		if(end&1) t[end^1] = (segtree){len, len, len, value};
		if(t[end>>1].overwrite==UNDEF) seg_update(end>>1);
	}
	for(auto k=begin>>1; k; k>>=1)
		if(t[k].overwrite==UNDEF) seg_update(k);
}

int seg_get_max(int begin, int end) // get the maximum contiguous area in range [begin, end)
{
	int l = begin+n_aligned;
	int r = end+n_aligned-1;

	int bmans=t[l].mid, emans=t[r].mid;
	int brans=bmans, elans=emans;
	
	for(int len=1, mask=n_aligned-1;;
		l>>=1,r>>=1,len<<=1,mask>>=1)
	{
		int end_this = len*(1+(l&mask));
		if(t[l].overwrite!=UNDEF)
			bmans = brans = min(t[l].mid, end_this-begin);
		int begin_this = len*(r&mask);
		if(t[r].overwrite!=UNDEF)
			emans = elans = min(t[r].mid, end-begin_this);

		if((l^r)<=1) break;

		if(!(l&1))
		{
			end_this+=len, l^=1;
			int x = min(end_this-begin, max(t[l].mid, brans+t[l].left));
			if(x>bmans) bmans = x;
			if(t[l].right==len) brans += t[l].right;
			else brans = t[l].right;
		}
		if(r&1)
		{
			begin_this-=len, r^=1;
			int x = min(end-begin_this, max(t[r].mid, t[r].right+elans));
			if(x>emans) emans = x;
			if(t[r].left==len) elans += t[r].left;
			else elans = t[r].left;
		}
	}
	int ans = max(max(bmans, emans), brans+elans);
	for(auto k=l>>1; k; k>>=1)
		if(t[k].overwrite!=UNDEF) ans = t[k].mid;
	return min(ans, end-begin);
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