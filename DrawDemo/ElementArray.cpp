#include "stdafx.h"
#include "ElementArray.h"

ElementArray::ElementArray() {
}

ElementArray::~ElementArray() {
}

/*
bool ElementArray::HasElement(Element & content) {
	bool result = HasElement(&content);
	return result;
}
*/

bool ElementArray::HasElement(Element * pContent) {
	bool result = false;
	for (int i = 0; i < GetSize(); i++) {
		if (GetAt(i) == pContent) {
			result = true;
			break;
		}
	}
	return result;
}

bool ElementArray::Remove(Element * pContent) {
	bool result = false;
	for (int i = 0; i < GetSize(); i++) {
		if (GetAt(i) == pContent) {
			RemoveAt(i);
			result = true;
			break;
		}
	}
	return result;
}