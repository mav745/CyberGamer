/*
cmd.c - script command processing module
Copyright (C) 2007 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "cmd2.hpp"

CCmd::CCmd()
{

	//	// register our commands
	//	addCommand ("echo", CCmd::Echo_f, "print a message to the console (useful in scripts)" );
	//	addCommand ("wait", CCmd::Wait_f, "make script execution wait for some rendered frames" );
	//	addCommand ("cmdlist", CCmd::List_f, "display all console commands beginning with the specified prefix" );
	//	addCommand ("stuffcmds", CCmd::StuffCmds_f, va( "execute commandline parameters (must be present in %s.rc script)", SI.ModuleName ));
	//	addCommand ("cmd", CCmd::ForwardToServer, "send a console commandline to the server" );
	//	addCommand ("alias", CCmd::Alias_f, "create a script function. Without arguments show the list of all alias" );
}

void CCmd::cbuf_clear()
{
	cmd_text.clear();
}

void CCmd::cbuf_addText( const QString &text )
{
	cmd_text.append(text);
}

void CCmd::cbuf_insertText( const QString &text )
{
	cmd_text.prepend(text);
}

/*
============
CCmd::Execute
============
*/
void CCmd::cbuf_execute()
{
	char	*text;
	char	line[MAX_CMD_LINE];
	int	i, quotes;

	while( cmd_text.size() )
	{
		if( cmd_wait )
		{
			cmd_wait = false;
			break;
		}

//		// find a \n or ; line break
//		text = (char *)cmd_text.data;

//		quotes = 0;

//		for( i = 0; i < cmd_text.cursize; i++ )
//		{
//			if( text[i] == '"' ) quotes++;
//			if(!( quotes & 1 ) &&  text[i] == ';' )
//				break; // don't break if inside a quoted string
//			if( text[i] == '\n' || text[i] == '\r' )
//				break;
//		}

//		if( i >= ( MAX_CMD_LINE - 1 ))
//			Sys_Error( "CCmd::Execute: command string owerflow\n" );

//		Q_memcpy( line, text, i );
//		line[i] = 0;

//		// delete the text from the command buffer and move remaining commands down
//		// this is necessary because commands (exec) can insert data at the
//		// beginning of the text buffer
//		if( i == cmd_text.cursize )
//		{
//			cmd_text.cursize = 0;
//		}
//		else
//		{
//			i++;
//			cmd_text.cursize -= i;
//			Q_memcpy( text, text + i, cmd_text.cursize );
//		}



//		//Sys_Print(line);
//		//Sys_Print("\n\0");
//		// execute the command line
//		CCmd::ExecuteString( line, src_command );
	}
}

/*
==============================================================================

			SCRIPT COMMANDS

==============================================================================
*/
/*
===============
CCmd::StuffCmds_f

Adds command line parameters as script statements
Commands lead with a +, and continue until a - or another +
xash -dev 3 +map c1a0d
xash -nosound -game bshift
===============
*/
void CCmd::stuffCmds_f()
{
//	int	i, j, l = 0;
//	QString build; // this is for all commandline options combined (and is bounds checked)

//	if( argc() != 1 )
//	{
//		msg( "Usage: stuffcmds : execute command line parameters\n" );
//		return;
//	}

//	// no reason to run the commandline arguments twice
//	if( host.stuffcmdsrun ) return;

//	host.stuffcmdsrun = true;
//	build[0] = 0;

//	for( i = 0; i < host.argc; i++ )
//	{
//		if( host.argv[i] && host.argv[i][0] == '+' && ( host.argv[i][1] < '0' || host.argv[i][1] > '9' ) && l + Q_strlen( host.argv[i] ) - 1 <= sizeof( build ) - 1 )
//		{
//			j = 1;

//			while( host.argv[i][j] )
//				build[l++] = host.argv[i][j++];

//			for( i++; i < host.argc; i++ )
//			{
//				if( !host.argv[i] ) continue;
//				if(( host.argv[i][0] == '+' || host.argv[i][0] == '-' ) && ( host.argv[i][1] < '0' || host.argv[i][1] > '9' ))
//					break;
//				if( l + Q_strlen( host.argv[i] ) + 4 > sizeof( build ) - 1 )
//					break;
//				build[l++] = ' ';

//				if( Q_strchr( host.argv[i], ' ' ))
//					build[l++] = '\"';

//				for( j = 0; host.argv[i][j]; j++ )
//					build[l++] = host.argv[i][j];

//				if( Q_strchr( host.argv[i], ' ' ))
//					build[l++] = '\"';
//			}
//			build[l++] = '\n';
//			i--;
//		}
//	}

//	// now terminate the combined string and prepend it to the command buffer
//	// we already reserved space for the terminator
//	build[l++] = 0;
//	insertText( build );
}

