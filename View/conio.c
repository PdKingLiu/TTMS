#include "conio.h"
#include<stdio.h>

char * fget_string (char * string, int n, FILE *stream)
{
	char * temp;

	fgets (string, n, stream);
	temp = strchr (string, '\n');
	if (*temp)
		*temp = '\0';

	return string;
}



