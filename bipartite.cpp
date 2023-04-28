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
    cout<<endl;
    return cut;
}

/// @brief Function that performs the Ford Fulkerson algorithm on the input graph
/// @param graph Residual Graph
/// @param source Source node of input graph
/// @param sink Sink node of input graph
/// @return Max flow value of input graph
vector<edge> ford_fulkerson(vector<edge>& graph, int source, int sink)
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
    cout<<"Max flow:"<<max_flow<<endl;
    return mincut;
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
        edge e1, e2;
        e1.source = graph[i][0];
        e1.dest = graph[i][1];
        e1.cap = 1;
        //e2.source = graph[i][1] - 1;
        //e2.dest = graph[i][0] - 1;
        //e2.cap = 0;
        residual.push_back(e1);
        //residual.push_back(e2);
    }

    int source = n;
    int sink = n + 1;

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
    cout << "Start FF" << endl;
    vector<edge> mincut=ford_fulkerson(residual, source, sink);
    cout << "End FF" << endl;
    return 0;
}


/// @brief Main function that takes in the input graph and performs ford fulkerson algorithm on it
/// @return Max flow of Input graph
int main()
{
    vector<vector<int>> matr;
    vector<int> l,r;
    cout<<"Enter the number of nodes:"<<endl;
    cin>>n;
    cout<<"Enter the number of edges:"<<endl;
    cin>>e;
    for(int i=0;i<e;i++)
    {
        cout<<"Enter Edge "<<i<<endl;
        int x,y;
        cin>>x>>y;
        vector<int> z;
        z.push_back(x);
        z.push_back(y);
        matr.push_back(z);
    }
    int le,ri;
    cout<<"Enter the number of nodes in left and right side: "<<endl;
    cin>>le>>ri;
    for(int i=0;i<le;i++)
    {
        int o;
        cin>>o;
        l.push_back(o);
    }
    for(int i=0;i<ri;i++)
    {
        int o;
        cin>>o;
        r.push_back(o);
    }
    bipartite_matching(matr,n,e,l,r);
    return 0;
}