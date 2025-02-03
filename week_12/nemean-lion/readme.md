## Nemean Lion

### Solution Approach

#### The Easy Parts:

**`a_2`**: To find the number of pairwise distinct admissible sets of size 2 with the smallest distance `d`, we create a Delaunay triangulation and observe the edge lengths. An edge always connects the two closest points in the triangulation. Thus, if we have an edge `e(u, v)`, there exists no other vertex `p` that is closer to `u` or `v`. We order the edges by length and examine the smallest edge. The number of edges with this smallest length in the triangulation is equal to `a_2`.

**`a_3`**: To find the number of pairwise distinct admissible sets of size 3 with the smallest distance `d`, we create a Delaunay triangulation and compute the dual of all faces. The dual of a face is the circumcenter of the triangle, which is equidistant from all three vertices—exactly what we need. We compute the distances from each circumcenter to one of the face’s vertices and sort them. We identify the smallest distance and count how many other faces share this distance: this is equal to `a_3`.

#### The Hard Parts:

**`a_s`**: The number of pairwise distinct admissible sets of size two with a distance `√s` between the points. To determine this, we need a distance interval for each edge. We count all edges where `s` falls within the interval of that edge: this is equal to `a_s`.

**`a_max`**: The maximum number of pairwise distinct admissible sets of size two for `d`, over all distances `d`. We once again need the distance interval for each edge. To compute this efficiently, we iterate through all intervals and count the number of times we enter an interval at `min_d` and exit at `max_d`. Using these counts, we process all distinct distances `d` and track how many intervals we fall into using the current distance:

```c++
std::map<K::FT, int> in_count;
std::map<K::FT, int> out_count;

for (Interval interval : intervals) {
    in_count[interval.first]++;     // Enter one interval at min_d
    out_count[interval.second]++;   // Exit one interval at max_d
}

int in = 0;
for (K::FT d : distinct_d) {
    in += in_count[d];  // Enter in_count[d] intervals at d
    a_max = std::max(a_max, in);  // Track the max intervals we are in
    in -= out_count[d]; // Exit out_count[d] intervals
}

```

### How to compute the distance intervals for the edges:

We need to figure out the interval of the valid distances for a pair of points `u` and `v`.
We must find the points between `u` and `v` such that these points have the same distance `d` to `u` and `v` but all other points `p` need to be further away than `d`. We need to find an lower and upper bound for this distance `d` for all pairs `u` and `v`.

To do this we use the Voronoi Dual. Refer to the image for the calculation of the interval:

![Nemean Lion](/week_12/nemean-lion/1.jpeg)



