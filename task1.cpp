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

/*void add_edges(vector<edge> res,edge bw)
{
    res.push_back()
}*/

bool augment_path( vector<edge> residual, int source, int sink)
{
    vector<bool> visited(n, false);
    queue<int> q;
    vector<int> parent(n, -1);

    visited[source] = true;
    q.push(source);
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && residual[v].cap > 0) {
                visited[v] = true;
                q.push(v);
                parent[v] = u;
                if (v == sink) {
                    return true; // Found an augmented path
                }
            }
        }
    }

    return false; // No augmented path found
}

int bottleneck(vector<edge> residual, int source, int sink, vector<int>& parent) 
{
    int b = INT_MAX;  // initialize the bottleneck to a large value
    int v = sink;  // start from the sink node
    while (v != source) {  // traverse the path from sink to source
        int u = parent[v];
        int index;

        for (int i = 0; i < residual.size(); i++) 
        {
            if(residual[i].source== u && residual[i].dest==v)
            {
                index=i;
                break;
            }

        }

        b = min(b, residual[index].cap);  // update bottleneck
        v = u;
    }
    return b;
}

int ford_fulkerson(vector<edge> residual, int source, int sink)
{
    int max_flow =0;

    int v = sink;  
    while (v != source)
    {
        vector<int> parent(n, -1);
        int u = parent[v];
        bool has_augmented_path = augment_path(residual, source, sink);
        if (has_augmented_path)
        {
            int bottle = bottleneck(residual, source, sink, parent);

            max_flow = max_flow + bottle;

            for (int i = 0; i < residual.size(); i++) 
            {
                if(residual[i].source== u && residual[i].dest==v)
                {
                    residual[i].cap = residual[i].cap - bottle;
                
                    edge backedge;
                    backedge.source = v;
                    backedge.dest = u;
                    backedge.cap = bottle;

                    residual.push_back(backedge);

                }

            }


        }
    }

    return max_flow;

}


int main()
{
    int s, t, u, v, c;
   
    vector<edge> input;
    vector<edge> residual_graph;
    cout<< "Enter number of nodes and edges";
    cin >> n >> e;

    cout<< "Enter the index of the start and sink nodes";

    cin >> s >> t;

    for (int i = 0; i < e; i++) 
    {
        cout<< "Enter the indices of the nodes of the edges along with it's capacity";
        cin>> a >> b >> c;

        edge in;
        in.source = a;
        in.dest = b;
        in.cap = c;

        input.push_back(in);
        
    }

    ford_fulkerson(input, s, t);

}




