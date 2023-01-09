#include "Utility.h"

                                                                    //REF: https://www.techiedelight.com/implement-strcpy-function-c/
char* copy_string(char* destination, const char* source) {
	char* ptr;                                                       //Member declaration

	if(destination == 0) {return 0;}                                //If destination is null, return 0
	ptr = destination;                                              //pointer to destination
	while((*destination++ = *source++) != '\0');                    //Copy source string to destination
	*destination = '\0';                                            //terminating null character
	return ptr;                                                     //return destination
}

                                                                    //REF: https://www.geeksforgeeks.org/convert-floating-point-number-string/
void reverse_string(char* str, int length) {
	int i = 0, j = length -1;                                       //member declaration
	char temp;

	while(i < j) {                                                  //Loop through half of string
			temp = str[i];                                              //Store current char in temp var
			str[i] = str[j];                                            //flip char in string
			str[j] = temp;
			i++;                                                        //Increment iterators
			j--;
	}
}

int int_to_string(int i, char str[], int d) {
	int digit, count, length = get_int_length(i), isNegative = 0;         //Member declaration, get length of input integer

	if(i < 0) {isNegative = 1;}
	i = (int)double_absolute(i);
	for(count = 0; count < length; count++) {                       //Loop through integer
			digit = i % 10;                                             //Get remainder from 10 / last digit
			i = i / 10;                                                 //Divide by 10
			str[count] = '0' + digit;                                   //Add digit to string as character
	}
	if(isNegative) {
		str[length] = '-';
		length++;
	}
	reverse_string(str, length);                                    //Reverse string
	//str[length] = '\0';                                             //Add terminating null character
	return length;                                                  //Return length
}

                                                                    //REF: https://www.geeksforgeeks.org/convert-floating-point-number-string/
void double_to_string(double f, char* str, int resolution) {
	int integerPart, length, isNegative = 0;                                        //Member variables
	double floatingPart;
	
	integerPart = (int)f;                                           //Extract integer part
	floatingPart = double_absolute(f - (double)integerPart);                          //Extract floating part (after decimal point)
	length = int_to_string(integerPart, str, 0);                    //get length of integer part and convert to string

	if(resolution != 0) {                                                       //If decimal places required
			str[length] = '.';                                                      //Add decimal point
			floatingPart = floatingPart * int_exp(10, resolution);                  //Shift floating part to be an integer
			int_to_string(floatingPart, str + length + 1, resolution);         //Convert floating part to string, adding it to buffer
	}
}

int get_int_length(int i) {
	int count = 1;                                                  //Int to store digit count
	i = (int)double_absolute(i);
	while(i > 9) {                                                  //Loop through digits
		i = i / 10;                                                 //Divide by 10
		count++;                                                    //Increment number of digits
	}
	return count;                                                   //Return number of digits
}

                                                                    //REF: https://stackoverflow.com/questions/29787310/does-pow-work-for-int-data-type-in-c
unsigned int_exp(unsigned base, unsigned exp) {
	unsigned result = 1;                                            //Var to store result

	while(exp) {                                                    //While exp > 0
			if(exp % 2) {                                               //If exp is odd
					result = result * base;                                 //Result*base
			}
			exp = exp / 2;                                              //exp/2
			base = base * base;                                         //base^2
	}
	return result;                                                  //Return result
}

void add_digit(double* val, int digit, int dF, int* dR) {
	switch(dF) {
		default: {																									//Not decimal
				*val *= 10;																							//Shift 1 decimal digit left
				*val += digit;																					//Add new digit
				break;
		}
		case 1: {																										//Decimal
				//*val *= int_exp(10, *dR);															  //Shift float to int
				*val += (double)digit / int_exp(10, *dR);																					//Add new digit
				//*val /= int_exp(10, *dR);																//Convert back to float
				*dR += 1;																								//Increment decimal place counter
				break;
		}
	}
}

double double_absolute(double in) {
	return in * ((in > 0) - (in < 0));
}