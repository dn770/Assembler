assembler : assembler.o firstscan.o secondscan.o printfiles.o
	gcc -g -ansi -Wall -pedantic assembler.o firstscan.o secondscan.o printfiles.o -o assembler 
assembler.o : assembler.c  assembler.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o
firstscan.o : firstscan.c  assembler.h scan.h
	gcc -c -ansi -Wall -pedantic firstscan.c -o assembler.o
secondscan.o : secondscan.c  assembler.h scan.h
	gcc -c -ansi -Wall -pedantic secondscan.c -o assembler.o
printfiles.o : printfiles.c  assembler.h scan.h
	gcc -c -ansi -Wall -pedantic printfiles.c -o assembler.o
