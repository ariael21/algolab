## Idefix

We are given the coordinates and radius of `n` disc-shaped trees (the disc represents the shadow of the tree). Additionally, we are given `m` coordinates for bones. We also receive the value `s = 4 * r^2` and `k`.

Our task is to collect bones. A set of bones can only be collected if they are within an area overlapped by trees. The area must not be disconnected, meaning all bones must be reachable within one contiguous region of tree shadows.

The task is to calculate two values:
1. `a`: The largest number of bones that can be collected if all trees have radius `r`.
2. `b`: The smallest radius (in the form `b = 4 * r^2`) such that at least `k` bones can be collected.

### Solution Idea

The solution is based on analyzing the tree-tree and bone-tree distances. We incrementally increase the radius of the trees and observe which trees merge into unions, and how many bones fall within these union areas.

To determine the appropriate steps for increasing the radius, we utilize:
- **Tree-tree distances:** These are the edges in the Delaunay Triangulation.
- **Bone-tree distances:** These are calculated using the `nearest_neighbor()` in the Delaunay Triangulation.

### Note

Make sure that you use the correct data types.

### Test Results

```
Test set 1        (20 pts / 1 s) : Correct answer      (0.016s)
Hidden test set 1 (05 pts / 1 s) : Correct answer      (0.016s)
Test set 2        (20 pts / 1 s) : Correct answer      (0.082s)
Hidden test set 2 (05 pts / 1 s) : Correct answer      (0.082s)
Test set 3        (20 pts / 1 s) : Correct answer      (0.026s)
Hidden test set 3 (05 pts / 1 s) : Correct answer      (0.026s)
Test set 4        (20 pts / 1 s) : Correct answer      (0.254s)
Hidden test set 4 (05 pts / 1 s) : Correct answer      (0.124s)
```