#include <cstdio>
#include <cstdlib>
#include <algorithm>
#define N 15000
using namespace std;

int a[N];

int get_max(int begin, int end)
{
	int now=0, ans=0;
	for(int i=begin; i<end; ++i)
	{
		if(a[i]) now++;
		else
		{
			ans = max(now, ans);
			now = 0;
		}
	}
	return max(now, ans);
}

int get_position_overall(int target, int n)
{
	if(target==0) return 0;
	int i = 0;
	while(i<n)
	{
		while(i<n && !a[i]) i++;
		int j = i;
		while(j<n && a[j]) j++;
		if(j-i>=target) return i;
		i = j;
	}
	// return target?-1:0;
	return -1;
}

int main()
{
	// freopen("t.in", "r", stdin);
	// freopen("std.out", "w", stdout);

	int n;
	scanf("%d\n", &n);
	for(int i=0; i<n; ++i) scanf("%d", &a[i]);

	int m;
	scanf("%d", &m);
	for(int i=0; i<m; ++i)
	{
		int begin, end, value;
		scanf("%d%d%d", &begin, &end, &value);
		for(int j=begin; j<end; ++j) a[j] = value;
	}

	int q;
	scanf("%d", &q);
	for(int i=0; i<q; ++i)
	{
		int begin, end;
		scanf("%d%d", &begin, &end);
		printf("%d\n", get_max(begin, end));
	}
	int ans = get_position_overall(get_max(0, n), n);
	if(ans==-1) puts("Fail");
	else printf("%d\n", ans);
	return 0;
}