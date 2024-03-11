#include "book.h"

FILE *bookFile;

void openBook()
{
	bookFile = fopen("book.txt", "r");
	if (!bookFile)
		exit(17);
}

void closeBook()
{
	if (bookFile)
		fclose(bookFile);
	bookFile = NULL;
}

char currentLine[1000] = "";
int lineNum;

int bookMove()
{
    int move = 0;
	openBook();
	for(int i = 0;endOfFile;i++)
	{
		readLine(i);
		if(isInBook(bookLine))
		{
			move = currentLine[lineNum]*8;
			move += currentLine[lineNum+1];
			move <<= 8;
			move += currentLine[lineNum+2]*8;
			move += currentLine[lineNum+3];
		}
	}
}


int isInBook(char *bookFileLine)
{
	for (int i = 0; i < (signed int)strlen(bookFileLine);i++)
    {
		if (movesLine[i] == '\0' || movesLine[i] != bookFileLine[i])
			return 0;
    }
	return 1;
}
