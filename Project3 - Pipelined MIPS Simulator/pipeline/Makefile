CC = gcc -O2
OBJS = CU.o DM.o FW.o HU.o IM.o RF.o ADR.o ALU.o CPU.o GSH.o LAT.o LOG.o MUX.o STT.o LTP1.o LTP2.o LVL2.o 
TARGET = cpu

clean:
	rm -f *.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

CU.o: CU.c
DM.o: DM.c
FW.o: FW.c
HU.o: HU.c
IM.o: IM.c
RF.o: RF.c
ADR.o: ADR.c
ALU.o: ALU.c
GSH.o: GSH.c
LAT.o: LAT.c
LOG.o: LOG.c
MUX.o: MUX.c
STT.o: STT.c
LTP1.o: LTP1.c
LTP2.o: LTP2.c
LVL2.o: LVL2.c
CPU.o: cpu.h mips_inst.h mips_reg.h CPU.c
