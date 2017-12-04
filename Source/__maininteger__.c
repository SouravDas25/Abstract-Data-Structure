
#include "Ads\integer.c"

int main()
{
	integer a = newinteger("99"),b = newinteger("999"),c;
	
	printinteger(a);
	printinteger(b);
	c = integer_subtraction(a,b);
	printinteger(c);
	
	freeinteger(a);
	freeinteger(b);
	freeinteger(c);
	//Garbagecheck();
	return 0;
}

