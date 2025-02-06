## Sith

### Approach

Find the biggest connected component: Use Delaunay Triangulation + Union Find datastructure

Speedup Note: Do not recompute the whole triangulation but add one point at a time and check the adjacent edges of that vertex in the triangulation. Add the newly added vertex to a union, if the edges are short enough


### Runtime

```
Test set 1 (25 pts / 2 s) : Correct answer      (0.015s)
Test set 2 (25 pts / 2 s) : Correct answer      (0.038s)
Test set 3 (25 pts / 2 s) : Correct answer      (0.124s)
Test set 4 (25 pts / 2 s) : Correct answer      (0.286s)
```
