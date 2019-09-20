#include <cstdio>
#include <cstdlib>
#include <algorithm>
#define N 100000
using namespace std;
struct contiguous{
	int left, mid, right, overwrite;
};
contiguous t[N*4];
int n_aligned;
int bq, eq, vq;

static inline int log2i(const int x)
{
	return sizeof(x)*8-1-__builtin_clz(x);
}

void seg_init(int k)
{
	int l = k*2, r = k*2+1;
	t[k] = (contiguous){
		t[l].left, t[l].right+t[r].left, t[r].right
	};
}

void seg_modify_range(int l, int r)
{
}

int seg_get_max(int int l, int r) // get the maximum contiguous area
{
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
		a[i+n_aligned] = (contiguous){x, x, x};
	}
	for(int i=n_aligned-1; i; --i)
		seg_init(i);

	int m;
	scanf("%d", &m);
	for(int i=0; i<m; ++i)
	{
		scanf("%d%d%d", &bq, &eq, &vq);
		seg_modify_range(1, 1, n_aligned);
	}

	int q;
	scanf("%d", &q);
	for(int i=0; i<q; ++i)
	{
		scanf("%d%d", &bq, &eq);
		printf("%d\n", seg_get_max(1, 1, n_aligned));
	}
	return 0;
}