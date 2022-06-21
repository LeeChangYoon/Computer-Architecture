main: main.o IM.o CU.o RF.o ALU.o DM.o ADDR.o 
	gcc -O2 -o main main.o IM.o CU.o RF.o ALU.o DM.o ADDR.o

main.o: main.c
	gcc -O2 -c -o main.o main.c

IM.o: IM.c
	gcc -O2 -c -o IM.o IM.c

CU.o: CU.c
	gcc -O2 -c -o CU.o CU.c

RF.o: RF.c
	gcc -O2 -c -o RF.o RF.c

ALU.o: ALU.c
	gcc -O2 -c -o ALU.o ALU.c

DM.o: DM.c
	gcc -O2 -c -o DM.o DM.c

ADDR.o:	ADDR.c
	gcc -O2 -c -o ADDR.o ADDR.c

clean: 
	rm *.o main
