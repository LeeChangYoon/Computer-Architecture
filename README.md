# Computer-Architecture
Repository for Computer Architecture class

# Index
1. [Project1 - Simple Calculator](#1.-Project1-----Simple-Calculator)
2. [Project2 - Single Cycle MIPS Simulator](#2.-Project2-----Single-Cycle-MIPS-Simulator)
3. [Project3 - Pipelined MIPS Simulator](#3.-Project3-----Pipelined-MIPS-Simulator)
4. [Project4 - Pipelined MIPS Simulator with Cache](#4.-----Project4-Pipelined-MIPS-Simulator-with_Cache)

<br><br>
# 1. Project1 - Simple Calculator

## Description

## Requirements

## Instruction Set Architecture
#### 사칙 연산 명령어

#### 레지스터 명령어

#### 흐름 제어 명령어


## Environment
- 사용 언어 : C
- 개발 환경 : Linux
- 파일 정보 : main.c error_check.c decode.c execute.c calculator.h

## How to Compile
1. git clone을 하여 소스파일이 모두 있는 디렉토리에 위치한다.
2. vim input.txt 를 입력한 후 i또는 a또는 o를 눌러 입력 모드를 통해 알맞은 식을 입력한다.
3. 입력이 완료가 되면 ESC를 누른 후, :wq를 입력한 후 엔터를 쳐서 파일 저장한다.
4. 저장이 되었다면, gcc -Wall -Wextra -Werror *.c 를 입력한다.
4-1) 옵션을 추가해준 이유는 모호한 코딩에 대한 경고와 모든 경고에 대해 컴파일 에러를 처리하여 더욱 완성도 있는 코드를 증명하기 위함이다.
5. 컴파일이 잘되었다면 a.out 실행 파일이 생성되었을 것이다. ./a.out 을 입력하여 실행을 한 후, 결과를 확인한다.

## Exception Handling

<br><br>
# 2. Project2 - Single Cycle MIPS

## Description

 
## Requirements


## Single-Cycle


## Environment
- 사용 언어 : C
- 개발 환경 : Linux
- 파일 정보 : src/main.c src/fetch.c src/decode.c src/execute.c src/memory_access.c src/write_back.c src/MUX.c src/single_cycle.h

## How to Compile
1. git clone을 하여 소스파일이 모두 있는 디렉토리(src)에 위치한다.
2. gcc -Wall -Wextra -Werror *.c -o single_cycle 를 입력하여 컴파일한다.
2-1) 옵션을 추가해준 이유는 모호한 코딩에 대한 경고와 모든 경고에 대해 컴파일 에러를 처리하여 더욱 완성도 있는 코드를 증명하기 위함이다.
3. 컴파일이 잘 되었다면 single_cycle 실행 파일이 생성되었을 것이다. ./single_cycle에 원하는 input 디렉토리의 binary 파일명을 입력하여 실행한 후, 결과를 확인한다.

## Exception Handling
- input 파일을 불러오지 못하거나 빈 파일이라면 에러 메시지 출력 후 프로그램을 종료한다.
- 명령어를 decode 할 때, opcode가 정해진 format이 아니라면 에러 메시지 출력
![image](https://user-images.githubusercontent.com/70425484/124956708-3588d780-e053-11eb-90a9-b00ab4f4e6a3.png)

<br><br>
# 3. Project3 - Pipelined MPIS Simulator

## Description

 
## Requirements


## Pipeline

## Environment
- 사용 언어 : C
- 개발 환경 : Linux
- 파일 정보 : src/main.c src/fetch.c src/decode.c src/execute.c src/memory_access.c src/write_back.c src/MUX.c src/load.c src/pipeline.h

## How to Compile
1. git clone을 하여 소스파일이 모두 있는 디렉토리(src)에 위치한다.
2. gcc -Wall -Wextra -Werror *.c -o pipeline 를 입력하여 컴파일한다.
2-1) 옵션을 추가해준 이유는 모호한 코딩에 대한 경고와 모든 경고에 대해 컴파일 에러를 처리하여 더욱 완성도 있는 코드를 증명하기 위함이다.
3. 컴파일이 잘 되었다면 pipeline 실행 파일이 생성되었을 것이다. ./pipeline 원하는 input 디렉토리의 binary 파일명을 입력하여 실행한 후, 결과를 확인한다.

## Exception Handling

<br><br>
# 4. Project4 - Pipelined MIPS Simulator with Cache

## Description


## Requirements


## Cache


## Environment
- 사용 언어 : C
- 개발 환경 : Linux
- 파일 정보 : src/main.c src/fetch.c src/decode.c src/execute.c src/memory_access.c src/write_back.c src/MUX.c src/load.c src/DMcache.c src/pipeline.h

## How to Compile
1. git clone을 하여 소스파일이 모두 있는 디렉토리(src)에 위치한다.
2. gcc -Wall -Wextra -Werror *.c -o direct_mapped_cache 를 입력하여 컴파일한다.
2-1) 옵션을 추가해준 이유는 모호한 코딩에 대한 경고와 모든 경고에 대해 컴파일 에러를 처리하여 더욱 완성도 있는 코드를 증명하기 위함이다.
3. 컴파일이 잘 되었다면 direct_mapped_cache 실행 파일이 생성되었을 것이다. ./direct_mapped_cache 원하는 input 디렉토리의 binary 파일명을 입력하여 실행한 후, 결과를 확인한다.
