#include "epidemic.h"
#include "include/igraph.h"
#include "mrand.h"
#include "my_vector.h"
#include "migraph.h"
#include <numeric>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdio>
using namespace std;
#pragma comment(lib,"igraph.lib")

extern igraph_rng_t *mrng;

extern void intersec(const vector<int> &, const vector<int>&, vector<int>&);
extern void permutation(vector<int> &res, const int msize);

extern void get_nei(vector<int> &res, const igraph_t *graph, const int id);
extern void printvec(const vector<int> &ve);

int population::initial_state()
{
	vector<int> state(size,-1);
	vector<int> sub;
	permutation(sub,size);
	for(int i=0;i!=size;++i)
	{
		agents[sub[i]].state=(i<int(size*initial_p)?(1):(-1));
	}
	return 0;
}
// initial _nei must be called after the network has been initailized
int population::initial_nei()
{
	for (int i=0;i!=size;++i)
	{
		vector<int> nei;
		get_nei(nei,&graph,agents[i].id);
		//agents[i].nei=nei;
		for (unsigned j=0;j!=nei.size();++j)
		{
			agents[i].nei.push_back(nei[j]);
		}
	}
	return 0;
}
//initial_sij must be called after the neighbors has been initialized
int population::initial_sij()
{
	//initialize sij
	for(int i=0;i!=size;++i)
	{
		//calculate sij
		int ineisize=agents[i].nei.size();

		for(unsigned j=0;j!=ineisize;++j)
		{
			//find common neighbor of i and j
			vector<int> com_nei;
			int nei_temp=agents[i].nei[j];
			intersec(agents[i].nei,agents[nei_temp].nei,com_nei);	
			agents[i].sij.push_back(double(com_nei.size()+delta)/sqrt(double\
				(ineisize*agents[nei_temp].nei.size())));
		}
	}
	return 0;
}
// initial_probability must be called after the sij has been initialized
int population::initial_probability()
{
	//initialize probability
	for(int i=0;i!=size;++i)
	{
		//calculate prob
		for(unsigned j=0;j!=agents[i].nei.size()+1;++j)
		{
			if(j)
			{
				double sum=agents[i].probability[j-1]+pow(\
					agents[i].sij[j-1],b);
				agents[i].probability.push_back(sum);
			}
			else
				agents[i].probability.push_back(0);

		}
		for(unsigned j=0;j!=agents[i].nei.size()+1;++j)
		{
			agents[i].probability[j]/=agents[i].\
				probability[agents[i].probability.size()-1];
		}
	}
	return 0;
}
population::population(const string &s_nwname,
	const igraph_t* sample_graph,
	const double sdelta,
	const double p,
	const double b_s,
	const int type,
	const int configure_rounds,
	const int shuffle_rounds)
{
	igraph_copy(&graph,sample_graph);
	nwname = s_nwname;
	size=int(igraph_vcount(&graph));
	delta=sdelta;
	initial_p=p;
	b=b_s;
	//initial agents
	individual sample;
	for (int i=0;i!=size;++i)
	{
		agents.push_back(sample);
	}
	//initial id
	for(int i=0;i!=size;++i)
	{
		agents[i].id=i;
	}
	//initial state
	initial_state();
	//initial neighbors
	initial_nei();
	initial_sij();
	switch(type)
	{
		case 1:configure_all(configure_rounds);break;
		case 2:shuffle_weight_all(shuffle_rounds);break;
		default:;break;
	}
	initial_probability();	
}
	
