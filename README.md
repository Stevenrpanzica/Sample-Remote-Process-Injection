# Sample-Remote-Process-Injection

This project is for testing and better understanding process injection and C++ functions that are called when the sample source code is compiled and executed. This project tracks the functions from high-level down to syscall leveraging and pivoting off of [Malware Morphology](https://github.com/jaredcatkinson/MalwareMorphology) and [function-call-stacks](https://github.com/jaredcatkinson/function-call-stacks) projects by @jaredcatkinson [jaredcatkinson](https://github.com/jaredcatkinson)

This project is designed for educational purposes only

This source code simply monitors for mspaint.exe to be opened, and once opened, it injects a remote .dll file to open a text box that says "Hello World".

Compile "HelloBoxDLL.cpp" to a .dll, be sure to compile it for x64 architecture.

Before you compile "HelloWorldRPI.cpp", be sure to update the directory of where "HelloBoxDLL.dll" will be located. "HelloWorldRPI.cpp" also needs to be compiled for x64 architecture. 

This source code is only intended for educational use.

Usage of this source code for nefarious, malicious, or illegal purposes is strictly prohibited.
