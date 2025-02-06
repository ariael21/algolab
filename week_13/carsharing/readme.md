## Carsharing

We have `S` stations where initially `l_i` cars are located. There are `N` bookings, each specifying the movement of a car from one station to another, including departure time, arrival time, and profit information. Our task is to optimize the bookings to maximize profit.

### Solution Approach

The problem can be solved using the **Max-Flow-Min-Cut** algorithm. The challenging part is defining the graph:

- Create a node for each `station-time` combination (e.g., if a booking requests a car from station 1 at time 30, we create a node `1-30`).
- Create a `zero` node for each station (e.g., node `s-0`).
- Create an `end` node for each station (e.g., node `s-100000`) since the maximum arrival time is 100,000.
- Create a `source` node and a `sink` node.

We connect the nodes as follows:

1. Connect the `source` node to all `zero` nodes of the stations. The capacity is the initially available cars (at that station), and the cost is 0.
2. Connect the `end` nodes of all stations to the `sink`. The capacity is unlimited, and the cost is 0.
3. Connect the `station-time` nodes within each station so that they are linked in chronological order.
4. Connect the `station-time` nodes of one station to the `station-time` nodes of other stations based on the bookings.

#### Edge Weights

The edge weights for points 3 and 4 require careful consideration since we want to ensure non-negative weights for the faster **Max-Flow-Min-Cut** algorithm.

To achieve this, we add a positive offset to the edge weights. Since the profit for any booking can be at most 100, we add a value of at least 100 to each booking edge. The edge weights are scaled based on the time difference:

- For a booking from `1-0` to `2-30` (taking 30 minutes), the edge weight is calculated as:  
  `100 * time_diff - profit = 100 * 30 - profit`.  

This guarantees that all edge weights are non-negative.

For nodes within the same station, there is no profit involved. For example, if a node `1-30` connects to `1-60`, the edge weight is:  
`(60 - 30) * 100 = 300`.

#### Adjusting the Cost

While the modifications ensure strictly non-negative weights, they inflate the cost. To calculate the actual cost, we use the formula:  
`-(scaled_cost - (flow * 100000 * 100))`.

![Graph Example](/week_13/carsharing/1.jpeg)
