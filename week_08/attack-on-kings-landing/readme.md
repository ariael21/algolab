## Attack on King's Landing

### Problem

We need to make as many roads **safe from fire** as possible. A road is **safe** if at each end node there is a **barricade**.

- We can only build barricades at nodes **within a distance `d`** of a barrack (any intersection can be a barrack).
- There are two types of nodes:
  - **Regular intersections** → Can have **one** barricade (seal **one** road).
  - **Plazas** → Can have **two** barricades (seal **two** roads).

### Solution Outline

1. **Graph Construction**
   - Nodes: Regular intersections and plazas.
   - **Duplicate** plaza nodes (since they can seal **two** roads).

2. **Prune Unreachable Nodes**
   - Run **Dijkstra** from each **barrack** to determine which nodes are **reachable** within distance `d`.
   - Remove **unreachable** nodes from the graph.

3. **Compute Maximum Matching**
   - The maximum matching in the final graph gives the number of streets that can be **sealed off**.

### Note

To delete unreachable nodes from a graph:

```c++
// Clear unreachable vertices
for (int i = 0; i < num_vertices; i++) {
    if (!reachable[i]) {
        boost::clear_vertex(i, g);
    }
}
