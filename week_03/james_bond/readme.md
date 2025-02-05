## James Bond

- We have a lineup of **n coins**: **x_0, x_1, …, x_(n-1)** (where x_i is the value of the coin at position i).
- We have **m people**. The people pick **one coin at a time from either side of the lineup**, and in each round, the lineup shrinks by one coin.

**→ Goal**: Determine the **maximum winnings of person k**, disregarding the behavior of the other people.

### How do you model it? 

Model it as a **decision tree**:

- At each step, a person can choose between two coins.
- Each subtree represents a smaller subproblem of the same structure.

The solution is obtained by traversing the tree while maximizing winnings when person k is choosing. 
If another person is choosing, take the minimum winnings, as their behavior is unknown.

### Solution approach

Solve it using **recursion with case distinction**:

1. Distinguish **who** is currently choosing a coin (person k or another person).
2. Distinguish **which coin** is chosen (start or end coin).

Model it as **recursion**, as **each case is a subproblem of the same structure**.
