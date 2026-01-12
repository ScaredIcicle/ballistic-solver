#include <cmath>
#include <cstdio>
#include <cstring>

#include "ballistic_solver_c_api.h"
#include "ballistic_solver_core.hpp"

void ballistic_inputs_init(BallisticInputs* in)
{
    if (in == nullptr)
    {
        return;
    }

    std::memset(in, 0, sizeof(*in));

    in->arcMode = 0;
    in->g       = 9.80665;
    in->dt      = 0.01;
    in->tMax    = 20.0;
    in->tolMiss = 1e-2;
    in->maxIter = 20;
}

int32_t ballistic_solve(const BallisticInputs* in, BallisticOutputs* out)
{
    if (in == nullptr || out == nullptr)
    {
        if (out != nullptr)
        {
            std::snprintf(out->message, sizeof(out->message), "Null pointer argument");
        }
        return -1;
    }

    std::memset(out, 0, sizeof(*out));
    std::snprintf(out->message, sizeof(out->message), "Unknown");

    try
    {
        BallisticParams P;

        if (in->arcMode == 1)
        {
            P.arcMode = ArcMode::High;
        }
        else
        {
            P.arcMode = ArcMode::Low;
        }
        if (std::isfinite(in->g) && in->g > 0.0) { P.g = in->g; }
        if (std::isfinite(in->dt) && in->dt > 0.0) { P.dt = in->dt; }
        if (std::isfinite(in->tMax) && in->tMax > 0.0) { P.tMax = in->tMax; }
        if (std::isfinite(in->tolMiss) && in->tolMiss > 0.0) { P.tolMiss = in->tolMiss; }
        if (in->maxIter > 0) { P.maxIter = static_cast<int>(in->maxIter); }

        Vec3 relPos0 = { in->relPos0[0], in->relPos0[1], in->relPos0[2] };
        Vec3 relVel  = { in->relVel[0],  in->relVel[1],  in->relVel[2]  };
        P.wind = { in->wind[0], in->wind[1], in->wind[2] };


        SolverResult r = solve_launch_angles(relPos0, relVel, in->v0, in->kDrag, P);

        out->success = r.success ? 1 : 0;
        out->status  = static_cast<int32_t>(r.report.status);

        out->theta = r.theta;
        out->phi   = r.phi;
        out->miss  = r.miss;
        out->tStar = r.tStar;

        out->relMissAtStar[0] = r.relMissAtStar.x;
        out->relMissAtStar[1] = r.relMissAtStar.y;
        out->relMissAtStar[2] = r.relMissAtStar.z;

        std::snprintf(out->message, sizeof(out->message), "%s", r.report.message.c_str());

        return 0;
    }
    catch (...)
    {
        out->success = 0;
        out->status = static_cast<int32_t>(SolveStatus::InvalidInput);
        std::snprintf(out->message, sizeof(out->message), "Exception caught in ballistic_solve");
        return -2;
    }
}

uint32_t ballistic_solver_abi_version(void)
{
    return (uint32_t)BALLISTIC_SOLVER_ABI_VERSION;
}

const char* ballistic_solver_version_string(void)
{
    return BALLISTIC_SOLVER_VERSION_STRING;
}
