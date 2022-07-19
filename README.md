# mips_project_test
This is a sample project to use with [MIPS_core](https://github.com/jobmarley/MIPS_core)/[VSMipsProjectExtension](https://github.com/jobmarley/VSMipsProjectExtension)
The project is compiled with clang.  
It requires clang to be installed in "C:\Program Files\LLVM" (default location).
VSMipsProjectExtension also needs to be installed.

It demonstrate basic timer/debugbreak/memory mapped registers to make the leds blink (YAY!)
## bootloader
As of now the processor starts executing at 0. It is pretty difficult to compile an elf with instruction at 0 (it erases the headers etc...) so
the best plan is probably to compile a raw binary file, that will then load an elf in memory and call the entry point.
Thats what the bootloader project does. There are a few custom options available in the settings to makes sure the output is as expected.

The elf is actually loaded in memory by the debugger (this is controlled by the **additionnal files to load** setting), but the bootloader loads the sections at their correct address.
Using an elf file instead of raw binary allows to use dwarf debug info, which is necessary for debugging.

## debugging
To launch the debugger [MipsRemoteDebugger](https://github.com/jobmarley/VSMipsProjectExtension) must be running on the target machine.  
Typical debug settings look like this:

![image](https://user-images.githubusercontent.com/99695100/177378302-60f3fbac-7b1e-452b-bf9b-d04ab1696cd7.png)

## convert_to_coe.py
It takes files and offsets as arguments and generate a .coe that contains the merged files data.  
It's usefull for testing or to initialize a block ram with an executable.
