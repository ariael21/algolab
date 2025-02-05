## Tracking

### Core Idea: Graph Stacking

To model the traversal through a river, we create **duplicates** of the base graph and **stack** them.

- Each time we pass through a **river**, we go **down a level** in the graph.
- The only edges connecting different levels of the graph are **rivers**.

### Solution Approach

- Construct multiple **stacked layers** of the graph.
- Ensure that **river edges** connect nodes across different layers.
- **Compute the shortest path** from `y` to `x` on level `k` to simulate traversing a river **k times**.
