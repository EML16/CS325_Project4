
#ifndef GRAPH_H_
#define GRAPH_H_

#include<vector>
#include<fstream>
#include<map>

namespace tsp {
typedef struct {int label; int x; int y; bool isVisited;} Node;
	class Graph {
		private:
			std::vector<int> V;
			std::vector<int> path;
			std::vector<std::vector<int> >weight;		//Denotes a square matrix where (u,v) is the distance from u to v
			int nearest_neighbor(int u);
			int find_min(int u, std::vector<bool> &visited);
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
