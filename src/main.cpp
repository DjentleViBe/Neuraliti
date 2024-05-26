#include <stdio.h>
#include <iostream>
#include "../dependencies/include/uielements.h"

int main()
{
	sharedlibrary("add");
	INITgraphics();
    Displayloop();
	
	return 0;
}
