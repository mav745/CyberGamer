#include "cvar2.hpp"

CCvar::CCvar()
{
	userinfo   = get( "@userinfo",   "0", CVAR_READ_ONLY, "" ); // use ->modified value only
	physinfo   = get( "@physinfo",   "0", CVAR_READ_ONLY, "" ); // use ->modified value only
	serverinfo = get( "@serverinfo", "0", CVAR_READ_ONLY, "" ); // use ->modified value only
	renderinfo = get( "@renderinfo", "0", CVAR_READ_ONLY, "" ); // use ->modified value only

	//	Cmd_AddCommand ("toggle", &CCvar::toggle_f, "toggles a console variable's values (use for more info)" );
	//	Cmd_AddCommand ("set", &CCvar::set_f, "create or change the value of a console variable" );
	//	Cmd_AddCommand ("sets", &CCvar::setS_f, "create or change the value of a serverinfo variable" );
	//	Cmd_AddCommand ("setu", &CCvar::setU_f, "create or change the value of a userinfo variable" );
	//	Cmd_AddCommand ("setp", &CCvar::setP_f, "create or change the value of a physicinfo variable" );
	//	Cmd_AddCommand ("setr", &CCvar::setR_f, "create or change the value of a renderinfo variable" );
	//	Cmd_AddCommand ("setgl", &CCvar::setGL_f, "create or change the value of a opengl variable" );
	//	Cmd_AddCommand ("seta", &CCvar::setA_f, "create or change the value of a console variable that will be saved to config.cfg" );
	//	Cmd_AddCommand ("reset", &CCvar::reset_f, "reset any type variable to initial value" );
	//	Cmd_AddCommand ("latch", &CCvar::latched_f, "apply latched values" );
	//	Cmd_AddCommand ("vidlatch", &CCvar::latchedVideo_f, "apply latched values for video subsystem" );
	//	Cmd_AddCommand ("cvarlist", &CCvar::list_f, "display all console variables beginning with the specified prefix" );
	//	Cmd_AddCommand ("unsetall", &CCvar::restart_f, "reset all console variables to their default values" );
	//	Cmd_AddCommand ("@unlink", &CCvar::unlink_f, "unlink static cvars defined in gamedll" );
}

QList<SConVar*> CCvar::getList()
{
	return cvar_vars;
}

QString CCvar::getName( SCvar *cvar )
{
	return cvar->name;
}

bool CCvar::validateString( const QString &s, bool isvalue )
{
	return !(s.contains("\\") && !isvalue || s.contains("\"") || s.contains(";"));
}

SConVar *CCvar::findVar( const QString &var_name )
{
	foreach(SConVar	*var, cvar_vars)
		if(var->name == var_name)
			return var;
	return nullptr;
}

float CCvar::variableValue( const QString &var_name )
{
	SConVar	*var = findVar( var_name );
	if( !var ) return 0.f;
	return var->value;
}

int CCvar::variableInteger( const QString &var_name )
{
	SConVar	*var = findVar( var_name );
	if( !var ) return 0;
	return var->integer;
}

QString &CCvar::variableString( const QString &var_name )
{
	SConVar	*var = findVar( var_name );
	if( !var ) return QString();
	return var->string;
}

void CCvar::lookupVars( int checkbit, void *buffer, void *ptr, setpair_t callback )
{
//	SConVar	*cvar;

//	// nothing to process ?
//	if( !callback ) return;

//	// force checkbit to 0 for lookup all cvars
//	for( cvar = cvar_vars; cvar; cvar = cvar->next )
//	{
//		if( checkbit && !( cvar->flags & checkbit ))
//			continue;

//		if( buffer )
//		{
//			callback( cvar->name, cvar->string, buffer, ptr );
//		}
//		else
//		{
//			// NOTE: dlls cvars doesn't have description
//			if( cvar->flags & CVAR_EXTDLL )
//				callback( cvar->name, cvar->string, "game cvar", ptr );
//			else callback( cvar->name, cvar->string, cvar->description, ptr );
//		}
//	}
}

