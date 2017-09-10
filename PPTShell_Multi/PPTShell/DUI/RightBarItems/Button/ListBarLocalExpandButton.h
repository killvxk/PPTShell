#pragma once
//
// ListItem.h
// 
#include "../ListBarExpandButton.h"

class CListBarLocalExpandButton : public CListBarExpandButton
{
public:
	CListBarLocalExpandButton();
	~CListBarLocalExpandButton();
	
	virtual void		DoClick(TNotifyUI*		pNotify) ;				//������

	virtual void		Expand(bool bExpand);
protected:
	//override control
	virtual	void			DoInit();

};