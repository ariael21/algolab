## Rapunzel

This is a hard problem. The idea is relatively simple: Perform a DFS from the root node while checking if the last `m` brightness values have a maximum variance of less than `k`. However, getting an appropricate runtime is hard.

### Solution Approach

Store the nodes in a simple adjacency list: `typedef std::vector<std::vector<int>> IVV;`

Perform DFS from node `0`:
- Keep track of the current stack using: `std::deque<int> IStack;`

  **Why?** This allows us to iterate through our stack and access relevant data, such as the brightness of the nodes currently in our stack.

- Store the occurrences of the **last `m` brightness values** using an occurrence counter map: `typedef std::map<int, int> IIMap;`

  **Why?** This enables us to save time, as we don't have to iterate over the last `m` elements to find the `max` and `min` values.

- Store the starting locations in a set: `typedef std::set<int> ISet;`


### Runtime

```
Test set 1 (20 pts / 4 s) : Correct answer      (0.018s)
Test set 2 (20 pts / 4 s) : Correct answer      (2.195s)
Test set 3 (20 pts / 4 s) : Correct answer      (0.005s)
Test set 4 (40 pts / 4 s) : Correct answer      (2.997s)
```
