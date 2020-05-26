#include "Text.h"
#include <conio.h>

using namespace std;

TTextMem TTextLink::MemHeader;

void main(){
	TTextLink::InitMem(50);
	try{
		TText t;
		int c;
		string str;
		int n;
		cout << "	Text Editor" << endl;
		cout << "Operation number:" << endl;
		cout << "1 - Print text on console" << endl;
		cout << "2 - Load new text" << endl;
		cout << "3 - Go to First" << endl;
		cout << "4 - Go to Next" << endl;
		cout << "5 - Go to Down" << endl;
		cout << "6 - Go to Prev" << endl;
		cout << "7 - Add a line to this level" << endl;
		cout << "8 - Add a line to down level" << endl;
		cout << "9 - Add a section to this level" << endl;
		cout << "10 - Add a section to down level" << endl;
		cout << "11 - Delete line from this level" << endl;
		cout << "12 - Delete line from down level" << endl;
		cout << "13 - Print current line" << endl;
		cout << "14 - Edit current line" << endl;
		cout << "15 - Save text to file" << endl;
		cout << "16 - Free Links" << endl;
		cout << "17 - Garbage" << endl;
		cout << "18 - Exit" << endl;
		do{
			cin >> n;
			switch (n){
			case 1: 
				cout << endl;
				t.PrintText();
				break;

			case 2: 
				//cout << "File's name: " << endl;
				t.Read("123.txt");
				TTextLink::MemCleaner(t, c);
				break;

			case 3: 
				t.GoFirstLink();
				break;

			case 4: 
				t.GoNextLink();
				break;

			case 5: 
				t.GoDownLink();
				break;

			case 6: 
				t.GoPrevLink();
				break;

			case 11:
				t.DelNextLine();
				cout << endl;
				break;

			case 12: 
				t.DelDownLine();
				cout << endl;
				break;

			case 13: 
				cout << t.GetLine() << endl;
				break;

			case 14: 
				cout << "Text: " << endl;
				cin >> str;
				t.SetLine(str);
				cout << endl;
				break;

			case 7: 
				cout << "Text: " << endl;
				cin >> str;
				t.InsNextLine(str);
				cout << endl;
				break;

			case 8: 
				cout << "Text: " << endl;
				cin >> str;
				t.InsDownLine(str);
				cout << endl;
				break;

			case 9: 
				cout << "Text: " << endl;
				cin >> str;
				t.InsNextSection(str);
				cout << endl;
				break;

			case 10: 
				cout << "Text: " << endl;
				cin >> str;
				t.InsDownSection(str);
				cout << endl;
				break;

			case 15: 
				cout << "File's name: " << endl;
				cin >> str;
				t.SaveText(str);
				break;

			case 16: 
				TTextLink::PrintFree();
				break;

			case 17:
				c = 0;
				TTextLink::MemCleaner(t, c);
				cout << "Deleted: " << c << endl;
				break;

			case 18: break;

			default: cout << "Enter from 1 to 18" << endl;
			}
			cout << endl;
		} while (n != 18);
	}

	catch (int n){
		cout << "exception " << n << endl;
	}
}
