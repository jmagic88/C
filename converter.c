/*
 * This program is meant to convert numbers from one base to another. The 
 * bases can vary anywhere between 2 and 25. The program is run from the
 * command line with the number, its base, and the desired base.
 *
 * Program written on Saturday 7 February 2015 by Eric Warden for CSC205.
 *
 */ 



/*
 * Stack that holds the information being converted by the converter. Its
 * meant to hold the string parameters until the time that they are needed
 * to perform the conversion. It holds a string rather than converting to an
 * because of the characters that could possibly appear if the base is larger
 * than 10. As a result, the values in the stack will be converted only for 
 * base conversion then will be converted back into a string to be placed in
 * the stack.
 */

#include <stdio.h>
#include <string.h>

#define STACK_MAX 100

struct Stack{
   int data[STACK_MAX];
   int size;
};

typedef struct Stack Stack;

//Initiate the stack with a size of 0.
void initStack(Stack *stack){stack->size = 0;}


//Pushes an int onto the top of the stack
void pushToStack(Stack *stack, int item){
   if(stack->size < STACK_MAX){stack->data[stack->size++] = item;}
   else{fprintf(stderr, "Error: Stack is full!\n\n");}
}

//Pops the top item off the stack and reduces the size counter.
int popTheStack(Stack *stack){
   if(stack->size == 0){fprintf(stderr, "Error: Stack is empty!\n\n");}
   else{
      stack->size--;
      return stack->data[stack->size];
   }
}
//Function to raise a base to a power
int ipow(int base, int size){
   int result = 1;
   for(size; size > 0; size--){result *= base;}
   return result;
}


//Converts a base of 11 - 25 to a decimal number.
int highBaseToDecimal(char value[], int base, int target){
	char digit;
	int sizeOfValue, sizeOfLetters, i, j, num;

	Stack stack;

	initStack(&stack);

	static char answer[16];
	char letters[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};

	sizeOfValue = strlen(value);
	sizeOfLetters = strlen(letters);
	
	//Go through the string and examine each character.
	for(i = 0; i < sizeOfValue; i++){
		num = 0;		
		digit = value[i];
		
		//Search the array of characters for the corresponding value plus 10.
		for(j = 0; j < sizeOfLetters; j++){
			if(letters[j] == digit){
				num = j + 10;
				break;
			}
		}

		//The only way num could be anything other than zero is if a character was found.
		if(num > 0){pushToStack(&stack, num);}
		else{
			num = value[i] - '0';			
			pushToStack(&stack, num);
		}		
	}

	//Pop the values off the stack and multiply them by the base raised to a power i.
	num = 0, i = 0;
	while(stack.size > 0){
		num += popTheStack(&stack) * ipow(base, i);
		i++;
	}	
	return num;
}

//Converts a decimal number into a number with a base from 11 - 25.
char * decimalToHighBase(int num, int base, int target){
	char letters[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};

	static char answer[16];
	int quotient, remainder, count = 0, size = 0;
	

	while(num > 0){
		quotient = num / target;
		remainder = num % target;

		if(remainder > 9){
			answer[count] = letters[remainder - 10];
			count++;
		}
		else{
			answer[count] = remainder + '0';
			count++;
		}
		num = quotient;
		size++;
	}
	
	//The answer string is backwards and needs to be reversed.
	char temp[size];	
	int i;
	for(i = 0; i < size; i++, count--){
		temp[i] = answer[count - 1];
	}
	
	strcpy(answer, temp);
	return answer;
}

/*
 * Function to convert a base of less than 10 into another base of less than ten or to 
 * a decimal. 
 */
int lowBaseToDecimal(char num[], int base, int target){
	Stack stack;

	initStack(&stack);	

	int i = 0, size = strlen(num), ans = 0;

	for(size; size > 0; size--){pushToStack(&stack, num[i++] - '0');}
	
	i = 0;
   while(stack.size > 0){ans += popTheStack(&stack) * ipow(base, i++);}
	return ans;
}

/*
 * Converts decimal number to number of a lower base.
 */
char * decimalToLowBase(int num, int base, int target){
	Stack stack;
	int value, size;
	static char answer[16];

	initStack(&stack);

	while(num > 0){
		pushToStack(&stack, (num % target));
		num /= target;
		size++;
	}

	int i = 0;
	char temp[size];
	while(stack.size > 0){
		value = popTheStack(&stack) + '0';
		temp[i++] = value;		
	}

	strcpy(answer, temp);
	return answer;
}








//Main function for the converter
int main(int argc, char *argv[]){

	char strNum[16];
	int num, base, target, size, i;
	base = atoi(argv[2]);
	target = atoi(argv[3]);

	Stack stack;

	initStack(&stack);

	if(argc != 4){fprintf(stderr, "usage: <number> <base of number> <target base>");}

	if(base > 10 && target == 10){
		num = highBaseToDecimal(argv[1], base, target);
		printf("%s (base %d) = %d (base %d)\n", argv[1], base, num, target);
	}
	else if(base == 10 && target > 10){		
		num = atoi(argv[1]);
		strcpy(strNum, decimalToHighBase(num, base, target));
		printf("%d (base %d) = %s (base %d)\n", num, base, strNum, target);		
	}
	else if(base < 10 && target == 10){
		num = lowBaseToDecimal(argv[1], base, target);
		printf("%s (base %d) = %d (base %d)\n", argv[1], base, num, target); 
	}
	else if(base == 10 && target < 10){
		num = atoi(argv[1]);
		strcpy(strNum, decimalToLowBase(num, base, target));
		printf("%s (base %d) = %s (base %d)\n", argv[1], base, strNum, target);
	}
	else if(base < 10 && target > 10){
		num = lowBaseToDecimal(argv[1], base, target);
		strcpy(strNum, decimalToHighBase(num, base, target));
		printf("%d (base %d) = %s (base %d)\n", argv[1], base, strNum, target);		
	}
	else if(base > 10 && target < 10){
		num = highBaseToDecimal(argv[1], base, target);
		strcpy(strNum, decimalToLowBase(num, base, target));
		printf("%d (base %d) = %s (base %d)\n", argv[1], base, strNum, target);		
	}	
	return 0;
}