/*
============
CCmd::Wait_f

Causes execution of the remainder of the command buffer to be delayed until
next frame.  This allows commands like:
bind g "cmd use rocket ; +attack ; wait ; -attack ; cmd use blaster"
============
*/
void CCmd::wait_f()
{
	cmd_wait = true;
}

/*
===============
CCmd::Echo_f

Just prints the rest of the line to the console
===============
*/
void CCmd::echo_f()
{
//	int	i;

//	for( i = 1; i < CCmd::Argc(); i++ )
//		Sys_Print( CCmd::Argv( i ));
//	Sys_Print( "\n" );
}

/*
===============
CCmd::Alias_f

Creates a new command that executes a command string (possibly ; seperated)
===============
*/
void CCmd::alias_f()
{
//	cmdalias_t	*a;
//	char		cmd[MAX_CMD_LINE];
//	int		i, c;
//	char		*s;

//	if( CCmd::Argc() == 1 )
//	{
//		Msg( "Current alias commands:\n" );
//		for( a = cmd_alias; a; a = a->next )
//			Msg( "^2%s^7 : ^3%s^7\n", a->name, a->value );
//		return;
//	}

//	s = CCmd::Argv( 1 );

//	if( Q_strlen( s ) >= MAX_ALIAS_NAME )
//	{
//		Msg( "Alias name is too long\n" );
//		return;
//	}

//	// if the alias allready exists, reuse it
//	for( a = cmd_alias; a; a = a->next )
//	{
//		if( !Q_strcmp( s, a->name ))
//		{
//			Z_Free( a->value );
//			break;
//		}
//	}

//	if( !a )
//	{
//		a = reinterpret_cast<cmdalias_t*>(Z_Malloc( sizeof( cmdalias_t )));
//		a->next = cmd_alias;
//		cmd_alias = a;
//	}

//	Q_strncpy( a->name, s, sizeof( a->name ));

//	// copy the rest of the command line
//	cmd[0] = 0; // start out with a null string

//	c = CCmd::Argc();

//	for( i = 2; i < c; i++ )
//	{
//		Q_strcat( cmd, CCmd::Argv( i ));
//		if( i != c ) Q_strcat( cmd, " " );
//	}

//	Q_strcat( cmd, "\n" );
//	a->value = copystring( cmd );
}

//uint CCmd::argc()
//{
//	return cmd_argc;
//}

//char *CCmd::argv( int arg )
//{
//	if((uint)arg >= cmd_argc )
//		return "";
//	return 0;//cmd_argv[arg];
//}

//char *CCmd::args()
//{
//	return 0;//cmd_args;
//}


SCmdAlias *CCmd::aliasGetList()
{
	return cmd_alias;
}

/*
============
CCmd::GetList
============
*/
SCmd *CCmd::getFirstFunctionHandle()
{
	return 0;//cmd_functions;
}

SCmd *CCmd::getNextFunctionHandle( SCmd *cmd )
{
	return (cmd) ? cmd->next : NULL;
}

QString CCmd::getName( SCmd *cmd )
{
	return cmd->name;
}



char *COM_ParseFile( char *data, char *token )
{
	int	c, len;

	if( !token )
		return NULL;

	len = 0;
	token[0] = 0;

	if( !data )
		return NULL;
// skip whitespace
skipwhite:
	while(( c = ((byte)*data)) <= ' ' )
	{
		if( c == 0 )
			return NULL;	// end of file;
		data++;
	}

	// skip // comments
	if( c=='/' && data[1] == '/' )
	{
		while( *data && *data != '\n' )
			data++;
		goto skipwhite;
	}

	// handle quoted strings specially
	if( c == '\"' )
	{
		data++;
		while( 1 )
		{
			c = (byte)*data++;
			if( c == '\"' || !c )
			{
				token[len] = 0;
				return data;
			}
			token[len] = c;
			len++;
		}
	}

	// parse single characters
	if( c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ',' )
	{
		token[len] = c;
		len++;
		token[len] = 0;
		return data + 1;
	}

	// parse a regular word
	do
	{
		token[len] = c;
		data++;
		len++;
		c = ((byte)*data);

		if( c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ',' )
			break;
	} while( c > 32 );

	token[len] = 0;

	return data;
}

