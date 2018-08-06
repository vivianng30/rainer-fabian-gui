// Web Address:   http://sig.sapp.org/include/sigBase/CircularBuffer.cpp
// Documentation: http://sig.sapp.org/doc/classes/CircularBuffer
// Syntax:        C++
//
// Description:   A Circular buffer designed to handle MIDI input,
//                but able to store any type of object.  Elements
//                can be read out of the buffer in two ways. 
//                (1) from a read pointer which extracts the
//                elements in order by following the write pointer,
//                and (2) from an index operator related to the
//                write pointer's location, for example,
//                object[0] is the last value written into the
//                buffer and object[-1] (or object[1]) is the
//                item written just before that.
//

#ifndef _CIRCULARBUFFER_H_INCLUDED
#define _CIRCULARBUFFER_H_INCLUDED
//

//#include "stdafx.h"
//#include <stdlib.h>
//#include <iostream>
//#include <sstream>
//#include <stdexcept>
//
//using namespace std;

//#if !defined __CFGDATA_H__
//#define __CFGDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "globDefs.h"


template<class type>
class CircularBuffer {
   public:
	   //////////////////////////////
	   //
	   // CircularBuffer::CircularBuffer -- Constructor.
	   //
	   CircularBuffer(void)
	   {
		   size = 0;
		   buffer = NULL;
		   reset();
	   }

	   CircularBuffer(int maxElements)
	   {
		   if (maxElements < 0) {
		   }
		   if (maxElements == 0) {
			   size = 0;
			   buffer = NULL;
			   reset();
		   } else {
			   size = maxElements;
			   buffer = new type[maxElements];
			   reset();
		   }
	   }
                    
	   CircularBuffer(const CircularBuffer<type>&anotherBuffer)
	   {
			size = anotherBuffer.size;
			if (getSize() == 0) {
				buffer = NULL;
				reset();
			} else {
				buffer = new type[getSize()];
				writeIndex = anotherBuffer.writeIndex;
				readIndex = anotherBuffer.readIndex;
				itemCount = anotherBuffer.itemCount;
				
				/*errno_t err;

				err = memcpy_s(buffer, sizeof(type)*getSize(), anotherBuffer.buffer, sizeof(type)*sizeof(anotherBuffer.getSize()) );    
				if (err)
				{
				}*/

				for (int i=0; i<getSize(); i++) 
				{
					buffer[i] = anotherBuffer.buffer[i];
				}
			}
		}

	   //////////////////////////////
	   //
	   // CircularBuffer::operator= -- copy one buffer to an other
	   //    
	   //
	   bool operator=(const CircularBuffer<type>&anotherBuffer)
	   {
		   if (anotherBuffer.size < 0) {
			   return false;
		   }

		   if(size != anotherBuffer.size)
		   {
				setSize(anotherBuffer.size);

				writeIndex = anotherBuffer.writeIndex;
				readIndex = anotherBuffer.readIndex;
				itemCount = anotherBuffer.itemCount;

				/*errno_t err;

				err = memcpy_s(buffer, sizeof(type)*getSize(), anotherBuffer.buffer, sizeof(type)*sizeof(anotherBuffer.getSize()) );    
				if (err)
				{
				}*/

				for (int i=0; i<getSize(); i++) 
				{
					buffer[i] = anotherBuffer.buffer[i];
				}
		   }
		   else
		   {
			   writeIndex = anotherBuffer.writeIndex;
			   readIndex = anotherBuffer.readIndex;
			   itemCount = anotherBuffer.itemCount;

			   for (int i=0; i<getSize(); i++) 
			   {
				   buffer[i] = anotherBuffer.buffer[i];
			   }
			   //errno_t err;

			  /* int iTest1=sizeof(buffer);
			   int iTest2=getSize();
			   int iTest3=sizeof(type);*/
			   
			  /* if( (sizeof(type)*getSize())   !=    (sizeof(type)*sizeof(anotherBuffer.getSize())))
			   {
				   int iStop=0;
			   }*/

			   
			  /* err = memcpy_s(buffer, sizeof(type)*getSize(), anotherBuffer.buffer, sizeof(type)*sizeof(anotherBuffer.getSize()) );    
			   if (err!=0)
			   {
				   int iStop=0;
			   }
			   else
			   {
				   int iStop=0;
			   }*/
		   }

		   return true;

	   }
		
