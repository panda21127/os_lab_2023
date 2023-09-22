#include "swap.h"

void Swap(char *left, char *right)
{
	char w = *left;
	*left=*right;
	*right=w;
}
