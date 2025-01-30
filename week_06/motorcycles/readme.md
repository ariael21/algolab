## Motorcycles

### Solution Approach

If we only have motorbikes which go upwards (no negative slopes):

- sort the motorbikes by start position along the y-axis
- go through all the slopes from highest y-value to lowest y-value and compare their slopes: if the biker below has a *lower* slope than the above one => lower one will be eliminated

### Learnings

Don't try to do your own fraction implementation:
- Eventho this: `a/b > c/d` => `a*d > c*b` theoretically works, we get overflows as the numbers get too big.

Use `Gmpq (Gmpz n, Gmpz d)` (which represents `n/d`) instead.