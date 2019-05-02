#include "stdafx.h"
#define SIZE 300 // this is the maximim size of the input string; the user should not
// input more than 100 characters. If he does, the programme still works as long as
// the input is no more than 200 characters, but asks the user to input the name again

void main() {   
	char msgProgrammeScope[] = "This programme accepts a string input of a name and outputs its reverse and lenght";
	char msgEnterName[] = "\nEnter the complete name (max 100 characters): ";
	char msgLongInput[] = "The entered name is too long. At most 100 characters allowed. Please try again...\n";
	char msgVoidInput[] = "The entered name is empty. At least 1 character allowed. Please try again...\n";
	char msgReverseString[] = "\nYour name in reverse is: ";
	char msgStringLenght[] = "\nName's lenght (number of characters): ";
	char msgIntegerToExit[] = "\n\nEnd of program.\nEnter any integer to exit ";
	char format[] = "%d";
	int savedESP;		// used to save ESP in case the input is too long
	int letterCounter;	// this will be the actual number of entered letters
	int waitUserInput;

	char name[SIZE]; // char array containing the entered name
	memset(name, 0, SIZE); // I inizialize it so it does not contain random values
	char nameRev[SIZE]; // char array containing the name in reverse
	memset(nameRev, 0, SIZE); // I inizialize it too

	_asm {
	// save the original value of ESP to use later in case the input is too long
		mov		savedESP, esp
	// print the intro message
		lea		eax, msgProgrammeScope	// loading the message's effective address
		push	eax						// onto the stack to call the function printf
		call	printf					// printing the intro message (scope of the
		add		esp, 4					// programme)
		jmp		INPUT_NAME

	LONG_INPUT:	// PRINT ERROR MESSAGE AND RE-ENTER NAME
		mov		esp, savedESP			// restore ESP to its original value, so
		lea		eax, msgLongInput		// that the stack does not get corrupted
		push	eax
		call	printf					// message containing error for long name
		add		esp, 4
		jmp		INPUT_NAME
	VOID_INPUT:	// PRINT ERROR MESSAGE AND RE-ENTER NAME
		lea		eax, msgVoidInput		// message containing error for void name
		push	eax
		call	printf
		add		esp, 4

	INPUT_NAME:	// INPUT THE NAME (CHAR ARRAY)
		lea		eax, msgEnterName
		push	eax
		call	printf					// promt user to input the name
		add		esp, 4
	// get user's input into name array
		mov		eax, SIZE-1				// load the actual lenght of the "name" array
		push	eax						// in eax and push it onto the stack;
		lea		eax, name				// "gets_s(name[], SIZE-1)" this is how the function
		push	eax						// call looks like in C. This function accepts as
		call	gets_s					// parameters the address of the array and its lenght;
		add		esp, 8					// it allows the user to input the string
																				
	// push the char array onto the stack
		mov		ecx, SIZE-1				// I want to loop at most a number of times equal
		mov		esi, 0					// to the actual lenght of the array
		mov		letterCounter, 0		// reset the letter counter
	PUSH_LOOP:
		movzx	eax, name[esi]			// movzx moves the value in name[esi] into eax and
		cmp		eax, 0					// fills the empty spaces (3 bytes) with zeros;
		je		CHECK_INPUT				// if the value of name[esi] is 0, I want to exit
		push	eax						// the loop because the string is finished;
		inc		esi						// if the value in name is not 0, I push it, I increment
		inc		letterCounter			// the letterCounter (=lenght of the array with letters)
		loop	PUSH_LOOP

	CHECK_INPUT:	// CHECK FOR INPUT ERRORS (VOID OR TOO LONG)
		mov		eax, letterCounter
		cmp		eax, 0
		je		VOID_INPUT				// if letterCounter = 0, jump to VOID_INPUT
		cmp		eax, 100				// 100 is the maximum allowed lenght of the name
		jg		LONG_INPUT				// if letterCounter > 100, jump to LONG_INPUT

	// retrieve the char array from the stack and copy it into nameRev
		mov		ecx, letterCounter		// this time I want to loop as many times as
		mov		esi, 0					// how many letters I actually have in the stack
	POP_LOOP:
		pop		eax						// load the value on top of the stack onto eax
		mov		nameRev[esi], al		// al = first 8 bits of eax, so it contains only
		inc		esi						// the character (which is 8 bits), no zeros
		loop	POP_LOOP

	// print out the reversed string
		lea		eax, msgReverseString
		push	eax
		call	printf					// print the message for the reversed string
		add		esp, 4

		lea		eax, nameRev
		push	eax
		call	printf					// print the reversed string
		add		esp, 4
			
	// print out the string lenght
		lea		eax, msgStringLenght
		push	eax
		call	printf					// print message of string lenght
		add		esp, 4

		mov		eax, letterCounter		// letterCounter is the number of characters
		push	eax						// entered into the array
		lea		eax, format
		push	eax
		call	printf					// print the lenght of the string (letterCounter)
		add		esp, 8

	// wait before exiting
		lea		eax, msgIntegerToExit
		push	eax
		call	printf					// prompt the user to enter an integer to exit
		add		esp, 4

		lea		eax, waitUserInput
		push	eax
		lea		eax, format				// as the input is an integer, I must push the format
		push	eax						// for decimal numbers;
		call	scanf_s					// wait for the user to input an integer
		add		esp, 8
	}
}