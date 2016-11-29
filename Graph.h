
#ifndef GRAPH_H_
#define GRAPH_H_

#include<vector>
#include<fstream>
#include<map>

namespace tsp {
	class Graph {
		private:
			std::vector<int> V;
			std::vector<int> path;
			std::vector<std::vector<int> >weight;		//Denotes a square matrix where (u,v) is the distance from u to v
			std::map<int, std::vector<int> >tree;
			void MST_Prim();
			void Perfect_Match();
			void Make_Eulerian_Circuit (int pos);
			int Make_Hamiltonian_Circuit();
			int twoOpt(std::vector<int> &, int &);
			void twoOptSwap(std::vector<int> &, int, int);

		public:
			Graph();
			Graph(std::ifstream &ifs);
			void tour(std::ofstream &ofs);
			virtual ~Graph();
	};

} /* namespace tsp */

#endif /* GRAPH_H_ */
