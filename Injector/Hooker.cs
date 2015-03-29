using System;
using System.IO;
using System.Reflection;
using System.Text;

namespace Injector
{
    internal class Hooker
    {
        public static string hName = "DS2Fix";
        public static string dllName = hName + ".dll";
        public static string windowName = "DARK SOULS II";

        public static bool InjectDll()
        {
            string dllPath = Assembly.GetExecutingAssembly().Location;
            dllPath = Path.Combine(Path.GetDirectoryName(dllPath), dllName);
            if (!File.Exists(dllPath))
            {
                throw new Exception(dllName + " not found");
            }

            IntPtr window = NativeMethods.FindWindow(null, windowName);
            if (window == IntPtr.Zero)
            {
                return false;
            }

            uint pid;
            NativeMethods.GetWindowThreadProcessId(window, out pid);
            if (pid == 0)
            {
                throw new Exception("Failed to get pid for " + windowName);
            }

            IntPtr processHandle = NativeMethods.OpenProcess(NativeMethods.ProcessAccessFlags.All, false, pid);
            if (processHandle == IntPtr.Zero)
            {
                throw new Exception("Failed to get handle for pid " + pid);
            }

            IntPtr loadLibraryAddr = NativeMethods.GetProcAddress(NativeMethods.GetModuleHandle("kernel32.dll"), "LoadLibraryA");
            if (loadLibraryAddr == IntPtr.Zero)
            {
                throw new Exception("Failed to get LoadLibraryA address in kernel32.dll");
            }

            IntPtr allocated = NativeMethods.VirtualAllocEx(processHandle, IntPtr.Zero, (uint)dllPath.Length, NativeMethods.AllocationType.Reserve | NativeMethods.AllocationType.Commit, NativeMethods.MemoryProtection.ReadWrite);
            if (allocated == IntPtr.Zero)
            {
                throw new Exception("Failed to allocate memory in remote thread");
            }

            IntPtr bytesWritten;
            if (!NativeMethods.WriteProcessMemory(processHandle, allocated, Encoding.UTF8.GetBytes(dllPath), dllPath.Length, out bytesWritten))
            {
                throw new Exception("Failed to write memory to process");
            }

            IntPtr threadId = NativeMethods.CreateRemoteThread(processHandle, IntPtr.Zero, 0, loadLibraryAddr, allocated, 0, IntPtr.Zero);
            if (threadId == IntPtr.Zero)
            {
                throw new Exception("Failed to create new thread in " + windowName);
            }

            NativeMethods.CloseHandle(processHandle);

            return true;
        }
    }
}