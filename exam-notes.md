## Exam Notes

### General

- CHECK **TYPES** ! (also function return types)

some things that may help / worth a try:
- add randomness
- **sort**
- binary search
- **easy base case** (using assumptions)
- solve it using the **naive approach**
- is it a **special graph**? tree, bipartite, connected, .. ? 

Calculations:
- **avoid roots** (very expensive), use squared results
- constructions are expensive: only use when really needed
- beware of **rounding errors**

### LP

- check lower and upper bounds
- turn max. into min. using minus
- `long(CGAL::to_double(s.objective_value()))` or `long(CGAL::to_double(-s.objective_value()))` (with minus) to display solution
- check input type: `typedef CGAL::Gmpq IT;`

### DP

- sliding window

- often overlapping subproblems (we recalculate the same thing several times => memoisation)
- reduce problem into subproblem (often recursion + memoisation: top-down)

- top down (with memoisation) (easier, hard to identify time complexity, runtime overhead)
- bottom up (iterative) (harder to code, harder to define relations, obvious timecomplexity, less overhead)

Approach:
- **divide problem into subproblems** (what is the funciton we try to compute?)
- define the subproblem: what is the **state** and what do we need to compute


### Greedy Algorithms

- when **locally optimal => global optimal** 

- **split and list**: todo

### Flow

Techniques:

multiple sources: add several fake-sources and add an edge from the source to those with inf capacity

add capacity to vertex: split vertex into two:
- one with all incoming edges
- one with all outgoing edges
- connect those two with and edge of the desired vertex capacity

undirected graph: add edges in both direction with same capacity

enforce minimum flow of `f_min` between `u` and `v`: 
- add node between them `w`: `u`->`w`->`v`
- add edge from source to `w` with capacity `f_min`
- reduce flow from source to `u` by `f_min`

Ideas:
- increase flow one by one using gauge
- scale all edges => to make all posivite
- 

### Delauny Triangulation

- contains all shortest distances to neighbors
- contains the MST
- Dual to Voronoi
- enhance vertices with additonal information 

- if we need several triangulations: can we just add the nodes or do we need to recompute the whole things?

### Max-Flow Min-Cut

- negative edges => cycle cancelling algo (or try to find a way to make all edges positive)


### Additional things 

- BFS / DFS
- Dijkstra Shortest Path
- Union Find
- Connected Components
- MST
- Binary Search
- Max Matching
- Graph Coloring


