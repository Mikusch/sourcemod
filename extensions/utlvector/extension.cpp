/**
 * vim: set ts=4 sw=4 tw=99 noet:
 * SourceMod CUtlVector Extension
 */

#include "extension.h"
#include "utlvectorhandler.h"

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

CUtlVectorExt g_CUtlVectorExt;

SMEXT_LINK(&g_CUtlVectorExt);

bool CUtlVectorExt::SDK_OnLoad(char* error, size_t maxlength, bool late)
{
	// Create our handle type
	g_CUtlVectorHandle = g_pHandleSys->CreateType("CUtlVector", &g_CUtlVectorHandler, 0, NULL, NULL, myself->GetIdentity(), NULL);

	if (!g_CUtlVectorHandle)
	{
		snprintf(error, maxlength, "Could not create CUtlVector handle type");
		return false;
	}

	// Register natives
	g_pShareSys->AddNatives(myself, utlvector_natives);

	return true;
}

void CUtlVectorExt::SDK_OnUnload()
{
	// Remove our handle type
	if (g_CUtlVectorHandle)
	{
		g_pHandleSys->RemoveType(g_CUtlVectorHandle, myself->GetIdentity());
		g_CUtlVectorHandle = 0;
	}
}