void population::s2i(const int id,
	int *res,
	bool *issucess,
	const double alpha)
{
	//get id's neighbor probability
	vector<double> accu_probability(agents[id].probability);

	double ramdn=igraph_rng_get_unif01(mrng);
	accu_probability.push_back(ramdn);
	sort(accu_probability.begin(),accu_probability.end());
	int po=find(accu_probability.begin(),accu_probability.end(),\
		ramdn)-accu_probability.begin()-1;
	*res=agents[id].nei[po];
	if(agents[*res].state==-1&&igraph_rng_get_unif01(mrng)<alpha)
	{
		*issucess=true;
		agents[*res].state=1;
	}
	else
		*issucess=false;
}
void population::population_dynamics(double *rres,
	const double alpha,
	const double beta,
	const int rounds)
{
	
	//to generate the name of the output file
	char temp_name[254];
	sprintf(temp_name, "_%d.txt",int( b));
	//output file name
	string output_name(nwname);
	output_name += temp_name;
	
//	FILE *outfile;
//	outfile = fopen(output_name.c_str(), "w");
	
	
	vector<double> res;
	//get all the I node
	vector<int> Inodes;
	for(int i=0;i!=size;++i)
	{
		if(1==agents[i].state)
			Inodes.push_back(agents[i].id);
	}
	//to update
	for(int t=0;t<rounds;++t)
	{
		res.push_back(Inodes.size());

		int current_size=Inodes.size();
//		fprintf(outfile,"%d\n",current_size);
//		fflush(outfile);
		vector<int> s2inodes;
		int snode=0;
		bool issucess=false;
		//S2I
		for(unsigned i=0;i!=current_size;++i)
		{
			s2i(Inodes[i],&snode,&issucess,alpha);
			if(issucess)
			{
				Inodes.push_back(snode);
			}
		}
		//I2S
		vector<int> i2s;
		for(unsigned i=0;i!=Inodes.size();++i)
		{
			if (igraph_rng_get_unif01(mrng)<beta)
			{
				agents[Inodes[i]].state=-1;
				i2s.push_back(Inodes[i]);
			}
		}
		for(unsigned i=0;i!=i2s.size();++i)
		{
			Inodes.erase(find(Inodes.begin(),Inodes.end()
				,i2s[i]));
		}
	}
//	fclose(outfile);
	*rres = accumulate(res.end() - 200, res.end(), 0) / 200;
}
void population::configure_pair(const int lid,const int rid)
{	
	int counter=0;
	//select one neighbors of lid and rid
	int lcid=0,rcid=0;
	bool legal_id=false;
	do 
	{
		lcid=agents[lid].nei[igraph_rng_get_integer(mrng,0,\
			agents[lid].nei.size()-1)];
		rcid=agents[rid].nei[igraph_rng_get_integer(mrng,0,\
			agents[rid].nei.size()-1)];
		//whether already connected
		legal_id=(find(agents[rid].nei.begin(),agents[rid].nei.end(),\
			lcid)==agents[rid].nei.end()&&find(agents[lid].nei.begin(),\
			agents[lid].nei.end(),rcid)==agents[lid].nei.end()&&
			lcid!=rid&&rcid!=lid);
		counter+=!legal_id;
		if(counter>30)
		{
			return;
		}
	} while (!legal_id);

	*(find(agents[lid].nei.begin(),agents[lid].nei.end(),lcid))=rcid;
	*(find(agents[rid].nei.begin(),agents[rid].nei.end(),rcid))=lcid;
	*(find(agents[lcid].nei.begin(),agents[lcid].nei.end(),lid))=rid;
	*(find(agents[rcid].nei.begin(),agents[rcid].nei.end(),rid))=lid;

	return;
}
void population::configure_all(const int rounds)
{
	int id1=0;
	int id2=0;
	for(int i=0;i!=rounds;++i)
	{
		id1=igraph_rng_get_integer(mrng,0,size-1);
		id2=igraph_rng_get_integer(mrng,0,size-1);
		while(id1==id2)
		{
			id2=igraph_rng_get_integer(mrng,0,size-1);
		}
		configure_pair(id1,id2);
	}
	return ;
}
void population::shuffle_weight_pair(const int lid, const int rid)
{
	int lcid=igraph_rng_get_integer(mrng,0,\
		agents[lid].sij.size()-1);
	int rcid=igraph_rng_get_integer(mrng,0,\
		agents[rid].sij.size()-1);
	double temp=agents[lid].sij[lcid];
	agents[lid].sij[lcid]=agents[rid].sij[rcid];
	agents[rid].sij[rcid]=temp;
}
void population::shuffle_weight_all(const int rounds)
{
	int id1=0;
	int id2=0;
	for(int i=0;i!=rounds;++i)
	{
		id1=igraph_rng_get_integer(mrng,0,size-1);
		id2=igraph_rng_get_integer(mrng,0,size-1);
		while(id1==id2)
		{
			id2=igraph_rng_get_integer(mrng,0,size-1);
		}
		shuffle_weight_pair(id1,id2);
	}
	return ;
}
