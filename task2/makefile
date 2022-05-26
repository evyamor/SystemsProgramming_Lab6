all:myshell 
LineParser.o:LineParser.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
myshell.o:myshell.c 
	gcc -g -m32 -c -o myshell.o myshell.c
myshell: myshell.o LineParser.o
	gcc -m32 -o myshell myshell.o LineParser.o

.PHONY: clean
clean:
	 rm -rf ./*.o myshell