/*
============
CCvar::Get

If the variable already exists, the value will not be set
The flags will be or'ed in if the variable exists.
============
*/
SConVar *CCvar::get(const QString &var_name, const QString &var_value, int flags, const QString &var_desc )
{
//	SConVar	*var;

//	if( !var_name )
//	{
//		Sys_Error( "CCvar::Get: passed NULL name\n" );
//		return NULL;
//	}

//	if( !var_value ) var_value = "0"; // just apply default value

//	// all broadcast cvars must be passed this check
//	if( flags & ( CVAR_USERINFO|CVAR_SERVERINFO|CVAR_PHYSICINFO ))
//	{
//		if( !validateString( var_name, false ))
//		{
//			MsgDev( D_ERROR, "invalid info cvar name string %s\n", var_name );
//			return NULL;
//		}

//		if( !validateString( var_value, true ))
//		{
//			MsgDev( D_WARN, "invalid cvar value string: %s\n", var_value );
//			var_value = "0"; // just apply default value
//		}
//	}

//	// check for command coexisting
//	if( Cmd_Exists( var_name ))
//	{
//		MsgDev( D_ERROR, "CCvar::Get: %s is a command\n", var_name );
//		return NULL;
//	}

//	var = findVar( var_name );

//	if( var )
//	{
//		// fast check for short cvars
//		if( var->flags & CVAR_EXTDLL )
//		{
//			var->flags |= flags;
//			return var;
//		}

//		// if the C code is now specifying a variable that the user already
//		// set a value for, take the new value as the reset value
//		if(( var->flags & CVAR_USER_CREATED ) && !( flags & CVAR_USER_CREATED ) && var_value[0] )
//		{
//			var->flags &= ~CVAR_USER_CREATED;
//			Mem_Free( var->reset_string );
//			var->reset_string = copystring( var_value );
//		}

//		var->flags |= flags;

//		// only allow one non-empty reset string without a warning
//		if( !var->reset_string[0] )
//		{
//			// we don't have a reset string yet
//			Mem_Free( var->reset_string );
//			var->reset_string = copystring( var_value );
//		}

//		// if we have a latched string, take that value now
//		if( var->latched_string )
//		{
//			char *s = var->latched_string;
//			var->latched_string = NULL; // otherwise cvar_set2 would free it
//			set2( var_name, s, true );
//			Mem_Free( s );
//		}

//		if( var_desc )
//		{
//			// update description if needs
//			if( var->description ) Mem_Free( var->description );
//			var->description = copystring( var_desc );
//		}
//		return var;
//	}

//	// allocate a new cvar
//	var = reinterpret_cast<SConVar*>(Z_Malloc( sizeof( *var )));
//	var->name = copystring( var_name );
//	var->string = copystring( var_value );
//	var->reset_string = copystring( var_value );
//	if( var_desc ) var->description = copystring( var_desc );
//	var->value = Q_atof( var->string );
//	var->integer = Q_atoi( var->string );
//	var->modified = true;
//	var->flags = flags;

//	// link the variable in
//	var->next = cvar_vars;
//	cvar_vars = var;

	return 0;//var;
}

