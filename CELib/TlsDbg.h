
#if !defined(__TLSDBG_H__)
#define __TLSDBG_H__


//#define AIS_MEMDEBUG

#ifdef AIS_MEMDEBUG


#undef DEBUG_NEW
#define DEBUG_NEW ::new(__FILE__, __LINE__)
#define DEBUG_MALLOC(a) dbgmalloc(a, __FILE__, __LINE__)
#define DEBUG_FREE(a) dbgfree(a)
#define DEBUG_REALLOC(a, b) dbgrealloc(a, b, __FILE__, __LINE__)
#define DEBUG_CALLOC(a, b) dbgcalloc(a, b, __FILE__, __LINE__)

#define FILENAMELEN 128

class CTlsDebugHeap
{
  // Tracing Utility 
  //typedef unsigned long DWORD;
  struct ALLOCATED_INFO{
    DWORD	address;
    DWORD	size;
    char	fileName[FILENAMELEN];
    DWORD	lineNumber;
  };
  
  typedef CList<ALLOCATED_INFO*, ALLOCATED_INFO*> AllocatedList;
  
  AllocatedList *allocatedList;

  CRITICAL_SECTION m_cs;

public:
  CTlsDebugHeap();
  ~CTlsDebugHeap();

  void AddTrace(DWORD addr,  DWORD asize,  const char *fname, DWORD lnum);
  void RemoveTrace(DWORD addr);

  void DumpMemoryLeaks();

  static CTlsDebugHeap& GetInstance();
};

inline void * __cdecl operator new(size_t size, const char *fileName, int lineNumber)
{
  void *ptr = (void *)malloc(size);
  CTlsDebugHeap::GetInstance().AddTrace((DWORD)ptr, size, fileName, lineNumber);

  ASSERT(ptr);
  if(!ptr)
    CTlsDebugHeap::GetInstance().DumpMemoryLeaks();

  return(ptr);
};

inline void __cdecl operator delete(void *p)
{
  CTlsDebugHeap::GetInstance().RemoveTrace((DWORD)p);
  free(p);
};

inline void* dbgmalloc( size_t size, const char *fileName, int lineNumber)
{
  void *ptr = (void *)malloc(size);
  CTlsDebugHeap::GetInstance().AddTrace((DWORD)ptr, size, fileName, lineNumber);
  
  ASSERT(ptr);
  if(!ptr)
    CTlsDebugHeap::GetInstance().DumpMemoryLeaks();

  return(ptr);
}

inline void dbgfree( void *p )
{
  CTlsDebugHeap::GetInstance().RemoveTrace((DWORD)p);
  free(p);
}

inline void* dbgrealloc( void *memblock, size_t size, const char *fileName, int lineNumber)
{
  CTlsDebugHeap::GetInstance().RemoveTrace((DWORD)memblock);
  void* ptr = realloc(memblock, size);
  CTlsDebugHeap::GetInstance().AddTrace((DWORD)ptr, size, fileName, lineNumber);
  
  if(!ptr)
    CTlsDebugHeap::GetInstance().DumpMemoryLeaks();
  ASSERT(ptr);

  return ptr;
}

inline void *dbgcalloc( size_t num, size_t size, const char *fileName, int lineNumber )
{
  void* ptr = calloc(num, size);
  CTlsDebugHeap::GetInstance().AddTrace((DWORD)ptr, size, fileName, lineNumber);

  if(!ptr)
    CTlsDebugHeap::GetInstance().DumpMemoryLeaks();
  ASSERT(ptr);
  
  return ptr;
}

#else
// Normal version
#define DEBUG_MALLOC(a) malloc(a)
#define DEBUG_FREE(a) free(a)
#define DEBUG_REALLOC(a, b) realloc(a, b)
#define DEBUG_CALLOC(a, b) calloc(a, b)

#endif

#endif
/*=============================================================================
    E O F
=============================================================================*/
