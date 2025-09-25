/**
 * vim: set ts=4 sw=4 tw=99 noet:
 * SourceMod CUtlVector Extension
 */

#ifndef _UTLVECTORWRAPPER_H_
#define _UTLVECTORWRAPPER_H_

#include <stdint.h>
#include <string.h>
#include "tier1/utlmemory.h"

class CUtlVectorWrapper
{
public:
	CUtlVectorWrapper(void* pVector, size_t elementSize);
	~CUtlVectorWrapper();

	// Element access
	void* Element(int i);
	const void* Element(int i) const;

	// Gets the base address
	void* Base();
	const void* Base() const;

	// Returns the number of elements in the vector
	int Count() const;

	// Are there no elements?
	bool IsEmpty() const { return Count() == 0; }

	// Is element index valid?
	bool IsValidIndex(int i) const;

	// Adds an element to the tail
	int AddToTail(const void* src);

	// Finds an element
	int Find(const void* src) const;

	// Element removal
	void Remove(int elem);
	void RemoveAll();

private:
	// Raw memory layout matching CUtlMemory<T>
	struct RawCUtlMemory
	{
		void* m_pMemory;
		int m_nAllocationCount;
		int m_nGrowSize;
	};

	// Raw memory layout matching CUtlVector<T>
	struct RawCUtlVector
	{
		RawCUtlMemory m_Memory;
		int m_Size;
#ifndef _X360
		void* m_pElements;
#endif
	};

	// Grows the vector
	void GrowVector(int num = 1);

	// Shifts elements
	void ShiftElementsLeft(int elem, int num = 1);

	// Memory allocation using Source engine's allocator
	void* InternalAlloc(int numElements);
	void InternalFree(void* pMemory);
	void InternalRealloc(int numElements);

	// Calculate growth following CUtlMemory's algorithm
	int CalcNewAllocationCount(int allocationCount, int growSize, int newSize, int bytesItem);

	inline void UpdateDebugPointer()
	{
#ifndef _X360
		m_pVector->m_pElements = m_pVector->m_Memory.m_pMemory;
#endif
	}

	void CopyConstruct(void* dest, const void* src);
	void Destruct(void* elem);

	RawCUtlVector* m_pVector;
	size_t m_ElementSize;
};

#endif // _UTLVECTORWRAPPER_H_