/*
============
CCvar::RegisterVariable

Adds a freestanding variable to the variable list.
============
*/
void CCvar::registerVariable( SCvar *var )
{
//	SConVar	**prev, *cur = NULL;
//	SConVar	*find;

//	ASSERT( var != NULL );

//	// check for overlap with a command
//	if( Cmd_Exists( var->name ))
//	{
//		MsgDev( D_ERROR, "CCvar::Register: %s is a command\n", var->name );
//		return;
//	}

//	// first check to see if it has already been defined
//	if(( cur = findVar( var->name )) != NULL )
//	{
//		// this cvar is already registered with CCvar::RegisterVariable
//		// so we can't replace it
//		if( cur->flags & CVAR_EXTDLL )
//		{
//			MsgDev( D_ERROR, "can't register variable %s, allready defined\n", var->name );
//			return;
//		}
//	}

//	if( cur )
//	{
//		prev = &cvar_vars;

//		while( 1 )
//		{
//			find = *prev;

//			ASSERT( find != NULL );

//			if( cur == cvar_vars )
//			{
//				// relink at tail
//				cvar_vars = (SConVar *)var;
//				break;
//			}

//			// search for previous cvar
//			if( cur != find->next )
//			{
//				prev = &find->next;
//				continue;
//			}

//			// link new variable
//			find->next = (SConVar *)var;
//			break;
//		}

//		var->string = cur->string;	// we already have right string
//		var->value = Q_atof( var->string );
//		var->flags |= CVAR_EXTDLL;	// all cvars passed this function are game cvars
//		var->next = (SCvar *)cur->next;

//		// release current cvar (but keep string)
//		if( cur->name ) Mem_Free( cur->name );
//		if( cur->latched_string ) Mem_Free( cur->latched_string );
//		if( cur->reset_string ) Mem_Free( cur->reset_string );
//		if( cur->description ) Mem_Free( cur->description );
//		Mem_Free( cur );
//	}
//	else
//	{
//		// copy the value off, because future sets will Z_Free it
//		var->string = copystring( var->string );
//		var->value = Q_atof( var->string );
//		var->flags |= CVAR_EXTDLL;		// all cvars passed this function are game cvars

//		// link the variable in
//		var->next = (SCvar *)cvar_vars;
//		cvar_vars = (SConVar *)var;
//	}
}

/*
============
CCvar::Set2
============
*/
SConVar *CCvar::set2(const QString &var_name, const QString &value, bool force )
{
//	SConVar		*var;
//	const char	*pszValue;
//	char		szNew[MAX_SYSPATH];
//	bool		dll_variable = false;

//	if( !validateString( var_name, false ))
//	{
//		MsgDev( D_ERROR, "invalid cvar name string: %s\n", var_name );
//		return NULL;
//	}

//	var = CCvar::FindVar( var_name );
//	if( !var )
//	{
//		// create it
//		if( force ) return get( var_name, value, 0, NULL );
//		return get( var_name, value, CVAR_USER_CREATED, NULL );

//	}

//	// use this check to prevent acessing for unexisting fields
//	// for SCvar: latched_string, description, etc
//	if( var->flags & CVAR_EXTDLL )
//		dll_variable = true;

//	if( !value )
//	{
//		if( dll_variable ) value = "0";
//		else value = var->reset_string;
//	}

//	if( !Q_strcmp( value, var->string ))
//		return var;

//	// any latched values not allowed for game cvars
//	if( dll_variable ) force = true;

//	if( !force )
//	{
//		if( var->flags & ( CVAR_READ_ONLY|CVAR_GLCONFIG ))
//		{
//			MsgDev( D_INFO, "%s is read only.\n", var_name );
//			return var;
//		}

//		if( var->flags & CVAR_INIT )
//		{
//			MsgDev( D_INFO, "%s is write protected.\n", var_name );
//			return var;
//		}

//		if( var->flags & ( CVAR_LATCH|CVAR_LATCH_VIDEO ))
//		{
//			if( var->latched_string )
//			{
//				if(!Q_strcmp( value, var->latched_string ))
//					return var;
//				Mem_Free( var->latched_string );
//			}
//			else
//			{
//				if( !Q_strcmp( value, var->string ))
//					return var;
//			}

//			if( var->flags & CVAR_LATCH && variableInteger( "host_serverstate" ))
//			{
//				MsgDev( D_INFO, "%s will be changed upon restarting.\n", var->name );
//				var->latched_string = copystring( value );
//			}
//			else if( var->flags & CVAR_LATCH_VIDEO )
//			{
//				MsgDev( D_INFO, "%s will be changed upon restarting video.\n", var->name );
//				var->latched_string = copystring( value );
//			}
//			else
//			{
//				Mem_Free( var->string );		// free the old value string
//				var->string = copystring( value );
//				var->value = Q_atof( var->string );
//				var->integer = Q_atoi( var->string );
//			}

//			var->modified = true;
//			return var;
//		}

//		if(( var->flags & CVAR_CHEAT ) && !variableInteger( "sv_cheats" ))
//		{
//			MsgDev( D_INFO, "%s is cheat protected.\n", var_name );
//			return var;
//		}
//	}
//	else
//	{
//		if( !dll_variable && var->latched_string )
//		{
//			Mem_Free( var->latched_string );
//			var->latched_string = NULL;
//		}
//	}

//	pszValue = value;

//	// This cvar's string must only contain printable characters.
//	// Strip out any other crap.
//	// We'll fill in "empty" if nothing is left
//	if( var->flags & CVAR_PRINTABLEONLY )
//	{
//		const char *pS;
//		char *pD;

//		// clear out new string
//		szNew[0] = '\0';

//		pS = pszValue;
//		pD = szNew;

//		// step through the string, only copying back in characters that are printable
//		while( *pS )
//		{
//			if( ((byte)*pS) < 32 || ((byte)*pS) > 255 )
//			{
//				pS++;
//				continue;
//			}
//			*pD++ = *pS++;
//		}

//		// terminate the new string
//		*pD = '\0';

//		// if it's empty, then insert a marker string
//		if( !Q_strlen( szNew ))
//		{
//			Q_strcpy( szNew, "default" );
//		}

//		// point the value here.
//		pszValue = szNew;
//	}

//	// nothing to change
//	if( !Q_strcmp( pszValue, var->string ))
//		return var;

//	if( var->flags & CVAR_USERINFO )
//		userinfo->modified = true;	// transmit at next oportunity

//	if( var->flags & CVAR_PHYSICINFO )
//		physinfo->modified = true;	// transmit at next oportunity

//	if( var->flags & CVAR_SERVERINFO )
//		serverinfo->modified = true;	// transmit at next oportunity

//	if( var->flags & CVAR_RENDERINFO )
//		renderinfo->modified = true;	// transmit at next oportunity

//	// free the old value string
//	Mem_Free( var->string );
//	var->string = copystring( pszValue );
//	var->value = Q_atof( var->string );

//	if( !dll_variable )
//	{
//		var->integer = Q_atoi( var->string );
//		var->modified = true;
//	}

	return 0;// var;
}

