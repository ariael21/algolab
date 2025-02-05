## Coin Tossing Tournament

### Problem Description

In a coin tossing tournament, some game results were not recorded. Given a scoreboard with `n` scores (`s_i`), we need to verify whether it is possible to achieve those scores based on the recorded game results.

### Game Rules

- Two players play against each other and toss a coin.
- The winner gets **one point**, and the loser gets **none**.

### Solution Approach

We solve the problem using **Max-Flow** by constructing the following graph:

1. **Match Edges**:
   - For each match, add an edge from the **source** to the match **with capacity 1** (each game awards exactly one point).
   
2. **Player Connections**:
   - If the match result is **known**, connect the match to the **winner** with **capacity 1** (point is assigned to that player).
   - If the match result is **unknown**, connect the match to **both participants**, each with **capacity 1**. (since the **match inflow is at most 1**, only one player can receive the point)

3. **Score Constraints**:
   - Connect each **player node** to the **sink** with **capacity equal to their recorded score (`s_i`)**.

4. **Max-Flow Computation**:
   - Run a **max-flow algorithm** on the constructed graph.
   - If the total flow **matches the sum of all scores**, the scores are **achievable**.
   - Otherwise, the scores are **invalid**.