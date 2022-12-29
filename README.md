# Computer-Architecture
Repository for Computer Architecture class

# Index
1. [Project1 - Simple Calculator](#Project1-Simple-Calculator)
2. [Project2 - Single Cycle MIPS Simulator](#Project2-Single-Cycle-MIPS-Simulator)
3. [Project3 - Pipelined MIPS Simulator](#Project3-Pipelined-MIPS-Simulator)
4. [Project4 - Pipelined MIPS Simulator with Cache](#Project4-Pipelined-MIPS-Simulator-with_Cache)

<br><br>
# Project1-Simple Calculator

## Description
 In this project, we will discuss the implementation of our own ISA and calculation program that executes each 
instruction sequentially. This program follows the sequence of executing an instruction of traditional computer 
architecture, von Neumann computer. The program has four phases of execution: Load, Fetch, and Execute. In the 
Load, it loads all of the instructions in “input.txt” and stores them in the memory array. Next, in the Fetch, IR gets 
the instructions from Memory which PC points at and decodes instruction into a set of [opcode operand1, 
operand2]. In the Execute, the program executes the instruction, prints out the result of the instruction to the 
console screen, and records log in “output.txt”. Execute phase includes binary arithmetic operations (+, -, x, /), 
mover instruction (M), halt instruction (H), jump instruction (J), compare instruction (C), and branch instruction (B).
 This project consists of 4 sections. First, we will talk about concepts used in the calculator program including the 
instruction set manual. Next is the program structure. We will discuss the sequence of program execution. Third, 
we will state the details for instructions. In the last of the project, we will present the build environment and image 
files for the results.

## Requirements
- After writing the necessary operation in input.txt, it starts by reading the file.
- The basic format of input.txt is OPCODE operand1 operand2. Each is separated by a space (' ') in white space.
- input.txt separates each operation with a newline ('\n') or termination ('\0').
- There are a total of ten registers from R0 to R9 that contain the results.
- The operand can be a hexadecimal number or a register. The format is as follows. "0xN, Rn. In this case, N is from 0 to F, and n is from 0 to 9"

## Instruction Set Architecture
- Instruction Set Architecture (ISA)

![image](https://user-images.githubusercontent.com/41320453/174940619-c53ec36d-3462-4ad4-a614-cdd3d94a1e40.png)

- Example of GCD code implemented by following ISA

![image](https://user-images.githubusercontent.com/41320453/174940715-3741309a-1312-481f-939f-fd829f06c8af.png)

## Environment
- Build Environments:
1. Linux environment – Vi editor, GCC complier
2. Program is built by using the Makefile.

- Make command:
1. $make main -> build the execution program
2. $make clean -> clean all the object files that builds main
<br><br>

# Project2-Single-Cycle-MIPS-Simulator

## Description
 In this project, we will describe how single-cycle microarchitecture works, its implementations, and the evaluation of the 
project. The work presented in this project is part of a large project which is designed to implement the pipelined and optimized multi-cycle microarchitecture which uses the instruction set of the MIPS. Before we get into the large scale of the project, we must build the microarchitecture that executes the instructions in a single cycle. 
 The first step in this project is to specify the requirements for the single-cycle microarchitecture. Second, we move on to concepts that are critically related to the implementation of the microarchitecture: Von Neumann Computer, ISA, MIPS, Data paths. Third, we will state the total data path that we have implemented, including the control signal table and program definitions. Fourth, we will describe how we implemented the data paths and single-cycle MIPS simulator according to the control signals and program definitions. Then, there will be some results by executing the binary programs using an
implemented simulator. In the end, we will evaluate the single-cycle simulator with some assumptions and the flow of the data paths.

 
## Requirements
- Requirements

![image](https://user-images.githubusercontent.com/41320453/174940950-4da92940-081a-406f-b6aa-661d6332a782.png)

## Single-Cycle
- Data Path of Single-Cycle Microarchitecture

![image](https://user-images.githubusercontent.com/41320453/174941038-cda1308f-1a7b-47de-9918-f2ee9bdde959.png)

## Environment
- Build Environments:
1. Linux environment – Vi editor, GCC complier
2. Program is built by using the Makefile.

- Make command:
1. $make main -> build the execution program
2. $make clean -> clean all the object files that builds main
<br><br>

# Project3-Pipelined-MIPS-Simulator

## Description
 The work presented in this project is the third part of a large project designed to implement and optimize the 
microarchitecture that uses the MISP ISA. We will apply the advanced concepts to the former single-cycle microarchitecture to enhance the performance of the microarchitecture. 
 The first step in this project is to specify the requirements for the pipelined microarchitecture. Second, we move on to 
concepts critically related to microarchitecture: multi-cycle microarchitecture, pipelined microarchitecture, performance 
analysis, data dependency handling, control dependency handling, and branch prediction. Third, we will state the data path, 
which includes the latches, forwarding unit, data hazard detection unit, branch prediction unit, and program definitions. Fourth, we will describe how we implemented the data paths and pipelined MIPS simulator according to the hardware components and program definitions. Then, there will be some results by executing the binary programs using an implemented simulator. In the end, we will evaluate the pipelined MIPS simulator with performance comparison with the single-cycle MISP simulator, and the flow of the data paths based on some assumptions.

## Requirements
- Requirements

![image](https://user-images.githubusercontent.com/41320453/174941467-6313ac64-9178-4dea-b314-b02767cb214a.png)

## Pipeline
- Pipelined Microarchitecture

![image](https://user-images.githubusercontent.com/41320453/174941521-258992a5-8749-4d88-8455-fe88843ce27c.png)

## Environment
- Build Environments:
1. Linux environment – Vi editor, GCC complier
2. Program is built by using the Makefile.

- Make Command:
1. $make cpu -> build the execution program
2. $make clean -> clean all the object files that builds main

- Execution Command
1. ./cpu test_prog/{$program} 1 -> Execute the program with Always Not Taken branch predictor.
2. ./cpu test_prog/{$program} 2 -> Execute the program with Always Taken branch predictor.
3. ./cpu test_prog/{$program} 3 -> Execute the program with 1-bit Last Time branch predictor.
4. ./cpu test_prog/{$program} 4 -> Execute the program with 2-bit Counter-Based branch predictor.
5. ./cpu test_prog/{$program} 5 -> Execute the program with 2-Level Global branch predictor.
6. ./cpu test_prog/{$program} 6 -> Execute the program with 2-Level Local branch predictor.
<br><br>

# Project4-Pipelined-MIPS-Simulator-with_Cache

## Description
In this project, we will mainly discuss the cache operation, implementation, and evaluation in the pipelined microarchitecture. 
The works presented in this project are the last part of a large project designed to implement and optimize the 
microarchitecture that uses MIPS ISA. We will add the concept of the cache into the pipelined microarchitecture to enhance the 
performance of the microarchitecture.
The first step in this project is to specify the requirements for the cache structure. Second, we will move on to the concepts 
for the cache: memory hierarchy, memory locality, performance, types of the cache, cache replacement policies, and writeback
policies. Third, we will state the program definition for our cache implementation. Fourth, we will describe how we 
implemented the cache with various cache replacements and write-back policies. Then, there will be some results for each 
execution of the binary program by the cache MIPS simulator. At the end of this project, we will evaluate the cache MIPS 
simulator with the performance comparison, and the memory latency based on some assumptions.

## Requirements
- Requirements

![image](https://user-images.githubusercontent.com/41320453/174941653-31c4663e-7e4a-46fa-9ed9-ca7fa22b6e86.png)

## Cache
- Cache Structure in CPU

![image](https://user-images.githubusercontent.com/41320453/174941715-5539b70b-f7e5-4067-b205-238a6611d3de.png)

## Environment
- Build Environments:
1. Linux environment – Vi editor, GCC complier
2. Program is built by using the Makefile.

- Make Command:
1. $make cpu -> build the execution program
2. $make clean -> clean all the object files that builds main

- Execution Command
5. ./cpu test_prog/{$program} 1-x-x x -> Execute the program with Fully Associative Cache.
6. ./cpu test_prog/{$program} 2-x-x x -> Execute the program with Direct Mapped Cache.
7. ./cpu test_prog/{$program} 3-x-x x -> Execute the program with Set Associative Cache.
8. ./cpu test_prog/{$program} x-0-x x -> Execute the program with Write Through Policy.
9. ./cpu test_prog/{$program} x-1-x x -> Execute the program with Write Back Policy.
10. ./cpu test_prog/{$program} x-x-0 x -> Execute the program with FIFO Algorithm.
11. ./cpu test_prog/{$program} x-x-1 x -> Execute the program with Random Algorithm.
12. ./cpu test_prog/{$program} x-x-2 x -> Execute the program with LRU Algorithm.
13. ./cpu test_prog/{$program} x-x-3 x -> Execute the program with LFU Algorithm.
14. ./cpu test_prog/{$program} x-x-4 x -> Execute the program with Second Chance Algorithm.
15. ./cpu test_prog/{$program} x-x-5 x -> Execute the program with Enhanced Second Chance Algorithm.

- CSV File Generation
1. First, if we execute the cache MIPS simulator, we can get the result.txt file. 
2. Second, open the Data_Purification.ipnyb and upload the result.txt on it, and then execute all the programs. 
3. As a result, we can get the following CSV files that contain the results of our cache MIPS simulator.