/*
============
CCvar::Set
============
*/
void CCvar::set(const QString &var_name, const QString &value )
{
	set2( var_name, value, true );
}

/*
============
CCvar::SetLatched
============
*/
void CCvar::setLatched(const QString &var_name, const QString &value )
{
	set2( var_name, value, false );
}

/*
============
CCvar::FullSet
============
*/
void CCvar::fullSet(const QString &var_name, const QString &value, int flags )
{
//	SConVar	*var;
//	bool	dll_variable = false;

//	var = findVar( var_name );
//	if( !var )
//	{
//		// create it
//		get( var_name, value, flags, "" );
//		return;
//	}

//	// use this check to prevent acessing for unexisting fields
//	// for SCvar: latechd_string, description, etc
//	if( var->flags & CVAR_EXTDLL )
//	{
//		dll_variable = true;
//	}

//	if( var->flags & CVAR_USERINFO )
//	{
//		// transmit at next oportunity
//		userinfo->modified = true;
//	}

//	if( var->flags & CVAR_PHYSICINFO )
//	{
//		// transmit at next oportunity
//		physinfo->modified = true;
//	}

//	if( var->flags & CVAR_SERVERINFO )
//	{
//		// transmit at next oportunity
//		serverinfo->modified = true;
//	}

//	if( var->flags & CVAR_RENDERINFO )
//	{
//		// transmit at next oportunity
//		renderinfo->modified = true;
//	}

//	Mem_Free( var->string ); // free the old value string
//	var->string = copystring( value );
//	var->value = Q_atof( var->string );
//	var->flags = flags;

//	if( dll_variable ) return;	// below fields doesn't exist in SCvar

//	var->integer = Q_atoi( var->string );
//	var->modified = true;
}

