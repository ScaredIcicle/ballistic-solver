using System;
using System.Runtime.InteropServices;
using System.Text;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct BallisticInputs
{
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
    public double[] relPos0;

    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
    public double[] relVel;

    public double v0;
    public double kDrag;

    public Int32 arcMode;
    public Int32 _pad0;

    public double g;

    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
    public double[] wind;

    public double dt;
    public double tMax;
    public double tolMiss;

    public Int32 maxIter;
    public Int32 _pad1;
}

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct BallisticOutputs
{
    public Int32 success;
    public Int32 status;

    public double theta;
    public double phi;
    public double miss;
    public double tStar;

    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
    public double[] relMissAtStar;

    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
    public byte[] message;
}

public static class BallisticNative
{
    [DllImport("ballistic_solver", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ballistic_inputs_init")]
    public static extern void ballistic_inputs_init(ref BallisticInputs input);

    [DllImport("ballistic_solver", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ballistic_solve")]
    public static extern Int32 ballistic_solve(ref BallisticInputs input, out BallisticOutputs output);
}

public class Program
{
    private static string CStr(byte[] bytes)
    {
        int n = Array.IndexOf(bytes, (byte)0);
        if (n < 0) { n = bytes.Length; }
        return Encoding.UTF8.GetString(bytes, 0, n);
    }

    public static void Main()
    {
        var input = new BallisticInputs
        {
            relPos0 = new double[3],
            relVel = new double[3],
            wind = new double[3],
        };

        BallisticNative.ballistic_inputs_init(ref input);

        input.relPos0[0] = 100.0;
        input.relPos0[1] = 30.0;
        input.relPos0[2] = 10.0;

        input.relVel[0] = -10.0;
        input.relVel[1] = 30.0;
        input.relVel[2] = 0.0;

        input.v0 = 80.0;
        input.kDrag = 0.005;

        // input.wind[0] = 0.0;
        // input.wind[1] = 5.0;
        // input.wind[2] = 0.0;

        BallisticOutputs output;
        int ok = BallisticNative.ballistic_solve(ref input, out output);

        Console.WriteLine($"call_ok={ok}");
        Console.WriteLine($"success={output.success}");
        Console.WriteLine($"status={output.status}");
        Console.WriteLine($"theta={output.theta}");
        Console.WriteLine($"phi={output.phi}");
        Console.WriteLine($"miss={output.miss}");
        Console.WriteLine($"tStar={output.tStar}");
        Console.WriteLine($"relMiss=[{output.relMissAtStar[0]}, {output.relMissAtStar[1]}, {output.relMissAtStar[2]}]");
        Console.WriteLine($"message={CStr(output.message)}");
    }
}
