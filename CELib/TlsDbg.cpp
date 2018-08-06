
#include "stdafx.h"
#include "TlsDbg.h"

#ifdef MEMDEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CTlsDebugHeap::CTlsDebugHeap()
{
  InitializeCriticalSection(&m_cs);
}

CTlsDebugHeap::~CTlsDebugHeap()
{
  DumpMemoryLeaks();
  DeleteCriticalSection(&m_cs);
}

void CTlsDebugHeap::AddTrace(DWORD addr,  DWORD asize,  const char *fname, DWORD lnum)
{
  EnterCriticalSection(&m_cs);

  ALLOCATED_INFO *info;
  
  if(!allocatedList) {
    allocatedList = new(AllocatedList);
  }
  
  info = new(ALLOCATED_INFO);
  info->address = addr;
  strncpy(info->fileName, fname, FILENAMELEN-1);
  info->lineNumber = lnum;
  info->size = asize;
  //cout<<info->fileName<<":"<<info->lineNumber<<endl;
  allocatedList->AddHead(info);

  LeaveCriticalSection(&m_cs);
}

void CTlsDebugHeap::RemoveTrace(DWORD addr)
{
  EnterCriticalSection(&m_cs);

  POSITION i;

  if(!allocatedList)
    return;
  for(i = allocatedList->GetHeadPosition(); i != NULL; allocatedList->GetNext(i))
  {
    if(allocatedList->GetAt(i)->address == addr)
    {
      allocatedList->RemoveAt(i);
      break;
    }
  }

  LeaveCriticalSection(&m_cs);
}

void CTlsDebugHeap::DumpMemoryLeaks()
{
  EnterCriticalSection(&m_cs);

  POSITION i;
  DWORD totalSize = 0;
  char buf[1024];
  
  if(!allocatedList)
    return;
  
  for(i = allocatedList->GetHeadPosition(); i; allocatedList->GetNext(i)) 
  {
    ALLOCATED_INFO* p = allocatedList->GetAt(i);
    char* psz = (char*)p->address;
    *(psz+12) = '\0';
    sprintf(buf, "%s(%d): \t\tADDRESS %d\t%d NOTFREED\"%s\"\n",
      p->fileName, p->lineNumber, p->address, p->size, psz);
    
    totalSize += p->size;
  }
  sprintf(buf, "\n-----------------------------------------------------------\n");
  
  if(!totalSize) 
  {
    sprintf(buf,"There are no MEMORY LEAKS\n");
    
  }
  else
  {
  	sprintf(buf, "Total UNFREED MEMORY: %d bytes\n", totalSize);
    
  }

  LeaveCriticalSection(&m_cs);
}

CTlsDebugHeap& CTlsDebugHeap::GetInstance()
{
  static CTlsDebugHeap heap;
  return heap;
}

#endif

/*=============================================================================
    E O F
=============================================================================*/
