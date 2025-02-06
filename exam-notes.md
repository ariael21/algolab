## Exam Notes

### General

- **Check Types**! (Including function return types)

Some strategies that may help or are worth trying:
- Add **randomness**.
- **Sort** the data.
- Use **binary search**.
- Find an **easy base case** (using assumptions).
- Solve it using the **naive approach** first.
- Check if it's a **special graph** (e.g., tree, bipartite, connected, etc.).

**Calculations**:
- **Avoid roots** (very expensive); use squared results instead.
- Constructions are expensive—only use them when necessary.
- Beware of **rounding errors**.

---

### Linear Programming (LP)

- Check **lower and upper bounds**.
- Convert maximization problems to minimization problems using negation.
- To display the solution:
  - Use `long(CGAL::to_double(s.objective_value()))` or `long(CGAL::to_double(-s.objective_value()))` (with negation).
- Check the input type:
  ```cpp
    typedef CGAL::Gmpq IT;
    ```
### Dynamic Programming (DP)

- **Sliding window** techniques.
- Often involves **overlapping subproblems** (recalculating the same thing multiple times → use **memoization**).
- Reduce the problem into subproblems (often using recursion + memoization → top-down approach).

**Approaches**:
1. **Top-down**:
   - With memoization.
   - Easier to implement but harder to identify time complexity and may have runtime overhead.
2. **Bottom-up**:
   - Iterative approach.
   - Harder to code and define relations but has obvious time complexity and less overhead.

**Steps**:
- **Divide the problem into subproblems** (what is the function we are trying to compute?).
- Define the subproblem:
  - What is the **state**?
  - What do we need to compute?

---

### Greedy Algorithms

- Works when **locally optimal = globally optimal**.
- **Split and list**: (To be defined further).

---

### Graphs

- Graph Stacking -> the levels represent a certain variable
- Node Duplication to represecent a certain property

---

### Flow

**Techniques**:
- **Multiple sources**: Add several fake sources and connect them to the actual sources with edges of infinite capacity.
- **Add capacity to a vertex**: Split the vertex into two:
  - One with all incoming edges.
  - One with all outgoing edges.
  - Connect these two with an edge of the desired vertex capacity.
- **Undirected graph**: Add edges in both directions with the same capacity.
- **Enforce minimum flow (`f_min`) between `u` and `v`**:
  - Add an intermediate node `w`: `u → w → v`.
  - Add an edge from the source to `w` with capacity `f_min`.
  - Reduce flow from the source to `u` by `f_min`.

**Ideas**:
- Increase flow one by one using a gauge.
- Scale all edges to make all capacities positive.

---

### Delaunay Triangulation

- Contains all **shortest distances** to neighbors.
- Contains the **minimum spanning tree (MST)**.
- Is the **dual to Voronoi diagrams**.
- Vertices can be enhanced with additional information.

- **If multiple triangulations are required**:
  - Can we just add the nodes, or do we need to recompute the entire triangulation?

---

### Max-Flow Min-Cut

- Negative edges → Use **cycle-canceling algorithms** or make all edges positive if possible.

---

### Additional Techniques

- **BFS / DFS**
- **Dijkstra’s Shortest Path**
- **Union-Find**
- **Connected Components**
- **Minimum Spanning Tree (MST)**
- **Binary Search**
- **Maximum Matching**
- **Graph Coloring**

