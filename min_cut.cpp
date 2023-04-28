#include <iostream>
#include <vector>
#include <queue> 
#include <limits>
#include <limits.h> 
#include <algorithm>
#include <fstream>
#include <string>

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

/// @brief Function to return the minimum cuts made and print the partition sets formed
/// @param graph It is the final residual graph we receive when the maximum flow is found
/// @param source Source node of the entire graph
/// @param sink Sink node of the entire graph
/// @return Vector of edges that form the minimum cut
vector<edge> min_cut(vector<edge> graph, int source, int sink)
{
    vector<bool> reachable(n, false);
    vector<bool> visited(n, false);
    queue<int> q;

    // BFS to mark all vertices reachable from source
    reachable[source] = true;
    visited[source] = true;
    q.push(source);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < graph.size(); i++) {
            edge e = graph[i];
            if (e.source == u && !visited[e.dest] && e.cap > 0) {
                reachable[e.dest] = true;
                visited[e.dest] = true;
                q.push(e.dest);
            }
        }
    }

    // Traverse the graph and return the sum of capacities of edges crossing the cut
    vector<edge> cut;
    vector<int> a,b;
    for (int i = 0; i < graph.size(); i++) 
    {
        edge e = graph[i];
        if (reachable[e.source] && !reachable[e.dest]) 
            cut.push_back(e);
        if(visited[e.source])
        {
            int flag=0;
            for(int j=0;j<a.size();j++)
            {
                if(a[j]==e.source)
                {
                    flag=1;
                    break;
                }
            }
            if(flag==0)
                a.push_back(e.source);
        }
        else
        {
            int flag=0;
            for(int j=0;j<b.size();j++)
            {
                if(b[j]==e.source)
                {
                    flag=1;
                    break;
                }
            }
            if(flag==0)
                b.push_back(e.source);
        }
    }
    b.push_back(sink);
    cout<<"Two partition sets are:"<<endl;
    for(int i=0;i<a.size();i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<endl;
    for(int i=0;i<b.size();i++)
    {
        cout<<b[i]<<" ";
    }
    return cut;
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
    vector<edge> mincut=min_cut(residual,source,sink);
    cout<<"The cut edges are:"<<endl;
    for(int j=0;j<mincut.size();j++)
        cout<<mincut[j].source<<" - "<<mincut[j].dest<<endl;
    return max_flow;
}

/// @brief Main function that takes in the input graph and performs ford fulkerson algorithm on it
/// @return Max flow of Input graph
int main()
{
    int s, t;

   
    vector<edge> input;

    /*
    ifstream inputFile("input.txt");
    inputFile >> n >> e;
    inputFile >> s >> t;
*/
    cout<<"Enter n"<<endl;
    cin>>n;
    cout<<"Enter e"<<endl;
    cin>>e;
    cout<<"Enter s and t"<<endl;
    cin>>s>>t;
    
    for (int i = 0; i < e; i++) {
        edge in;
        cout<<"Enter the source, destination and capacity for "<<i<<endl;
        
        cin >> in.source >> in.dest >> in.cap;
        input.push_back(in);

    }

    int max_flow = ford_fulkerson(input, s, t);
    cout << "Max flow: " << max_flow << endl;

    return 0;
}
