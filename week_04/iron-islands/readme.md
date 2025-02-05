## Iron Islands

### Problem Description

We need to calculate the **longest sequence of islands** that require exactly `k` men: `longest_sequence`.

### Solution Approach Outline

#### Step 1: Single Waterway Calculation
- Compute `longest_sequence` for a **single waterway** using a **sliding window approach**.

#### Step 2: Tracking Across Waterways
- Maintain a map (`best_sequence`) to store the **longest sequence** given a specific number of men.
- For each **waterway**, compute **partial sums** starting from Pyke, stopping when `sum >= k`.
- For each computed sum, check for its **complementary sum** in `best_sequence` to see if we can form a **new longer sequence**.
- Update `best_sequence` with the **partial sums** of the current waterway.

From both those steps: select the longest sequence which uses exactly `k` men (or women?).