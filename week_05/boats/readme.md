## Boats

### Approach

`dp[i]` = minimum distance from the start using `i+1` boats (e.g. right boundary using `i+1` boats)

For each boat:
- Check if the current boat's mounting point `p_i` overlaps with the previous right boundary (`dp[i-1]`), thus check if `dp[i-1] > p_i`:
  - If **yes**, check if replacing the previous boat with the current boat results in a smaller `dp[i-1]`.
  - If **no**, set `dp[i] = max(p_i, dp[i-1] + l_i)`.

### Runtime

```
Test set 1 (30 pts / 2 s) : Correct answer      (0.002s)
Test set 2 (30 pts / 2 s) : Correct answer      (0.005s)
Test set 3 (40 pts / 2 s) : Correct answer      (0.679s)
```