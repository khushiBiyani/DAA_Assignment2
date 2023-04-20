#include <iostream>
#include <vector>
#include <queue> 
#include <limits>
#include <limits.h> 
#include <algorithm>

using namespace std;

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
            v = residual[i].source;

            //Add backward edge
            edge back_edge;
            back_edge.source = residual[i].dest;
            back_edge.dest = residual[i].source;
            back_edge.cap = b;
            residual.push_back(back_edge);
        }
    }

    return max_flow;
}

int main()
{
    int s, t;
   
    vector<edge> input;

    cout<< "Enter number of nodes and edges: ";
    cin >> n >> e;

    cout<< "Enter the index of the start and sink nodes: ";
    cin >> s >> t;

    for (int i = 0; i < e; i++) {
        edge in;
        cout << "Enter values for element " << i+1 << ": ";
        cin >> in.source >> in.dest >> in.cap;
        input.push_back(in);


    }

    int max_flow = ford_fulkerson(input, s, t);
    cout << "Max flow: " << max_flow << endl;

    return 0;
}




