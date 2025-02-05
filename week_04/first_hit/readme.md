## First Hit

### Key Idea

- Once the **first intersection** is found, transform the **ray into a segment**.
- This minimizes the number of possible collisions.
- For each **new intersection**, **shorten the segment** to reduce further checks.

### Solution Approach

1. **Find the first intersection**.
2. **Convert the ray into a segment** from the origin to this intersection.
3. **Iterate through all segments**, updating the segment **whenever a closer intersection is found**.
4. **Output the endpoint** of the final ray segment.

### Note

To avoid worse time runtime, we need to shuffle the segments:

```c++
std::random_shuffle(segments.begin(), segments.end());
```