import os
import ctypes
from ctypes import c_double, c_int32, c_char, POINTER, Structure

_HERE = os.path.dirname(os.path.abspath(__file__))
_DLL_PATH = os.path.join(_HERE, "_native", "ballistic_c.dll")

class BallisticInputs(Structure):
    _fields_ = [
        ("relPos0", c_double * 3),
        ("relVel",  c_double * 3),
        ("v0",      c_double),
        ("kDrag",   c_double),
        ("dt",      c_double),
        ("tMax",    c_double),
        ("tolMiss", c_double),
        ("maxIter", c_int32),
    ]

class BallisticOutputs(Structure):
    _fields_ = [
        ("success", c_int32),
        ("status",  c_int32),
        ("theta",   c_double),
        ("phi",     c_double),
        ("miss",    c_double),
        ("tStar",   c_double),
        ("relMissAtStar", c_double * 3),
        ("message", c_char * 256),
    ]

def _load_dll():
    if not os.path.exists(_DLL_PATH):
        raise FileNotFoundError(
            f"Native library not found: {_DLL_PATH}\n"
            f"Build the DLL and copy it into python/ballistic_solver/_native/."
        )

    dll = ctypes.CDLL(_DLL_PATH)
    dll.ballistic_solve.argtypes = [POINTER(BallisticInputs), POINTER(BallisticOutputs)]
    dll.ballistic_solve.restype  = c_int32
    return dll

_DLL = _load_dll()

def solve(relPos0, relVel, v0, kDrag, *, dt=0.01, tMax=20.0, tolMiss=1e-2, maxIter=20):
    inp = BallisticInputs()
    inp.relPos0[:] = (float(relPos0[0]), float(relPos0[1]), float(relPos0[2]))
    inp.relVel[:]  = (float(relVel[0]),  float(relVel[1]),  float(relVel[2]))
    inp.v0 = float(v0)
    inp.kDrag = float(kDrag)
    inp.dt = float(dt)
    inp.tMax = float(tMax)
    inp.tolMiss = float(tolMiss)
    inp.maxIter = int(maxIter)

    out = BallisticOutputs()
    ok = _DLL.ballistic_solve(ctypes.byref(inp), ctypes.byref(out))

    return {
        "call_ok": int(ok),
        "success": int(out.success),
        "status": int(out.status),
        "theta": float(out.theta),
        "phi": float(out.phi),
        "miss": float(out.miss),
        "tStar": float(out.tStar),
        "relMissAtStar": [float(out.relMissAtStar[0]), float(out.relMissAtStar[1]), float(out.relMissAtStar[2])],
        "message": out.message.decode(errors="replace"),
    }