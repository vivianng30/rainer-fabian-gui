/**********************************************************************************************//**
 * \file	ParaDataPRESET.h.
 *
 * Declares the para data preset class
 **************************************************************************************************/

#pragma once
#include "paradata.h"

class CParaDataPRESET :
	public CParaData
{
public:
	

	
	static CParaDataPRESET* GetInstance();
	static void DestroyInstance();

	void Init();
	//void SetData(eVentMode mode);

private:
//singleton
	static CParaDataPRESET* thePRESETData;

protected:
	CParaDataPRESET(void);
	virtual ~CParaDataPRESET(void);

	
};
