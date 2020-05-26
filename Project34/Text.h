#pragma once
#include "Stack.h"
#include <iostream>
#include <fstream>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

class TTextLink;
class TText;

class TTextMem{
	TTextLink *pFirst, *pLast, *pFree;
	friend class TTextLink;
};

class TTextLink{
public:
	TTextLink *pNext, *pDown;
	static TTextMem MemHeader;
	char str[81];
	TTextLink(const char *_str = NULL, TTextLink *_pNext = NULL, TTextLink *_pDown = NULL) {
		pNext = _pNext;
		pDown = _pDown;
		if (!_str) str[0] = '\0';
		else
			strcpy_s(str, _str);
	}
	~TTextLink() {}

	static void InitMem(int size){
		MemHeader.pFirst = (TTextLink*) new char[size * sizeof(TTextLink)];
		MemHeader.pFree = MemHeader.pFirst;
		MemHeader.pLast = MemHeader.pFirst + (size - 1);
		TTextLink *tmp = MemHeader.pFirst;
		for (int i = 0; i < size - 1; i++){
			tmp->str[0] = '\0';
			tmp->pNext = tmp + 1;
			tmp++;
		}
		MemHeader.pLast->str[0] = '\0';
		tmp->pNext = NULL;
	}

	void* operator new(size_t size){
		TTextLink *tmp = MemHeader.pFree;
		if (tmp)
			MemHeader.pFree = tmp->pNext;
		return tmp;
	}

	void operator delete(void* p){
		TTextLink *tmp = (TTextLink*)p;
		tmp->pNext = MemHeader.pFree;
		MemHeader.pFree = tmp;
	}

	static void TTextLink::MemCleaner(TText &txt, int& c);

	static void PrintFree(){
		TTextLink *tmp = MemHeader.pFree;
		if (!tmp)
			cout << "No free" << endl;
		else{
			int c = 0;
			cout << "Free lines:" << endl;
			while (tmp){
				if (tmp->str[0] != '\0')
					cout << tmp->str << endl;
				tmp = tmp->pNext;
				c++;
			}
			cout << c << endl;
		}
	}

};



class TText{
private:
	TTextLink *pFirst, *pCurr;
	TStack <TTextLink*> Stack;
public:
	TText();
	void GoFirstLink();		   
	void GoDownLink();		   
	void GoNextLink();		 
	void GoPrevLink();		  
	string GetLine();		    
	void SetLine(string s);	          
	void InsDownLine(string s);	  
	void InsDownSection(string s);	   
	void InsNextLine(string s);	    
	void InsNextSection(string s);	  
	void DelDownLine();		         
	void DelNextLine();	            	    
	void Read(string ifs);       
	void SaveText(string ifs);       
	TTextLink* ReadRec(ifstream& ifs);  
	void SaveSection(TTextLink *p, ofstream& ofs);  
	void PrintSection(TTextLink *p);    
	void PrintText(); 

	int Reset(void) {
		while (!Stack.IsEmpty()) Stack.Pop();
		pCurr = pFirst;
		if (pCurr) {
			Stack.Push(pCurr);
			if (pCurr->pNext) Stack.Push(pCurr->pNext);
			if (pCurr->pDown) Stack.Push(pCurr->pDown);
		}
		return IsEnd();
	}

	int IsEnd(void){
		return !Stack.getsize();
	}

	int GoNext(void) {
		if (!IsEnd()) {
			pCurr = Stack.Pop(); 
			if (pCurr != pFirst) {
				if (pCurr->pNext) Stack.Push(pCurr->pNext);
				if (pCurr->pDown) Stack.Push(pCurr->pDown);
			}
			return true;
		}
	return false;	
	}
};

TText::TText(){
	pFirst = NULL;
	Stack.Clear();
}

void TText::GoFirstLink(){
	pCurr = pFirst;
	Stack.Clear();
}

void TText::GoNextLink(){
	if (!pCurr) throw 1;
	if (!pCurr->pNext) return;
	Stack.Push(pCurr);
	pCurr = pCurr->pNext;
}

void TText::GoDownLink(){
	if (!pCurr) throw 2;
	if (!pCurr->pDown) return;
	Stack.Push(pCurr);
	pCurr = pCurr->pDown;
}

