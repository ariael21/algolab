## Hiking Map

We need to purchase the cheapest (conencted) sequence of triangular maps (`t_b, ..., t_e`), which completely cover a given path `p_0, ..., p_{m-1}`. Each leg (one edge with endpoint vertices) of the path needs to be fully covered by one map.

### Solution Approach

- Determine which path sections (legs) are covered by which map: check if both endpoints are inside the map.
- Use the sliding-window approach to "slide" over the map sequences and check if they fully cover all legs.
- Select the cheapest such sequence.

### How to check if a point is within a map?

The maps are defined using six points, which is not the usual way. Thus, we need to check if our points lie within the triangle. To do this, we verify whether the point lies on one side of the edges. For this, we can use `CGAL::right_turn()`.

Before performing this check, we need to order the points of the triangle appropriately to ensure correctness.

### Runtime

```
Test set 1 (20 pts / 0.400 s) : Correct answer      (0.004s)
Test set 2 (20 pts / 0.400 s) : Correct answer      (0.009s)
Test set 3 (20 pts / 0.400 s) : Correct answer      (0.090s)
Test set 4 (20 pts / 0.400 s) : Correct answer      (0.267s)
Test set 5 (20 pts / 0.400 s) : Correct answer      (0.181s)
```