/**
 * vim: set ts=4 sw=4 tw=99 noet:
 * SourceMod CUtlVector Extension
 */

#ifndef _UTLVECTOR_HANDLER_H_
#define _UTLVECTOR_HANDLER_H_

#include "extension.h"
#include "utlvectorwrapper.h"
#include <map>

extern HandleType_t g_CUtlVectorHandle;
extern std::map<Handle_t, CUtlVectorWrapper*> g_VectorWrappers;

class CUtlVectorHandler : public IHandleTypeDispatch
{
public:
	void OnHandleDestroy(HandleType_t type, void* object)
	{
		CUtlVectorWrapper* wrapper = (CUtlVectorWrapper*)object;

		for (auto it = g_VectorWrappers.begin(); it != g_VectorWrappers.end(); ++it)
		{
			if (it->second == wrapper)
			{
				g_VectorWrappers.erase(it);
				break;
			}
		}

		delete wrapper;
	}
};

extern CUtlVectorHandler g_CUtlVectorHandler;

#endif // _UTLVECTOR_HANDLER_H_