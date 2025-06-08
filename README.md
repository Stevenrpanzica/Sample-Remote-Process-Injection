# Sample-Remote-Process-Injection
This is sample C++ source code used for Process Injection function call tracing

This source code simply monitors for mspaint.exe to be opened, and once opened, it injects a remote .dll file to open a text box that says "Hello World".

Compile "HelloBoxDLL.cpp" to a .dll, be sure to compile it for x64 architecture.

Before you compile "HelloWorldRPI.cpp", be sure to update the directory of where "HelloBoxDLL.dll" will be located. "HelloWorldRPI.cpp" also needs to be compiled for x64 architecture. 

This source code is only intended for educational use.

Usage of this source code for nefarious, malicious, or illegal purposes is strictly prohibited.