/*
============
CCmd::TokenizeString

Parses the given string into command line tokens.
The text is copied to a seperate buffer and 0 characters
are inserted in the apropriate place, The argv array
will point into this temporary buffer.
============
*/
QStringList CCmd::tokenizeString( const QString &text )
{
	QStringList result;
	QString buffer = text.trimmed();

	if( buffer.isEmpty() )
		return result;

	while( 1 )
	{
		// skip whitespace up to a /n
		while( *text && ((byte)*text) <= ' ' && *text != '\n' )
			text++;

		if( *text == '\n' )
		{
			// a newline seperates commands in the buffer
			text++;
			break;
		}

		if( !*text )
			return;

		if( cmd_argc == 1 )
			 cmd_args = text;

		text = COM_ParseFile( text, cmd_token );
		if( !text ) return;

		if( cmd_argc < MAX_CMD_TOKENS )
		{
			cmd_argv[cmd_argc] = copystring( cmd_token );
			cmd_argc++;
		}
	}
}

void CCmd::addCommand(const QString &cmdName, CCmdCode &cmdCode, const QString &cmdDesc )
{
//	cmd_t	*cmd;

//	// fail if the command is a variable name
//	if( Cvar_FindVar( cmd_name ))
//	{
//		MsgDev( D_INFO, "CCmd::AddCommand: %s already defined as a var\n", cmd_name );
//		return;
//	}

//	// fail if the command already exists
//	if( CCmd::Exists( cmd_name ))
//	{
//		MsgDev( D_INFO, "CCmd::AddCommand: %s already defined\n", cmd_name );
//		return;
//	}

//	// use a small malloc to avoid zone fragmentation
//	cmd = reinterpret_cast<cmd_t*>(Z_Malloc( sizeof( cmd_t )));
//	cmd->name = copystring( cmd_name );
//	cmd->desc = copystring( cmd_desc );
//	cmd->function = function;
//	cmd->next = cmd_functions;
//	cmd_functions = cmd;
}

void CCmd::addGameCommand(const QString &cmdName, CCmdCode &cmdCode )
{
//	SCmd	*cmd;

//	// fail if the command is a variable name
//	if( Cvar_FindVar( cmd_name ))
//	{
//		MsgDev( D_INFO, "CCmd::AddCommand: %s already defined as a var\n", cmd_name );
//		return;
//	}

//	// fail if the command already exists
//	if( CCmd::Exists( cmd_name ))
//	{
//		MsgDev(D_INFO, "CCmd::AddCommand: %s already defined\n", cmd_name);
//		return;
//	}

//	// use a small malloc to avoid zone fragmentation
//	cmd = reinterpret_cast<cmd_t*>(Z_Malloc( sizeof( cmd_t )));
//	cmd->name = copystring( cmd_name );
//	cmd->desc = copystring( "game command" );
//	cmd->function = function;
//	cmd->flags = CMD_EXTDLL;
//	cmd->next = cmd_functions;
//	cmd_functions = cmd;
}

void CCmd::addClientCommand(const QString &cmd_name, CCmdCode &cmdCode )
{
//	SCmd	*cmd;

//	// fail if the command is a variable name
//	if( Cvar_FindVar( cmd_name ))
//	{
//		MsgDev( D_INFO, "CCmd::AddCommand: %s already defined as a var\n", cmd_name );
//		return;
//	}

//	// fail if the command already exists
//	if( CCmd::Exists( cmd_name ))
//	{
//		MsgDev(D_INFO, "CCmd::AddCommand: %s already defined\n", cmd_name);
//		return;
//	}

//	// use a small malloc to avoid zone fragmentation
//	cmd = reinterpret_cast<cmd_t*>(Z_Malloc( sizeof( cmd_t )));
//	cmd->name = copystring( cmd_name );
//	cmd->desc = copystring( "client command" );
//	cmd->function = function;
//	cmd->flags = CMD_CLIENTDLL;
//	cmd->next = cmd_functions;
//	cmd_functions = cmd;
}

void CCmd::removeCommand( const QString &cmd_name )
{
//	SCmd	*cmd, **back;

//	if( !cmd_name || !*cmd_name )
//		return;

//	back = &cmd_functions;
//	while( 1 )
//	{
//		cmd = *back;
//		if( !cmd ) return;

//		if( !Q_strcmp( cmd_name, cmd->name ))
//		{
//			*back = cmd->next;

//			if( cmd->name )
//				Mem_Free( cmd->name );

//			if( cmd->desc )
//				Mem_Free( cmd->desc );

//			Mem_Free( cmd );
//			return;
//		}
//		back = &cmd->next;
//	}
}

void CCmd::lookupCmds( char *buffer, void *ptr, setpair_t callback )
{
//	SCmd	*cmd;
//	cmdalias_t	*alias;

//	// nothing to process ?
//	if( !callback ) return;

//	for( cmd = cmd_functions; cmd; cmd = cmd->next )
//	{
//		if( !buffer ) callback( cmd->name, (char *)cmd->function, cmd->desc, ptr );
//		else callback( cmd->name, (char *)cmd->function, buffer, ptr );
//	}

//	// lookup an aliases too
//	for( alias = cmd_alias; alias; alias = alias->next )
//		callback( alias->name, alias->value, buffer, ptr );
}

