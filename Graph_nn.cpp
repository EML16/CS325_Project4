#include<cmath>
#include<fstream>
#include<queue>
#include<stack>
#include<limits.h>
#include<limits>
#include<iterator>
#include<map>
#include<set>
#include<list>
#include<exception>
#include<iostream>
#include "Graph_nn.h"
using namespace std;
namespace tsp {

Graph::Graph() {}
Graph::~Graph() {}

Graph::Graph(std::ifstream &ifs)
{
	int label;
	int x;
	int y;

	typedef struct {int label; int x; int y;} Node;
	std::vector<Node> vertices;

	//Read each line of the input file
	while(ifs>>label>>x>>y)
	{
		Node n = {label, x, y};
		vertices.push_back(n);
		V.push_back(label);
	}

	std::vector<int> row;	//Euclidean distance from a vertex to each of the vertices, including itself
	for(auto u : vertices){
		for(auto v : vertices){
			auto l = (u.x - v.x);
			auto h = (u.y - v.y);

			int d = std::round(std::sqrt(l*l + h*h));	//Calculate Euclidean distance

			row.push_back(d);
		}
		weight.push_back(row);
		row.clear();
	}
}

int Graph::nearest_neighbor(int start) {
	int pathLength = 0;

	std::vector<bool> visited(V.size(), false);
	int u = start;

	//add start city u in list of vertices V to path
	path.push_back(V.at(u));


	//remove u city from list of cities
	visited.at(u) = true;


	//nearest neighbor
	int v;

	//add city nearest u to path, update path length, and update isVisited
	while (path.size() < V.size()) {

		//find city with shortest edge
		v = find_min(u, visited);

		if (!visited.at(v)) {

			//add city with shortest edge to path
			path.push_back(V.at(v));

			//update path length
			pathLength += weight.at(u).at(v);


			//remove v from list of unvisited vertices and set source city u to previous destination v
			u = v;  //set u to v

			visited.at(v) = true;

		}

	}
	//add path length to return to starting location
	pathLength += weight.at(v).at(start);
	
	return pathLength;

}

int Graph::find_min(int u, vector<bool> &visited){

	int min = INT_MAX;
	int v = 0;

	for(int i = 0; i < V.size(); i++){

		//if edge weight less than min and city has not been visited, return city label
		if ((weight.at(u).at(i) < min) && (i != u) && !visited.at(i)) {

			min = weight.at(u).at(i);
			v = i;
		}
	}

	return v;
}


int Graph::twoOpt(std::vector<int> &inPath, int &pathLength)
{
    int counter = 0;
    int old_distance = pathLength;
    int n = V.size();
    int v1, v2, u1, u2;

    for (int i = 0; i < n; i++)
    {
        u1 = i;
        v1 = (i+1) % n; 

        for (int j = i + 2; (j + 1) % n != i; j++)
        {
            u2 = j % n;
            v2 = (j+1) % n;

            auto w1 = weight.at(inPath[u1]).at(inPath[u2]) + weight.at(inPath[v1]).at(inPath[v2]);
            auto w2 = weight.at(inPath[u1]).at(inPath[v1]) + weight.at(inPath[u2]).at(inPath[v2]);

            if(w1 < w2)
            {
            	pathLength -= w2 - w1;

                
                twoOptSwap(inPath, i + 1, j); 

                if (pathLength - old_distance < 5 && counter == 5)
                    break;

                
                if (pathLength - old_distance > 5 )
                    i = 0;

                old_distance = pathLength;
                counter++;
            }
        }
    }
    return pathLength;
}

void Graph::twoOptSwap(std::vector<int> &inPath, int start, int end)
{
	int n = V.size();
    while (end - start > 0)
    {
        int temp = inPath[start % n];
        inPath[start % n] = inPath[end % n];
        inPath[end % n] = temp;
        start++;
        end--;
    }
}


void Graph::tour(std::ofstream &ofs)
{
	int pathLength;

	pathLength = nearest_neighbor(0);
        twoOpt(path, pathLength);
	twoOpt(path, pathLength);
	twoOpt(path, pathLength);
	twoOpt(path, pathLength);
	twoOpt(path, pathLength);

	ofs<<pathLength<<endl;
	for(auto v: path)
		ofs<<v<<endl;
}

} /* namespace tsp */
