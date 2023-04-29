import random

# Generate a random connected graph with n vertices and m edges
def generate_random_connected_graph(n, m):
    if m < n-1:
        raise ValueError("Number of edges must be at least n-1 for a connected graph")
    # Create a minimum spanning tree
    graph = [[] for _ in range(n)]
    edges = []
    for i in range(n):
        for j in range(i+1, n):
            edges.append((random.randint(1, 10), i, j))
    edges.sort()

    parents = list(range(n))
    sizes = [1] * n
    def find(u):
        while u != parents[u]:
            parents[u] = parents[parents[u]]
            u = parents[u]
        return u
    def union(u, v):
        u, v = find(u), find(v)
        if u != v:
            if sizes[u] < sizes[v]:
                u, v = v, u
            parents[v] = u
            sizes[u] += sizes[v]

    for w, u, v in edges:
        if find(u) != find(v):
            union(u, v)
            graph[u].append((v, w))
            graph[v].append((u, w))

    # Add random edges until the graph has m edges
    while sum(len(graph[u]) for u in range(n)) // 2 < m:
        u, v = random.sample(range(n), 2)
        if v not in [v for v, _ in graph[u]]:
            w = random.randint(1, 100)
            graph[u].append((v, w))
            graph[v].append((u, w))

    # Check if the graph is connected
    visited = [False] * n
    stack = [0]
    while stack:
        u = stack.pop()
        visited[u] = True
        for v, _ in graph[u]:
            if not visited[v]:
                stack.append(v)
    if not all(visited):
        raise ValueError("Generated graph is not connected")

    return graph

# Write the graph to a file in the format required by the Ford-Fulkerson algorithm
def write_graph_to_file(graph, source, sink, filename):
    if source not in range(len(graph)):
        raise ValueError("Source node {} not in graph".format(source))
    if sink not in range(len(graph)):
        raise ValueError("Sink node {} not in graph".format(sink))
    with open(filename, "w") as f:
        f.write("{} {}\n".format(len(graph), sum(len(graph[u]) for u in range(len(graph)))//2))
        f.write("{} {}\n".format(source, sink))
        for u in range(len(graph)):
            for v, w in graph[u]:
                if u < v:
                    f.write("{} {} {}\n".format(u, v, w))

# Example usage
graph = generate_random_connected_graph(250, 25000)
write_graph_to_file(graph, 0, 249, "input.txt")