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

void min_cut(vector<edge>& residual, int source, int sink, vector<edge>& graph)
{
    vector<bool> visited(n, false);
    queue<int> q;
    visited[source] = true;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int i = 0; i < residual.size(); i++) {
            edge e = residual[i];
            if (e.source == u && !visited[e.dest] && e.cap > 0) {
                visited[e.dest] = true;
                q.push(e.dest);
            }
        }
    }
    vector<int> a,b;
    // Find minimum cut by iterating over all edges in the input graph
    for (int i = 0; i < graph.size(); i++) {
        
        edge e = graph[i];
        // Check if one endpoint is in visited set and the other is not
        if (visited[e.source] && !visited[e.dest]) {
            cout << "Minimum cut: " << e.source << " - " << e.dest << endl;
        }
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
    min_cut(residual,source,sink,graph);
    return max_flow;
}

int max_bipartite_matching(vector<vector<int>>& graph) {
    int left_size = graph.size();
    int right_size = graph[0].size() - left_size;

    vector<edge> edges;
    for (int i = 0; i < left_size; i++) {
        for (int j = left_size; j < left_size + right_size; j++) {
            if (graph[i][j-left_size]) {
                edge e;
                e.source = i;
                e.dest = j;
                e.cap = 1;
                edges.push_back(e);
            }
        }
    }

    // Add source and sink
    int source = left_size + right_size;
    int sink = left_size + right_size + 1;

    // Add edges from source to left partition
    for (int i = 0; i < left_size; i++) {
        edge e;
        e.source = source;
        e.dest = i;
        e.cap = 1;
        edges.push_back(e);
    }

    // Add edges from right partition to sink
    for (int j = left_size; j < left_size + right_size; j++) {
        edge e;
        e.source = j;
        e.dest = sink;
        e.cap = 1;
        edges.push_back(e);
    }

    n = left_size + right_size + 2; // Set the number of vertices

    // Call the Ford-Fulkerson algorithm to find the maximum flow
    return ford_fulkerson(edges, source, sink);
}

/*
int main()
{
    int s, t;
   
    vector<edge> input;

    cout<< "Enter number of nodes and edges: "<<endl;
    cin >> n >> e;

    cout<< "Enter the index of the start and sink nodes: "<<endl;
    cin >> s >> t;

    for (int i = 0; i < e; i++) {
        edge in;
        cout << "Enter source for element " << i+1 << ": ";
        cin >> in.source;
        cout << "Enter dest for element " << i+1 << ": ";
        cin >> in.dest;
        cout << "Enter cap for element " << i+1 << ": ";
        cin >> in.cap;
        cout<<endl;
        input.push_back(in);


    }

    int max_flow = ford_fulkerson(input, s, t);
    cout << "Max flow: " << max_flow << endl;

    return 0;
}
*/

int main() {
    int n, m;
    cout<<"Enter n and m"<<endl;
    cin >> n >> m;
    
    vector<vector<int>> graph(n, vector<int>(m, 0));
    for (int i = 0; i < n; i++) 
    {
        cout<<"Enter elements of "<<i<<endl;
        for (int j = 0; j < m; j++) 
        {
            cin >> graph[i][j];
        }
    }
    int max_matching = max_bipartite_matching(graph);
    cout << max_matching << endl;
}
