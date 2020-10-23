CPU: CPU/CPU.cpp CPU_main.cpp libr/Stack.cpp libr/Guard.cpp
	g++ CPU/CPU.cpp CPU_main.cpp libr/Stack.cpp libr/Guard.cpp -o cpu
Assembler: Assembler/Assembler.cpp Assembler_main.cpp libr/Str_lib.cpp
	g++ Assembler/Assembler.cpp Assembler_main.cpp libr/Str_lib.cpp -o asm
Disassembler: Disassembler/Disassembler.cpp Disassembler_main.cpp libr/Str_lib.cpp
	g++ Disassembler/Disassembler.cpp Disassembler_main.cpp libr/Str_lib.cpp -o dis