main: calculator.o load.o fetch.o decode.o execute.o 
	gcc -O2 -o main calculator.o load.o fetch.o decode.o execute.o 

calculator.o: calculator.c
	gcc -O2 -c -o calculator.o calculator.c

load.o: load.c
	gcc -O2 -c -o load.o load.c

fetch.o: fetch.c
	gcc -O2 -c -o fetch.o fetch.c

decode.o: decode.c
	gcc -O2 -c -o decode.o decode.c

execute.o: execute.c
	gcc -O2 -c -o execute.o execute.c

clean:	
	rm *.o main
