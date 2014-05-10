#include "my_vector.h"
#include "epidemic.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include "include/igraph.h"
#include "migraph.h"
#pragma comment(lib,"igraph.lib")

igraph_rng_t *mrng=igraph_rng_default();
extern void gen_graph(igraph_t* res,
	const string & name);

int main(int argc,char*argv[])
{
	if(argc!=2)
	{
		cout<<"USAGE:SIS datafile  "<<endl;
		exit(1);
	}
	//define the entry

	string networkname(argv[1]); //network name

	double a=1;
	double alpha=0.4;
	double beta=0.1;
	int game_rounds=500; //MC rounds
	int aver_rounds=10;  //do average
	double delta=1;//delta


	string output_name_pre;  //the network name 
	for(int i=0;i!=networkname.size()-4;i++)
	{
		output_name_pre.push_back(networkname[i]);
	}

	//random generator
	//to initialize the random seed
	igraph_rng_init(mrng,&igraph_rngtype_mt19937);

	unsigned long idum;  // random number seed
	idum=(unsigned long)time(NULL);
	igraph_rng_seed(mrng,idum);

	//construct the graph
	igraph_t network;
   
	gen_graph(&network,networkname);
	int nwsize=igraph_vcount(&network);  //the size of the network
   
   
	//to generate the name of the output file
	//output file name
	string resname("res.txt");
	string totalname=output_name_pre+resname;	
	FILE *outfile;
	outfile = fopen(totalname.c_str(), "w");
	
	//construct the population
	for(a=-6;a<6.1;a+=0.1 )
	{
		population popu(output_name_pre,&network,delta,0.01,a,2,3*nwsize,3*nwsize);
		double res=0;
		popu.population_dynamics(&res,alpha,beta,1000);
		fprintf(outfile,"%f\t%f\n",a,res/ nwsize);
		fflush(outfile);
	}
	fclose(outfile);
/*  
	igraph_t network;
	int nwsize = igraph_vcount(&network);  //the size of the network
	igraph_watts_strogatz_game(&network, 1, 500, 4, 0.1, false, false);
	string output_name_pre("small_world");
	population test(output_name_pre,&network, 1, 0.01, 1, 0, 3 * nwsize, 3 * nwsize);
	test.initial_state();

	double res = 0;

	test.population_dynamics(&res,0.3,0.1,300);
*/
	//for (a=-5;a!=5;a+=0.2)
	//
	//{

 //       population test(&network,delta,0.01,a,type,3*nwsize,3*nwsize);
	//	vector<double> average_rres;

	//	for(int i=0;i!=aver_rounds;++i)
	//	{
	//		double rres=0;
	//		test.initial_state();
	//		test.population_dynamics(&rres,0.4,0.1,game_rounds);
	////		printf("%f\n", rres);
	//		average_rres.push_back(rres);

	//	}
	//	fprintf(outfile,"%f\t%f\n",a,double (accumulate(average_rres.begin(),\
	//		average_rres.end(),0)/average_rres.size()));
	//	fflush(outfile);
	//}
	//fclose(outfile);
	igraph_destroy(&network);
	return 0;
}
