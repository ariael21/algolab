## Ludo Bagman

### Solution Approach

This is a `max-flow min-cut` problem. 

- The tricky part is modeling the constraint that a minimum flow of `l` must pass through the graph for all teams. 

See image below of how the graph is constructed:
![Graph Example](/week_13/ludo-bagman/ludo-bagman-graph.jpeg)


If the flow is equal to `p`, we know we have an optimal solution. Otherwise we cannot solve the problem.