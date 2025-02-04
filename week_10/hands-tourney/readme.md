## Hands Tourney

We need to calculate two things:

- `s` = the minimum squared distance such that there exists an `(f_0, s)`-reasonable assignment.
- `f` = the maximum number of families such that there exists an `(f, s_0)`-reasonable assignment.

An assignment is `(f, s)`-reasonable if:
- Every tent is assigned to exactly one of `f` families.
- Each of the `f` families has at least `k` assigned tents (`k` is given, `k ∈ {1, 2, 3, 4}`).
- Whenever two tents are assigned to different families, they are at least `s` squared Euclidean distance apart.

### Solution Approach

1. Create a **Delaunay Triangulation** of all the tents.
2. Sort the edges in the triangulation by length.
3. Process the edges one by one, connecting tents by grouping them into connected components.

While doing this, keep track of:
- The **number of components** and their **size** (`1, 2, 3, 4+`).
- During each step:
  - If `f_0 == num_of_families_of_size_k_or_bigger`, store the largest squared distance `s` for which this holds.
  - If `s_0 <= current_squared_distance`, store the maximum number of families of size `k+` as `f`.

`num_of_families_of_size_k_or_bigger` is determined by **combining** component counts and their sizes.  
For example, if `k = 4` (we need clusters of size 4+), we count:
- The number of clusters of size 4+.
- The number of smaller clusters that can be combined to reach size 4+.
