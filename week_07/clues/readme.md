## Clues

### Solution Approach

We need to determine the following:

1) Can the stations be configured using two frequencies? **(Is the graph `G` 2-colorable / bipartite?)**
2) Can Holmes and Watson communicate?
   - **(1) Direct communication**: Holmes and Watson are within reach of each other.
   - **(2) Indirect communication**: The stations they connect to are connected to each other, forming a path from Holmes to Watson using stations that are within reach of each other.


### Is the graph G 2-colorable / bipartite? → Can we configure the stations using two frequencies?

#### **Inefficient approach**
A naive way to check this is to compute the distances between all pairs of stations and connect those within reach (`O(V^2)`). On this graph, we run `boost::is_bipartite(connected_stations_graph)` (`O(V + E)`) to check for bipartiteness. While correct, this approach is much too slow.

#### **Efficient approach**
A more optimized method involves the following steps:

1. **Create a Delaunay Triangulation of the stations.**
2. **Perform a DFS (or BFS) traversal** of the graph, considering only edges that are smaller than or equal to the communication range of the stations. While traversing, we attempt to **color the graph with two colors**.
3. If this fails, the graph is **not bipartite**, and we can stop here.
4. If successful, we split the vertices into two groups based on their color.
5. **Create two separate Delaunay Triangulations** (one for each color group).
6. **Check if both triangulations are bipartite** (again, only considering edges within the communication range). If both are bipartite, then the stations can be configured using two frequencies.


### Can Holmes and Watson communicate?

1) **Direct communication check**: If Holmes is directly within reach of Watson, we are done.
2) **Indirect communication check**:
   - Create a **Delaunay Triangulation**.
   - Using these edges, compute the **connected components** with the **union-find data structure**.
   - Two stations belong to the same connected component if the edge connecting them is **within the communication range**.

If Holmes and Watson connect to stations that are in the same connected component, they can communicate.