/*
============
CCvar::DirectSet
============
*/
void CCvar::directSet(SCvar *var, const QString &value )
{
//	SCvar		*test;
//	const char	*pszValue;
//	char		szNew[MAX_SYSPATH];

//	if( !var ) return;	// GET_CVAR_POINTER is failed ?

//	// make sure what is really pointer to the cvar
//	test = (SCvar *)findVar( var->name );
//	ASSERT( var == test );

//	if( value && !CCvar::ValidateString( value, true ))
//	{
//		MsgDev( D_WARN, "invalid cvar value string: %s\n", value );
//		value = "0";
//	}

//	if( !value ) value = "0";

//	if( var->flags & ( CVAR_READ_ONLY|CVAR_GLCONFIG|CVAR_INIT|CVAR_RENDERINFO|CVAR_LATCH|CVAR_LATCH_VIDEO ))
//	{
//		// CCvar::DirectSet cannot change these cvars at all
//		return;
//	}

//	if(( var->flags & CVAR_CHEAT ) && !variableInteger( "sv_cheats" ))
//	{
//		// cheats are disabled
//		return;
//	}

//	pszValue = value;

//	// This cvar's string must only contain printable characters.
//	// Strip out any other crap.
//	// We'll fill in "empty" if nothing is left
//	if( var->flags & CVAR_PRINTABLEONLY )
//	{
//		const char	*pS;
//		char		*pD;

//		// clear out new string
//		szNew[0] = '\0';

//		pS = pszValue;
//		pD = szNew;

//		// step through the string, only copying back in characters that are printable
//		while( *pS )
//		{
//			if( *pS < 32 || *pS > 255 )
//			{
//				pS++;
//				continue;
//			}
//			*pD++ = *pS++;
//		}

//		// terminate the new string
//		*pD = '\0';

//		// if it's empty, then insert a marker string
//		if( !Q_strlen( szNew ))
//		{
//			Q_strcpy( szNew, "default" );
//		}

//		// point the value here.
//		pszValue = szNew;
//	}

//	if( !Q_strcmp( pszValue, var->string ))
//		return;

//	if( var->flags & CVAR_USERINFO )
//		userinfo->modified = true;	// transmit at next oportunity

//	if( var->flags & CVAR_PHYSICINFO )
//		physinfo->modified = true;	// transmit at next oportunity

//	if( var->flags & CVAR_SERVERINFO )
//		serverinfo->modified = true;	// transmit at next oportunity

//	if( var->flags & CVAR_RENDERINFO )
//		renderinfo->modified = true;	// transmit at next oportunity

//	// free the old value string
//	Mem_Free( var->string );
//	var->string = copystring( pszValue );
//	var->value = Q_atof( var->string );
}

/*
============
CCvar::SetFloat
============
*/
void CCvar::setFloat(const QString &var_name, float value )
{
//	char	val[32];

//	if( value == (int)value )
//		Q_sprintf( val, "%i", (int)value );
//	else Q_sprintf( val, "%f", value );
//	set( var_name, val );
}

/*
============
CCvar::Reset
============
*/
void CCvar::reset(const QString &var_name )
{
	set2( var_name, NULL, false );
}

/*
============
CCvar::SetCheatState

Any testing variables will be reset to the safe values
============
*/
void CCvar::setCheatState()
{
//	SConVar	*var;

//	// set all default vars to the safe value
//	for( var = cvar_vars; var; var = var->next )
//	{
//		// can't process dll cvars - missed latched_string, reset_string
//		if( var->flags & CVAR_EXTDLL )
//			continue;

//		if( var->flags & CVAR_CHEAT )
//		{
//			// the CVAR_LATCHED|CVAR_CHEAT vars might escape the reset here
//			// because of a different var->latched_string
//			if( var->latched_string )
//			{
//				Mem_Free( var->latched_string );
//				var->latched_string = NULL;
//			}

//			if( Q_strcmp( var->reset_string, var->string ))
//			{
//				set( var->name, var->reset_string );
//			}
//		}
//	}
}

