## Ceryneian Hind

We are given `n` locations with convenience scores `c_i` for location `i`. Our task is to find semi-dead ends (a set of non-empty nodes from which it is not possible to reach any other nodes than the ones in the set) that have a positive sum of all the convenience scores of the nodes within them.

### Solution Approach

After a failed attempt to create semi-dead end components, I realized that the problem can be solved using `Max Flow`. The hard part is figuring out how to model the problem and why it works.

We create a flow graph:

- **Edges** between nodes within the graph with **capacity:** `inf`
- Connect the **source** to nodes `i` that have a positive convenience score `c_i` with **capacity:** `c_i`
- Connect nodes `i` that have a negative convenience score `c_i` to the **sink** with **capacity:** `-c_i`

Now, we let it flow... what happens?

- Any semi-dead ends with a *negative* convenience score will have the **same in-flow as out-flow**.
- Any semi-dead ends with a *positive* convenience score will have **more in-flow** than out-flow.

Thus, the maximum semi-dead end convenience score is `in-flow - actual_flow`. If this value is `> 0`, we can capture the beast!
