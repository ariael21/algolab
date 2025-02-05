## Ant Challenge

### Problem Description

- Each insect type has its own **Minimum Spanning Tree (MST)** in the forest.
- The task is to **calculate the MST for each insect type** using **Kruskal’s algorithm**.
- After constructing the individual MSTs, combine them by **choosing the lowest-cost edge** to connect any two nodes.
- Finally, use **Dijkstra’s algorithm** to find the cheapest path from `a` to `b`.

### Solution Approach

1. **Compute the MST for each insect type** using **Kruskal’s algorithm**.
2. **Merge the MSTs** by selecting the **minimum-cost edge** to connect separate components.
3. **Find the shortest path** between nodes `a` and `b` using **Dijkstra’s algorithm**.

### Notes

To check if an edge is in a graph: `!boost::edge(u, v, tree).second`

To get the reference to an edge to update weightmap:

```c++
 Edge old_edge = boost::edge(u, v, tree).first;
        
// add smallest edge
weightmap[old_edge] = std::min(weightmap[old_edge], weightmaps[i][e]);
```