#pragma once

enum inst_type  { NONE, IMMEDIATE, REGISTER };
enum inst_op    { NO_OP, ADD, SUB, MUL, DIV, MOVE, HALT, JUMP, COMPARE, BRANCH, GCD };

// operand
struct operand {
    enum inst_type  type;
    int             value;
};

// instruction
struct inst_t {
    enum inst_op    op;
    struct operand  opr1;
    struct operand  opr2;
}; 
