## Alastor Moody

### Problem

We need to determine the maximum number of people who can fly through a network and arrive at the target location **at the exact same time as each other**. All must reach the target location as quickly as possible.  

The network is represented by edges with a capacity `c` and a duration `d`. We are given a start location `s` and a target location `p`.


### Solution Idea

1. Calculate the flow cost for one person to travel through the network: `flow_cost_1`.

2. Incrementally increase the number of people flying through the network: `i = 1, 2, ...` and verify whether `flow_cost_i = flow_cost_1 * i`.  
   - This ensures that all individuals can fly through the network at minimal cost and arrive at the target location at the same time.

3. If we reach a point where `flow_cost_i > flow_cost_1 * i`, it indicates that the `i`-th person had to use a more expensive route to reach the target, arriving **later** at `p` than the others.

4. The maximum number of people who can arrive simultaneously is therefore `i - 1`.


### Modeling the Network

- Represent the network with the given locations and add undirected edges between vertices `u` and `v` with capacity `c` and cost `d`.  
- Add an additional edge from the `source` to `s` with capacity `1` and cost `0`.  
  - Increment this capacity one by one to compute the values for `flow_cost_i`.


### Make it fast

Calculating the flow cost for every `i = 1, ...` is **very slow**. To speed this up:
- Use **binary search** over the values of `i` up to `max_flow`.  
- Here, `max_flow` represents the maximum amount of flow that can pass from `s` to `p`.

