#include"extras.h"

void delay(int ms)
{
	clock_t end = clock() +	ms*CLOCKS_PER_SEC/1000;
	while(end > clock());
}
