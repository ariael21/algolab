## Algocoon

We are given a sculpture containing `n` individual figures and `m` limbs. The figures are connected to one another using several limbs. Our task is to cut the sculpture into two disjoint sets of figures. Each set must be non-empty, and the split should minimize the cutting cost.

At first glance, this problem may seem like a simple min-cut problem. However, the challenge lies in finding the minimum cutting costs between all possible non-empty, disjoint sets. To achieve this, we need to calculate the flow (both forward and backward, as flows are not symmetric) between the source (0) and all other vertices (≠0) in the graph.
