#include <iostream>
#include <vector>
#include <queue> 
#include <limits>
#include <limits.h> 
#include <algorithm>
#include <fstream>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

int n, e;

struct edge 
{
    int source, dest, cap;
};

/// @brief Function to fdetermine if an augmented path exists from source to sink for the given residual graph
/// @param residual Vector of type structure edgee that stores the network flow
/// @param source Source node
/// @param sink Sink node
/// @param parent Placehold Vector of type int that stores that maps a node to it's previous node in a given augmented path
/// @return True or false for whether an augmented path exists for given residual graph
bool augment_path(vector<edge>& residual, int source, int sink, vector<int>& parent)
{
    vector<bool> visited(n, false);
    queue<int> q;

    visited[source] = true;
    q.push(source);
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int i = 0; i < residual.size(); i++) {
            edge e = residual[i];
            if (e.source == u && !visited[e.dest] && e.cap > 0) {
                visited[e.dest] = true;
                q.push(e.dest);
                parent[e.dest] = i;
                if (e.dest == sink) {
                    return true;
                }
            }
        }
    }

    return false;
}

/// @brief Function to calculate the bottleneck value for a given augmented path
/// @param residual The residual graph 
/// @param source Source node of the input graph
/// @param sink Sink node of the input graph
/// @param parent Vector that maps a node to it's previous node in a given augmented path
/// @return Bottleneck value of the given augmented path
int bottleneck(vector<edge>& residual, int source, int sink, vector<int>& parent) 
{
    int b = INT_MAX;
    int v = sink;

    while (v != source) {
        int i = parent[v];
        edge e = residual[i];
        b = min(b, e.cap);
        v = e.source;
    }

    return b;
}

/// @brief Function to add back edges to the network flow after every iteration of the Ford Fulkerson algorithm
/// @param edges Placeholder vector of type edge which is traversed through to find a particular edge in it
/// @param u Source node of backedge
/// @param v Destination node of backedge
/// @param b Bottleneck value

void add_backedge(vector<edge>& edges, int u, int v, int b) {
    for (edge& e : edges) {
        if (e.source == u && e.dest == v) { //if backedge already exists in the residual graph, increase capacity by bottleneck
            e.cap += b;
            return; // return after the first match is found
        }
    }
    //if backedge already does not exist in the residual graph, push backedge into residual graph with capcity equal to bottleneck
    edge back_edge;
    back_edge.source = u;
    back_edge.dest = v;
    back_edge.cap = b;
    edges.push_back(back_edge);
}

/// @brief Function that performs the Ford Fulkerson algorithm on the input graph
/// @param graph Residual Graph
/// @param source Source node of input graph
/// @param sink Sink node of input graph
/// @return Max flow value of input graph
int ford_fulkerson(vector<edge>& graph, int source, int sink)
{
    vector<edge> residual = graph;
    vector<int> parent(n);
    int max_flow = 0;


    while (augment_path(residual, source, sink, parent)) {
        int b = bottleneck(residual, source, sink, parent);
        max_flow += b;

        int v = sink;
        while (v != source) {
            int i = parent[v];
            residual[i].cap -= b;
            int u = v;
            v = residual[i].source;

            //Add backward edge
            add_backedge(residual, u, v, b);

        }
    }

    return max_flow;
}

/// @brief Function that performs bipartite matching on a given graph using Ford Fulkerson algorithm
/// @param graph Adjacency list of the input graph
/// @param n Number of vertices in the graph
/// @param m Number of edges in the graph
/// @param a Vertices in partition A
/// @param b Vertices in partition B
/// @return Maximum number of matchings in the bipartite graph
int bipartite_matching(vector<vector<int>>& graph, int n, int m, vector<int>& a, vector<int>& b) 
{
    vector<edge> residual;
    for (int i = 0; i < m; i++) {
        edge e1;
        e1.source = graph[i][0];
        e1.dest = graph[i][1];
        e1.cap = 1;
        residual.push_back(e1);
    }

    int source = n + 1;
    int sink = n + 2;

    // Add edges from source to vertices in partition A with infinite capacity
    for (int i = 0; i < a.size(); i++) {
        edge e1;
        e1.source = source;
        e1.dest = a[i];
        e1.cap = 1;
        residual.push_back(e1);
    }

    // Add edges from vertices in partition B to sink with infinite capacity
    for (int i = 0; i < b.size(); i++) {
        edge e1;
        e1.source = b[i];
        e1.dest = sink;
        e1.cap = 1;
        residual.push_back(e1);
    }
    return ford_fulkerson(residual, source, sink);
}


/// @brief Main function that takes in the input graph and performs maximum bipartite matching on it
/// @return Max flow of Input graph
int main()
{
    auto start = high_resolution_clock::now();

    ifstream inputFile("input.txt");
    vector<vector<int>> matr;
    vector<int> l,r;
    //cout<<"Enter the number of nodes:"<<endl;
    inputFile>>n>>e;
    //cout<<"Enter the number of edges:"<<endl;
    //cin>>e;
    for(int i=0;i<e;i++)
    {
        //cout<<"Enter Edge "<<i<<endl;
        int x,y;
        inputFile>>x>>y;
        vector<int> z;
        z.push_back(x);
        z.push_back(y);
        matr.push_back(z);
    }
    int le,ri;
    //cout<<"Enter the number of nodes in left and right side: "<<endl;
    inputFile>>le>>ri;
    for(int i=0;i<le;i++)
    {
        int o;
        inputFile>>o;
        l.push_back(o);
    }
    for(int i=0;i<ri;i++)
    {
        int o;
        inputFile>>o;
        r.push_back(o);
    }
    ofstream outFile("output.txt");
    int numOfMatches = bipartite_matching(matr,n,e,l,r);
    outFile<<"Number of Matches: "<<numOfMatches<<endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    outFile << "Time: " << duration.count() << " microseconds" << endl;

    return 0;
}
