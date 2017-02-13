#include "WllTrace.h"

void f()
{
	FOOT();
	//FOOT_KEEPER(__FUNCTION__);
}

int main()
{
	f();
	return 0;
}

