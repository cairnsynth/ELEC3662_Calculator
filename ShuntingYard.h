#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

/**
 * ShuntingYard.h
 *   ------------------
 *  AUTHOR: Luke Coles
 *  SID: 201331120
 *  ------------------
 *  Basic implementation of Dijkstra's Shunting Yard Algorithm
 *  REF: https://brilliant.org/wiki/shunting-yard-algorithm/
 * 
 * Functions enable conversion of infix to postfix and calculation
*/

/**
 * Optype Bitfield:
 * Bitfield used to save memory
 *      0xAPO
 * A = associativity    (0 = left, 1 = right)
 * P = precedence
 * O = operation
*/
enum OpType {                               //Enum used to set operation number and precidence
    OPERAND = 0,                            //No operation or precedence
    ERR = 0x00C,                            //Syntax Error
    NAN = 0x00D,                            //Math Error
	  OOR = 0x00E,														//Out of range
    NUL = 0x00F,
    EXP = 0x111,                            //Precedence = 1, Op = 1, right-associative
    MUL = 0x022,                            //Precedence = 2, Op = 2, left-associative
    DIV = 0x023,                            //Precedence = 2, Op = 3, left-associative
    ADD = 0x034,                            //Precedence = 3, Op = 4, left-associative
    SUB = 0x035                             //Precedence = 3, Op = 5, left-associative
    
};

/**
 * Token: holds operand and value
 * struct used instead of union to enable distiction between operand and operation
*/
typedef struct Token {
    int op;
    double value;
}Token;

/**
 * Stack: holds array of tokens and iterator
*/
typedef struct Stack {
    Token stack[50];
    int stackCnt;
}Stack;

/**
 * shunting_yard: returns calculation result and error codes as Token struct
 * inputArray[] = user input
 * returns token -> value = calculation result
 *               -> op    = any error messages
*/
Token shunting_yard(char inputArray[]);

/**
 * tokenise: converts user input char array into number/operation tokens to be used in calculation
 * t = pointer to stack to write tokens
 * i[] = user input
*/
int tokenise(Stack* t, char i[]);

/**
 * shunt: shunting yard algorithm implementation, converts infix to postfix/RPN
 * tokenS = pointer to input token stack
 * opS    = pointer to operator stack
 * outS   = pointer to output stack
*/
void shunt(Stack* tokenS, Stack* opS, Stack* outS);

/**
 * result: completes postfix calculation
 * outS = pointer to shunt output stack
 * resS = pointer to result stack, used in calculation
*/
int result(Stack * outS, Stack* resS);

/**
 * stack_pop: remove last token in stack
 * s = pointer to stack to pop
*/
void stack_pop(Stack* s);

/**
 * stack_push: add token to head of stack
 * s = stack to push
*/
void stack_push(Stack* s, Token t);

/**
 * stack_init: initialise stack for use
 * s = stack to init
*/
void stack_init(Stack* s);

/**
 * print_stack: print each token of stack on a line
 * DEBUG ONLY
*/


/**
 * get_precedence: returns precedence of given operation
 * op = operation
*/
int get_precedence(int op);

/**
 * get_associativity: returns associativity of given operation
 * op = operation
*/
int get_associativity(int op);

#endif //SHUNTINGYARD_H