/*
============
CCvar::Command

Handles variable inspection and changing from the console
============
*/
bool CCvar::command()
{
//	SConVar	*v;

//	// check variables
//	v = findVar( Cmd_Argv( 0 ));
//	if( !v ) return false;

//	// perform a variable print or set
//	if( Cmd_Argc() == 1 )
//	{
//		if( v->flags & ( CVAR_INIT|CVAR_EXTDLL )) Msg( "%s: %s\n", v->name, v->string );
//		else Msg( "%s: %s ( ^3%s^7 )\n", v->name, v->string, v->reset_string );
//		return true;
//	}

//	// set the value if forcing isn't required
//	set2( v->name, Cmd_Argv( 1 ), false );
	return true;
}

/*
============
CCvar::Toggle_f

Toggles a cvar for easy single key binding
============
*/
void CCvar::toggle_f()
{
//	int	v;

//	if( Cmd_Argc() != 2 )
//	{
//		Msg( "Usage: toggle <variable>\n" );
//		return;
//	}

//	v = (int)variableValue( Cmd_Argv( 1 ));
//	v = !v;

//	set2( Cmd_Argv( 1 ), va( "%i", v ), false );
}

/*
============
CCvar::Set_f

Allows setting and defining of arbitrary cvars from console, even if they
weren't declared in C code.
============
*/
void CCvar::set_f()
{
//	int	i, c, l = 0, len;
//	char	combined[MAX_CMD_TOKENS];

//	c = Cmd_Argc();
//	if( c < 3 )
//	{
//		Msg( "Usage: set <variable> <value>\n" );
//		return;
//	}
//	combined[0] = 0;

//	for( i = 2; i < c; i++ )
//	{
//		len = Q_strlen( Cmd_Argv(i) + 1 );
//		if( l + len >= MAX_CMD_TOKENS - 2 )
//			break;
//		Q_strcat( combined, Cmd_Argv( i ));
//		if( i != c-1 ) Q_strcat( combined, " " );
//		l += len;
//	}

//	set2( Cmd_Argv( 1 ), combined, false );
}

/*
============
CCvar::SetU_f

As CCvar::Set, but also flags it as userinfo
============
*/
void CCvar::setU_f()
{
//	SConVar	*v;

//	if( Cmd_Argc() != 3 )
//	{
//		Msg( "Usage: setu <variable> <value>\n" );
//		return;
//	}

//	set_f();
//	v = findVar( Cmd_Argv( 1 ));

//	if( !v ) return;
//	v->flags |= CVAR_USERINFO;
}

/*
============
CCvar::SetP_f

As CCvar::Set, but also flags it as physinfo
============
*/
void CCvar::setP_f()
{
//	SConVar	*v;

//	if( Cmd_Argc() != 3 )
//	{
//		Msg( "Usage: setp <variable> <value>\n" );
//		return;
//	}

//	set_f();
//	v = findVar( Cmd_Argv( 1 ));

//	if( !v ) return;
//	v->flags |= CVAR_PHYSICINFO;
}

/*
============
CCvar::SetS_f

As CCvar::Set, but also flags it as serverinfo
============
*/
void CCvar::setS_f()
{
//	SConVar	*v;

//	if( Cmd_Argc() != 3 )
//	{
//		Msg( "Usage: sets <variable> <value>\n" );
//		return;
//	}
//	set_f();
//	v = CCvar::FindVar( Cmd_Argv( 1 ));

//	if( !v ) return;
//	v->flags |= CVAR_SERVERINFO;
}

