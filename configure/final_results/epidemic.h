
#include <vector>
#include <string>
#include<algorithm>
#include "include/igraph.h"
//#pragma comment(lib,"igraph.lib")
#include <numeric>
#include <vector>
#include <map>

using namespace std;


extern igraph_rng_t *mrng; /** random number generator */
//! individual class
/*!
	to define some basic properties of invidual
 */
struct individual
{
	int id;  /*!the id of the invidual */
	int state;/*!the current state of the invidual */
	vector <int> nei;/*!the neighbors of invidual*/
	vector <double> sij;/*!the common */
	vector <double> probability; /*!the probability that an node i select its
											 neighbor j*/ 
};
//!  the pupulation class
/*!
 invidual in a network
 */
struct population
{
public:
	igraph_t graph; /*!the network */
	int size;      /*! the size of the network */
	double initial_p;/* the initial fraction of innovation seed */
	double b;      /* the power of the weight */
	vector<individual> agents; /* all the invidual  */
	double delta;   /* the constant give the chance to the tie connecting two 
  nodes without common neighbors	*/
	string nwname;  /* the name of the emperical network */
public:
	 /*!
	  A constructor function of the population class 
	  *@param s_nwname the name of the network
	  *@param sample_graph the sample graph
	  *@param sdelta the value to initialize delta
	  *@param b to initialize the power of weight
	  *@param type the configure mode of the graph
	  *@param config_round the rounds to configure the graph
	  *@param shuffle_rounds the rounds to shuffle the weight of the network
	   */
	population(const string &s_nwname,
		const igraph_t* sample_graph,
		const double sdelta,
		const double initial_p,
		const double b_s,
		const int type,
		const int config_round,
		const int shuffle_rounds);
	/*!
	 *the destructor of the population class
	 */
	~population()
	{
		igraph_destroy(&graph);
	}
	/*!
	 * to initialize the state of the invidual in the network
	 */
	int initial_state();
	/*!
	 * to initialize the neighbors of the invidual
	 */
	int initial_nei();
	/*!
	 *to initialize the tie strength of the invidual
	 */
	int initial_sij();
	/* !
	 *to initialize the probability invidual i select invidual j
	 */
	int initial_probability();
	/*!
	 * the process of invidual i with state S change to state I
	 * @param id the id of one infected node 
	 * @param res if one of the neighbors with S state is infected successfully,
	 * then res will return the id of that node
	 * @param issucess whether the infection process is successful
	 * @param alpha the possibility of invidual with S state is infected
	 */
	void s2i(const int id,
		int *res,
		bool *issucess,
		const double alpha);
	/*!
	 * the transmisson process of the total population
	 * @param res the final  infection rate of the total population
	 * @param alpha the possibility of one susceptible is infected
	 * @param beta the possibility of one infected invidual is recovered
	 */
	void population_dynamics(double *res,
		const double alpha,
		const double beta,
		const int rounds);
	/*!
	 * to swap a pair of neighbors between a pair of nodes
	 * @param lid the id of the first node
	 * @param rid the id of the second node
	 */
	void configure_pair(const int lid, const int rid );
	/* !
	 *to conduct the configure model
	 *@param rounds the rounds to swap pairs of neighbors between a pair of nodes
	 */
	void configure_all(const int rounds);
	/* !
	 *to swap the weight between a pair of neighbors of two nodes
	 *@param lid the first node
	 *@param rld the second node
	 */
	void shuffle_weight_pair(const int lid, const int rid);
	/* !
	 * to shuffle the weight of node for many rounds
	 * @param rounds the rounds of the shuffling process
	 */
	void shuffle_weight_all(const int rounds);
};
