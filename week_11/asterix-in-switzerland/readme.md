## Asterix in Switzerland

We have `n` provinces, each with a balance `b_i` (which might be negative). Between them, we have `m` debt relations, where province `i` owes `d_i_j` to province `j`.  

Some provinces want to form a union. Is it possible to form a union of provinces such that the union as a whole is **self-sufficient**? This means that the total amount of money within the union is greater than the debt owed to provinces outside the union: sum(b_i) > sum(d_i_j), for all `j` outside the union and all `i` inside the union.

### Solution

We solve this problem using **Max-Flow**.

We construct the following graph:

- Each province `i` with a *positive* balance gets an edge from the **source** with capacity `b_i` (representing the money flowing into the network).
- Each province `i` with a *negative* balance gets an edge to the **sink** with capacity `-b_i` (representing the money flowing out of the network).
- Two provinces `i` and `j` are connected if they have a debt relation: if `i` owes `j` an amount `d_i_j`, we add an edge from `i` to `j` with capacity `d_i_j` (representing the money flowing from `i` to `j`).

We let it flow... What happens?

We simulate the maximum possible money flow between the provinces and their debt relations. If the **total outflow value is smaller than the total inflow**, then there must exist a subset of provinces where the total balance is greater than the total debt owed to provinces outside the union. => As more money flows into that union, than can flow out using the dept relations.
