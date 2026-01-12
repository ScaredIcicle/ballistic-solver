using System;
using System.Runtime.InteropServices;
using System.Text;

public static class BallisticSolver
{
    [DllImport("ballistic_solver", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ballistic_inputs_init")]
    private static extern void ballistic_inputs_init(ref BallisticInputs input);

    [DllImport("ballistic_solver", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ballistic_solve")]
    private static extern Int32 ballistic_solve(ref BallisticInputs input, out BallisticOutputs output);

    private static string CStr(byte[] bytes)
    {
        int n = Array.IndexOf(bytes, (byte)0);
        if (n < 0) { n = bytes.Length; }
        return Encoding.UTF8.GetString(bytes, 0, n);
    }

    public static (int callOk, BallisticOutputs output, string message) Solve(ref BallisticInputs input)
    {
        if (input.relPos0 == null || input.relPos0.Length != 3) { input.relPos0 = new double[3]; }
        if (input.relVel == null || input.relVel.Length != 3) { input.relVel = new double[3]; }
        if (input.wind == null || input.wind.Length != 3) { input.wind = new double[3]; }

        ballistic_inputs_init(ref input);

        int ok = ballistic_solve(ref input, out BallisticOutputs output);

        string msg = (output.message != null) ? CStr(output.message) : string.Empty;

        return (ok, output, msg);
    }
}