/*
============
CCvar::SetA_f

As CCvar::Set, but also flags it as archived
============
*/
void CCvar::setA_f()
{
//	SConVar	*v;

//	if( Cmd_Argc() != 3 )
//	{
//		Msg( "Usage: seta <variable> <value>\n" );
//		return;
//	}

//	set_f();
//	v = CCvar::findVar( Cmd_Argv( 1 ));

//	if( !v ) return;
//	v->flags |= CVAR_ARCHIVE;
}

/*
============
CCvar::SetR_f

As CCvar::Set, but also flags it as renderinfo
============
*/
void CCvar::setR_f()
{
//	if( Cmd_Argc() != 3 )
//	{
//		Msg( "Usage: setr <variable> <value>\n" );
//		return;
//	}

//	fullSet( Cmd_Argv( 1 ), Cmd_Argv( 2 ), CVAR_RENDERINFO );
}

/*
============
CCvar::SetGL_f

As CCvar::Set, but also flags it as glconfig
============
*/
void CCvar::setGL_f()
{
//	if( Cmd_Argc() != 3 )
//	{
//		Msg( "Usage: setgl <variable> <value>\n" );
//		return;
//	}

//	fullSet( Cmd_Argv( 1 ), Cmd_Argv( 2 ), CVAR_GLCONFIG );
}

/*
============
CCvar::Reset_f
============
*/
void CCvar::reset_f()
{
//	if( Cmd_Argc() != 2 )
//	{
//		Msg( "Usage: reset <variable>\n" );
//		return;
//	}
//	reset( Cmd_Argv( 1 ));
}

/*
============
CCvar::List_f
============
*/
void CCvar::list_f()
{
//	SConVar	*var;
//	char	*match = NULL;
//	int	i = 0, j = 0;

//	if( Cmd_Argc() > 1 )
//		match = Cmd_Argv( 1 );

//	for( var = cvar_vars; var; var = var->next, i++ )
//	{
//		if( var->name[0] == '@' )
//			continue;	// never shows system cvars

//		if( match && !Q_stricmpext( match, var->name ))
//			continue;

//		if( var->flags & CVAR_SERVERINFO ) Msg( "SV    " );
//		else Msg( " " );

//		if( var->flags & CVAR_USERINFO ) Msg( "USER  " );
//		else Msg( " " );

//		if( var->flags & CVAR_PHYSICINFO ) Msg( "PHYS  " );
//		else Msg( " " );

//		if( var->flags & CVAR_READ_ONLY ) Msg( "READ  " );
//		else Msg( " " );

//		if( var->flags & CVAR_INIT ) Msg( "INIT  " );
//		else Msg( " " );

//		if( var->flags & CVAR_ARCHIVE ) Msg( "ARCH  " );
//		else Msg( " " );

//		if( var->flags & CVAR_LATCH ) Msg( "LATCH " );
//		else Msg( " " );

//		if( var->flags & CVAR_LATCH_VIDEO ) Msg( "VIDEO " );
//		else Msg( " " );

//		if( var->flags & CVAR_GLCONFIG ) Msg( "OPENGL" );
//		else Msg( " " );

//		if( var->flags & CVAR_CHEAT ) Msg( "CHEAT " );
//		else Msg( " " );

//		if( var->flags & CVAR_EXTDLL )
//			Msg(" %s \"%s\" %s\n", var->name, var->string, "game cvar" );
//		else Msg(" %s \"%s\" %s\n", var->name, var->string, var->description );
//		j++;
//	}

//	Msg( "\n%i cvars\n", j );
//	Msg( "%i total cvars\n", i );
}

