import os
import ctypes
from ctypes import c_double, c_int32, c_char, POINTER, Structure

HERE = os.path.dirname(os.path.abspath(__file__))

# Prefer Release if present, else Debug
dll_release = os.path.abspath(os.path.join(HERE, "..", "build", "Release", "ballistic_c.dll"))
dll_debug   = os.path.abspath(os.path.join(HERE, "..", "build", "Debug",   "ballistic_c.dll"))

DLL_PATH = dll_release if os.path.exists(dll_release) else dll_debug

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

dll = ctypes.CDLL(DLL_PATH)
dll.ballistic_solve.argtypes = [POINTER(BallisticInputs), POINTER(BallisticOutputs)]
dll.ballistic_solve.restype  = c_int32

inp = BallisticInputs()
inp.relPos0[:] = (120.0, 30.0, 5.0)
inp.relVel[:]  = (2.0, -1.0, 0.0)
inp.v0 = 90.0
inp.kDrag = 0.002
inp.dt = 0.01
inp.tMax = 30.0
inp.tolMiss = 0.5
inp.maxIter = 30

out = BallisticOutputs()

ok = dll.ballistic_solve(ctypes.byref(inp), ctypes.byref(out))

print("call_ok =", ok)
print("success =", out.success)
print("status  =", out.status)
print("theta   =", out.theta)
print("phi     =", out.phi)
print("miss    =", out.miss)
print("tStar   =", out.tStar)
print("relMiss =", list(out.relMissAtStar))
print("message =", out.message.decode(errors="replace"))