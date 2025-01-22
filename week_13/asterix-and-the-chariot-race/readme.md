## Asterix and the Chariot Race

A tricky problem... It took me three attempts and quite some time to solve it. Let's embrace the process. :)

### Solution for 0% (It's just wrong):

Assuming all the costs of repairing the roads in the city are equal, we arrive at a pretty straightforward solution:

- Calculate the maximum matching: `max_matching`.
- Output `c_i` * `max_matching`.

Actually... this is just wrong. Okay, let's try again.

### Solution for 0% (Due to Time Limit)

See `slow.cpp` for an alternative solution, which is too slow. In the worst case, there are two recursive calls to all children, making it way too slow. We need to reduce this to only one recursive call to the children.

### Solution for 100% (Finally!)

The hard part is distinguishing the three cases. For this, we define three states for each node:

1. **`taken`**: We take this node, *ensuring that all its children are covered.*
2. **`covered`**: This node is covered if *at least one of its children is taken*.
3. **`not covered`**: This node is neither taken nor are any of its children taken. (You might wonder why this case is relevant—it becomes important because this node can *become* covered if its parent is taken. Thus we still need this information.)

Finally, we traverse the tree from the leaf nodes to the root (`node 0`) while calculating these three values for each subtree. At the root, we simply choose the minimal cost from the `covered` state.

### Runtime

```
Test set 1 (25 pts / 3 s) : Correct answer      (0.102s)
Test set 2 (25 pts / 3 s) : Correct answer      (0.112s)
Test set 3 (25 pts / 3 s) : Correct answer      (0.111s)
Test set 4 (25 pts / 3 s) : Correct answer      (1.67s)
```