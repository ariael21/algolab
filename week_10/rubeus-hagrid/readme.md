## Rubeus Hagrid

### Solution Approach

We can use recursion and a greedy approach to solve this problem.  
The tunnels form a tree. At the root of each subtree, we need to calculate three things:

1. `max_earnings`: The maximum amount of earnings (disregarding the money we lose to travel to this root).
2. `used_time`: The time required to travel down and up the entire subtree.
3. `nodes`: The number of nodes in the subtree.

The main question we have to ask ourselves in this task is the following:  
If we are at the root of a subtree that has more than one child subtree, **which subtree should we explore first to maximize our earnings?**  
The answer: The one where we lose fewer coins!

Thus, if we have more than two subtrees, we calculate `{max_earnings, used_time, nodes}` for each subtree and order them based on the amount of coins we lose when choosing one subtree before another:

```c++
std::sort(child_results.begin(), child_results.end(), [](const Result& e1, const Result& e2) -> bool {
    long loss_e1_first = e1.used_time * e2.nodes; // Loss if we choose e1 before e2
    long loss_e2_first = e2.used_time * e1.nodes; // Loss if we choose e2 before e1
    
    bool e1_first = loss_e1_first < loss_e2_first; // We go into e1 first if we lose less money
    
    return e1_first;
});
