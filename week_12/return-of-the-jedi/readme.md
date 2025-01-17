## Return of the Jedi

### Problem Description

The task describes that Princess Leia computes a Minimum Spanning Tree (MST). Our task is to calculate another spanning tree, which differs from Leia's (or any other) MST in **one edge**.


### Approach

#### Naive Approach

One straightforward approach is as follows:
1. Calculate the MST cost.
2. For each of the `n-1` edges in the MST:
   - Remove that edge from the fully connected, initial graph.
   - Recompute another MST (let's call it MST2).
3. Compare the cost of the initial MST with the cost of the newly calculated MST2.
4. Output the MST2 with the minimal cost.

While this approach works, it is computationally too slow. How can we improve it?


#### Optimized Approach

1. **Calculate the MST cost:** Compute the cost of the initial MST, `mst_cost`.

2. **Determine the maximum weighted edge between any two nodes in the MST:**  
   For all pairs of nodes `u` and `v` in the MST, use Depth-First Search (DFS) to calculate the maximal weighted edge on the path between `u` and `v`: `max_weight(u, v)`.

3. **Iterate through all edges not in the MST:**  
   For each edge `e(u, v)` in the fully connected, initial graph that is **not** part of the MST:
   - Calculate the difference between `max_weight(u, v)` and the weight of the current edge `e(u, v)`.
   - Track the minimum of all these differences: `min_difference`.

4. **Output the result:**  
   The result is given by `mst_cost + min_difference`.

### Runtime

```
Test set 1 (40 pts / 2 s) : Correct answer      (0.013s)
Test set 2 (40 pts / 2 s) : Correct answer      (0.902s)
Test set 3 (20 pts / 2 s) : Correct answer      (1.54s)
```