# Ballistic Launch Angle Solver

Header-only C++17 solver that computes 3D launch angles for a projectile to intercept a moving target under gravity and air drag.

The solver does **not** assume an analytic solution. Instead, it numerically simulates projectile motion and iteratively adjusts the launch direction until the projectile comes as close as possible to the target.

---

## What this does

Given:
- Relative initial position of a target
- Relative constant velocity of the target
- Projectile launch speed
- Gravity and drag parameters

The solver finds:
- Launch elevation angle (`theta`)
- Launch azimuth angle (`phi`)
- Time of closest approach
- Miss distance at closest approach

If an exact hit is not achievable, the solver returns the **best achievable solution** found within the given constraints.

---

## Core idea

1. **Numerical trajectory simulation**  
   Projectile motion is integrated forward in time using RK4, including gravity and velocity-dependent drag.

2. **Closest-approach detection**  
   During simulation, the solver detects when the distance between projectile and target reaches a minimum.  
   This minimum is refined using a local 1D optimization step.

3. **Angular residual formulation**  
   The miss vector at closest approach is converted into an angular correction problem by comparing two idealized reference launch directions:
   - One aiming directly at the target
   - One aiming at a corrected point offset by the miss vector

   The difference between these directions defines a 2D angular residual.

4. **Iterative angle correction**  
   Launch angles are updated using a damped least-squares method (Levenberg–Marquardt) with:
   - Line search for robustness
   - Broyden updates to reduce expensive Jacobian evaluations

This process repeats until the miss distance is below tolerance or iteration limits are reached.

---

## Design goals

- Robust convergence in the presence of drag
- No reliance on closed-form ballistic equations
- Fully parameterized behavior for experimentation
- Safe failure: always returns the best-known solution

---

## Intended use cases

- Physics-based games or simulations
- Guidance and interception experiments
- Numerical methods and control research
- Educational demonstrations of nonlinear solvers

---

## Limitations

- Accuracy depends on time step and integration limits
- Target motion is assumed linear
- Multiple valid solutions may exist; the solver converges to one local solution

---

## Usage

See `examples/basic.cpp` for a minimal working example.
