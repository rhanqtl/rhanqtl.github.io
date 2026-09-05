edges = []

def solve(coins: [int], target: int):
    def helper(curr: int, i: int):
        if curr == target:
            return 1
        if i >= len(coins) or curr + coins[i] > target:
            return 0
        ans = 0
        while curr <= target:
            # print((curr, i), (curr,i+1))
            edges.append((f"({curr}, {i})", f"({curr}, {i+1})"))
            ans += helper(curr, i+1)
            curr += coins[i]
        return ans
    return helper(0, 0)

solve([1, 2, 5, 10, 20, 50, 100], 5)

from graphviz import Digraph

g = Digraph()

seen = set()
for u, v in edges:
    if u not in seen:
        g.node(u)
        seen.add(u)
    if v not in seen:
        g.node(v)
        seen.add(v)

for u, v in edges:
    g.edge(u, v)

g.render("dependency_graph", format="png")
