/**
 * vim: set ts=4 sw=4 tw=99 noet:
 * SourceMod CUtlVector Extension
 */

#include "utlvectorwrapper.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//-----------------------------------------------------------------------------
// Constructor / Destructor
//-----------------------------------------------------------------------------
CUtlVectorWrapper::CUtlVectorWrapper(void* pVector, size_t elementSize)
	: m_pVector(reinterpret_cast<RawCUtlVector*>(pVector)), m_ElementSize(elementSize)
{
	assert(m_pVector != nullptr);
	assert(m_ElementSize > 0);
}

CUtlVectorWrapper::~CUtlVectorWrapper()
{
}

//-----------------------------------------------------------------------------
// Element access
//-----------------------------------------------------------------------------
void* CUtlVectorWrapper::Element(int i)
{
	assert(IsValidIndex(i));
	return reinterpret_cast<char*>(m_pVector->m_Memory.m_pMemory) + (i * m_ElementSize);
}

const void* CUtlVectorWrapper::Element(int i) const
{
	assert(IsValidIndex(i));
	return reinterpret_cast<const char*>(m_pVector->m_Memory.m_pMemory) + (i * m_ElementSize);
}

//-----------------------------------------------------------------------------
// Gets the base address
//-----------------------------------------------------------------------------
void* CUtlVectorWrapper::Base()
{
	return m_pVector->m_Memory.m_pMemory;
}

const void* CUtlVectorWrapper::Base() const
{
	return m_pVector->m_Memory.m_pMemory;
}

//-----------------------------------------------------------------------------
// Count
//-----------------------------------------------------------------------------
int CUtlVectorWrapper::Count() const
{
	return m_pVector->m_Size;
}

//-----------------------------------------------------------------------------
// Is element index valid?
//-----------------------------------------------------------------------------
bool CUtlVectorWrapper::IsValidIndex(int i) const
{
	return (i >= 0) && (i < m_pVector->m_Size);
}

//-----------------------------------------------------------------------------
// Adds an element to the tail
//-----------------------------------------------------------------------------
int CUtlVectorWrapper::AddToTail(const void* src)
{
	int iNew = Count();
	GrowVector(1);

	if (src)
		CopyConstruct(reinterpret_cast<char*>(m_pVector->m_Memory.m_pMemory) + (iNew * m_ElementSize), src);
	else
		memset(reinterpret_cast<char*>(m_pVector->m_Memory.m_pMemory) + (iNew * m_ElementSize), 0, m_ElementSize);

	return iNew;
}

//-----------------------------------------------------------------------------
// Finds an element
//-----------------------------------------------------------------------------
int CUtlVectorWrapper::Find(const void* src) const
{
	for (int i = 0; i < Count(); ++i)
	{
		if (memcmp(Element(i), src, m_ElementSize) == 0)
			return i;
	}
	return -1;
}

//-----------------------------------------------------------------------------
// Element removal
//-----------------------------------------------------------------------------
void CUtlVectorWrapper::Remove(int elem)
{
	assert(IsValidIndex(elem));

	Destruct(Element(elem));
	ShiftElementsLeft(elem);
	--m_pVector->m_Size;
	UpdateDebugPointer();
}

void CUtlVectorWrapper::RemoveAll()
{
	for (int i = m_pVector->m_Size; --i >= 0;)
		Destruct(Element(i));

	m_pVector->m_Size = 0;
	UpdateDebugPointer();
}

//-----------------------------------------------------------------------------
// Grows the vector
//-----------------------------------------------------------------------------
void CUtlVectorWrapper::GrowVector(int num)
{
	if (m_pVector->m_Size + num > m_pVector->m_Memory.m_nAllocationCount)
	{
		int newAllocationCount = CalcNewAllocationCount(
			m_pVector->m_Memory.m_nAllocationCount,
			m_pVector->m_Memory.m_nGrowSize,
			m_pVector->m_Size + num,
			m_ElementSize
		);
		InternalRealloc(newAllocationCount);
	}

	m_pVector->m_Size += num;
	UpdateDebugPointer();
}

//-----------------------------------------------------------------------------
// Shifts elements
//-----------------------------------------------------------------------------
void CUtlVectorWrapper::ShiftElementsLeft(int elem, int num)
{
	assert(IsValidIndex(elem));

	int numToMove = m_pVector->m_Size - (elem + num);
	if (numToMove > 0 && num > 0)
	{
		memmove(
			Element(elem),
			reinterpret_cast<char*>(Element(elem + num)),
			numToMove * m_ElementSize
		);
	}
}

//-----------------------------------------------------------------------------
// Memory allocation
//-----------------------------------------------------------------------------
void* CUtlVectorWrapper::InternalAlloc(int numElements)
{
	return malloc(numElements * m_ElementSize);
}

void CUtlVectorWrapper::InternalFree(void* pMemory)
{
	if (pMemory)
		free(pMemory);
}

void CUtlVectorWrapper::InternalRealloc(int numElements)
{
	void* pNewMemory = InternalAlloc(numElements);

	if (m_pVector->m_Memory.m_pMemory && m_pVector->m_Size > 0)
		memcpy(pNewMemory, m_pVector->m_Memory.m_pMemory, m_pVector->m_Size * m_ElementSize);

	InternalFree(m_pVector->m_Memory.m_pMemory);

	m_pVector->m_Memory.m_pMemory = pNewMemory;
	m_pVector->m_Memory.m_nAllocationCount = numElements;
	UpdateDebugPointer();
}

//-----------------------------------------------------------------------------
// Calculate growth size
//-----------------------------------------------------------------------------
int CUtlVectorWrapper::CalcNewAllocationCount(int allocationCount, int growSize, int newSize, int bytesItem)
{
	if (growSize)
	{
		allocationCount = ((newSize - 1) / growSize + 1) * growSize;
	}
	else
	{
		if (!allocationCount)
		{
			allocationCount = (bytesItem > 256) ? 1 : (256 / bytesItem);
			if (allocationCount < 1)
				allocationCount = 1;
		}

		while (allocationCount < newSize)
			allocationCount *= 2;
	}

	return allocationCount;
}

//-----------------------------------------------------------------------------
// Construction/Destruction helpers
//-----------------------------------------------------------------------------
void CUtlVectorWrapper::CopyConstruct(void* dest, const void* src)
{
	memcpy(dest, src, m_ElementSize);
}

void CUtlVectorWrapper::Destruct(void* elem)
{
	// For POD types, nothing to do
}