/*
============
CCvar::Restart_f

Resets all cvars to their hardcoded values
============
*/
void CCvar::restart_f()
{
//	SConVar	*var;
//	SConVar	**prev;

//	prev = &cvar_vars;

//	while( 1 )
//	{
//		var = *prev;
//		if( !var ) break;

//		// don't mess with rom values, or some inter-module
//		// communication will get broken (cl.active, etc)
//		if( var->flags & ( CVAR_READ_ONLY|CVAR_GLCONFIG|CVAR_INIT|CVAR_RENDERINFO|CVAR_EXTDLL ))
//		{
//			prev = &var->next;
//			continue;
//		}

//		// throw out any variables the user created
//		if( var->flags & CVAR_USER_CREATED )
//		{
//			*prev = var->next;
//			if( var->name ) Mem_Free( var->name );
//			if( var->string ) Mem_Free( var->string );
//			if( var->latched_string ) Mem_Free( var->latched_string );
//			if( var->reset_string ) Mem_Free( var->reset_string );
//			if( var->description ) Mem_Free( var->description );
//			Mem_Free( var );

//			continue;
//		}

//		set( var->name, var->reset_string );
//		prev = &var->next;
//	}
}

/*
============
CCvar::Latched_f

Now all latched strings is valid
============
*/
void CCvar::latched_f()
{
//	SConVar	*var;
//	SConVar	**prev;

//	prev = &cvar_vars;

//	while( 1 )
//	{
//		var = *prev;
//		if( !var ) break;

//		if( var->flags & CVAR_EXTDLL )
//		{
//			prev = &var->next;
//			continue;
//		}

//		if( var->flags & CVAR_LATCH && var->latched_string )
//		{
//			fullSet( var->name, var->latched_string, var->flags );
//			Mem_Free( var->latched_string );
//			var->latched_string = NULL;
//		}
//		prev = &var->next;
//	}
}

/*
============
CCvar::LatchedVideo_f

Now all latched video strings is valid
============
*/
void CCvar::latchedVideo_f()
{
//	SConVar	*var;
//	SConVar	**prev;

//	prev = &cvar_vars;

//	while ( 1 )
//	{
//		var = *prev;
//		if( !var ) break;

//		if( var->flags & CVAR_EXTDLL )
//		{
//			prev = &var->next;
//			continue;
//		}

//		if( var->flags & CVAR_LATCH_VIDEO && var->latched_string )
//		{
//			fullSet( var->name, var->latched_string, var->flags );
//			Mem_Free( var->latched_string );
//			var->latched_string = NULL;
//		}
//		prev = &var->next;
//	}
}

/*
============
CCvar::Unlink_f

unlink all cvars with flag CVAR_EXTDLL
============
*/
void CCvar::unlink_f()
{
//	SConVar	*var;
//	SConVar	**prev;
//	int	count = 0;

//	if( variableInteger( "host_gameloaded" ))
//	{
//		MsgDev( D_NOTE, "can't unlink cvars while game is loaded\n" );
//		return;
//	}

//	prev = &cvar_vars;

//	while( 1 )
//	{
//		var = *prev;
//		if( !var ) break;

//		// ignore all non-game cvars
//		if( !( var->flags & CVAR_EXTDLL ))
//		{
//			prev = &var->next;
//			continue;
//		}

//		// throw out any variables the game created
//		*prev = var->next;
//		if( var->string ) Mem_Free( var->string );
//		count++;
//	}
}

/*
============
CCvar::Unlink

unlink all cvars with flag CVAR_CLIENTDLL
============
*/
void CCvar::unlink()
{
//	SConVar	*var;
//	SConVar	**prev;
//	int	count = 0;

//	if( variableInteger( "host_clientloaded" ))
//	{
//		MsgDev( D_NOTE, "can't unlink cvars while client is loaded\n" );
//		return;
//	}

//	prev = &cvar_vars;

//	while( 1 )
//	{
//		var = *prev;
//		if( !var ) break;

//		// ignore all non-client cvars
//		if(!( var->flags & CVAR_CLIENTDLL ))
//		{
//			prev = &var->next;
//			continue;
//		}

//		// throw out any variables the game created
//		*prev = var->next;
//		if( var->name ) Mem_Free( var->name );
//		if( var->string ) Mem_Free( var->string );
//		if( var->latched_string ) Mem_Free( var->latched_string );
//		if( var->reset_string ) Mem_Free( var->reset_string );
//		if( var->description ) Mem_Free( var->description );
//		Mem_Free( var );
//		count++;
//	}
}

