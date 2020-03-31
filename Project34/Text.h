#pragma once
#include "Stack.h"
#include <iostream>
#include <fstream>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;



class TTextLink
{
public:
	TTextLink *pNext, *pDown;
	char str[100];
	TTextLink(const char *_str = NULL, TTextLink *_pNext = NULL, TTextLink *_pDown = NULL) {
		pNext = _pNext;
		pDown = _pDown;
		if (!_str) str[0] = '\0';
		else
			strcpy_s(str, _str);
	}
	~TTextLink() {}
};


class TText
{
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
	void DelDownSection();	           
	void DelNextLine();	            
	void DelNextSection();		  	    
	void Read(string f_name);       
	void SaveText(string f_name);       
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
	if (!pCurr) throw - 1;
	if (!pCurr->pNext) return;
	Stack.Push(pCurr);
	pCurr = pCurr->pNext;
}

void TText::GoDownLink(){
	if (!pCurr) throw - 1;
	if (!pCurr->pDown) return;
	Stack.Push(pCurr);
	pCurr = pCurr->pDown;
}

void TText::GoPrevLink(){
	if (!pCurr ) throw - 1;
	if (Stack.IsEmpty()) return;
	pCurr = Stack.Pop();
}

void TText::SetLine(string str1){
	if (!pCurr) throw - 1;
	strcpy_s(pCurr->str, str1.c_str());
}

string TText::GetLine(){
	return string(pCurr->str);
}


void TText::InsNextLine(string str1) {
	if (!pCurr) throw - 1;
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
			pCurr->pDown = pCurr->pDown->pNext;
			delete p;
		}
}

void TText::DelDownSection(){
	if (pCurr)
		if (pCurr->pDown){
			TTextLink* p1 = pCurr->pDown;
			TTextLink* p2 = p1->pNext;
			pCurr->pDown = p2;
		}
}

void TText::DelNextLine(){
	if (pCurr)
		if (pCurr->pNext){
			TTextLink *p = pCurr->pNext;
			pCurr->pNext = pCurr->pNext->pNext;
			delete p;
		}
}

void TText::DelNextSection(){
	if (pCurr)
		if (pCurr->pNext){
			TTextLink* p1 = pCurr->pDown;
			TTextLink* p2 = p1->pNext;
			pCurr->pNext = p2;
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
			Stack.Pop();
			break;
		}
		if (_str[0] == '{') { 
			Stack.Push(p);
			p->pDown = ReadRec(ifs); 	
		}
		else {
			string s;
			int n = Stack.getsize();
			for (int i = 0; i <= n; i++) {
				s += ' ';
			}
			s += _str;
			const char *st = s.c_str();
			TTextLink *tmp = new TTextLink(st);
			if (!pHead) pHead = p = tmp;
			else {
				p->pNext = tmp;
				p = tmp;
			}
		}
	}
	return pHead;
}

void TText::Read(string f_name){
	ifstream ifs(f_name);
	Stack.Clear();
	pFirst = ReadRec(ifs);
	pCurr = pFirst;
}

void TText::PrintSection(TTextLink *p){
	if (p){
		cout << p->str << endl;
		if (p->pDown){
			PrintSection(p->pDown);
		}
		if (p->pNext) PrintSection(p->pNext);
	}
}

void TText::PrintText() { PrintSection(pFirst); }

void TText::SaveSection(TTextLink *p, ofstream& ofs){
	if (p){
		ofs << p->str << endl;
		if (p->pDown){
			ofs << '{';
			SaveSection(p->pDown, ofs);
			ofs << '}';
		}
		if (p->pNext) SaveSection(p->pNext, ofs);
	}
}

void TText::SaveText(string f_name){
	ofstream ofs(f_name);
	SaveSection(pFirst, ofs);
}



