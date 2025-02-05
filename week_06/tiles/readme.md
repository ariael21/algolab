## Tiles

### Problem Description

We need to calculate the **maximum number** of `1x2` or `2x1` tiles that can be placed on a given grid. The grid may contain **holes**, which cannot be covered by tiles.

### Solution Approach

1. **Graph Representation**:
   - Each tile position is represented as a **node**.
   - Each node is connected to its **valid neighboring nodes** (top, right, bottom, left), excluding holes.

2. **Maximum Matching**:
   - Compute the **maximum matching** on this graph.
   - The **size of the maximum matching** gives the **maximum number of tiles** that can be placed.

### Result

The final result is the size of the **maximum matching** in the constructed graph.