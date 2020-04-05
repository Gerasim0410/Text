#include "Stack.h"
#include "Text.h"
#include <conio.h>

int main() {
	TText t;
	t.Read("123.txt");
	t.PrintText();
	int k = 1, str = 10;
	for (t.Reset(); !t.IsEnd(); t.GoNext()) {
		if (k == str) {
			cout << t.GetLine() << endl;
			break;
		}
		k++;
	}

	_getch();
	return 0;
}