bool CCmd::exists(const QString &cmdName )
{
//	SCmd	*cmd;

//	for( cmd = cmd_functions; cmd; cmd = cmd->next )
//	{
//		if( !Q_strcmp( cmd_name, cmd->name ))
//			return true;
//	}
	return false;
}

/*
============
CCmd::ExecuteString

A complete command line has been parsed, so try to execute it
============
*/
void CCmd::executeString(QString &text, ECmdSource src )
{
	SCmd *cmd;
	SCmdAlias *a;

	// execute the command line
	CCmd::TokenizeString( text );

	if( !CCmd::Argc()) return; // no tokens

	// check alias
	for( a = cmd_alias; a; a = a->next )
	{
		if( !Q_stricmp( cmd_argv[0], a->name ))
		{
			CCmd::InsertText( a->value );
			return;
		}
	}

	// check functions
	for( cmd = cmd_functions; cmd; cmd = cmd->next )
	{
		if( !Q_stricmp( cmd_argv[0], cmd->name ) && cmd->function )
		{
			cmd->function();
			return;
		}
	}

	// check cvars
	if( Cvar_Command( )) return;

	// forward the command line to the server, so the entity DLL can parse it
	if( src == src_command && host.type == HOST_NORMAL )
	{
		if( cls.state >= ca_connected )
		{
			CCmd::ForwardToServer();
			return;
		}
	}
	else if( text[0] != '@' && host.type == HOST_NORMAL )
	{
		// commands with leading '@' are hidden system commands
		MsgDev( D_INFO, "Unknown command \"%s\"\n", text );
	}
}

/*
===================
CCmd::ForwardToServer

adds the current command line as a clc_stringcmd to the client message.
things like godmode, noclip, etc, are commands directed to the server,
so when they are typed in at the console, they will need to be forwarded.
===================
*/
void CCmd::forwardToServer()
{
//	char	str[MAX_CMD_BUFFER];

//	if( cls.demoplayback )
//	{
//		if( !Q_stricmp( CCmd::Argv( 0 ), "pause" ))
//			cl.refdef.paused ^= 1;
//		return;
//	}

//	if( cls.state != ca_connected && cls.state != ca_active )
//	{
//		MsgDev( D_INFO, "Can't \"%s\", not connected\n", CCmd::Argv( 0 ));
//		return; // not connected
//	}

//	BF_WriteByte( &cls.netchan.message, clc_stringcmd );

//	str[0] = 0;
//	if( Q_stricmp( CCmd::Argv( 0 ), "cmd" ))
//	{
//		Q_strcat( str, CCmd::Argv( 0 ));
//		Q_strcat( str, " " );
//	}

//	if( CCmd::Argc() > 1 )
//		Q_strcat( str, CCmd::Args( ));
//	else Q_strcat( str, "\n" );

//	BF_WriteString( &cls.netchan.message, str );
}

/*
============
CCmd::List_f
============
*/
void CCmd::list_f()
{
//	cmd_t	*cmd;
//	int	i = 0;
//	char	*match;

//	if( CCmd::Argc() > 1 ) match = CCmd::Argv( 1 );
//	else match = NULL;

//	for( cmd = cmd_functions; cmd; cmd = cmd->next )
//	{
//		if( match && !Q_stricmpext( match, cmd->name ))
//			continue;
//		Msg( "%10s            %s\n", cmd->name, cmd->desc );
//		i++;
//	}
//	Msg( "%i commands\n", i );
}

/*
============
CCmd::Unlink

unlink all commands with flag CVAR_EXTDLL
============
*/
void CCmd::unlink( int group )
{
//	SCmd	*cmd;
//	SCmd	**prev;
//	int	count = 0;

//	if( Cvar_VariableInteger( "host_gameloaded" ) && ( group & CMD_EXTDLL ))
//	{
//		Msg( "can't unlink cvars while game is loaded\n" );
//		return;
//	}

//	if( Cvar_VariableInteger( "host_clientloaded" ) && ( group & CMD_CLIENTDLL ))
//	{
//		Msg( "can't unlink cvars while client is loaded\n" );
//		return;
//	}

//	prev = &cmd_functions;
//	while( 1 )
//	{
//		cmd = *prev;
//		if( !cmd ) break;

//		if( group && !( cmd->flags & group ))
//		{
//			prev = &cmd->next;
//			continue;
//		}

//		*prev = cmd->next;

//		if( cmd->name )
//			Mem_Free( cmd->name );

//		if( cmd->desc )
//			Mem_Free( cmd->desc );

//		Mem_Free( cmd );
//		count++;
//	}
}

/*
============
CCmd::Null_f

null function for some cmd stubs
============
*/
void CCmd::null_f()
{
}