	   //////////////////////////////
	   //
	   // CircularBuffer::~CircularBuffer -- Destructor.
	   //    deallocates buffer memory.
	   //
	   ~CircularBuffer()
	   {
		   __try
		   {
			   if (buffer != NULL) {
				   delete [] buffer;
			   }
			   buffer = NULL;
		   }
		   __except(EXCEPTION_EXECUTE_HANDLER)
		   {
			   //AfxMessageBox( _T("EXCEPTION:  ~CircularBuffer()") );

		   }
		 
			
		}

	   //////////////////////////////
	   //
	   // CircularBuffer::capacity -- returns the number of items which
	   //    can be added to the buffer.  Returns a positive number
	   //    if the buffer has empty locations available.  Returns 0 if the
	   //    buffer is 100% full.  Returns a negative number if the
	   //    buffer has overflowed.
		int capacity(void) const
		{
			return getSize() - getCount();
		}

		//////////////////////////////
		//
		// CircularBuffer::extract -- reads the next value from the buffer.
		//
		type extract(void)
		{
			itemCount--;
			if (itemCount < 0) {
			}
			increment(readIndex);
			return buffer[readIndex];
		}

		//////////////////////////////
		//
		// CircularBuffer::getCount -- returns the number of elements
		//    between the write index and the read index.
		//
		int getCount(void) const
		{
			return itemCount;
		}

		//////////////////////////////
		//
		// CircularBuffer::getSize -- returns the allocated size of the buffer.
		//
		int getSize(void) const
		{
			return size;
		}
		
		//////////////////////////////
		//
		// CircularBuffer::insert -- add an element to the circular buffer
		//
		void insert(const type& anItem)
		{
			__try
			{
				itemCount++;
				if(itemCount>size)
					itemCount=size;
				increment(writeIndex);
				buffer[writeIndex] = anItem;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				//AfxMessageBox( _T("EXCEPTION:  CircularBuffer::insert") );

			}
			
			
		}

		//////////////////////////////
		//
		// CircularBuffer::insert -- add an element to the circular buffer
		//
		int getWriteIndex()
		{
			__try
			{
				return writeIndex;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				//AfxMessageBox( _T("EXCEPTION:  CircularBuffer::insert") );

			}


		}

		//////////////////////////////
		//
		// CircularBuffer::operator[] -- access an element relative to the
		//    currently written element
		//
		type& operator[](int index)
		{
			if (buffer == NULL) 
			{
			}
			int realIndex = (index < 0) ? -index : index;
			if (realIndex >= getSize()) 
			{
			}
			realIndex = writeIndex - realIndex;

			// should need to go through this loop a max of one time:
			while (realIndex < 0) 
			{
				realIndex += getSize();
			}

			return buffer[realIndex];
		}

		//////////////////////////////
		//
		// CircularBuffer::read -- an alias for the extract function.
		//
	  	type read(void)
		{
			return extract();
		}

		//////////////////////////////
		//
		// CircularBuffer::reset -- throws out all previous data and
		//    sets the read/write/count to initial values.  The size
		//    data variable must be valid before this function is
		//    called.
		//
		void reset(void)
		{
			readIndex = writeIndex = getSize() - 1;
			itemCount = 0;
		}
		
		//////////////////////////////
		//
		// CircularBuffer::setSize -- warning: will throw out all previous data 
		//    stored in buffer.
		//
		void setSize(int aSize)
		{
			if (aSize < 0) {
				return;
			}
			if (buffer != NULL) {
				delete [] buffer;
			}

			if (aSize == 0) {
				size = aSize;
				buffer = NULL;
				reset();
			} else {
				size = aSize;
				buffer = new type[aSize];
				reset();
			}
		}   
			
		//////////////////////////////
		//
		// CircularBuffer::write --  an alias for the insert function.
		//
		void write(const type& anElement)
		{
			insert(anElement);
		}

   protected:

	  type*         buffer;
	  int           size;
      int           writeIndex;
      int           readIndex;
      int           itemCount;

	  //////////////////////////////
	  //
	  // CircularBuffer::increment -- adds one to specified index and
	  //    will automatically wrap the index when it gets too large.
	  //
	  void increment(int& index)
	  {
		 index++;
		  if (index >= getSize()) 
		  {
			  index = 0;
		  }
	  }
};

#endif  /* _CIRCULARBUFFER_CPP_INCLUDED */


//getModel()->getDATAHANDLER()->csSPIDataBuffer.Lock();
//getModel()->getDATAHANDLER()->gBUFSPIData[i];
