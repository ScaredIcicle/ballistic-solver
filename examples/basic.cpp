#include <iostream>
#include "ballistic/ballistic_solver.hpp"

int main()
{
    // Target state relative to the projectile at t = 0
    Vec3 relPos0 = { 100.0, 30.0, 10.0 };
    Vec3 relVel  = { 10.0, -30.0, 0.0 };

    // Projectile parameters
    double v0 = 80.0;     // initial speed magnitude
    double kDrag = 0.005; // drag coefficient

    // Solver configuration
    BallisticParams P;
    P.dt = 0.01;          // integration step
    P.tMax = 20.0;        // max simulation time
    P.tolMiss = 1e-4;     // success tolerance

    // Solve launch angles
    SolverResult r = solve_launch_angles(relPos0, relVel, v0, kDrag, P);

    // Output results
    std::cout << "success    : " << r.success << "\n";
    std::cout << "elevation  : " << r.theta * 180.0 / M_PI << " deg\n";
    std::cout << "azimuth    : " << r.phi * 180.0 / M_PI << " deg\n";
    std::cout << "miss       : " << r.miss << "\n";
    std::cout << "time       : " << r.tStar << "\n";
    std::cout << "iterations : " << r.report.iterations
              << " (accepted : " << r.report.acceptedSteps << ")\n";
    std::cout << "message    : " << r.report.message
              << " (status : " << static_cast<int>(r.report.status) << ")\n";

    return 0;
}
