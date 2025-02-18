Simple Device Driver

Run the command:
"make"

This will generate the ".ko" file for the module.

Usage
Loading the Module

Load the module into the kernel:
"sudo insmod kernel_threads_module.ko"

Verify that the module has been loaded:
"lsmod | grep kernel_threads_module"

Check the kernel log to see messages from the threads:
"dmesg | tail"

Unloading the Module

Unload the module:
"sudo rmmod kernel_threads_module"

Verify the module is removed:
"lsmod | grep kernel_threads_module"

Check the kernel log for cleanup messages:
"dmesg | tail"

Notes

Use "dmesg" or "journalctl -k" to view kernel log messages.

Kernel modules run in privileged mode, so incorrect code can crash the system. Use caution when developing and testing.

References

Linux Kernel Documentation: https://www.kernel.org/doc/html/latest/

Kernel Threads in Linux: https://elinux.org/Threads_in_Linux_Kernel

