## Alastor Moody

### Problem

We need to determine the maximum number of people who can fly through a network and arrive at the target location **at the exact same time as each other**. All must reach the target location as quickly as possible.  

The network is represented by edges with a capacity `c` and a duration `d`. We are given a start location `s` and a target location `p`.

### Solution Idea

1. Calculate the flow cost for one person to travel through the network: `f_1`.

2. Incrementally increase the number of people flying through the network: `i = 1, 2, ...` and verify whether `f_i = f_1 * i`.  (This ensures that all individuals can fly through the network at minimal cost and arrive at the target location at the same time)

3. If we reach a point where `f_i > f_1 * i`, this means that the `i`-th person had to use a more expensive route to reach the target, arriving **later** at `p` than the others.

4. The maximum number of people who can arrive simultaneously is therefore `i - 1`.

