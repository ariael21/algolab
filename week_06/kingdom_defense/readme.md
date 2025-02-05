### Assessment 2: Kingdom Defense

#### Problem Description

We have **l locations**, where:
- At **location i**, **g_i** soldiers are present, and **d_i** soldiers are needed.
- There are **p paths** connecting two locations.
  - Each path has a **minimum** number of soldiers that **must** traverse it.
  - Each path has a **maximum** number of soldiers that **can** traverse it.

**Goal:** Determine if we can move soldiers such that:
- Each location has at least **d_i** soldiers.
- Each path carries **at least the minimum** but **no more than the maximum** number of soldiers.

#### Mathematical Model

We model the problem as a **flow network**:

1. **Nodes:**
   - Each location is a **node** with a **resource value**:
     - **Negative value** → needs troops.
     - **Positive value** → can provide troops to neighbors.

2. **Edges:**
   - If two locations are connected by a path:
     - Subtract the **minimum required troops** from `s` and add it to `t`:
       - `s_res = s_res - min`
       - `t_res = t_res + min`
     - Edge **capacity** = `max - min` (additional troops that can be moved along this edge).

3. **Source & Sink:**
   - Connect **nodes with surplus troops** (`r_i > 0`) from the **source** with **capacity r_i**.
   - Connect **nodes with troop deficits** (`r_i < 0`) to the **sink** with **capacity -r_i**.
   - The total required flow must match the **absolute sum of all deficits**.

#### Algorithm

1. **Read Input**: `O(l + p)`
2. **Construct Flow Graph**: `O(l)`
3. **Compute Max Flow**:
   - Using **Push-Relabel Max-Flow**: `O(V^3)`
   - With `V = l + 2`, complexity becomes `O((l + 2)^3) ≈ O(l^3)`

