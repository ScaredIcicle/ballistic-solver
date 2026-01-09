# Ballistic launch angle solver (RK4 + quadratic drag)

## Model
- Gravity: a_g = (0,0,-g)
- Drag acceleration: a_d = -kDrag * |v| * v
  - Units: [kDrag] = 1/m  (equivalent to 0.5*rho*Cd*A/m)

## API
- solve_launch_angles(relPos0, relVel, v0, kDrag, params)

## Notes / Limitations
- Time step dt affects accuracy and robustness.
- Returns best-so-far if tolerance not reached.

## Example
See 'examples/basic.cpp'.