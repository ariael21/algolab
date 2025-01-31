## Asterix and the Tour of Gaul

This is a really tricky problem. For one its hard to see that this is a flow problem and its is even harder to see how to model the graph if the `c_i` values may differ.

### Solution Approach (Both 80%)

We construct the graph as follows:

For all capacities `c_i` (connect node `i` to `i+1`)
- add `edge(source, i, c_i, 0)`
- add `edge(i+1, sink, c_i, 0)`
- add `edge(i, i+1, c_i, 100)`

For all food items `a b d`:
- add `edge(a, b, 1, (b - a) * 100 - d)`

=> result = flow * 100 - cost

-----------------

Alternate graph:

For all capacities `c_i` (connect node `i` to `i+1`)
- add `edge(i, i+1, c_i, 100)`

For all food items `a b d`:
- add `edge(a, b, 1, (b - a) * 100 - d)`

- add `edge(source, 0, c_0, 0)`
- add `edge(n-1, sink, c_{n-2}, 0)`

=> result = (c_0 + ... c_n-2) * 100 - cost