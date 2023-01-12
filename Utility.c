#include "Utility.h"

/**
 * REF:
 * Techie Delight, Implement strcpy() function in C,
 * <https://www.techiedelight.com/implement-strcpy-function-c/>
 * [accessed 12/01/2023]
*/
char* copy_string(char* destination, const char* source) {
	char* ptr;
	// if destination is null, return 0
	if(destination == 0) {return 0;}
	// point to destination
	ptr = destination;
	// copy source string to destination, adding null terminator
	while((*destination++ = *source++) != '\0');
	*destination = '\0';
	// return destination
	return ptr;
}

/**
 * REF:
 * GeeksForGeeks, Convert a floating point number to string in C,
 * <https://www.geeksforgeeks.org/convert-floating-point-number-string/>
 * [accessed 12/01/2023]
*/
void reverse_string(char* str, int length) {
	int i = 0, j = length -1;
	char temp;
	// i loops through first half of string, j through second half (backwards)
	// swap characters
	// increment/decrement iterators
	while(i < j) {
			temp = str[i];
			str[i] = str[j];
			str[j] = temp;
			i++;
			j--;
	}
}

int int_to_string(int i, char str[], int d) {
	// get length of input integer
	int digit, count, length = get_int_length(i), isNegative = 0;
	// set negative flag if integer is negative
	if(i < 0) {isNegative = 1;}
	// get absolute value (remove negative)
	i = (int)double_absolute(i);
	// loop through integer
	// get remainder from 10 (last digit)
	// divide input by 10
	// insert digit into string as character
	for(count = 0; count < length; count++) {
			digit = i % 10;
			i = i / 10;
			str[count] = '0' + digit;
	}
	// insert negative sign to string if input was negative
	if(isNegative) {
		str[length] = '-';
		length++;
	}
	// reverse string
	reverse_string(str, length);
	// return length, used in double_to_string
	return length;
}

/**
 * GeeksForGeeks, Convert a floating point number to string in C,
 * <https://www.geeksforgeeks.org/convert-floating-point-number-string/>
 * [accessed 12/01/2023]
*/
void double_to_string(double f, char* str, int resolution) {
	int integerPart, length, isNegative = 0;
	double floatingPart;
	// extract integer and floating parts of double
	integerPart = (int)f; 
	floatingPart = double_absolute(f - (double)integerPart);
	// get length of integer part and convert to string
	length = int_to_string(integerPart, str, 0);
	// if decimal places are required, add decimal place
	// shift floating part to be integer with digits == resolution
	// convert to string, adding to buffer after decimal place
	if(resolution != 0) {
			str[length] = '.';
			floatingPart = floatingPart * int_exp(10, resolution);
			int_to_string(floatingPart, str + length + 1, 0);
	}
}

int get_int_length(int i) {
	int count = 1;  // stores length
	// get absolute value
	i = (int)double_absolute(i);
	// loop through digits, dividing input by 10 and incrementing counter
	while(i > 9) {
		i = i / 10;
		count++;
	}
	// return number of digits
	return count;
}

/**
 * REF:
 * haccks, Does pow() work for int data type in C? [duplicate],
 * <https://stackoverflow.com/questions/29787310/does-pow-work-for-int-data-type-in-c>
 * [accessed 12/01/2023]
*/
unsigned int_exp(unsigned base, unsigned exp) {
	// initialised at 1 to avoid times by 0
	unsigned result = 1;
	// while exp > 0
	while(exp) {
		// if exp is odd, result*base
		if(exp % 2) {
			result = result * base;
		}
		exp = exp / 2;
		base = base * base;
	}
	// return result
	return result;
}

void add_digit(double* val, int digit, int dF, int* dR) {
	// switch whether decimal place is added
	switch(dF) {
		// if there is no decimal place, shift 1 decimal digit left (*10)
		// add new digit
		default: {
				*val *= 10;
				*val += digit;
				break;
		}
		// if there is a decimal place, digit x10^decimal resolution
		// add new value
		// increment decimal resolution
		case 1: {
				*val += (double)digit / int_exp(10, *dR);
				*dR += 1;
				break;
		}
	}
}

double double_absolute(double in) {
	// if input is positive:
	// 		input is multiplied by (1)
	//		input remains positive
	// if input is negative:
	// 		input is multiplied by (-1)
	//		input becomes positive
	return in * ((in > 0) - (in < 0));
}