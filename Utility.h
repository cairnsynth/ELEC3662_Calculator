#ifndef UTILITY_H
#define UTILITY_H

char* copy_string(char* destination, const char* source);
void reverse_string(char* str, int length);
int int_to_string(int i, char* str, int d);
void double_to_string(double f, char* str, int res);
int get_int_length(int i);
unsigned int_exp(unsigned x, unsigned y);
void add_digit(double* val, int digit, int dF, int* dR);
double double_absolute(double in);

#endif