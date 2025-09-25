/**
 * vim: set ts=4 sw=4 tw=99 noet:
 * SourceMod CUtlVector Extension
 */

#include "extension.h"
#include "utlvectorhandler.h"

// Global variable definitions
HandleType_t g_CUtlVectorHandle = 0;
std::map<Handle_t, CUtlVectorWrapper*> g_VectorWrappers;
CUtlVectorHandler g_CUtlVectorHandler;

// Native: CUtlVector.CUtlVector(Address address, int elementSize)
static cell_t Native_CUtlVector_CUtlVector(IPluginContext* pContext, const cell_t* params)
{
	void* address = smutils->FromPseudoAddress(params[1]);
	size_t elementSize = static_cast<size_t>(params[2]);

	if (!address)
	{
		return pContext->ThrowNativeError("Invalid address for CUtlVector");
	}

	if (elementSize == 0 || elementSize > 1024 * 1024) // Sanity check
	{
		return pContext->ThrowNativeError("Invalid element size %d", elementSize);
	}

	CUtlVectorWrapper* wrapper = new CUtlVectorWrapper(address, elementSize);

	HandleError err;
	Handle_t handle = g_pHandleSys->CreateHandle(g_CUtlVectorHandle, wrapper,
		pContext->GetIdentity(), myself->GetIdentity(), &err);

	if (handle == BAD_HANDLE)
	{
		delete wrapper;
		return pContext->ThrowNativeError("Failed to create handle (error %d)", err);
	}

	g_VectorWrappers[handle] = wrapper;
	return handle;
}

// Native: int CUtlVector.Count.get()
static cell_t Native_CUtlVector_Count(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	return wrapper->Count();
}

// Native: bool CUtlVector.IsEmpty()
static cell_t Native_CUtlVector_IsEmpty(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	return wrapper->IsEmpty();
}

// Native: bool CUtlVector.IsValidIndex(int i)
static cell_t Native_CUtlVector_IsValidIndex(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	return wrapper->IsValidIndex(params[2]);
}

// Native: any CUtlVector.Get(int index)
static cell_t Native_CUtlVector_Get(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	int index = params[2];
	if (!wrapper->IsValidIndex(index))
	{
		return pContext->ThrowNativeError("Invalid index %d (count: %d)", index, wrapper->Count());
	}

	void* element = wrapper->Element(index);
	return *reinterpret_cast<cell_t*>(element);
}

// Native: void CUtlVector.Set(int index, any value)
static cell_t Native_CUtlVector_Set(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	int index = params[2];
	if (!wrapper->IsValidIndex(index))
	{
		return pContext->ThrowNativeError("Invalid index %d (count: %d)", index, wrapper->Count());
	}

	void* element = wrapper->Element(index);
	*reinterpret_cast<cell_t*>(element) = params[3];

	return 0;
}

// Native: int CUtlVector.AddToTail(any value)
static cell_t Native_CUtlVector_AddToTail(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	cell_t value = params[2];
	return wrapper->AddToTail(&value);
}

// Native: void CUtlVector.Remove(int index)
static cell_t Native_CUtlVector_Remove(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	int index = params[2];
	if (!wrapper->IsValidIndex(index))
	{
		return pContext->ThrowNativeError("Invalid index %d (count: %d)", index, wrapper->Count());
	}

	wrapper->Remove(index);
	return 0;
}

// Native: void CUtlVector.RemoveAll()
static cell_t Native_CUtlVector_RemoveAll(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	wrapper->RemoveAll();
	return 0;
}

// Native: int CUtlVector.Find(any value)
static cell_t Native_CUtlVector_Find(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	cell_t value = params[2];
	return wrapper->Find(&value);
}

// Native: Address CUtlVector.GetBase()
static cell_t Native_CUtlVector_GetBase(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	return smutils->ToPseudoAddress(wrapper->Base());
}

// Native: Address CUtlVector.GetAddress(int index)
static cell_t Native_CUtlVector_GetAddress(IPluginContext* pContext, const cell_t* params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = pContext->GetIdentity();
	sec.pIdentity = myself->GetIdentity();

	CUtlVectorWrapper* wrapper;
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CUtlVectorHandle, &sec, (void**)&wrapper)) != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid CUtlVector handle %x (error %d)", hndl, err);
	}

	int index = params[2];
	if (!wrapper->IsValidIndex(index))
	{
		return pContext->ThrowNativeError("Invalid index %d (count: %d)", index, wrapper->Count());
	}

	return smutils->ToPseudoAddress(wrapper->Element(index));
}

const sp_nativeinfo_t utlvector_natives[] =
{
	{"CUtlVector.CUtlVector",       Native_CUtlVector_CUtlVector},
	{"CUtlVector.Count.get",         Native_CUtlVector_Count},
	{"CUtlVector.IsEmpty",           Native_CUtlVector_IsEmpty},
	{"CUtlVector.IsValidIndex",      Native_CUtlVector_IsValidIndex},
	{"CUtlVector.Get",               Native_CUtlVector_Get},
	{"CUtlVector.Set",               Native_CUtlVector_Set},
	{"CUtlVector.AddToTail",         Native_CUtlVector_AddToTail},
	{"CUtlVector.Remove",            Native_CUtlVector_Remove},
	{"CUtlVector.RemoveAll",         Native_CUtlVector_RemoveAll},
	{"CUtlVector.Find",              Native_CUtlVector_Find},
	{"CUtlVector.GetBase",           Native_CUtlVector_GetBase},
	{"CUtlVector.GetAddress",        Native_CUtlVector_GetAddress},
	{NULL,                           NULL}
};