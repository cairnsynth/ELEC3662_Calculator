#include "ShuntingYard.h"
#include "Utility.h"
#include "LCD.h"

Token shunting_yard(char inputArray[]) {
	debug_mess("BEGIN");
    // stacks used in each stage of conversion/calculation
    Stack tokenStack;    
    Stack outputStack;
    Stack operatorStack;
    Stack resultStack;
    // int to hold error flags output by functions
    int errorFlag = 0;  
    errorFlag = tokenise(&tokenStack, inputArray);  // tokenise user input
    shunt(&tokenStack, &operatorStack, &outputStack);  // convert to postfix
    errorFlag |= result(&outputStack, &resultStack);  // calculate result
    resultStack.stack[0].op = errorFlag;  // encode error flag
    return resultStack.stack[0];  // return result
}

int tokenise(Stack* t, char i[]) {
    // variables
    int count = 0, decimalFlag = 0, decimalPlace = 1, valueFlag = 0;
    double tempVal = 0.0;
    Token tempToken = {-1, 0.0};
    // initialise token stack
    stack_init(t);
    // loop through input string until end reached
    while(i[count] != '\0') {
        // if char is numeric add digit to temporary value and set flag
        if(i[count] <= 0x39 && i[count] >= 0x30) {
            add_digit(&tempVal, i[count] - '0', decimalFlag, &decimalPlace);
            valueFlag = 1;
        }
        // if char is . set decimal flag, 
        // digits will be added after decimal place
        else if(i[count] == '.') {
            decimalFlag = 1;
        }
        // if char is operation
        else {
            // store temporary value to token with OPERAND OpCode
            tempToken.op = OPERAND;
            tempToken.value = tempVal;
            // if 2 operations are in series in the array
            if(count > 2 && (t->stack[t->stackCnt-1].op > 0 
            && t->stack[t->stackCnt-2].op > 0)) {
                // store 2nd operation and pop from stack
				int tmpOp = t->stack[t->stackCnt-1].op;
                stack_pop(t);
                // switch based on 2nd operation
                switch(tmpOp) {
                    // 'bake' negative into subsequent value ('-','1' = '-1')
                    case SUB: {                                   
                        tempToken.value = tempVal * -1.0;
                        break;	
                    }
                    // double addition has no effect
                    case ADD: {break;}
                    // return syntax error for anything else
                    default: {return ERR;}
                }
            }
            // push new value to token stack if it is a valid value 
            if(valueFlag) {
                stack_push(t, tempToken);
            }
            // reset flags
            valueFlag = 0;
            tempVal = 0.0;
            decimalFlag = 0;
            decimalPlace = 1;
            // push operation as token with correct OpCode
            int opType = 0;
            switch (i[count])
            {
            case '+':
                opType = ADD;
                break;
            case '-': {
                if(i[count+1] == '-'){
                    opType = ADD;
                    count++;
                }
                else {
                    opType = SUB;
                } 
                break;
            }
            case 'x':
                opType = MUL;
                break;
            case 7:
                opType = DIV;
                break;
            case 'E':
                opType = EXP;
                break;
            default:
                break;
            }
            tempToken.op = opType;
            stack_push(t, tempToken);
        
        }
        // iterate through char array
        count++;
    } 
    // error handling for final operation (see ln46-62)
    if(count > 2 && (t->stack[t->stackCnt-1].op > 0 
    && t->stack[t->stackCnt-2].op > 0)) {
        int tmpOp = t->stack[t->stackCnt-1].op;
        stack_pop(t);
        switch(tmpOp) {
            case SUB: {
                tempVal = tempVal * -1.0;
                break;
            }
            case ADD: {
                break;
            }
            default: {
                return ERR;
            }
        }
    }
    // push final operand
    tempToken.value = tempVal;
    tempToken.op = OPERAND;
    stack_push(t, tempToken);
    // exponent handling
    count = 0;
    while(count < 50) {
        // if (value , exp , value) exists in stack
        if((t->stack[count].op == 0) && (t->stack[count+1].op == EXP) && (t->stack[count+2].op == 0)) {
            // if exponent is an integer
            if(t->stack[count+2].value == (int)t->stack[count+2].value) {
                // calculate exponent and empty other values
                t->stack[count].value = t->stack[count].value * int_exp(10, (int)t->stack[count+2].value);
                t->stack[count+1].op = NUL;
                t->stack[count+2].op = NUL;
            }
            // return syntax error if exponent is not integer
            else {
                return ERR;
            }
        }
        count++;
    }
    // return 0 if no errors occur
    return 0;
}

