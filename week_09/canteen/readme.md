## Canteen

We are given the following inputs:
- `n`: Number of days to plan ahead.
- `a_i`: Number of menus producible on day `i`.
- `c_i`: Cost of producing a menu on day `i`.
- `s_i`: Number of students wanting to eat on day `i`.
- `p_i`: Price of a menu on day `i`.
- `v_i`: Freezer capacity for transferring food from day `i` to day `i+1`.
- `e_i`: Cost of freezing and storing food.

The objective is to determine:
1. Whether all students can be served over the `n` days: possible / impossible.
2. The total number of students served over the `n` days.
3. The revenue or loss incurred over the `n` days.

### Solution Approach

We model the problem as a **max-flow, min-cost** optimization. The flow represents the number of students served, and the costs include menu production costs, freezer costs, and menu prices. 

To solve the problem, we construct a flow network with `n + 2` nodes:
- A **source** node.
- A **sink** node.
- One node for each of the `n` days.

The edges and capacities are defined as follows:
1. **Source to Day `i` Node:**  
   - Capacity: `a_i`  
   - Cost: `c_i`  
2. **Day `i` Node to Sink:**  
   - Capacity: `s_i`  
   - Cost: `-p_i + 20`  
   *(Note: We add `+20` to all prices to ensure no negative costs, allowing us to use `successive_shortest_path_nonnegative_weights`.)*  
3. **Day `i` Node to Day `i+1` Node:**  
   - Capacity: `v_i`  
   - Cost: `e_i`  

