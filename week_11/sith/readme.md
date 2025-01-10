## Sith
Note: only 75% passing (!) I couldn't / didn't have time to find out what the issue is... but its pretty fast, i guess?

### Approach

Find the biggest connected component: Use Delaunay Triangulation + Union Find datastructure

Speedup Note: Do not recompute the whole triangulation but add one point at a time and check the adjacent edges of that vertex in the triangulation. Add the newly added vertex to a union, if the edges are short enough


### Runtime

```
Test set 1 (25 pts / 2 s) : Correct answer      (0.015s)
Test set 2 (25 pts / 2 s) : Correct answer      (0.039s)
Test set 3 (25 pts / 2 s) : Correct answer      (0.124s)
Test set 4 (25 pts / 2 s) : Wrong answer        (0.287s)
```
