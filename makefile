assembler: assembler.o assemblerCons.o InstructionTrans.o DataTrans.o my_binary.o my_label.o utilities.o my_line.o my_macro.o preAssembler.o
	gcc -ansi -Wall -pedantic assembler.o assemblerCons.o InstructionTrans.o DataTrans.o my_binary.o my_label.o utilities.o my_line.o my_macro.o preAssembler.o -o assembler
assembler.o: assembler.c header.h
	gcc -c -ansi -Wall -pedantic assembler.c
assemblerCons.o:	assemblerCons.c header.h
	gcc -c -ansi -Wall -pedantic assemblerCons.c
InstructionTrans.o:	InstructionTrans.c header.h
	gcc -c -ansi -Wall -pedantic InstructionTrans.c
DataTrans.o:	DataTrans.c header.h
	gcc -c -ansi -Wall -pedantic DataTrans.c
my_binary.o:	my_binary.c header.h
	gcc -c -ansi -Wall -pedantic my_binary.c
my_label.o:	my_label.c header.h
	gcc -c -ansi -Wall -pedantic my_label.c
utilities.o:	utilities.c header.h
	gcc -c -ansi -Wall -pedantic utilities.c
my_line.o:	my_line.c header.h
	gcc -c -ansi -Wall -pedantic my_line.c
my_macro.o:	my_macro.c header.h
	gcc -c -ansi -Wall -pedantic my_macro.c
preAssembler.o:	preAssembler.c header.h
	gcc -c -ansi -Wall -pedantic preAssembler.c
