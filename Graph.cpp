#include<cmath>
#include<fstream>
#include<queue>
#include<stack>
#include<limits>
#include<iterator>
#include<map>
#include<set>
#include<list>
#include<exception>
#include<iostream>
#include "Graph.h"
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

void Graph::MST_Prim()
{
	typedef struct {int label; int key; int parent;} Node;

	//Priority queue element comparator
	auto cmp = [](const Node *n1, const Node *n2){return n1->key > n2->key;};
	std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)>Q(cmp);

	// of visited nodes
	std::vector<bool> visited(V.size(), false);
	std::vector<Node* > vertices;

	//Initialize each vertex's parent node and the distance to the parent node
	for(size_t i = 0; i < V.size(); ++i)
	{
		Node *n = new Node();
		n->label = i;
		n->key = std::numeric_limits<int>::max();
		n->parent = -1;

		vertices.push_back(n);
	}

	vertices.at(0)->key = 0;
	visited.at(0) = true;

	//Initialize the priority queue
	for(auto u : vertices)
	{
		Q.push(u);
	}
	while(!Q.empty()){
		auto u = Q.top();
		Q.pop();
		for(size_t i = 0; i < weight.at(u->label).size(); ++i)
		{
			int w = weight.at(u->label).at(i);
			auto v = vertices.at(i);

			if(!visited.at(i) && w > 0 && w < v->key)
			{
				v->parent = u->label;
				v->key = w;
			}
		}
		visited.at(u->label) = true;
	}

	//For each vertex that has a parent, associate the vertex with a list of adjacent vertices
	for(auto u : vertices)
	{
		if(u->parent != -1)
		{
			tree[u->label].push_back(u->parent);
			tree[u->parent].push_back(u->label);
		}
	}

	for(auto v: vertices)
		delete v;
}

void Graph::Perfect_Match()
{
	//A list of MST vertices with odd number of edges
	std::list<int> oddVertices;
	for(auto &entry : tree)
		if(entry.second.size() % 2 != 0)
			oddVertices.insert(oddVertices.end(), entry.first);

	int shortestDistance;
	int nearestNode;

	while (!oddVertices.empty()) {
		auto first = oddVertices.begin();
		auto second = std::next(first, 1);
		auto nearestIt = first;
		shortestDistance = std::numeric_limits<int>::max();

		for (auto it = second; it != oddVertices.end(); ++it) {

			//Weight from vertex '*first' to vertex '*it'
			int w = weight.at(*first).at(*it);

			if (w < shortestDistance) {
				shortestDistance = w;
				nearestNode = *it;
				nearestIt = it;
			}
		}

		tree[*first].push_back(nearestNode);
		tree[nearestNode].push_back(*first);

		oddVertices.erase(first);
		oddVertices.erase(nearestIt);
	}
}
void Graph::Make_Eulerian_Circuit (int pos)
{
	std::map<int, std::vector<int> > multigraph(tree);

    path.clear();
    auto n = V.at(pos);

    std::stack<int> S;
    while (!S.empty() || !multigraph[n].empty())
    {
    	if (multigraph[n].empty())
        {
			path.push_back(n);
			auto last = S.top();
			S.pop();
			n = last;
        }
        else
        {
        	S.push(n);
			auto neighbor = multigraph[n].back();
			multigraph[n].pop_back();
			for (size_t i = 0; i < multigraph[neighbor].size(); i++)
			{
				if (multigraph[neighbor].at(i) == n)
				{
					multigraph[neighbor].erase (multigraph[neighbor].begin() + i);
					break;
				}
			}
			n = neighbor;
        }
    }
    path.push_back(n);
}

int Graph::Make_Hamiltonian_Circuit()
{
    std::vector<bool> visited(V.size(), false);
    int path_dist = 0;

    auto root = path.front();
    auto curr = path.begin();
    auto next = path.begin()+1;
    visited.at(root) = true;

    while ( next != path.end() ) {
        if (!visited.at(*next)) {
            path_dist += weight.at(*curr).at(*next);
            curr = next;
            visited.at(*curr) = true;
            next = curr + 1;
        }else {
            next = path.erase(next);
        }
    }

    path_dist += weight.at(*curr).at(*next);

    return path_dist;
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
        v1 = (i+1) % n; // wrap around to first node if u1 is last node

        for (int j = i + 2; (j + 1) % n != i; j++)
        {
            u2 = j % n;
            v2 = (j+1) % n;

            auto w1 = weight.at(inPath[u1]).at(inPath[u2]) + weight.at(inPath[v1]).at(inPath[v2]);
            auto w2 = weight.at(inPath[u1]).at(inPath[v1]) + weight.at(inPath[u2]).at(inPath[v2]);

            if(w1 < w2)
            {
            	pathLength -= w2 - w1;

                // Swap u1--v1 and u2--v2
                twoOptSwap(inPath, i + 1, j); // v1, u2

                if (pathLength - old_distance < 5 && counter == 5)
                    break;

                // reset i
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

	MST_Prim();
	Perfect_Match();

	int best_start_pos = 0;
	int shortest_tour = std::numeric_limits<int>::max();
	for(auto u : V)
	{
		Make_Eulerian_Circuit(u);
		pathLength = Make_Hamiltonian_Circuit();

		//Optimize route
		twoOpt(path, pathLength);
		twoOpt(path, pathLength);
		twoOpt(path, pathLength);
		twoOpt(path, pathLength);
		twoOpt(path, pathLength);

		if(pathLength < shortest_tour)
		{
			shortest_tour = pathLength;
			best_start_pos = u;
		}
	}

	Make_Eulerian_Circuit(best_start_pos);
	pathLength = Make_Hamiltonian_Circuit();

	//Optimize route
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
