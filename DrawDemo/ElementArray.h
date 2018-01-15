#pragma once

#include "Element.h"

/* Element
	CTypedPtrArray ��ָ���б��ɴ��� CObject ��������ʵ�岢���л�
		Refrence: https://msdn.microsoft.com/zh-cn/library/sdbk3yw6(v=vs.120).aspx#%E6%88%90%E5%91%98

	����
		CTypedPtrArray<CObArray, Element *>
	��������
*/

class ElementArray: public CTypedPtrArray<CObArray, Element *> {
// ����
public:
	ElementArray();
	~ElementArray();
	//bool HasElement(Element & content);	// ����ͼ���Ƿ����б��У����÷�����
	bool HasElement(Element * pContent);	// ����ͼ���Ƿ����б��У�ָ�뷽����
	bool Remove(Element * pContent);		// ɾ��ͼ��
};

