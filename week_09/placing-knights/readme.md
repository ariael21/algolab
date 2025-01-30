## Placing Knights

### Solution Approach

This problem seems easy but requires quite some insight.

A crucial thing to notice is: **knights only attack fields of the opposite field color** (a knight on a dark square only attacks light squares).

So, what do we actually need to compute?  
We need to place as many knights as possible, such that no two knights attack each other.

Let's model a graph:  
For each square, we connect it to the 8 places (if it's not a hole or outside the board), which it would attack when a knight is placed on it. Now, in this graph, **we need to select the squares such that no two of those selected squares are connected** (attacking each other). This is equal to the **Maximum Independent Set** of this graph.  

This problem is (usually) NP-Complete; however, we have a special graph. This graph is **Bipartite**, as the knights only attack the fields of opposite color! Thus, the graph is split into white and black fields.

The Maximum Independent Set `MaxIS` is equal to `V \ MinVC`, meaning it's equal to the set of vertices that are **not in the Minimum Vertex Cover**.

=> MaxIS = V \ MinVC 

For **Bipartite** graphs, the **Minimum Vertex Cover is equal to the number of edges in a maximum matching**.

=> MinVC = #edges in Maximum Matching