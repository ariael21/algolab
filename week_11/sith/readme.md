## Sith

### Approach

Find the biggest connected component: Use Delaunay Triangulation + Union Find datastructure

Speedup Note: Do not recompute the whole triangulation but add one point at a time and check the adjacent edges of that vertex in the triangulation. Add the newly added vertex to a union, if the edges are short enough


### Useful Stuff

Manually insert one point into a delauny triangulation:

```c++
Vertex_handle v = t.insert(K::Point_2(x, y));
v->info() = i; // assign vertex id (or other information)
```

Accessing all incident edges of a vertex in a delauny triangulation:

```c++
Edge_circulator edge = t.incident_edges(v);
Edge_circulator start = edge; // store a reference to the start

do {
    // do something with the incident edges...
    
    edge++; // go to the next one
} while (start != edge);
```

### Runtime

```
Test set 1 (25 pts / 2 s) : Correct answer      (0.015s)
Test set 2 (25 pts / 2 s) : Correct answer      (0.038s)
Test set 3 (25 pts / 2 s) : Correct answer      (0.124s)
Test set 4 (25 pts / 2 s) : Correct answer      (0.286s)
```
