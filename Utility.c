#include "Utility.h"

/*Function to copy given string to another string*/
//https://www.techiedelight.com/implement-strcpy-function-c/
char* copy_string(char* destination, const char* source) {
	if(destination == 0) {return 0;}
	char* ptr = destination;
	while((*destination++ = *source++) != '\0');
	*destination = '\0';
	return ptr;
}

void int_to_string(int i, char* str) {
	int length = get_int_length(i), count;
	char tmpString[length], outString[length+1];
	for(count = 0; count < length; count++) {
		tmpString[count] = (i % 10) + '0';
		i = i / 10;
	}
	for(count = 0; count < length; count++) {
		outString[count] = tmpString[length - count];
	}
	outString[length] = '\0';
	
	copy_string(str, outString);
}

void float_to_string(float f, char* str, int res) {
	
}

int get_int_length(int i) {
	int count = 1;
	while(i > 9) {
		i = i / 10;
		count++;
	}
	return count;
}
