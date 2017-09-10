#pragma once
//
// ListItem.h
// 
#include "../ListBarExpandButton.h"

class CListBarCloudExpandButton : public CListBarExpandButton
{
public:
	CListBarCloudExpandButton();
	~CListBarCloudExpandButton();
	
	virtual void		DoClick(TNotifyUI*		pNotify) ;				//������

	virtual void		Expand(bool bExpand);
protected:
	//override control
	virtual	void			DoInit();

};