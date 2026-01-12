#pragma once

#include <stdint.h>

#if defined(_WIN32)
    #if defined(BALLISTIC_SOLVER_EXPORTS)
        #define BALLISTIC_SOLVER_C_API __declspec(dllexport)
    #else
        #define BALLISTIC_SOLVER_C_API __declspec(dllimport)
    #endif
#else
    #define BALLISTIC_SOLVER_C_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BALLISTIC_SOLVER_ABI_VERSION
    #define BALLISTIC_SOLVER_ABI_VERSION 2u
#endif

#ifndef BALLISTIC_SOLVER_VERSION_STRING
    #define BALLISTIC_SOLVER_VERSION_STRING "0.3.0"
#endif

BALLISTIC_SOLVER_C_API uint32_t ballistic_solver_abi_version(void);
BALLISTIC_SOLVER_C_API const char* ballistic_solver_version_string(void);

typedef struct BallisticInputs
{
    double relPos0[3];
    double relVel[3];
    double v0;
    double kDrag;

    int32_t arcMode; // 0=Low, 1=High
    int32_t _pad0;
    double g;
    double wind[3];
    double dt;
    double tMax;
    double tolMiss;
    int32_t maxIter;
    int32_t _pad1;
} BallisticInputs;

typedef struct BallisticOutputs
{
    int32_t success;
    int32_t status;

    double theta;
    double phi;
    double miss;
    double tStar;

    double relMissAtStar[3];

    char message[256];
} BallisticOutputs;

BALLISTIC_SOLVER_C_API void ballistic_inputs_init(BallisticInputs* in);

BALLISTIC_SOLVER_C_API int32_t ballistic_solve(const BallisticInputs* in, BallisticOutputs* out);

#ifdef __cplusplus
}
#endif