#include "Stack.h"
#include "Text.h"
#include <conio.h>

int main() {
	TText t;
	t.Read("123.txt");
	t.PrintText();
	_getch();
	return 0;
}