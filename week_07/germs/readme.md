## Germs

We are given a dish with four boundaries (x-axis: left and right, y-axis: top and bottom). On the dish, `n` bacteria are placed, each of whose radius grows quadratically over time: `radius(t) = t² + 0.5`

A bacterium dies as soon as it touches another bacterium or the edge of the dish. To simplify the problem, dead bacteria continue to grow at the same rate as living bacteria.

### Task

Our task is to calculate the following:
1. The time of death of the first bacterium.
2. The time when 50% of the bacteria have died.
3. The time when all bacteria have died.

### Naive Approach

A naive approach would involve checking at each time step `t` whether any of the bacteria's discs (with radius `radius(t)`) overlap with other bacteria or the boundaries of the dish. However, this approach is computationally expensive and inefficient.

### Optimized Algorithm

To achieve a faster algorithm, we use **Delaunay Triangulation** to observe the edge lengths between the bacteria. The key idea is that bacteria connected by smaller edges are likely to die earlier. By sorting these edge lengths, we can iterate through them and determine the time at which bacteria die due to collisions.

However, this does not account for bacteria dying when they touch the dish boundaries. To handle this, we calculate the smallest distance from each bacterium to the four boundaries (left, right, top, bottom). Using a similar approach, we determine the times at which bacteria die due to contact with the dish edges.

This combined strategy allows us to efficiently compute the required death times for all bacteria.
