#include "include/igraph.h"
#include <iostream>
#include "migraph.h"
#include <vector>
#include <string>
#include <fstream>
using namespace std;

void get_nei(vector<int> &res,
	const igraph_t *graph,
	const int id)
{
	igraph_vector_t neis;
	igraph_vector_init(&neis,1);
	igraph_neighbors(graph,&neis,id,IGRAPH_ALL);//

	for(int i=0;i!=neis.stor_end-neis.stor_begin;++i)
	{
		res.push_back(*(neis.stor_begin+i));
	}
	igraph_vector_destroy(&neis);
}

void gen_graph(igraph_t* res,
	const string & name)
{
	vector<int> edges;
	int temp;
	ifstream indata;
	indata.open(name.c_str(),ios::in);

	if(!indata)
	{
		cout<<"open error!"<<endl;
		exit(1);
	}
	for(;!indata.eof();)
	{
		indata>>temp;
		edges.push_back(temp);
	}
	edges.erase(edges.end()-1);
	igraph_vector_t vdeges;
	igraph_vector_init(&vdeges,edges.size());
	for(int i=0;i!=edges.size();++i)
	{
		*(vdeges.stor_begin+i)=edges[i];
	}
	edges.clear();
	igraph_create(res,&vdeges,0,true);
	igraph_vector_destroy(&vdeges);
	printf("graph finished\n");
}
