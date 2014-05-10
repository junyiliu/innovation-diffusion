#include "mrand.h"
#include "include/igraph.h"
#include <vector>

using namespace std;

extern igraph_rng_t *mrng;

void permutation(	vector<int> &res,
	const int msize)
{
	for(int i=0;i!=msize;++i)
		res.push_back(i);

	int j=0,temp=0;
	for(int i=0;i!=msize;++i)
	{
		j=i+(msize-i)*igraph_rng_get_unif01(mrng);
		temp=res[i];
		res[i]=res[j];
		res[j]=temp;
	}
}
