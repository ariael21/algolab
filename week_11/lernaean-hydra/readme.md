## Lernaean Hydra

We need to cut `n` heads in order: `0, ..., n-1`. A head `i` is cut if we follow one of the `m` cutting patterns. Once head `i` has been cut, it cannot be cut again in later cutting patterns.

### Solution Approach

- **Grouping Patterns**: Group the patterns into `n` groups based on the head `i` that is cut after the pattern is executed (the last head in the pattern).

  **Why?** We must first execute a pattern that eliminates head `0`, then head `1`, and so on until we reach head `n-1`. This grouping minimizes the number of possible patterns at each step.

- **Pattern Compacting**: Push patterns together as much as possible to minimize cuts. Two patterns can be merged if they overlap.

  For example, given patterns `000` and `021`, executing these sequentially (`000 021`) can be compacted into a single cutting pattern: `00021`.

Using these ideas, we can create a graph with `m+2` nodes and get the minimum cuts by calculating the `shortest path` from `start` to `end`:

- **Nodes**:
  - Each of the `m` patterns is represented as a node.
  - Start node.
  - End node.

- **Layers**:
  The graph consists of `n+2` layers:
  - Start node.
  - Nodes of patterns in group `0` (patterns that cut head `0`).
  - Nodes of patterns in group `1` (patterns that cut head `1`).
  - ...
  - Nodes of patterns in group `n-1` (patterns that cut head `n-1`).
  - End node.

- **Connections**:
  - Connect the start node to all nodes in group `0` with weight `0`.
  - Connect all nodes in group `n-1` to the end node with weight `k`.
  - Connect nodes in group `i` to nodes in group `i+1` if they form a valid cutting pattern:
    - If there is an overlap, the weight is `k - overlap`.
    - Otherwise, the weight is `k`.

- **Validation**:
  To check if two cutting patterns form a valid connection:
  - TODO