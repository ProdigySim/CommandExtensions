/**
 * vim: set ts=4 :
 * =============================================================================
 * CommandExtensions
 * Copyright (C) 2012 Michael "ProdigySim" Busby.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include <compat_wrappers.h>
#include "icvar.h"

/**
 * @file extension.cpp
 * @brief Implementation of CommandExtensions
 */

CommandExtensions g_CommandExtensions;		/**< Global singleton for extension's main interface */

ICvar *icvar = NULL;

SMEXT_LINK(&g_CommandExtensions);

static cell_t smn_DispatchServerCommand(IPluginContext *pContext, const cell_t *params);
static CCommand g_sCCommand; // Don't need more than one.

extern sp_nativeinfo_s g_CommandExtNatives[];

bool CommandExtensions::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	sharesys->AddNatives(myself, g_CommandExtNatives);

#if SOURCE_ENGINE >= SE_ORANGEBOX
	g_pCVar = icvar;
#endif
	CONVAR_REGISTER(this);
	return true;
}
	
void CommandExtensions::SDK_OnUnload()
{

}

void CommandExtensions::SDK_OnAllLoaded()
{
}

bool CommandExtensions::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);

	return true;
}

bool CommandExtensions::RegisterConCommandBase(ConCommandBase *pVar)
{
#if defined METAMOD_PLAPI_VERSION
	return g_SMAPI->RegisterConCommandBase(g_PLAPI, pVar);
#else
	return g_SMAPI->RegisterConCmdBase(g_PLAPI, pVar);
#endif
}

static cell_t smn_DispatchServerCommand(IPluginContext *pContext, const cell_t *params)
{
	char *cmdString;
	pContext->LocalToString(params[1], &cmdString);

	if(!g_sCCommand.Tokenize(cmdString) || g_sCCommand.ArgC() == 0)
	{
		return pContext->ThrowNativeError("Failed to tokenize command string: %s", cmdString);
	}

	ConCommand * pCmd = icvar->FindCommand(g_sCCommand[0]);

	if(pCmd == NULL)
	{
		return pContext->ThrowNativeError("Console Command '%s' not found", g_sCCommand[0]);
	}
	
	pCmd->Dispatch(g_sCCommand);
	

	return 0;
}

sp_nativeinfo_s g_CommandExtNatives[] =
{
	{"DispatchServerCommand",		smn_DispatchServerCommand},
	{NULL,							NULL}
};
