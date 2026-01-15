from ballistic_solver import BallisticParams, solve

res = solve(relPos0=(100,30,5), relVel=(-10,30,0), v0=80.0, kDrag=0.005)

assert isinstance(res, dict)
assert "success" in res
assert "theta" in res
assert "phi" in res