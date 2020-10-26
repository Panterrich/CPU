all: CPU/CPU.cpp CPU/CPU_main.cpp libr/Stack.cpp libr/Guard.cpp Assembler/Assembler.cpp Assembler/Assembler_main.cpp  \
	 libr/Str_lib.cpp Disassembler/Disassembler.cpp Disassembler/Disassembler_main.cpp
	g++ CPU/CPU.cpp CPU/CPU_main.cpp libr/Stack.cpp libr/Guard.cpp -o cpu
	g++ Assembler/Assembler.cpp Assembler/Assembler_main.cpp libr/Str_lib.cpp -o asm
	g++ Disassembler/Disassembler.cpp Disassembler/Disassembler_main.cpp libr/Str_lib.cpp -o dis

CPU: CPU/CPU.cpp CPU/CPU_main.cpp libr/Stack.cpp libr/Guard.cpp
	g++ CPU/CPU.cpp CPU/CPU_main.cpp libr/Stack.cpp libr/Guard.cpp -o cpu

Assembler: Assembler/Assembler.cpp Assembler/Assembler_main.cpp libr/Str_lib.cpp
	g++ Assembler/Assembler.cpp Assembler/Assembler_main.cpp libr/Str_lib.cpp -o asm

Disassembler: Disassembler/Disassembler.cpp Disassembler/Disassembler_main.cpp libr/Str_lib.cpp
	g++ Disassembler/Disassembler.cpp Disassembler/Disassembler_main.cpp libr/Str_lib.cpp -o dis