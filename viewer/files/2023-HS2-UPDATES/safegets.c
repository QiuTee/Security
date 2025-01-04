/*FILE safegets.c
* Author: J Hamlyn-Harris
* Last Modified 25/10/2010
*/
#include <stdio.h>
#include <string.h>
#define SIZE 12
/*fgets reads characters from the required stream up to character SIZE-1.
* It then appends a NULL
* if fewer than SIZE-1 characters are input, a NEWLINE character terminates 
* input and is appended to the string, followed by a NULL
*/

void fixgets(char* text, const int SZE);

int main()
{
	char name[SIZE];
		
	printf("Enter string: ");
	fgets(name, SIZE, stdin);
	fixgets(name, SIZE);

	printf("Hello %s\n", name);

	return 0;
}

/*Flushes the keyboard buffer and corrects fgets input
* @param text - pointer to c-string filled by fgets
* @param SZE - max length of text
*/ 
void fixgets(char* text, const int SZE)
{
	char ch;
	if (strlen(text) >=  (unsigned)SZE - 1) 
	{
		/*character limit reached. Last char is NULL*/
		/*time to flush input*/
		do { ch = getchar();} while (ch != '\n' && ch != EOF);
	}
	else 
	{
		/*Enter entered. last char is NULL, 2nd-last char is NEWLINE*/
		/*overwrite penultimate char with NULL*/
		text[strlen(text) - 1] = '\0';
		/*no need to fluch buffer*/
	}
	return;
}

