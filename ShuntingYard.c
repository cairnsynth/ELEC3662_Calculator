#include "ShuntingYard.h"
#include "Utility.h"
#include "LCD.h"

Token shunting_yard(char inputArray[]) {
		debug_mess("BEGIN");
    Stack tokenStack;                                   //stacks used in each stage of conversion/calculation
    Stack outputStack;
    Stack operatorStack;
    Stack resultStack;
    int errorFlag = 0;                                  //int to hold error flags output by functions
		debug_mess("T");
    errorFlag = tokenise(&tokenStack, inputArray);      //tokenise user input
	debug_mess("S");
    shunt(&tokenStack, &operatorStack, &outputStack);   //convert to postfix
	debug_mess("R");
    errorFlag |= result(&outputStack, &resultStack);    //calculate result
    resultStack.stack[0].op = errorFlag;                //encode error flag
	debug_mess("RETURN");
    return resultStack.stack[0];                        //return result
}

int tokenise(Stack* t, char i[]) {
	
    int count = 0, decimalFlag = 0, decimalPlace = 1, valueFlag = 0;                        //Variables
    double tempVal = 0.0;
    Token tempToken = {-1, 0.0};

    stack_init(t);                                                                      //Initialise token stack
    while(i[count] != '\0') {
        if(i[count] <= 0x39 && i[count] >= 0x30) {                                      //If char is numeric
            add_digit(&tempVal, i[count] - '0', decimalFlag, &decimalPlace);            //Add digit to temp value
            valueFlag = 1;                                                              //Set value flag
        }
        else if(i[count] == '.') {                                                      //If char is decimal place
            decimalFlag = 1;                                                            //Set decimal flag
        }
        else {                                                                          //If char is operand
					debug_mess("OPERATION");
            tempToken.op = OPERAND;                                                     //Store operand and value in temp token
            tempToken.value = tempVal;
            if(count > 2 && (t->stack[t->stackCnt-1].op > 0 && t->stack[t->stackCnt-2].op > 0)) {                       //Double operator handling
              debug_mess("DBL OP");  
							int tmpOp = t->stack[t->stackCnt-1].op;                                 //Store 2nd operation before pop
                stack_pop(t);                                                           //Pop
                switch(tmpOp) {                                                         //Switch based on 2nd operation
                    case SUB: {                                   
												debug_mess("NEG DBL");;
                        tempToken.value = tempVal * -1.0;																//'Bake' negative into value, i.e (1  *  -  2) = (1  *  -2)
                        break;	
                    }
                    case ADD: {                                                         //Double addition has no impact
											debug_mess("ADD DBL");
                        break;
                    }
                    default: {                                                          //Anything else, return SYNTAX ERROR
											debug_mess("OTH DBL");
                        return ERR;
                    }
                }
            }
            if(valueFlag) {
                stack_push(t, tempToken);               //Push temp token to token stack
            }
            valueFlag = 0;                              //Reset flags for next token
            tempVal = 0.0;
            decimalFlag = 0;
            decimalPlace = 1;

            int opType = 0;                             //Push operation as token
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
        count++;                                            //Iterate through char array
    } 

    if(count > 2 && (t->stack[t->stackCnt-1].op > 0 && t->stack[t->stackCnt-2].op > 0)) {                       //Error handling for final operation
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

    tempToken.value = tempVal;                              //Push final operand
    tempToken.op = OPERAND;
    stack_push(t, tempToken);
    
    count = 0;                                              //Exponent handling
    while(count < 50) {
        if((t->stack[count].op == 0) && (t->stack[count+1].op == EXP) && (t->stack[count+2].op == 0)) {     //if (value - exp - value)
            if(t->stack[count+2].value == (int)t->stack[count+2].value) {                                   //if exponent is an integer
                t->stack[count].value = t->stack[count].value * int_exp(10, (int)t->stack[count+2].value);  //Calculate exponent and store
                t->stack[count+1].op = NUL;                                                                 //Empty other values
                t->stack[count+2].op = NUL;
            }
            else {
                return ERR;
            }
        }
        count++;
    }
    return 0;
}

void shunt(Stack* tokenS, Stack* opS, Stack* outS) {
    int c = 0;
		
//		debug_mess("SHUNTING");
    stack_init(opS);                                            //Initialise stacks
    stack_init(outS);

    while(tokenS->stack[c].op != -1) {                          //While end of stack not reached
        if(tokenS->stack[c].op != NUL) {                        //If token is not empty
            switch(tokenS->stack[c].op) {                       //switch whether token is operand or operation
                case OPERAND: {
                    stack_push(outS, tokenS->stack[c]);         //push to output stack
//										debug_mess("OPERAND");
                    break;
                }
                default: {
//										debug_mess("OPERATION");
                    if(get_associativity(tokenS->stack[c].op)) {
//											debug_mess("RIGHT ASS");
                        stack_push(outS, tokenS->stack[c]);     //Push to output stack if right associative (EXP)
                    }
                    else {  
//												debug_mess("LEFT ASS");											
                        while(opS->stackCnt > 0 && (get_precedence(opS->stack[opS->stackCnt-1].op) <= get_precedence(tokenS->stack[c].op))){     //While head of op stack is higher (lower) precedence, push to output then pop
//													debug_mess("POP");
                            stack_push(outS, opS->stack[opS->stackCnt-1]);
                            stack_pop(opS);
                        }
//												debug_mess("PUSH");
                        stack_push(opS, tokenS->stack[c]);      //Push op to op stack
                    }
                    break;
                }
            }   

        }
        c++;
    }
//		debug_mess("SHUNTED");

    while(opS->stackCnt > 0) {                                  //While operations remain in op stack
        stack_push(outS, opS->stack[opS->stackCnt-1]);          //push to output
        stack_pop(opS);                                         //Clear op stack
    }
}

int result(Stack* outS, Stack* resS) {
//		debug_mess("CALCULATING");
    int c = 0;

    stack_init(resS);

    while(outS->stack[c].op != -1) {

        if(outS->stack[c].op != NUL) {
            Token tmpA, tmpB, tmpR;
            if(outS->stack[c].op == 0) {
                stack_push(resS, outS->stack[c]);
            }
    
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
            c++;
        }
    }
//		debug_mess("CALCULATED");
    return 0;
}

void stack_pop(Stack* s) {
    if(s->stackCnt > 0) {
        s->stack[s->stackCnt].op = -1;
        s->stackCnt--;
    }
}

void stack_push(Stack* s, Token t) {
    if(s->stackCnt < sizeof(s->stack)/sizeof(Token)) {
        s->stack[s->stackCnt] = t;
        s->stackCnt++;
    }
}

void stack_init(Stack* s) {
    int i;
    s->stackCnt = sizeof(s->stack)/sizeof(s->stack[0]);
    for(i = 0; i < sizeof(s->stack)/sizeof(s->stack[0]); i++) {
        s->stack[i].op = -1;
        s->stack[i].value = 0.0;
    }
    s->stackCnt = 0;
}

int get_precedence(int op) {
    return ((op & 0xF0) >> 4);
}

int get_associativity(int op) {
    return ((op & 0xF00) >> 8);
}
