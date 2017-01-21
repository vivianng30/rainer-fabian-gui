#pragma once
#include "interface.h"
#include "globDefs.h"

class CInterfaceSocket :
	public CInterface/*, CCESocket*/
{
public:
	static CInterfaceSocket* GetInstance();
	static void DestroyInstance();

	bool Init();
	void Deinit();

	bool Initialize();


	
private:
	//singleton
	static CInterfaceSocket* theSocketInterface;



protected:
	CInterfaceSocket(void);
	virtual ~CInterfaceSocket(void);





protected:


protected:	


};
