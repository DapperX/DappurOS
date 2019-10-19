#include <cstdio>
#include <cstdlib>
#include <sys/timeb.h>
#include <random>
#include <algorithm>
#define N 2000
#define M 80
#define Q 100
using namespace std;


unsigned long get_time_current()
{
	struct timeb tb;
	ftime(&tb);
	return tb.time * 1000 + tb.millitm;
}

int main()
{
	default_random_engine e(get_time_current());
	// freopen("t.in", "w", stdout);
	int n = e()%N+1;
	printf("%d\n", n);
	for(int i=0; i<n; ++i)
		printf("%d ", e()&1);
	putchar('\n');

	int m = e()%M+1;
	printf("%d\n", m);
	for(int i=0; i<m; ++i)
	{
		int x = rand()%n;
		int y = rand()%n;
		if(x>y) swap(x, y);
		printf("%d %d %d\n", x, y+1, e()&1);
	}

	int q = e()%Q+1;
	printf("%d\n", q);
	for(int i=0; i<q; ++i)
	{
		int x = rand()%n;
		int y = rand()%n;
		if(x>y) swap(x, y);
		printf("%d %d\n", x, y+1);
	}
	return 0;
}