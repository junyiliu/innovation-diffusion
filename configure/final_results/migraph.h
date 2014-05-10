#ifdef _MIGRAPH_H
#define  _MIGRAPH_H

#include "include/igraph.h"
#include <string>

using namespace std;

void get_nei(vector<int> &res,
	const igraph_t *graph,
	const int id);
void gen_graph(igraph_t*,
	const string &);
#endif