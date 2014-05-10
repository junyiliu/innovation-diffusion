#include "my_vector.h"
#include <vector>
#include <map>
using namespace std;

void intersec(const vector<int> & lv,
	const vector<int>& rv,
	vector<int>& res)
{
	//copy lv to the hash table
	map<int, int> temp;
	for(int i=0;i!=lv.size();++i)
	{
		temp.insert(map<int,int>::value_type(lv[i],1));
	}
	for(int i=0;i!=rv.size();++i)
	{
		if(temp.find(rv[i])!=temp.end())
			temp[rv[i]]=2;
	}
	for(map<int,int>::iterator it=temp.begin();
		it!=temp.end();++it)
	{
		if(2==(it->second))
			res.push_back(it->first);
	}
}
 void printvec(const vector<int> &ve)
{
	for(int i=0;i!=ve.size();++i)
	{
		printf("%d ",ve[i]);
	}
	printf("\n");
}