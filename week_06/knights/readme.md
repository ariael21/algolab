## Knights

We solve the problem using **Max-Flow**.

### Solution Approach

1. **Intersections as Nodes**:
   - Each intersection is represented as a **node** with capacity `C`.
   - To enforce capacity constraints, apply the **node duplication trick**:
     - Create an **in-node** (for all incoming edges) and an **out-node** (for all outgoing edges).
     - Connect them with an edge of **capacity `C`** from the in-node to the out-node.

2. **Flow Setup**:
   - Add **in-flow** to intersections where the knights start (from the source).
   - Connect all **exit nodes** to the **sink**.

3. **Max-Flow Calculation**:
   - The **max-flow value** represents the **maximum number of knights that can escape**.