void TText::GoPrevLink(){
	if (!pCurr ) throw 3;
	if (Stack.IsEmpty()) return;
	pCurr = Stack.Pop();
}

void TText::SetLine(string str1){
	if (!pCurr) throw 4;
	strcpy_s(pCurr->str, str1.c_str());
}

string TText::GetLine(){
	return string(pCurr->str);
}

void TText::InsNextLine(string str1) {
	if (!pCurr) throw 5;
	TTextLink *p = new TTextLink(str1.c_str());
	p->pNext = pCurr->pNext;
	pCurr->pNext = p;
}

void TText::InsDownLine(string s){
	if (pCurr){
		TTextLink *p = new TTextLink(s.c_str());
		p->pNext = pCurr->pDown;
		pCurr->pDown = p;
	}
}

void TText::InsDownSection(string str) {
	TTextLink *p = new TTextLink(str.c_str());
	p->pDown = pCurr->pDown;
	pCurr->pDown = p;
}

void TText::InsNextSection(string s){
	if (pCurr){
		TTextLink *p = new TTextLink(s.c_str());
		p->pDown = pCurr->pNext;
		pCurr->pNext = p;
	}
}

void TText::DelDownLine(){
	if (pCurr)
		if (pCurr->pDown){
			TTextLink *p = pCurr->pDown;
			pCurr->pDown = p->pNext;
			delete p;
		}
}

void TText::DelNextLine(){
	if (pCurr)
		if (pCurr->pNext){
			TTextLink *p = pCurr->pNext;
			pCurr->pNext = p->pNext;
			delete p;
		}
}

TTextLink* TText::ReadRec(ifstream& ifs) {
	char _str[81];
	TTextLink *pHead, *p;
	pHead = new TTextLink;
	p = pHead = NULL;
	while (!ifs.eof()) {
		ifs.getline(_str, 80, '\n');
		if (_str[0] == '}') {
			break;
		}
		if (_str[0] == '{') { 
			p->pDown = ReadRec(ifs); 	
		}
		else {
			TTextLink *tmp = new TTextLink(_str);
			if (!pHead) pHead = p = tmp;
			else {
				p->pNext = tmp;
				p = tmp;
			}
		}
	}
	return pHead;
}

void TText::Read(string ifs){
	ifstream ifss(ifs);
	Stack.Clear();
	pFirst = ReadRec(ifss);
	pCurr = pFirst;
}

void TText::PrintSection(TTextLink *p){
	if (p){
		string s;
		int n = Stack.getsize()-1;
		for (int i = 0; i <= n; i++) {
			s += '	';
		}
		s += p->str;
		cout << s << endl;
		if (p->pDown){
			Stack.Push(p);
			PrintSection(p->pDown);
			Stack.Pop();
		}
		if (p->pNext) PrintSection(p->pNext);
	}
}

void TText::PrintText(){
	Stack.Clear();
	PrintSection(pFirst);		
}

void TText::SaveSection(TTextLink *p, ofstream& ofs){
	if (p){
		ofs << p->str << endl;
		if (p->pDown){
			ofs << '{' << endl;
			SaveSection(p->pDown, ofs);
			ofs << '}' << endl;
		}
		if (p->pNext) SaveSection(p->pNext, ofs);
	}
}

void TText::SaveText(string ifs){
	ofstream ofs(ifs);
	SaveSection(pFirst, ofs);
}


void TTextLink::MemCleaner(TText &txt, int &c) {
	c = 0;
	for (txt.Reset(); !txt.IsEnd(); txt.GoNext()) {
		string s = "&&&";
		s += txt.GetLine();
		txt.SetLine(s.c_str());
	}

	for (TTextLink *tmp = MemHeader.pFree; tmp; tmp = tmp->pNext) {
		strcpy_s(tmp->str, "&&&");
	}
	for (TTextLink *tmp = MemHeader.pFirst; tmp <= MemHeader.pLast; tmp++) {
		if (strstr(tmp->str, "&&&"))
			strcpy_s(tmp->str, tmp->str + 3);
		else { 
			delete tmp; 
			c++;
		}
	}
}
