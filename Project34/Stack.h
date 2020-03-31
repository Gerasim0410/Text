#pragma once
#include <iomanip>
#include <string>

using namespace std;

const int MaxStackSize = 100;

template <class T>
class TStack {
	T* arr;
	int size;
	int num;

public:
	TStack(int _size = 100) {
		size = _size;
		num = -1;
		if ((size < 0) || (size > MaxStackSize))
			throw "size";
		arr = new T[size];
	}

	TStack(const TStack& st) {
		size = st.size;
		num = st.num;
		arr = new T[size];
		for (int i = 0; i < size; i++) {
			arr[i] = st.arr[i];
		}
	}

	~TStack() {
		delete[] arr;
	}

	void Push(const T& val) {
		if (IsFull()) throw 1;
		arr[++num] = val;
	}

	T Pop(void) {
		if (IsEmpty()) throw 2;
		return arr[num--];
	}

	int getsize() {
		return num;
	}

	bool IsEmpty() {
		return num == -1;
	}

	bool IsFull() {
		return num == size;
	}
	void Clear() {
		num = -1;
	}

	T Top(){
		if (num == -1) throw -1;
		return arr[num];
	}
};

