#include <sourcemod>
#include <commandextensions>

public OnPluginStart()
{
	RegConsoleCmd("sm_cheatcmd", CheatCmd, "Run any command on the server using a direct Dispatch call");
	RegConsoleCmd("shake", ShakeCmd);
}

public Action:ShakeCmd(client, args)
{
	PrintToChatAll("Shake callback hit");
	return Plugin_Handled;
}

public Action:CheatCmd(client, args)
{
	if(args < 1)
	{
		ReplyToCommand(client, "Please provide a command to run");
		return Plugin_Handled;
	}

	static String:cmdBuf[255];
	GetCmdArgString(cmdBuf, sizeof(cmdBuf));
	DispatchServerCommand(cmdBuf);
	
	return Plugin_Handled;
	
}