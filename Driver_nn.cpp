#include<iostream>
#include<vector>
#include<fstream>
#include<string>

#include "Graph_nn.h"

using namespace std;
using namespace tsp;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cerr<<"Usage: "<<argv[0]<<" <input file>"<<endl;
		return -1;
	}

	string input_filename(argv[1]);
	string output_filename(input_filename);
	output_filename.append(".tour");

	ifstream ifs(input_filename);
	ofstream ofs(output_filename);

	Graph G(ifs);
    G.tour(ofs);

    return 0;
}
