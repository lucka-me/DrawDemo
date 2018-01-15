#pragma once

#include "Element.h"

/* Element
	CTypedPtrArray 是指针列表，可储存 CObject 及其子类实体并序列化
		Refrence: https://msdn.microsoft.com/zh-cn/library/sdbk3yw6(v=vs.120).aspx#%E6%88%90%E5%91%98

	基类
		CTypedPtrArray<CObArray, Element *>
	新增方法
*/

class ElementArray: public CTypedPtrArray<CObArray, Element *> {
// 方法
public:
	ElementArray();
	~ElementArray();
	//bool HasElement(Element & content);	// 查找图形是否在列表中（引用方法）
	bool HasElement(Element * pContent);	// 查找图形是否在列表中（指针方法）
	bool Remove(Element * pContent);		// 删除图形
};

