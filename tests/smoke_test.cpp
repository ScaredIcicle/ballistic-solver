#include <cassert>
#include <iostream>

#include "ballistic/ballistic_solver.hpp"

static bool is_finite_vec3(const Vec3& v)
{
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
}

int main()
{
    // Smoke test: validate API returns finite outputs.
    Vec3 relPos0 = { 120.0, 30.0, 5.0 };
    Vec3 relVel  = { 2.0, -1.0, 0.0 };

    double v0 = 90.0;
    double kDrag = 0.002;

    BallisticParams P;
    P.dt = 0.01;
    P.tMax = 30.0;
    P.tolMiss = 0.5;

    SolverResult r = solve_launch_angles(relPos0, relVel, v0, kDrag, P);

    assert(std::isfinite(r.theta));
    assert(std::isfinite(r.phi));
    assert(std::isfinite(r.miss));
    assert(std::isfinite(r.tStar));
    assert(is_finite_vec3(r.relMissAtStar));
    assert(r.miss >= 0.0);

    std::cout << "success=" << r.success << "\n";
    std::cout << "theta=" << r.theta << " rad\n";
    std::cout << "phi=" << r.phi << " rad\n";
    std::cout << "miss=" << r.miss << " m\n";
    std::cout << "t*=" << r.tStar << " s\n";
    std::cout << "status=" << static_cast<int>(r.report.status) << "\n";
    std::cout << "message=" << r.report.message << "\n";

    return 0;
}
