#ifndef UTILITY_H
#define UTILITY_H

/**
 * Utility.h
 * ------------------
 * AUTHOR: Luke Coles
 * 
 * ------------------
 * Library providing various utility functions used throughout calculator
 * firmware.
 * ------------------
 * REFS:
 * 
 * Techie Delight, Implement strcpy() function in C,
 * <https://www.techiedelight.com/implement-strcpy-function-c/>
 * [accessed 12/01/2023]
 * 
 * GeeksForGeeks, Convert a floating point number to string in C,
 * <https://www.geeksforgeeks.org/convert-floating-point-number-string/>
 * [accessed 12/01/2023]
 * 
 * haccks, Does pow() work for int data type in C? [duplicate],
 * <https://stackoverflow.com/questions/29787310/does-pow-work-for-int-data-type-in-c>
 * [accessed 12/01/2023]
 * 
 * 
*/

/**
 * Copies value of char array into another char array
 * 
 * destination: char array to be copied into
 * source: char array to copy
*/
char* copy_string(char* destination, const char* source);

/**
 * Reverses the order of characters in a char array
 * 
 * str: string to be reversed
 * length: length of string
*/
void reverse_string(char* str, int length);

/**
 * Converts an integer variable into a char array for display on the LCD
 * and use in double_to_string
 * 
 * i: integer input
 * str: string buffer to store output into
 * d: additional 0s
*/
int int_to_string(int i, char* str, int d);

/**
 * Converts a double variable into a char array for display on the LCD
 * 
 * f: input double
 * str: string buffer to store output into
 * res: decimal resolution, number of significant figures
*/
void double_to_string(double f, char* str, int res);

/**
 * Returns the number of digits in an integer
 * i: input integer
*/
int get_int_length(int i);

/**
 * Calculates an integer exponent
 * 
 * base: base integer
 * exp: exponent
*/
unsigned int_exp(unsigned base, unsigned exp);

/**
 * Insert a digit into a double value, before or after the decimal place
 * 
 * val: value to insert digit into
 * digit: digit to insert
 * dF: decimal flag
 * dR: decimal resolution
*/
void add_digit(double* val, int digit, int dF, int* dR);

/**
 * Returns the absolute value of a double
 * 
 * in: double input value
*/
double double_absolute(double in);

#endif  // UTILITY_H