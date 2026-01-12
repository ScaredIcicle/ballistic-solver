#ifndef _WIN32
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#endif

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ballistic_solver_c_api.h"

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884
#endif

static double now_ms(void)
{
#if defined(_WIN32)
    return (double)clock() * 1000.0 / (double)CLOCKS_PER_SEC;
#else
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    {
        return (double)clock() * 1000.0 / (double)CLOCKS_PER_SEC;
    }
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
#endif
}

int main(void)
{
    BallisticInputs in;
    BallisticOutputs out;

    ballistic_inputs_init(&in);
    memset(&out, 0, sizeof(out));

    /* Target state relative to the projectile at t = 0 */
    in.relPos0[0] = 100.0;
    in.relPos0[1] = 30.0;
    in.relPos0[2] = 10.0;

    in.relVel[0] = -10.0;
    in.relVel[1] = 30.0;
    in.relVel[2] = 0.0;

    /* Projectile parameters */
    in.v0 = 80.0;
    in.kDrag = 0.005;

    /* Solver configuration */
    in.arcMode = 0;      /* 0=Low, 1=High */
    in.g = 9.80665;

    in.wind[0] = 0.0;
    in.wind[1] = 0.0;
    in.wind[2] = 0.0;

    in.dt = 0.01;
    in.tMax = 20.0;
    in.tolMiss = 1e-2;
    in.maxIter = 20;

    {
        double t0 = now_ms();
        int32_t rc = ballistic_solve(&in, &out);
        double t1 = now_ms();

        if (rc != 0)
        {
            printf("call failed: rc=%d\n", (int)rc);
            printf("status=%d\n", (int)out.status);
            printf("message=%s\n", out.message);
            return 1;
        }

        printf("success    : %d\n", (int)out.success);
        printf("elevation  : %.17g deg\n", out.theta * 180.0 / M_PI);
        printf("azimuth    : %.17g deg\n", out.phi * 180.0 / M_PI);
        printf("miss       : %.17g\n", out.miss);
        printf("time       : %.17g\n", out.tStar);
        printf("status     : %d\n", (int)out.status);
        printf("message    : %s\n", out.message);
        printf("elapsed    : %.6f ms\n", (t1 - t0));
    }

    return 0;
}