void shunt(Stack* tokenS, Stack* opS, Stack* outS) {
    int c = 0;  // loop iterator
	// initialise stacks
    stack_init(opS);
    stack_init(outS);
    // iterate through token stack
    while(tokenS->stack[c].op != -1) {
        // check current token is not empty
        if(tokenS->stack[c].op != NUL) {
            // switch based on token OpCode
            switch(tokenS->stack[c].op) {
                // if is operand push to output stack
                case OPERAND: {
                    stack_push(outS, tokenS->stack[c]);
                    break;
                }
                // if is operation
                default: {
                    // push to output stack if right associative
                    if(get_associativity(tokenS->stack[c].op)) {
                        stack_push(outS, tokenS->stack[c]);
                    }
                    // if left associative:
                    // while operator at top of operator stack has greater
                    // precedence:
                    //     pop operator from operator stack onto output stack
                    // push operator from token stack onto operator stack
                    else {  										
                        while(opS->stackCnt > 0 && 
                        (get_precedence(opS->stack[opS->stackCnt-1].op) 
                        <= get_precedence(tokenS->stack[c].op))){
                            stack_push(outS, opS->stack[opS->stackCnt-1]);
                            stack_pop(opS);
                        }
                        stack_push(opS, tokenS->stack[c]);
                    }
                    break;
                }
            }   

        }
        // iterate
        c++;
    }
    // while operations remain in op stack push to output stack
    while(opS->stackCnt > 0) {
        stack_push(outS, opS->stack[opS->stackCnt-1]);
        stack_pop(opS);
    }
}

int result(Stack* outS, Stack* resS) {
    int c = 0;  // loop iterator
    // initialise result stack
    stack_init(resS);
    // iterate through output stack
    while(outS->stack[c].op != -1) {
        // if token is not empty
        if(outS->stack[c].op != NUL) {
            // temp tokens for calculation
            Token tmpA, tmpB, tmpR;
            // push operands to result stack
            if(outS->stack[c].op == 0) {
                stack_push(resS, outS->stack[c]);
            }
            // if operation: pop top 2 operands from result stack and
            // perform operation on these numbers
            // push result back onto result stack
            else {
                tmpA = resS->stack[resS->stackCnt-1];
                stack_pop(resS);
                tmpB = resS->stack[resS->stackCnt-1];
                stack_pop(resS);
                switch(outS->stack[c].op) {
                    case EXP: {

                        break;
                    }
                    case MUL: {
                        tmpR.value = tmpB.value * tmpA.value;
                        tmpR.op = 0;
                        break;
                    }
                    case DIV: {
                        // return math error if divide by 0
                        if(tmpA.value == 0.0) {
                            return NAN;
                        }
                        else {
                        tmpR.value = tmpB.value / tmpA.value;
                        tmpR.op = 0;
                        break;
                        }
                    }
                    case ADD: {
                        tmpR.value = tmpB.value + tmpA.value;
                        tmpR.op = 0;
                        break;
                    }
                    case SUB: {
                        tmpR.value = tmpB.value - tmpA.value;
                        tmpR.op = 0;
                        break;
                    }
                }
                stack_push(resS, tmpR);
            }
            // iterate
            c++;
        }
    }
    // return 0 if no errors generated
    return 0;
}

void stack_pop(Stack* s) {
    // if there are items in the stack, empty top item and decrement count
    if(s->stackCnt > 0) {
        s->stack[s->stackCnt].op = -1;
        s->stackCnt--;
    }
}

void stack_push(Stack* s, Token t) {
    // if there is space in the stack, add t to the top and increment count
    if(s->stackCnt < sizeof(s->stack)/sizeof(Token)) {
        s->stack[s->stackCnt] = t;
        s->stackCnt++;
    }
}

void stack_init(Stack* s) {
    int i;  // loop iterator
    // go to end of stack
    s->stackCnt = sizeof(s->stack)/sizeof(s->stack[0]);
    // empty each token
    for(i = 0; i < sizeof(s->stack)/sizeof(s->stack[0]); i++) {
        s->stack[i].op = -1;
        s->stack[i].value = 0.0;
    }
    // reset count to 0
    s->stackCnt = 0;
}

int get_precedence(int op) {
    // decode precedence bits from OpCode bitfield
    return ((op & 0x0F0) >> 4);
}

int get_associativity(int op) {
    // decode associativity bits from OpCode bitfield
    return ((op & 0xF00) >> 8);
}
