/*
filesystem.c - game filesystem based on DP fs
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

#include <fcntl.h>
#include <direct.h>
#include <sys/stat.h>
#include <io.h>
#include <time.h>

#include "fs.hpp"
//#include "library.h"
//#include "mathlib.h"


/*
=============================================================================

FILEMATCH COMMON SYSTEM

=============================================================================
*/
int Fs::matchpattern( const char *in, const char *pattern, bool caseinsensitive )
{
	int	c1, c2;

	while( *pattern )
	{
		switch( *pattern )
		{
		case 0:   return 1; // end of pattern
		case '?': // match any single character
			if( *in == 0 || *in == '/' || *in == '\\' || *in == ':' )
				return 0; // no match
			in++;
			pattern++;
			break;
		case '*': // match anything until following string
			if( !*in ) return 1; // match
			pattern++;
			while( *in )
			{
				if( *in == '/' || *in == '\\' || *in == ':' )
					break;
				// see if pattern matches at this offset
				if( matchpattern( in, pattern, caseinsensitive ))
					return 1;
				// nope, advance to next offset
				in++;
			}
			break;
		default:
			if( *in != *pattern )
			{
				if( !caseinsensitive )
					return 0; // no match
				c1 = *in;
				if( c1 >= 'A' && c1 <= 'Z' )
					c1 += 'a' - 'A';
				c2 = *pattern;
				if( c2 >= 'A' && c2 <= 'Z' )
					c2 += 'a' - 'A';
				if( c1 != c2) return 0; // no match
			}
			in++;
			pattern++;
			break;
		}
	}

	if( *in ) return 0; // reached end of pattern but not end of input
	return 1; // success
}

void Fs::listdirectory( QStringList list, const char *path )
{
	int		i;
	char		pattern[4096], *c;
	struct _finddata_t	n_file;
	long		hFile;

//	Q_strncpy( pattern, path, sizeof( pattern ));
//	Q_strncat( pattern, "*", sizeof( pattern ));

//	// ask for the directory listing handle
//	hFile = _findfirst( pattern, &n_file );
//	if( hFile == -1 ) return;

//	// start a new chain with the the first name
//	stringlistappend( list, n_file.name );
//	// iterate through the directory
//	while( _findnext( hFile, &n_file ) == 0 )
//		stringlistappend( list, n_file.name );
//	_findclose( hFile );

//	// convert names to lowercase because windows doesn't care, but pattern matching code often does
//	for( i = 0; i < list->numstrings; i++ )
//	{
//		for( c = list->strings[i]; *c; c++ )
//		{
//			if( *c >= 'A' && *c <= 'Z' )
//				*c += 'a' - 'A';
//		}
//	}
}

/*
=============================================================================

OTHER PRIVATE FUNCTIONS

=============================================================================
*/
/*
====================
FS_AddFileToPack

Add a file to the list of files contained into a package
====================
*/
SPackFile* Fs::FS_AddFileToPack( const char* name, SPack* pack, int offset, int size )
{
	int		left, right, middle;
	SPackFile	*pfile;

//	// look for the slot we should put that file into (binary search)
//	left = 0;
//	right = pack->numfiles - 1;
//	while( left <= right )
//	{
//		int diff;

//		middle = (left + right) / 2;
//		diff = Q_stricmp( pack->files[middle].name, name );

//		// If we found the file, there's a problem
//		if( !diff ) MsgDev( D_NOTE, "Package %s contains the file %s several times\n", pack->filename, name );

//		// If we're too far in the list
//		if( diff > 0 ) right = middle - 1;
//		else left = middle + 1;
//	}

//	// We have to move the right of the list by one slot to free the one we need
//	pfile = &pack->files[left];
//	Q_memmove( pfile + 1, pfile, (pack->numfiles - left) * sizeof( *pfile ));
//	pack->numfiles++;

//	Q_strncpy( pfile->name, name, sizeof( pfile->name ));
//	pfile->offset = offset;
//	pfile->realsize = size;

	return pfile;
}

/*
============
FS_CreatePath

Only used for FS_Open.
============
*/
void Fs::FS_CreatePath( char *path )
{
	char *ofs, save;

	for( ofs = path+1; *ofs; ofs++ )
	{
		if( *ofs == '/' || *ofs == '\\' )
		{
			// create the directory
			save = *ofs;
			*ofs = 0;
			_mkdir( path );
			*ofs = save;
		}
	}
}

/*
============
FS_Path_f

debug info
============
*/
void Fs::FS_Path_f()
{
	SSearchPath	*s;

//	Msg( "Current search path:\n" );

//	for( s = fs_searchpaths; s; s = s->next )
//	{
//		if( s->pack ) Msg( "%s (%i files)", s->pack->filename, s->pack->numfiles );
//		else if( s->wad ) Msg( "%s (%i files)", s->wad->filename, s->wad->numlumps );
//		else Msg( "%s", s->filename );

//		if( s->flags & FS_GAMEDIR_PATH ) Msg( " ^2gamedir^7\n" );
//		else Msg( "\n" );
//	}
}

/*
============
FS_ClearPath_f

only for debug targets
============
*/
void Fs::FS_ClearPaths_f()
{
//	FS_ClearSearchPath();
}

/*
============
FS_FileBase

Extracts the base name of a file (no path, no extension, assumes '/' as path separator)
============
*/
void Fs::FS_FileBase( const char *in, char *out )
{
	int	len, start, end;

	len = 0;//Q_strlen( in );
	if( !len ) return;

	// scan backward for '.'
	end = len - 1;

	while( end && in[end] != '.' && in[end] != '/' && in[end] != '\\' )
		end--;

	if( in[end] != '.' )
		end = len-1; // no '.', copy to end
	else end--; // found ',', copy to left of '.'


	// scan backward for '/'
	start = len - 1;

	while( start >= 0 && in[start] != '/' && in[start] != '\\' )
		start--;

	if( start < 0 || ( in[start] != '/' && in[start] != '\\' ))
		start = 0;
	else start++;

	// length of new sting
	len = end - start + 1;

	// Copy partial string
	//Q_strncpy( out, &in[start], len + 1 );
	out[len] = 0;
}

/*
=================
FS_LoadPackPAK

Takes an explicit (not game tree related) path to a pak file.

Loads the header and directory, adding the files at the beginning
of the list so they override previous pack files.
=================
*/
SPack *Fs::FS_LoadPackPAK( const char *packfile, int *error )
{
	//SDpackHeader	header;
	int		i, numpackfiles;
	int		packhandle;
	SPack		*pack;
//	SDPackFile	*info;

//	packhandle = open( packfile, O_RDONLY|O_BINARY );

//	if( packhandle < 0 )
//	{
//		MsgDev( D_NOTE, "%s couldn't open\n", packfile );
//		if( error ) *error = PAK_LOAD_COULDNT_OPEN;
//		return NULL;
//	}

//	read( packhandle, (void *)&header, sizeof( header ));

//	if( header.ident != IDPACKV1HEADER )
//	{
//		MsgDev( D_NOTE, "%s is not a packfile. Ignored.\n", packfile );
//		if( error ) *error = PAK_LOAD_BAD_HEADER;
//		close( packhandle );
//		return NULL;
//	}

//	if( header.dirlen % sizeof( dSPackFile ))
//	{
//		MsgDev( D_ERROR, "%s has an invalid directory size. Ignored.\n", packfile );
//		if( error ) *error = PAK_LOAD_BAD_FOLDERS;
//		close( packhandle );
//		return NULL;
//	}

//	numpackfiles = header.dirlen / sizeof( dSPackFile );

//	if( numpackfiles > MAX_FILES_IN_PACK )
//	{
//		MsgDev( D_ERROR, "%s has too many files ( %i ). Ignored.\n", packfile, numpackfiles );
//		if( error ) *error = PAK_LOAD_TOO_MANY_FILES;
//		close( packhandle );
//		return NULL;
//	}

//	if( numpackfiles <= 0 )
//	{
//		MsgDev( D_NOTE, "%s has no files. Ignored.\n", packfile );
//		if( error ) *error = PAK_LOAD_NO_FILES;
//		close( packhandle );
//		return NULL;
//	}

//	info = (dSPackFile *)Mem_Alloc( fs_mempool, sizeof( *info ) * numpackfiles );
//	lseek( packhandle, header.dirofs, SEEK_SET );

//	if( header.dirlen != read( packhandle, (void *)info, header.dirlen ))
//	{
//		MsgDev( D_NOTE, "%s is an incomplete PAK, not loading\n", packfile );
//		if( error ) *error = PAK_LOAD_CORRUPTED;
//		close( packhandle );
//		Mem_Free( info );
//		return NULL;
//	}

//	pack = (SPack *)Mem_Alloc( fs_mempool, sizeof( SPack ));
//	Q_strncpy( pack->filename, packfile, sizeof( pack->filename ));
//	pack->handle = packhandle;
//	pack->numfiles = 0;
//	pack->files = (SPackFile *)Mem_Alloc( fs_mempool, numpackfiles * sizeof( SPackFile ));
//	pack->filetime = FS_SysFileTime( packfile );

//	// parse the directory
//	for( i = 0; i < numpackfiles; i++ )
//	{
//		FS_AddFileToPack( info[i].name, pack, info[i].filepos, info[i].filelen );
//	}

//	MsgDev( D_NOTE, "Adding packfile: %s (%i files)\n", packfile, numpackfiles );
//	if( error ) *error = PAK_LOAD_OK;
//	Mem_Free( info );

	return pack;
}

/*
================
FS_AddPack_Fullpath

Adds the given pack to the search path.
The pack type is autodetected by the file extension.

Returns true if the file was successfully added to the
search path or if it was already included.

If keep_plain_dirs is set, the pack will be added AFTER the first sequence of
plain directories.
================
*/
bool Fs::FS_AddPack_Fullpath( const char *pakfile, bool *already_loaded, bool keep_plain_dirs, int flags )
{
	return false;
//	SSearchPath	*search;
//	SPack		*pak = NULL;
//	const char	*ext = FS_FileExtension( pakfile );
//	int		errorcode = PAK_LOAD_COULDNT_OPEN;

//	for( search = fs_searchpaths; search; search = search->next )
//	{
//		if( search->pack && !Q_stricmp( search->pack->filename, pakfile ))
//		{
//			if( already_loaded ) *already_loaded = true;
//			return true; // already loaded
//		}
//	}

//	if( already_loaded ) *already_loaded = false;

//	if( !Q_stricmp( ext, "pak" )) pak = FS_LoadPackPAK( pakfile, &errorcode );
//	else MsgDev( D_ERROR, "\"%s\" does not have a pack extension\n", pakfile );

//	if( pak )
//	{
//		if( keep_plain_dirs )
//		{
//			// find the first item whose next one is a pack or NULL
//			SSearchPath *insertion_point = 0;
//			if( fs_searchpaths && !fs_searchpaths->pack )
//			{
//				insertion_point = fs_searchpaths;
//				while( 1 )
//				{
//					if( !insertion_point->next ) break;
//					if( insertion_point->next->pack ) break;
//					insertion_point = insertion_point->next;
//				}
//			}

//			// if insertion_point is NULL, this means that either there is no
//			// item in the list yet, or that the very first item is a pack. In
//			// that case, we want to insert at the beginning...
//			if( !insertion_point )
//			{
//				search = (SSearchPath *)Mem_Alloc( fs_mempool, sizeof( SSearchPath ));
//				search->pack = pak;
//				search->next = fs_searchpaths;
//				search->flags |= flags;
//				fs_searchpaths = search;
//			}
//			else // otherwise we want to append directly after insertion_point.
//			{
//				search = (SSearchPath *)Mem_Alloc( fs_mempool, sizeof( SSearchPath ));
//				search->pack = pak;
//				search->next = insertion_point->next;
//				search->flags |= flags;
//				insertion_point->next = search;
//			}
//		}
//		else
//		{
//			search = (SSearchPath *)Mem_Alloc( fs_mempool, sizeof( SSearchPath ));
//			search->pack = pak;
//			search->next = fs_searchpaths;
//			search->flags |= flags;
//			fs_searchpaths = search;
//		}
//		return true;
//	}
//	else
//	{
//		if( errorcode != PAK_LOAD_NO_FILES )
//			MsgDev( D_ERROR, "FS_AddPack_Fullpath: unable to load pak \"%s\"\n", pakfile );
//		return false;
//	}
}

/*
====================
FS_AddWad_Fullpath
====================
*/
bool Fs::FS_AddWad_Fullpath( const char *wadfile, bool *already_loaded, bool keep_plain_dirs )
{
	SSearchPath	*search;
	SWfile		*wad = NULL;
//	const char	*ext = FS_FileExtension( wadfile );
	return false;
//	for( search = fs_searchpaths; search; search = search->next )
//	{
//		if( search->wad && !Q_stricmp( search->wad->filename, wadfile ))
//		{
//			if( already_loaded ) *already_loaded = true;
//			return true; // already loaded
//		}
//	}

//	if( already_loaded ) *already_loaded = false;
//	if( !Q_stricmp( ext, "wad" )) wad = W_Open( wadfile, "rb" );
//	else MsgDev( D_ERROR, "\"%s\" doesn't have a wad extension\n", wadfile );

//	if( wad )
//	{
//		if( keep_plain_dirs )
//		{
//			// find the first item whose next one is a wad or NULL
//			SSearchPath *insertion_point = NULL;
//			if( fs_searchpaths && !fs_searchpaths->wad )
//			{
//				insertion_point = fs_searchpaths;
//				while( 1 )
//				{
//					if( !insertion_point->next ) break;
//					if( insertion_point->next->wad ) break;
//					insertion_point = insertion_point->next;
//				}
//			}
//			// if insertion_point is NULL, this means that either there is no
//			// item in the list yet, or that the very first item is a wad. In
//			// that case, we want to insert at the beginning...
//			if( !insertion_point )
//			{
//				search = (SSearchPath *)Mem_Alloc( fs_mempool, sizeof( SSearchPath ));
//				search->wad = wad;
//				search->next = fs_searchpaths;
//				fs_searchpaths = search;
//			}
//			else // otherwise we want to append directly after insertion_point.
//			{
//				search = (SSearchPath *)Mem_Alloc( fs_mempool, sizeof( SSearchPath ));
//				search->wad = wad;
//				search->next = insertion_point->next;
//				insertion_point->next = search;
//			}
//		}
//		else
//		{
//			search = (SSearchPath *)Mem_Alloc( fs_mempool, sizeof( SSearchPath ));
//			search->wad = wad;
//			search->next = fs_searchpaths;
//			fs_searchpaths = search;
//		}

//		MsgDev( D_NOTE, "Adding wadfile %s (%i files)\n", wadfile, wad->numlumps );
//		return true;
//	}
//	else
//	{
//		MsgDev( D_ERROR, "FS_AddWad_Fullpath: unable to load wad \"%s\"\n", wadfile );
//		return false;
//	}
}

/*
================
FS_AddGameDirectory

Sets fs_gamedir, adds the directory to the head of the path,
then loads and adds pak1.pak pak2.pak ...
================
*/
void Fs::FS_AddGameDirectory( const char *dir, int flags )
{
//	QStringList	list;
//	SSearchPath	*search;
//	string		fullpath;
//	int		i;

//	if(!( flags & FS_NOWRITE_PATH ))
//		Q_strncpy( fs_gamedir, dir, sizeof( fs_gamedir ));

//	stringlistinit( &list );
//	listdirectory( &list, dir );
//	stringlistsort( &list );

//	// add any PAK package in the directory
//	for( i = 0; i < list.numstrings; i++ )
//	{
//		if( !Q_stricmp( FS_FileExtension( list.strings[i] ), "pak" ))
//		{
//			Q_sprintf( fullpath, "%s%s", dir, list.strings[i] );
//			FS_AddPack_Fullpath( fullpath, NULL, false, flags );
//		}
//	}

//	// add any WAD package in the directory
//	for( i = 0; i < list.numstrings; i++ )
//	{
//		if( !Q_stricmp( FS_FileExtension( list.strings[i] ), "wad" ))
//		{
//			Q_sprintf( fullpath, "%s%s", dir, list.strings[i] );
//			FS_AddWad_Fullpath( fullpath, NULL, false );
//		}
//	}

//	stringlistfreecontents( &list );

//	// add the directory to the search path
//	// (unpacked files have the priority over packed files)
//	search = (SSearchPath *)Mem_Alloc( fs_mempool, sizeof( SSearchPath ));
//	Q_strncpy( search->filename, dir, sizeof ( search->filename ));
//	search->next = fs_searchpaths;
//	search->flags = flags;
//	fs_searchpaths = search;


}

/*
================
FS_AddGameHierarchy
================
*/
void Fs::FS_AddGameHierarchy( const char *dir, int flags )
{
	// Add the common game directory
//	if( dir && *dir ) FS_AddGameDirectory( va( "%s%s/", fs_basedir, dir ), flags );
}

/*
============
FS_FileExtension
============
*/
const char *Fs::FS_FileExtension( const char *in )
{
	const char *separator, *backslash, *colon, *dot;

//	separator = Q_strrchr( in, '/' );
//	backslash = Q_strrchr( in, '\\' );
//	if( !separator || separator < backslash ) separator = backslash;
//	colon = Q_strrchr( in, ':' );
//	if( !separator || separator < colon ) separator = colon;

//	dot = Q_strrchr( in, '.' );
//	if( dot == NULL || ( separator && ( dot < separator )))
//		return "";
	return dot + 1;
}

/*
============
FS_FileWithoutPath
============
*/
const char *Fs::FS_FileWithoutPath( const char *in )
{
	const char *separator, *backslash, *colon;

//	separator = Q_strrchr( in, '/' );
//	backslash = Q_strrchr( in, '\\' );
//	if( !separator || separator < backslash ) separator = backslash;
//	colon = Q_strrchr( in, ':' );
//	if( !separator || separator < colon ) separator = colon;
	return separator ? separator + 1 : in;
}

/*
============
FS_ExtractFilePath
============
*/
void Fs::FS_ExtractFilePath( const char* const path, char* dest )
{
	const char	*src;
//	src = path + Q_strlen( path ) - 1;

//	// back up until a \ or the start
//	while( src != path && !(*(src - 1) == '\\' || *(src - 1) == '/' ))
//		src--;

//	if( src != path )
//	{
//		Q_memcpy( dest, path, src - path );
//		dest[src - path - 1] = 0; // cutoff backslash
//	}
//	else Q_strcpy( dest, "" ); // file without path
}

/*
================
FS_ClearSearchPath
================
*/
void Fs::FS_ClearSearchPath()
{
//	while( fs_searchpaths )
//	{
//		SSearchPath	*search = fs_searchpaths;

//		if( search->flags & FS_STATIC_PATH )
//		{
//			// skip read-only pathes
//			if( search->next )
//				fs_searchpaths = search->next->next;
//			else break;
//		}
//		else fs_searchpaths = search->next;

//		if( search->pack )
//		{
//			if( search->pack->files )
//				Mem_Free( search->pack->files );
//			Mem_Free( search->pack );
//		}
//		if( search->wad )
//		{
//			W_Close( search->wad );
//		}

//		if( search ) Mem_Free( search );
//	}
}

/*
====================
FS_CheckNastyPath

Return true if the path should be rejected due to one of the following:
1: path elements that are non-portable
2: path elements that would allow access to files outside the game directory,
   or are just not a good idea for a mod to be using.
====================
*/
int Fs::FS_CheckNastyPath( const char *path, bool isgamedir )
{
//	// all: never allow an empty path, as for gamedir it would access the parent directory and a non-gamedir path it is just useless
//	if( !path[0] ) return 2;

//	// Mac: don't allow Mac-only filenames - : is a directory separator
//	// instead of /, but we rely on / working already, so there's no reason to
//	// support a Mac-only path
//	// Amiga and Windows: : tries to go to root of drive
//	if( Q_strstr( path, ":" ) && !fs_ext_path ) return 1; // non-portable attempt to go to root of drive

//	// Amiga: // is parent directory
//	if( Q_strstr( path, "//" )  && !fs_ext_path ) return 1; // non-portable attempt to go to parent directory

//	// all: don't allow going to parent directory (../ or /../)
//	if( Q_strstr( path, ".." ) && !fs_ext_path ) return 2; // attempt to go outside the game directory

//	// Windows and UNIXes: don't allow absolute paths
//	if( path[0] == '/' && !fs_ext_path ) return 2; // attempt to go outside the game directory
//#if 0
//	// all: don't allow . characters before the last slash (it should only be used in filenames, not path elements),
//	// this catches all imaginable cases of ./, ../, .../, etc
//	if( Q_strchr( path, '.' ) && !fs_ext_path )
//	{
//		if( isgamedir ) return 2; // gamedir is entirely path elements, so simply forbid . entirely
//		if( Q_strchr( path, '.' ) < Q_strrchr( path, '/' )) return 2; // possible attempt to go outside the game directory
//	}
//#endif
//	// all: forbid trailing slash on gamedir
//	if( isgamedir && !fs_ext_path && path[Q_strlen( path )-1] == '/' ) return 2;

//	// all: forbid leading dot on any filename for any reason
//	if( Q_strstr( path, "/." ) && !fs_ext_path ) return 2; // attempt to go outside the game directory

//	// after all these checks we're pretty sure it's a / separated filename
//	// and won't do much if any harm
	return 0;
}

/*
================
FS_Rescan
================
*/
void Fs::FS_Rescan()
{
//	MsgDev( D_NOTE, "FS_Rescan( %s )\n", GI->title );

//	FS_ClearSearchPath();

//	if( Q_stricmp( GI->basedir, GI->gamedir ))
//		FS_AddGameHierarchy( GI->basedir, 0 );
//	if( Q_stricmp( GI->basedir, GI->falldir ) && Q_stricmp( GI->gamedir, GI->falldir ))
//		FS_AddGameHierarchy( GI->falldir, 0 );
//	FS_AddGameHierarchy( GI->gamedir, FS_GAMEDIR_PATH );
}

void Fs::FS_Rescan_f()
{
	FS_Rescan();
}

bool Fs::FS_ParseVector( char **pfile, float *v, size_t size )
{
//	string	token;
//	bool	bracket = false;
//	char	*saved;
//	uint	i;

//	if( v == NULL || size == 0 )
//		return false;

//	Q_memset( v, 0, sizeof( *v ) * size );

//	if( size == 1 )
//	{
//		*pfile = COM_ParseFile( *pfile, token );
//		v[0] = Q_atof( token );
//		return true;
//	}

//	saved = *pfile;

//	if(( *pfile = COM_ParseFile( *pfile, token )) == NULL )
//		return false;

//	if( token[0] == '(' )
//		bracket = true;
//	else *pfile = saved; // restore token to right get it again

//	for( i = 0; i < size; i++ )
//	{
//		*pfile = COM_ParseFile( *pfile, token );
//		v[i] = Q_atof( token );
//	}

//	if( !bracket ) return true;	// done

//	if(( *pfile = COM_ParseFile( *pfile, token )) == NULL )
//		return false;

//	if( token[0] == ')' )
//		return true;
	return false;
}

/*
================
FS_WriteGameInfo

assume GameInfo is valid
================
*/
bool Fs::FS_WriteGameInfo( const char *filepath, SGameInfo *GameInfo )
{
//	SFile	*f;
//	int	i;

//	if( !GameInfo ) return false;
//	f = FS_Open( filepath, "w", false );	// we in binary-mode
//	if( !f ) return false;

//	FS_Print( f, "// generated by Xash3D\n\n\n" );

//	if( Q_strlen( GameInfo->basedir ))
//		FS_Printf( f, "basedir\t\t\"%s\"\n", GameInfo->basedir );

//	if( Q_strlen( GameInfo->gamedir ))
//		FS_Printf( f, "gamedir\t\t\"%s\"\n", GameInfo->gamedir );

//	if( Q_strlen( GameInfo->falldir ))
//		FS_Printf( f, "fallback_dir\t\"%s\"\n", GameInfo->falldir );

//	if( Q_strlen( GameInfo->title ))
//		FS_Printf( f, "title\t\t\"%s\"\n", GameInfo->title );

//	if( Q_strlen( GameInfo->startmap ))
//		FS_Printf( f, "startmap\t\t\"%s\"\n", GameInfo->startmap );

//	if( Q_strlen( GameInfo->trainmap ))
//		FS_Printf( f, "trainmap\t\t\"%s\"\n", GameInfo->trainmap );

//	if( GameInfo->version != 0.0f )
//		FS_Printf( f, "version\t\t%g\n", GameInfo->version );

//	if( GameInfo->size != 0 )
//		FS_Printf( f, "size\t\t%i\n", GameInfo->size );

//	if( Q_strlen( GameInfo->game_url ))
//		FS_Printf( f, "url_info\t\t\"%s\"\n", GameInfo->game_url );

//	if( Q_strlen( GameInfo->update_url ))
//		FS_Printf( f, "url_update\t\t\"%s\"\n", GameInfo->update_url );

//	if( Q_strlen( GameInfo->type ))
//		FS_Printf( f, "type\t\t\"%s\"\n", GameInfo->type );

//	if( Q_strlen( GameInfo->date ))
//		FS_Printf( f, "date\t\t\"%s\"\n", GameInfo->date );

//	if( Q_strlen( GameInfo->dll_path ))
//		FS_Printf( f, "dllpath\t\t\"%s\"\n", GameInfo->dll_path );
//	if( Q_strlen( GameInfo->game_dll ))
//		FS_Printf( f, "gamedll\t\t\"%s\"\n", GameInfo->game_dll );

//	if( Q_strlen( GameInfo->iconpath ))
//		FS_Printf( f, "icon\t\t\"%s\"\n", GameInfo->iconpath );

//	switch( GameInfo->gamemode )
//	{
//	case 1: FS_Print( f, "gamemode\t\t\"singleplayer_only\"\n" ); break;
//	case 2: FS_Print( f, "gamemode\t\t\"multiplayer_only\"\n" ); break;
//	}

//	if( Q_strlen( GameInfo->sp_entity ))
//		FS_Printf( f, "sp_entity\t\t\"%s\"\n", GameInfo->sp_entity );
//	if( Q_strlen( GameInfo->mp_entity ))
//		FS_Printf( f, "mp_entity\t\t\"%s\"\n", GameInfo->mp_entity );

//	if( GameInfo->secure )
//		FS_Printf( f, "secure\t\t\"%i\"\n", GameInfo->secure );

//	if( GameInfo->nomodels )
//		FS_Printf( f, "nomodels\t\t\"%i\"\n", GameInfo->nomodels );

//	if( GameInfo->soundclip_dist > 0 )
//		FS_Printf( f, "soundclip_dist\t\"%i\"\n", GameInfo->soundclip_dist );

//	for( i = 0; i < 4; i++ )
//	{
//		float	*min, *max;

//		if( i && ( VectorIsNull( GameInfo->client_mins[i] ) || VectorIsNull( GameInfo->client_maxs[i] )))
//			continue;

//		min = GameInfo->client_mins[i];
//		max = GameInfo->client_maxs[i];
//		FS_Printf( f, "hull%i\t\t( %g %g %g ) ( %g %g %g )\n", i, min[0], min[1], min[2], max[0], max[1], max[2] );
//	}

//	if( GameInfo->max_edicts > 0 )
//		FS_Printf( f, "max_edicts\t%i\n", GameInfo->max_edicts );
//	if( GameInfo->max_tents > 0 )
//		FS_Printf( f, "max_tempents\t%i\n", GameInfo->max_tents );
//	if( GameInfo->max_beams > 0 )
//		FS_Printf( f, "max_beams\t\t%i\n", GameInfo->max_beams );
//	if( GameInfo->max_particles > 0 )
//		FS_Printf( f, "max_particles\t%i\n", GameInfo->max_particles );

//	FS_Print( f, "\n\n\n" );
//	FS_Close( f );	// all done

	return true;
}

/*
================
FS_CreateDefaultGameInfo
================
*/
void Fs::FS_CreateDefaultGameInfo( const char *filename )
{
//	gameinfo_t	defGI;

//	Q_memset( &defGI, 0, sizeof( defGI ));

//	// setup default values
//	defGI.max_edicts = 900;	// default value if not specified
//	defGI.max_tents = 500;
//	defGI.max_beams = 128;
//	defGI.soundclip_dist = 1536;
//	defGI.max_particles = 4096;
//	defGI.version = 1.0;
//	defGI.falldir[0] = '\0';

//	Q_strncpy( defGI.title, "New Game", sizeof( defGI.title ));
//	Q_strncpy( defGI.gamedir, gs_basedir, sizeof( defGI.gamedir ));
//	Q_strncpy( defGI.basedir, SI.ModuleName, sizeof( defGI.basedir ));
//	Q_strncpy( defGI.sp_entity, "info_player_start", sizeof( defGI.sp_entity ));
//	Q_strncpy( defGI.mp_entity, "info_player_deathmatch", sizeof( defGI.mp_entity ));
//	Q_strncpy( defGI.dll_path, "cl_dlls", sizeof( defGI.dll_path ));
//	Q_strncpy( defGI.game_dll, "dlls/hl.dll", sizeof( defGI.game_dll ));
//	Q_strncpy( defGI.startmap, "newmap", sizeof( defGI.startmap ));
//	Q_strncpy( defGI.iconpath, "game.ico", sizeof( defGI.iconpath ));

//	VectorSet( defGI.client_mins[0],   0,   0,  0  );
//	VectorSet( defGI.client_maxs[0],   0,   0,  0  );
//	VectorSet( defGI.client_mins[1], -16, -16, -36 );
//	VectorSet( defGI.client_maxs[1],  16,  16,  36 );
//	VectorSet( defGI.client_mins[2], -32, -32, -32 );
//	VectorSet( defGI.client_maxs[2],  32,  32,  32 );
//	VectorSet( defGI.client_mins[3], -16, -16, -18 );
//	VectorSet( defGI.client_maxs[3],  16,  16,  18 );

//	// make simple gameinfo.txt
//	FS_WriteGameInfo( filename, &defGI );
}

bool Fs::FS_ParseLiblistGam( const char *filename, const char *gamedir, SGameInfo *GameInfo )
{
//	char	*afile, *pfile;
//	string	token;

//	if( !GameInfo ) return false;
//	afile = reinterpret_cast<char*>(FS_LoadFile( filename, NULL, false ));
//	if( !afile ) return false;

//	// setup default values
//	GameInfo->max_edicts = 900;	// default value if not specified
//	GameInfo->max_tents = 500;
//	GameInfo->max_beams = 128;
//	GameInfo->soundclip_dist = 1536;
//	GameInfo->max_particles = 4096;
//	GameInfo->version = 1.0f;
//	GameInfo->falldir[0] = '\0';

//	Q_strncpy( GameInfo->title, "New Game", sizeof( GameInfo->title ));
//	Q_strncpy( GameInfo->gamedir, gamedir, sizeof( GameInfo->gamedir ));
//	Q_strncpy( GameInfo->basedir, SI.ModuleName, sizeof( GameInfo->basedir ));
//	Q_strncpy( GameInfo->sp_entity, "info_player_start", sizeof( GameInfo->sp_entity ));
//	Q_strncpy( GameInfo->mp_entity, "info_player_deathmatch", sizeof( GameInfo->mp_entity ));
//	Q_strncpy( GameInfo->game_dll, "dlls/hl.dll", sizeof( GameInfo->game_dll ));
//	Q_strncpy( GameInfo->startmap, "newmap", sizeof( GameInfo->startmap ));
//	Q_strncpy( GameInfo->dll_path, "cl_dlls", sizeof( GameInfo->dll_path ));
//	Q_strncpy( GameInfo->iconpath, "game.ico", sizeof( GameInfo->iconpath ));

//	VectorSet( GameInfo->client_mins[0],   0,   0,  0  );
//	VectorSet( GameInfo->client_maxs[0],   0,   0,  0  );
//	VectorSet( GameInfo->client_mins[1], -16, -16, -36 );
//	VectorSet( GameInfo->client_maxs[1],  16,  16,  36 );
//	VectorSet( GameInfo->client_mins[2], -32, -32, -32 );
//	VectorSet( GameInfo->client_maxs[2],  32,  32,  32 );
//	VectorSet( GameInfo->client_mins[3], -16, -16, -18 );
//	VectorSet( GameInfo->client_maxs[3],  16,  16,  18 );

//	pfile = afile;

//	while(( pfile = COM_ParseFile( pfile, token )) != NULL )
//	{
//		if( !Q_stricmp( token, "game" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->title );
//		}
//		if( !Q_stricmp( token, "gamedir" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->gamedir );
//		}
//		if( !Q_stricmp( token, "fallback_dir" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->falldir );
//		}
//		else if( !Q_stricmp( token, "startmap" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->startmap );
//			FS_StripExtension( GameInfo->startmap ); // HQ2:Amen has extension .bsp
//		}
//		else if( !Q_stricmp( token, "trainmap" ) || !Q_stricmp( token, "trainingmap" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->trainmap );
//			FS_StripExtension( GameInfo->trainmap ); // HQ2:Amen has extension .bsp
//		}
//		else if( !Q_stricmp( token, "url_info" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->game_url );
//		}
//		else if( !Q_stricmp( token, "url_dl" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->update_url );
//		}
//		else if( !Q_stricmp( token, "gamedll" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->game_dll );
//			COM_FixSlashes( GameInfo->game_dll );
//		}
//		else if( !Q_stricmp( token, "icon" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->iconpath );
//			COM_FixSlashes( GameInfo->iconpath );
//			FS_DefaultExtension( GameInfo->iconpath, ".ico" );
//		}
//		else if( !Q_stricmp( token, "type" ))
//		{
//			pfile = COM_ParseFile( pfile, token );

//			if( !Q_stricmp( token, "singleplayer_only" ))
//			{
//				GameInfo->gamemode = 1;
//				Q_strncpy( GameInfo->type, "Single", sizeof( GameInfo->type ));
//			}
//			else if( !Q_stricmp( token, "multiplayer_only" ))
//			{
//				GameInfo->gamemode = 2;
//				Q_strncpy( GameInfo->type, "Multiplayer", sizeof( GameInfo->type ));
//			}
//			else
//			{
//				// pass type without changes
//				GameInfo->gamemode = 0;
//				Q_strncpy( GameInfo->type, token, sizeof( GameInfo->type ));
//			}
//		}
//		else if( !Q_stricmp( token, "version" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->version = Q_atof( token );
//		}
//		else if( !Q_stricmp( token, "size" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->size = Q_atoi( token );
//		}
//		else if( !Q_stricmp( token, "mpentity" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->mp_entity );
//		}
//		else if( !Q_stricmp( token, "secure" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->secure = Q_atoi( token );
//		}
//		else if( !Q_stricmp( token, "nomodels" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->nomodels = Q_atoi( token );
//		}
//	}

//	if( !FS_SysFolderExists( va( "%s\\%s", host.rootdir, GameInfo->gamedir )))
//		Q_strncpy( GameInfo->gamedir, gamedir, sizeof( GameInfo->gamedir ));

//	if( !FS_SysFolderExists( va( "%s\\%s", host.rootdir, GameInfo->falldir )))
//		GameInfo->falldir[0] = '\0';

//	Mem_Free( afile );

	return true;
}

/*
================
FS_ConvertGameInfo
================
*/
void Fs::FS_ConvertGameInfo( const char *gamedir, const char *gameinfo_path, const char *liblist_path )
{
	SGameInfo	GameInfo;

//	Q_memset( &GameInfo, 0, sizeof( GameInfo ));

//	if( FS_ParseLiblistGam( liblist_path, gamedir, &GameInfo ))
//	{
//		if( FS_WriteGameInfo( gameinfo_path, &GameInfo ))
//			MsgDev( D_INFO, "Convert %s to %s\n", liblist_path, gameinfo_path );
//	}
}

/*
================
FS_ParseGameInfo
================
*/
bool Fs::FS_ParseGameInfo( const char *gamedir, SGameInfo *GameInfo )
{
//	char	*afile, *pfile;
//	string	fs_path, filepath;
//	string	liblist, token;

//	Q_snprintf( filepath, sizeof( filepath ), "%s/gameinfo.txt", gamedir );
//	Q_snprintf( liblist, sizeof( liblist ), "%s/liblist.gam", gamedir );

//	// if user change liblist.gam update the gameinfo.txt
//	if( FS_FileTime( liblist, false ) > FS_FileTime( filepath, false ))
//		FS_ConvertGameInfo( gamedir, filepath, liblist );

//	// force to create gameinfo for specified game if missing
//	if( !Q_stricmp( gs_basedir, gamedir ) && !FS_FileExists( filepath, false ))
//		FS_CreateDefaultGameInfo( filepath );

//	if( !GameInfo ) return false;	// no dest

//	afile = reinterpret_cast<char*>(FS_LoadFile( filepath, NULL, false ));
//	if( !afile ) return false;

//	// setup default values
//	Q_strncpy( GameInfo->gamefolder, gamedir, sizeof( GameInfo->gamefolder ));
//	GameInfo->max_edicts = 900;	// default value if not specified
//	GameInfo->max_tents = 500;
//	GameInfo->max_beams = 128;
//	GameInfo->soundclip_dist = 1536;
//	GameInfo->max_particles = 4096;
//	GameInfo->version = 1.0f;
//	GameInfo->falldir[0] = '\0';

//	Q_strncpy( GameInfo->title, "New Game", sizeof( GameInfo->title ));
//	Q_strncpy( GameInfo->sp_entity, "info_player_start", sizeof( GameInfo->sp_entity ));
//	Q_strncpy( GameInfo->mp_entity, "info_player_deathmatch", sizeof( GameInfo->mp_entity ));
//	Q_strncpy( GameInfo->dll_path, "cl_dlls", sizeof( GameInfo->dll_path ));
//	Q_strncpy( GameInfo->game_dll, "dlls/hl.dll", sizeof( GameInfo->game_dll ));
//	Q_strncpy( GameInfo->startmap, "", sizeof( GameInfo->startmap ));
//	Q_strncpy( GameInfo->iconpath, "game.ico", sizeof( GameInfo->iconpath ));

//	VectorSet( GameInfo->client_mins[0],   0,   0,  0  );
//	VectorSet( GameInfo->client_maxs[0],   0,   0,  0  );
//	VectorSet( GameInfo->client_mins[1], -16, -16, -36 );
//	VectorSet( GameInfo->client_maxs[1],  16,  16,  36 );
//	VectorSet( GameInfo->client_mins[2], -32, -32, -32 );
//	VectorSet( GameInfo->client_maxs[2],  32,  32,  32 );
//	VectorSet( GameInfo->client_mins[3], -16, -16, -18 );
//	VectorSet( GameInfo->client_maxs[3],  16,  16,  18 );

//	pfile = afile;

//	while(( pfile = COM_ParseFile( pfile, token )) != NULL )
//	{
//		if( !Q_stricmp( token, "basedir" ))
//		{
//			pfile = COM_ParseFile( pfile, fs_path );
//			if( Q_stricmp( fs_path, GameInfo->basedir ) || Q_stricmp( fs_path, GameInfo->gamedir ))
//				Q_strncpy( GameInfo->basedir, fs_path, sizeof( GameInfo->basedir ));
//		}
//		else if( !Q_stricmp( token, "fallback_dir" ))
//		{
//			pfile = COM_ParseFile( pfile, fs_path );
//			if( Q_stricmp( fs_path, GameInfo->basedir ) || Q_stricmp( fs_path, GameInfo->falldir ))
//				Q_strncpy( GameInfo->falldir, fs_path, sizeof( GameInfo->falldir ));
//		}
//		else if( !Q_stricmp( token, "gamedir" ))
//		{
//			pfile = COM_ParseFile( pfile, fs_path );
//			if( Q_stricmp( fs_path, GameInfo->basedir ) || Q_stricmp( fs_path, GameInfo->gamedir ))
//				Q_strncpy( GameInfo->gamedir, fs_path, sizeof( GameInfo->gamedir ));
//		}
//		else if( !Q_stricmp( token, "title" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->title );
//		}
//		else if( !Q_stricmp( token, "sp_entity" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->sp_entity );
//		}
//		else if( !Q_stricmp( token, "mp_entity" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->mp_entity );
//		}
//		else if( !Q_stricmp( token, "gamedll" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->game_dll );
//		}
//		else if( !Q_stricmp( token, "dllpath" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->dll_path );
//		}
//		else if( !Q_stricmp( token, "startmap" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->startmap );
//			FS_StripExtension( GameInfo->startmap ); // HQ2:Amen has extension .bsp
//		}
//		else if( !Q_stricmp( token, "trainmap" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->trainmap );
//			FS_StripExtension( GameInfo->trainmap ); // HQ2:Amen has extension .bsp
//		}
//		else if( !Q_stricmp( token, "icon" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->iconpath );
//			FS_DefaultExtension( GameInfo->iconpath, ".ico" );
//		}
//		else if( !Q_stricmp( token, "url_info" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->game_url );
//		}
//		else if( !Q_stricmp( token, "url_update" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->update_url );
//		}
//		else if( !Q_stricmp( token, "date" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->date );
//		}
//		else if( !Q_stricmp( token, "type" ))
//		{
//			pfile = COM_ParseFile( pfile, GameInfo->type );
//		}
//		else if( !Q_stricmp( token, "version" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->version = Q_atof( token );
//		}
//		else if( !Q_stricmp( token, "size" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->size = Q_atoi( token );
//		}
//		else if( !Q_stricmp( token, "max_edicts" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->max_edicts = bound( 600, Q_atoi( token ), 4096 );
//		}
//		else if( !Q_stricmp( token, "max_tempents" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->max_tents = bound( 300, Q_atoi( token ), 2048 );
//		}
//		else if( !Q_stricmp( token, "max_beams" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->max_beams = bound( 64, Q_atoi( token ), 512 );
//		}
//		else if( !Q_stricmp( token, "max_particles" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->max_particles = bound( 1024, Q_atoi( token ), 131072 );
//		}
//		else if( !Q_stricmp( token, "gamemode" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			if( !Q_stricmp( token, "singleplayer_only" ))
//				GameInfo->gamemode = 1;
//			else if( !Q_stricmp( token, "multiplayer_only" ))
//				GameInfo->gamemode = 2;
//		}
//		else if( !Q_stricmp( token, "secure" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->secure = Q_atoi( token );
//		}
//		else if( !Q_stricmp( token, "nomodels" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->nomodels = Q_atoi( token );
//		}
//		else if( !Q_stricmp( token, "soundclip_dist" ))
//		{
//			pfile = COM_ParseFile( pfile, token );
//			GameInfo->soundclip_dist = Q_atoi( token );
//		}
//		else if( !Q_strnicmp( token, "hull", 4 ))
//		{
//			int	hullNum = Q_atoi( token + 4 );

//			if( hullNum < 0 || hullNum > 3 )
//			{
//				MsgDev( D_ERROR, "FS_ParseGameInfo: Invalid hull number %i. Ignored.\n", hullNum );
//			}
//			else
//			{
//				FS_ParseVector( &pfile, GameInfo->client_mins[hullNum], 3 );
//				FS_ParseVector( &pfile, GameInfo->client_maxs[hullNum], 3 );
//			}
//		}
//		else if( !Q_strnicmp( token, "ambient", 7 ))
//		{
//			int	ambientNum = Q_atoi( token + 7 );

//			if( ambientNum < 0 || ambientNum > ( NUM_AMBIENTS - 1 ))
//			{
//				MsgDev( D_ERROR, "FS_ParseGameInfo: Invalid ambient number %i. Ignored.\n", ambientNum );
//			}
//			else
//			{
//				pfile = COM_ParseFile( pfile, GameInfo->ambientsound[ambientNum] );
//			}
//		}
//	}

//	// make sure what gamedir is really exist
//	if( !FS_SysFolderExists( va( "%s\\%s", host.rootdir, GameInfo->gamedir )))
//		Q_strncpy( GameInfo->gamedir, gamedir, sizeof( GameInfo->gamedir ));

//	if( !FS_SysFolderExists( va( "%s\\%s", host.rootdir, GameInfo->falldir )))
//		GameInfo->falldir[0] = '\0';

//	Mem_Free( afile );

	return true;
}

/*
================
FS_LoadGameInfo

can be passed null arg
================
*/
void Fs::FS_LoadGameInfo( const char *rootfolder )
{
	int	i;

	// lock uplevel of gamedir for read\write
//	fs_ext_path = false;

//	if( rootfolder ) Q_strcpy( gs_basedir, rootfolder );
//	MsgDev( D_NOTE, "FS_LoadGameInfo( %s )\n", gs_basedir );

//	// clear any old pathes
//	FS_ClearSearchPath();

//	// validate gamedir
//	for( i = 0; i < SI.numgames; i++ )
//	{
//		if( !Q_stricmp( SI.games[i]->gamefolder, gs_basedir ))
//			break;
//	}

//	if( i == SI.numgames )
//		Sys_Error( "Couldn't find game directory '%s'\n", gs_basedir );

//	SI.GameInfo = SI.games[i];
//	FS_Rescan(); // create new filesystem

//	Host_InitDecals ();	// reload decals
}

/*
================
FS_Init
================
*/
void Fs::FS_Init()
{
//	QStringList	dirs;
//	bool		hasDefaultDir = false;
//	int		i;

//	FS_InitMemory();

//	Cmd_AddCommand( "fs_rescan", FS_Rescan_f, "rescan filesystem search pathes" );
//	Cmd_AddCommand( "fs_path", FS_Path_f, "show filesystem search pathes" );
//	Cmd_AddCommand( "fs_clearpaths", FS_ClearPaths_f, "clear filesystem search pathes" );

//	// ignore commandlineoption "-game" for other stuff
//	if( host.type == HOST_NORMAL || host.type == HOST_DEDICATED )
//	{
//		stringlistinit( &dirs );
//		listdirectory( &dirs, "./" );
//		stringlistsort( &dirs );
//		SI.numgames = 0;

//		if( !Sys_GetParmFromCmdLine( "-game", gs_basedir ))
//			Q_strcpy( gs_basedir, SI.ModuleName ); // default dir

//		if( FS_CheckNastyPath( gs_basedir, true ))
//		{
//			MsgDev( D_ERROR, "FS_Init: invalid game directory \"%s\"\n", gs_basedir );
//			Q_strcpy( gs_basedir, SI.ModuleName ); // default dir
//		}

//		// validate directories
//		for( i = 0; i < dirs.numstrings; i++ )
//		{
//			if( !Q_stricmp( SI.ModuleName, dirs.strings[i] ))
//				hasDefaultDir = true;

//			if( !Q_stricmp( gs_basedir, dirs.strings[i] ))
//				break;
//		}

//		if( i == dirs.numstrings )
//		{
//			MsgDev( D_INFO, "FS_Init: game directory \"%s\" not exist\n", gs_basedir );
//			if( hasDefaultDir ) Q_strncpy( gs_basedir, SI.ModuleName, sizeof( gs_basedir )); // default dir
//		}

//		// build list of game directories here
//		FS_AddGameDirectory( "./", 0 );

//		for( i = 0; i < dirs.numstrings; i++ )
//		{
//			if( !FS_SysFolderExists( dirs.strings[i] ) || (!Q_stricmp( dirs.strings[i], ".." ) && !fs_ext_path ))
//				continue;

//			if( !SI.games[SI.numgames] )
//				SI.games[SI.numgames] = (gameinfo_t *)Mem_Alloc( fs_mempool, sizeof( gameinfo_t ));

//			if( FS_ParseGameInfo( dirs.strings[i], SI.games[SI.numgames] ))
//				SI.numgames++; // added
//		}

//		stringlistfreecontents( &dirs );
//	}

//	MsgDev( D_NOTE, "FS_Init: done\n" );
}

void Fs::FS_AllowDirectPaths( bool enable )
{
//	fs_ext_path = enable;
}

/*
================
FS_Shutdown
================
*/
void Fs::FS_Shutdown()
{
//	int	i;

//	// release gamedirs
//	for( i = 0; i < SI.numgames; i++ )
//		if( SI.games[i] ) Mem_Free( SI.games[i] );

//	Q_memset( &SI, 0, sizeof( sysinfo_t ));

//	FS_ClearSearchPath(); // release all wad files too
//	Mem_FreePool( &fs_mempool );
}

/*
====================
FS_SysFileTime

Internal function used to determine filetime
====================
*/
long Fs::FS_SysFileTime( const char *filename )
{
	struct stat buf;

	if( stat( filename, &buf ) == -1 )
		return -1;

	return (long)buf.st_mtime;
}

/*
====================
FS_SysOpen

Internal function used to create a SFile and open the relevant non-packed file on disk
====================
*/
SFile* Fs::FS_SysOpen( const char* filepath, const char* mode )
{
	SFile	*file;
//	int	mod, opt;
//	uint	ind;

//	// Parse the mode string
//	switch( mode[0] )
//	{
//	case 'r':
//		mod = O_RDONLY;
//		opt = 0;
//		break;
//	case 'w':
//		mod = O_WRONLY;
//		opt = O_CREAT | O_TRUNC;
//		break;
//	case 'a':
//		mod = O_WRONLY;
//		opt = O_CREAT | O_APPEND;
//		break;
//	default:
//		MsgDev( D_ERROR, "FS_SysOpen(%s, %s): invalid mode\n", filepath, mode );
//		return NULL;
//	}

//	for( ind = 1; mode[ind] != '\0'; ind++ )
//	{
//		switch( mode[ind] )
//		{
//		case '+':
//			mod = O_RDWR;
//			break;
//		case 'b':
//			opt |= O_BINARY;
//			break;
//		default:
//			MsgDev( D_ERROR, "FS_SysOpen: %s: unknown char in mode (%c)\n", filepath, mode, mode[ind] );
//			break;
//		}
//	}

//	file = (SFile *)Mem_Alloc( fs_mempool, sizeof( *file ));
//	file->filetime = FS_SysFileTime( filepath );
//	file->ungetc = EOF;

//	file->handle = open( filepath, mod|opt, 0666 );
//	if( file->handle < 0 )
//	{
//		Mem_Free( file );
//		return NULL;
//	}

//	file->real_length = lseek( file->handle, 0, SEEK_END );

//	// For files opened in append mode, we start at the end of the file
//	if( mod & O_APPEND ) file->position = file->real_length;
//	else lseek( file->handle, 0, SEEK_SET );

	return file;
}


/*
===========
FS_OpenPackedFile

Open a packed file using its package file descriptor
===========
*/
SFile *Fs::FS_OpenPackedFile( SPack *pack, int pack_ind )
{
	SPackFile	*pfile;
	int		dup_handle;
	SFile		*file;

	pfile = &pack->files[pack_ind];

//	if( lseek( pack->handle, pfile->offset, SEEK_SET ) == -1 )
//		return NULL;

//	dup_handle = dup( pack->handle );

//	if( dup_handle < 0 )
//		return NULL;

//	file = (SFile *)Mem_Alloc( fs_mempool, sizeof( *file ));
//	Q_memset( file, 0, sizeof( *file ));
//	file->handle = dup_handle;
//	file->real_length = pfile->realsize;
//	file->offset = pfile->offset;
//	file->position = 0;
//	file->ungetc = EOF;

	return file;
}

/*
==================
FS_SysFileExists

Look for a file in the filesystem only
==================
*/
bool Fs::FS_SysFileExists( const char *path )
{
	int desc;

	desc = open( path, O_RDONLY|O_BINARY );

	if( desc < 0 ) return false;
	close( desc );
	return true;
}

/*
==================
FS_SysFolderExists

Look for a existing folder
==================
*/
bool Fs::FS_SysFolderExists( const char *path )
{
	return false;
//	quint32	dwFlags = GetFileAttributes( path );

//	return ( dwFlags != -1 ) && ( dwFlags & FILE_ATTRIBUTE_DIRECTORY );
}

/*
====================
FS_FindFile

Look for a file in the packages and in the filesystem

Return the searchpath where the file was found (or NULL)
and the file index in the package if relevant
====================
*/
SSearchPath *Fs::FS_FindFile( const char *name, int* index, bool gamedironly )
{
	SSearchPath	*search;
	char		*pEnvPath;
	SPack		*pak;

	// search through the path, one element at a time
//	for( search = fs_searchpaths; search; search = search->next )
//	{
//		if( gamedironly & !( search->flags & FS_GAMEDIR_PATH ))
//			continue;

//		// is the element a pak file?
//		if( search->pack )
//		{
//			int left, right, middle;

//			pak = search->pack;

//			// look for the file (binary search)
//			left = 0;
//			right = pak->numfiles - 1;
//			while( left <= right )
//			{
//				int diff;

//				middle = (left + right) / 2;
//				diff = Q_stricmp( pak->files[middle].name, name );

//				// Found it
//				if( !diff )
//				{
//					if( index ) *index = middle;
//					return search;
//				}

//				// if we're too far in the list
//				if( diff > 0 )
//					right = middle - 1;
//				else left = middle + 1;
//			}
//		}
//		else if( search->wad )
//		{
//			SDlumpInfo	*lump;
//			char		type = W_TypeFromExt( name );
//			bool		anywadname = true;
//			string		wadname, wadfolder;
//			string		shortname;

//			// quick reject by filetype
//			if( type == TYP_NONE ) continue;
//			FS_ExtractFilePath( name, wadname );
//			wadfolder[0] = '\0';

//			if( Q_strlen( wadname ))
//			{
//				FS_FileBase( wadname, wadname );
//				Q_strncpy( wadfolder, wadname, sizeof( wadfolder ));
//				FS_DefaultExtension( wadname, ".wad" );
//				anywadname = false;
//			}

//			// make wadname from wad fullpath
//			FS_FileBase( search->wad->filename, shortname );
//			FS_DefaultExtension( shortname, ".wad" );

//			// quick reject by wadname
//			if( !anywadname && Q_stricmp( wadname, shortname ))
//				continue;

//			// NOTE: we can't using long names for wad,
//			// because we using original wad names[16];
//			FS_FileBase( name, shortname );

//			lump = W_FindLump( search->wad, shortname, type );
//			if( lump )
//			{
//				if( index )
//					*index = lump - search->wad->lumps;
//				return search;
//			}
//		}
//		else
//		{
//			char	netpath[MAX_SYSPATH];
//			Q_sprintf( netpath, "%s%s", search->filename, name );
//			if( FS_SysFileExists( netpath ))
//			{
//				if( index != NULL ) *index = -1;
//				return search;
//			}
//		}
//	}

//	if( fs_ext_path && ( pEnvPath = getenv( "Path" )))
//	{
//		char	netpath[MAX_SYSPATH];

//		// clear searchpath
//		search = &fs_directpath;
//		Q_memset( search, 0, sizeof( SSearchPath ));

//		// root folder has a more priority than netpath
//		Q_strncpy( search->filename, host.rootdir, sizeof( search->filename ));
//		Q_strcat( search->filename, "\\" );
//		Q_snprintf( netpath, MAX_SYSPATH, "%s%s", search->filename, name );

//		if( FS_SysFileExists( netpath ))
//		{
//			if( index != NULL )
//				*index = -1;
//			return search;
//		}

//		// search for environment path
//		while( pEnvPath )
//		{
//			char *end = Q_strchr( pEnvPath, ';' );
//			if( !end ) break;
//			Q_strncpy( search->filename, pEnvPath, (end - pEnvPath) + 1 );
//			Q_strcat( search->filename, "\\" );
//			Q_snprintf( netpath, MAX_SYSPATH, "%s%s", search->filename, name );

//			if( FS_SysFileExists( netpath ))
//			{
//				if( index != NULL )
//					*index = -1;
//				return search;
//			}
//			pEnvPath += (end - pEnvPath) + 1; // move pointer
//		}
//	}

//	if( index != NULL )
//		*index = -1;

	return NULL;
}


/*
===========
FS_OpenReadFile

Look for a file in the search paths and open it in read-only mode
===========
*/
SFile *Fs::FS_OpenReadFile( const char *filename, const char *mode, bool gamedironly )
{
	SSearchPath	*search;
	int		pack_ind;

	search = FS_FindFile( filename, &pack_ind, gamedironly );

	// not found?
	if( search == NULL )
		return NULL;

	if( search->pack )
		return FS_OpenPackedFile( search->pack, pack_ind );
	else if( search->wad )
		return NULL; // let W_LoadFile get lump correctly
	else if( pack_ind < 0 )
	{
		// found in the filesystem?
		char	path [MAX_SYSPATH];
//		Q_sprintf( path, "%s%s", search->filename, filename );
		return FS_SysOpen( path, mode );
	}
	return NULL;
}

/*
=============================================================================

MAIN PUBLIC FUNCTIONS

=============================================================================
*/
/*
====================
FS_Open

Open a file. The syntax is the same as fopen
====================
*/
SFile *Fs::FS_Open( const char *filepath, const char *mode, bool gamedironly )
{
//	if( host.type == HOST_NORMAL || host.type == HOST_DEDICATED )
//		  {
//		// some stupid mappers used leading '/' or '\' in path to models or sounds
//		if( filepath[0] == '/' || filepath[0] == '\\' ) filepath++;
//		if( filepath[0] == '/' || filepath[0] == '\\' ) filepath++;
//		  }

//	if( FS_CheckNastyPath( filepath, false ))
//		return NULL;

//	// if the file is opened in "write", "append", or "read/write" mode
//	if( mode[0] == 'w' || mode[0] == 'a' || Q_strchr( mode, '+' ))
//	{
//		char	real_path[MAX_SYSPATH];

//		// open the file on disk directly
//		Q_sprintf( real_path, "%s/%s", fs_gamedir, filepath );
//		FS_CreatePath( real_path );// Create directories up to the file
//		return FS_SysOpen( real_path, mode );
//	}

	// else, we look at the various search paths and open the file in read-only mode
	return FS_OpenReadFile( filepath, mode, gamedironly );
}

/*
====================
FS_Close

Close a file
====================
*/
int Fs::FS_Close( SFile *file )
{
	if( close( file->handle ))
		return EOF;

//	Mem_Free( file );
	return 0;
}

/*
====================
FS_Write

Write "datasize" quint8s into a file
====================
*/
int Fs::FS_Write( SFile *file, const void *data, size_t datasize )
{
	int	result;

	if( !file ) return 0;

	// if necessary, seek to the exact file position we're supposed to be
	if( file->buff_ind != file->buff_len )
		lseek( file->handle, file->buff_ind - file->buff_len, SEEK_CUR );

	// purge cached data
//	FS_Purge( file );

	// write the buffer and update the position
	result = write( file->handle, data, (int)datasize );
	file->position = lseek( file->handle, 0, SEEK_CUR );
	if( file->real_length < file->position )
		file->real_length = file->position;

	if( result < 0 )
		return 0;
	return result;
}

/*
====================
FS_Read

Read up to "buffersize" quint8s from a file
====================
*/
int Fs::FS_Read( SFile *file, void *buffer, size_t buffersize )
{
	int	count, done;
	int	nb;

	// nothing to copy
	if( buffersize == 0 ) return 1;

	// Get rid of the ungetc character
	if( file->ungetc != EOF )
	{
		((char*)buffer)[0] = file->ungetc;
		buffersize--;
		file->ungetc = EOF;
		done = 1;
	}
	else done = 0;

	// first, we copy as many quint8s as we can from "buff"
	if( file->buff_ind < file->buff_len )
	{
		count = file->buff_len - file->buff_ind;

		done += ((int)buffersize > count ) ? count : (int)buffersize;
//		Q_memcpy( buffer, &file->buff[file->buff_ind], done );
		file->buff_ind += done;

		buffersize -= done;
		if( buffersize == 0 )
			return done;
	}

	// NOTE: at this point, the read buffer is always empty

	// we must take care to not read after the end of the file
	count = file->real_length - file->position;

	// if we have a lot of data to get, put them directly into "buffer"
	if( buffersize > sizeof( file->buff ) / 2 )
	{
		if( count > (int)buffersize )
			count = (int)buffersize;
		lseek( file->handle, file->offset + file->position, SEEK_SET );
		nb = read (file->handle, &((quint8 *)buffer)[done], count );

		if( nb > 0 )
		{
			done += nb;
			file->position += nb;
			// Purge cached data
//			FS_Purge( file );
		}
	}
	else
	{
		if( count > (int)sizeof( file->buff ))
			count = (int)sizeof( file->buff );
		lseek( file->handle, file->offset + file->position, SEEK_SET );
		nb = read( file->handle, file->buff, count );

		if( nb > 0 )
		{
			file->buff_len = nb;
			file->position += nb;

			// copy the requested data in "buffer" (as much as we can)
			count = (int)buffersize > file->buff_len ? file->buff_len : (int)buffersize;
//			Q_memcpy( &((quint8 *)buffer)[done], file->buff, count );
			file->buff_ind = count;
			done += count;
		}
	}

	return done;
}

/*
====================
FS_Print

Print a string into a file
====================
*/
int Fs::FS_Print( SFile *file, const char *msg )
{
	return 0;//FS_Write( file, msg, Q_strlen( msg ));
}

/*
====================
FS_Printf

Print a string into a file
====================
*/
int Fs::FS_Printf( SFile *file, const char* format, ... )
{
	int	result;
	va_list	args;

	va_start( args, format );
//	result = FS_VPrintf( file, format, args );
	va_end( args );

	return result;
}

/*
====================
FS_VPrintf

Print a string into a file
====================
*/
int Fs::FS_VPrintf( SFile *file, const char* format, va_list ap )
{
	int		len;
//	int	buff_size = MAX_SYSPATH;
//	char		*tempbuff;

//	if( !file ) return 0;

//	while( 1 )
//	{
//		tempbuff = (char *)Mem_Alloc( fs_mempool, buff_size );
//		len = Q_vsprintf( tempbuff, format, ap );
//		if( len >= 0 && len < buff_size ) break;
//		Mem_Free( tempbuff );
//		buff_size *= 2;
//	}

//	len = write( file->handle, tempbuff, len );
//	Mem_Free( tempbuff );

	return len;
}

/*
====================
FS_Getc

Get the next character of a file
====================
*/
int Fs::FS_Getc( SFile *file )
{
	char	c;

	if( FS_Read( file, &c, 1) != 1 )
		return EOF;

	return c;
}

/*
====================
FS_UnGetc

Put a character back into the read buffer (only supports one character!)
====================
*/
int Fs::FS_UnGetc( SFile *file, quint8 c )
{
	// If there's already a character waiting to be read
	if( file->ungetc != EOF )
		return EOF;

	file->ungetc = c;
	return c;
}

/*
====================
FS_Gets

Same as fgets
====================
*/
int Fs::FS_Gets( SFile *file, quint8 *string, size_t bufsize )
{
	int	c, end = 0;

	while( 1 )
	{
		c = FS_Getc( file );
		if( c == '\r' || c == '\n' || c < 0 )
			break;
		if( end < bufsize - 1 )
			string[end++] = c;
	}
	string[end] = 0;

	// remove \n following \r
	if( c == '\r' )
	{
		c = FS_Getc( file );
		if( c != '\n' ) FS_UnGetc( file, (quint8)c );
	}

	return c;
}

/*
====================
FS_Seek

Move the position index in a file
====================
*/
int Fs::FS_Seek( SFile *file, int offset, int whence )
{
//	// compute the file offset
//	switch( whence )
//	{
//	case SEEK_CUR:
//		offset += file->position - file->buff_len + file->buff_ind;
//		break;
//	case SEEK_SET:
//		break;
//	case SEEK_END:
//		offset += file->real_length;
//		break;
//	default:
//		return -1;
//	}

//	if( offset < 0 || offset > (long)file->real_length )
//		return -1;

//	// if we have the data in our read buffer, we don't need to actually seek
//	if( file->position - file->buff_len <= offset && offset <= file->position )
//	{
//		file->buff_ind = offset + file->buff_len - file->position;
//		return 0;
//	}

//	// Purge cached data
//	FS_Purge( file );

//	if( lseek( file->handle, file->offset + offset, SEEK_SET ) == -1 )
//		return -1;
//	file->position = offset;

	return 0;
}

/*
====================
FS_Tell

Give the current position in a file
====================
*/
int Fs::FS_Tell( SFile* file )
{
	if( !file ) return 0;
	return file->position - file->buff_len + file->buff_ind;
}

/*
====================
FS_Eof

indicates at reached end of file
====================
*/
bool Fs::FS_Eof( SFile* file )
{
	if( !file ) return true;
	return (file->position == file->real_length) ? true : false;
}

/*
====================
FS_Purge

Erases any buffered input or output data
====================
*/
void Fs::FS_Purge( SFile* file )
{
	file->buff_len = 0;
	file->buff_ind = 0;
	file->ungetc = EOF;
}

/*
============
FS_LoadFile

Filename are relative to the xash directory.
Always appends a 0 quint8.
============
*/
quint8 *Fs::FS_LoadFile( const char *path, int *filesizeptr, bool gamedironly )
{
	SFile		*file;
	quint8		*buf = NULL;
	int	filesize = 0;

	file = FS_Open( path, "rb", gamedironly );

	if( file )
	{
		filesize = file->real_length;
//		buf = (quint8 *)Mem_Alloc( fs_mempool, filesize + 1 );
		buf[filesize] = '\0';
		FS_Read( file, buf, filesize );
		FS_Close( file );
	}
	else
	{
//		buf = W_LoadFile( path, &filesize, gamedironly );
	}

	if( filesizeptr )
		*filesizeptr = filesize;

	return buf;
}

/*
============
FS_OpenFile

Simply version of FS_Open
============
*/
SFile *Fs::FS_OpenFile( const char *path, int *filesizeptr, bool gamedironly )
{
	SFile	*file = FS_Open( path, "rb", gamedironly );

	if( filesizeptr )
	{
		if( file ) *filesizeptr = file->real_length;
		else *filesizeptr = 0;
	}
	return file;
}

/*
============
FS_WriteFile

The filename will be prefixed by the current game directory
============
*/
bool Fs::FS_WriteFile( const char *filename, const void *data, int len )
{
	SFile *file;

	file = FS_Open( filename, "wb", false );

	if( !file )
	{
//		MsgDev( D_ERROR, "FS_WriteFile: failed on %s\n", filename);
		return false;
	}

	FS_Write (file, data, len);
	FS_Close (file);
	return true;
}

/*
=============================================================================

OTHERS PUBLIC FUNCTIONS

=============================================================================
*/
/*
============
FS_StripExtension
============
*/
void Fs::FS_StripExtension( char *path )
{
	size_t	length;

//	length = Q_strlen( path ) - 1;
	while( length > 0 && path[length] != '.' )
	{
		length--;
		if( path[length] == '/' || path[length] == '\\' || path[length] == ':' )
			return; // no extension
	}
	if( length ) path[length] = 0;
}

/*
==================
FS_DefaultExtension
==================
*/
void Fs::FS_DefaultExtension( char *path, const char *extension )
{
	const char *src;

	// if path doesn't have a .EXT, append extension
	// (extension should include the .)
//	src = path + Q_strlen( path ) - 1;

	while( *src != '/' && src != path )
	{
		// it has an extension
		if( *src == '.' ) return;
		src--;
	}
//	Q_strcat( path, extension );
}

/*
==================
FS_FileExists

Look for a file in the packages and in the filesystem
==================
*/
bool Fs::FS_FileExists( const char *filename, bool gamedironly )
{
	if( FS_FindFile( filename, NULL, gamedironly ))
		return true;
	return false;
}

/*
==================
FS_GetDiskPath

Build direct path for file in the filesystem
return NULL for file in pack
==================
*/
const char *Fs::FS_GetDiskPath( const char *name, bool gamedironly )
{
	int		index;
	SSearchPath	*search;

	search = FS_FindFile( name, &index, gamedironly );

	if( search )
	{
		if( index != -1 )
		{
			// file in pack or wad
			return NULL;
		}
//		return va( "%s%s", search->filename, name );
	}
	return NULL;
}

/*
==================
FS_CheckForCrypt

return true if library is crypted
==================
*/
bool Fs::FS_CheckForCrypt( const char *dllname )
{
	SFile	*f;
	int	key;

	f = FS_Open( dllname, "rb", false );
	if( !f ) return false;

	FS_Seek( f, 64, SEEK_SET );	// skip first 64 quint8s
	FS_Read( f, &key, sizeof( key ));
	FS_Close( f );

	return ( key == 0x12345678 ) ? true : false;
}

/*
==================
FS_FindLibrary

search for library, assume index is valid
only for internal use
==================
*/
SDllUser *Fs::FS_FindLibrary( const char *dllname, bool directpath )
{
	QString		dllpath;
	SSearchPath	*search;
	SDllUser	*hInst;
	int		i, index;
	int		start = 0;

//	// check for bad exports
//	if( !dllname || !*dllname )
//		return NULL;

//	fs_ext_path = directpath;

//	// HACKHACK: remove absoulte path to valve folder
//	if( !Q_strnicmp( dllname, "..\\valve\\", 9 ) || !Q_strnicmp( dllname, "../valve/", 9 ))
//		start += 9;

//	// replace all backward slashes
//	for( i = 0; i < Q_strlen( dllname ); i++ )
//	{
//		if( dllname[i+start] == '\\' ) dllpath[i] = '/';
//		else dllpath[i] = Q_tolower( dllname[i+start] );
//	}
//	dllpath[i] = '\0';

//	FS_DefaultExtension( dllpath, ".dll" );	// apply ext if forget
//	search = FS_FindFile( dllpath, &index, false );

//	if( !search )
//	{
//		fs_ext_path = false;
//		if( directpath ) return NULL;	// direct paths fails here

//		// trying check also 'bin' folder for indirect paths
//		Q_strncpy( dllpath, dllname, sizeof( dllpath ));
//		search = FS_FindFile( dllpath, &index, false );
//		if( !search ) return NULL;	// unable to find
//	}

//	// all done, create dll_user_t struct
//	hInst = reinterpret_cast<dll_user_t*>(Mem_Alloc( host.mempool, sizeof( dll_user_t )));

//	// save dllname for debug purposes
//	Q_strncpy( hInst->dllName, dllname, sizeof( hInst->dllName ));

//	// shortPath is used for LibraryLoadSymbols only
//	Q_strncpy( hInst->shortPath, dllpath, sizeof( hInst->shortPath ));

//	hInst->encrypted = FS_CheckForCrypt( dllpath );

//	if( index < 0 && !hInst->encrypted )
//	{
//		Q_snprintf( hInst->fullPath, sizeof( hInst->fullPath ), "%s%s", search->filename, dllpath );
//		hInst->custom_loader = false;	// we can loading from disk and use normal debugging
//	}
//	else
//	{
//		Q_strncpy( hInst->fullPath, dllpath, sizeof( hInst->fullPath ));
//		hInst->custom_loader = true;	// loading from pack or wad - for release, debug don't working
//	}
//	fs_ext_path = false; // always reset direct paths

	return hInst;
}

/*
==================
FS_FileSize

return size of file in quint8s
==================
*/
int Fs::FS_FileSize( const char *filename, bool gamedironly )
{
	SFile	*fp;
	int	length = 0;

	fp = FS_Open( filename, "rb", gamedironly );

	if( fp )
	{
		// it exists
		FS_Seek( fp, 0, SEEK_END );
		length = FS_Tell( fp );
		FS_Close( fp );
	}
	return length;
}

/*
==================
FS_FileLength

return size of file in quint8s
==================
*/
int Fs::FS_FileLength( SFile *f )
{
	if( !f ) return 0;
	return f->real_length;
}

/*
==================
FS_FileTime

return time of creation file in seconds
==================
*/
int Fs::FS_FileTime( const char *filename, bool gamedironly )
{
	SSearchPath	*search;
	int		pack_ind;

	search = FS_FindFile( filename, &pack_ind, gamedironly );
	if( !search ) return -1; // doesn't exist

	if( search->pack ) // grab pack filetime
		return (int)search->pack->filetime;
	else if( search->wad ) // grab wad filetime
		return (int)search->wad->filetime;
	else if( pack_ind < 0 )
	{
		// found in the filesystem?
		char	path [MAX_SYSPATH];

//		Q_sprintf( path, "%s%s", search->filename, filename );
		return FS_SysFileTime( path );
	}
	return -1; // doesn't exist
}

/*
==================
FS_Rename

rename specified file from gamefolder
==================
*/
bool Fs::FS_Rename( const char *oldname, const char *newname )
{
	char	oldpath[MAX_SYSPATH], newpath[MAX_SYSPATH];
	bool	iRet;

	if( !oldname || !newname || !*oldname || !*newname )
		return false;

//	Q_snprintf( oldpath, sizeof( oldpath ), "%s%s", fs_gamedir, oldname );
//	Q_snprintf( newpath, sizeof( newpath ), "%s%s", fs_gamedir, newname );

//	COM_FixSlashes( oldpath );
//	COM_FixSlashes( newpath );

	iRet = rename( oldpath, newpath );

	return (iRet == 0);
}

/*
==================
FS_Delete

delete specified file from gamefolder
==================
*/
bool Fs::FS_Delete( const char *path )
{
	char	real_path[MAX_SYSPATH];
	bool	iRet;

	if( !path || !*path )
		return false;

//	Q_snprintf( real_path, sizeof( real_path ), "%s%s", fs_gamedir, path );
//	COM_FixSlashes( real_path );
	iRet = remove( real_path );

	return (iRet == 0);
}

/*
==================
FS_FileCopy

==================
*/
void Fs::FS_FileCopy( SFile *pOutput, SFile *pInput, int fileSize )
{
	char	buf[MAX_SYSPATH];	// A small buffer for the copy
	int	size;

	while( fileSize > 0 )
	{
		if( fileSize > MAX_SYSPATH )
			size = MAX_SYSPATH;
		else size = fileSize;

		FS_Read( pInput, buf, size );
		FS_Write( pOutput, buf, size );

		fileSize -= size;
	}
}

/*
===========
FS_Search

Allocate and fill a search structure with information on matching filenames.
===========
*/
SSearch *Fs::FS_Search( const char *pattern, int caseinsensitive, int gamedironly )
{
	SSearch		*search = NULL;
	SSearchPath	*searchpath;
	SPack		*pak;
	SWfile		*wad;
	int		i, basepathlength, numfiles, numchars;
	int		resultlistindex, dirlistindex;
	const char	*slash, *backslash, *colon, *separator;
	QString		netpath, temp;
	QStringList	resultlist;
	QStringList	dirlist;
	char		*basepath;

	for( i = 0; pattern[i] == '.' || pattern[i] == ':' || pattern[i] == '/' || pattern[i] == '\\'; i++ );

//	if( i > 0 )
//	{
//		MsgDev( D_INFO, "FS_Search: don't use punctuation at the beginning of a search pattern!\n");
//		return NULL;
//	}

//	stringlistinit( &resultlist );
//	stringlistinit( &dirlist );
//	slash = Q_strrchr( pattern, '/' );
//	backslash = Q_strrchr( pattern, '\\' );
//	colon = Q_strrchr( pattern, ':' );
//	separator = max( slash, backslash );
//	separator = max( separator, colon );
//	basepathlength = separator ? (separator + 1 - pattern) : 0;
//	basepath = reinterpret_cast<char*>(Mem_Alloc( fs_mempool, basepathlength + 1 ));
//	if( basepathlength ) Q_memcpy( basepath, pattern, basepathlength );
//	basepath[basepathlength] = 0;

//	// search through the path, one element at a time
//	for( searchpath = fs_searchpaths; searchpath; searchpath = searchpath->next )
//	{
//		if( gamedironly && !( searchpath->flags & FS_GAMEDIR_PATH ))
//			continue;

//		// is the element a pak file?
//		if( searchpath->pack )
//		{
//			// look through all the pak file elements
//			pak = searchpath->pack;
//			for( i = 0; i < pak->numfiles; i++ )
//			{
//				Q_strncpy( temp, pak->files[i].name, sizeof( temp ));
//				while( temp[0] )
//				{
//					if( matchpattern( temp, (char *)pattern, true ))
//					{
//						for( resultlistindex = 0; resultlistindex < resultlist.numstrings; resultlistindex++ )
//						{
//							if( !Q_strcmp( resultlist.strings[resultlistindex], temp ))
//								break;
//						}

//						if( resultlistindex == resultlist.numstrings )
//						{
//							stringlistappend( &resultlist, temp );
//						}
//					}
//					// strip off one path element at a time until empty
//					// this way directories are added to the listing if they match the pattern
//					slash = Q_strrchr( temp, '/' );
//					backslash = Q_strrchr( temp, '\\' );
//					colon = Q_strrchr( temp, ':' );
//					separator = temp;
//					if( separator < slash )
//						separator = slash;
//					if( separator < backslash )
//						separator = backslash;
//					if( separator < colon )
//						separator = colon;
//					*((char *)separator) = 0;
//				}
//			}
//		}
//		else if( searchpath->wad )
//		{
//			string	wadpattern, wadname, temp2;
//			char	type = W_TypeFromExt( pattern );
//			bool	anywadname = true;
//			string	wadfolder;

//			// quick reject by filetype
//			if( type == TYP_NONE ) continue;
//			FS_ExtractFilePath( pattern, wadname );
//			FS_FileBase( pattern, wadpattern );
//			wadfolder[0] = '\0';

//			if( Q_strlen( wadname ))
//			{
//				FS_FileBase( wadname, wadname );
//				Q_strncpy( wadfolder, wadname, sizeof( wadfolder ));
//				FS_DefaultExtension( wadname, ".wad" );
//				anywadname = false;
//			}

//			// make wadname from wad fullpath
//			FS_FileBase( searchpath->wad->filename, temp2 );
//			FS_DefaultExtension( temp2, ".wad" );

//			// quick reject by wadname
//			if( !anywadname && Q_stricmp( wadname, temp2 ))
//				continue;

//			// look through all the wad file elements
//			wad = searchpath->wad;
//			for( i = 0; i < wad->numlumps; i++ )
//			{
//				// if type not matching, we already have no chance ...
//				if( type != TYP_ANY && wad->lumps[i].type != type )
//					continue;

//				Q_strncpy( temp, wad->lumps[i].name, sizeof( temp ));
//				while( temp[0] )
//				{
//					if( matchpattern( temp, wadpattern, true ))
//					{
//						for( resultlistindex = 0; resultlistindex < resultlist.numstrings; resultlistindex++ )
//						{
//							if( !Q_strcmp( resultlist.strings[resultlistindex], temp ))
//								break;
//						}

//						if( resultlistindex == resultlist.numstrings )
//						{
//							// build path: wadname/lumpname.ext
//							Q_snprintf( temp2, sizeof(temp2), "%s/%s", wadfolder, temp );
//							FS_DefaultExtension( temp2, va(".%s", W_ExtFromType( wad->lumps[i].type )));
//							stringlistappend( &resultlist, temp2 );
//						}
//					}

//					// strip off one path element at a time until empty
//					// this way directories are added to the listing if they match the pattern
//					slash = Q_strrchr( temp, '/' );
//					backslash = Q_strrchr( temp, '\\' );
//					colon = Q_strrchr( temp, ':' );
//					separator = temp;
//					if( separator < slash )
//						separator = slash;
//					if( separator < backslash )
//						separator = backslash;
//					if( separator < colon )
//						separator = colon;
//					*((char *)separator) = 0;
//				}
//			}
//		}
//		else
//		{
//			// get a directory listing and look at each name
//			Q_sprintf( netpath, "%s%s", searchpath->filename, basepath );
//			stringlistinit( &dirlist );
//			listdirectory( &dirlist, netpath );
//			for( dirlistindex = 0; dirlistindex < dirlist.numstrings; dirlistindex++ )
//			{
//				Q_sprintf( temp, "%s%s", basepath, dirlist.strings[dirlistindex] );
//				if( matchpattern( temp, (char *)pattern, true ))
//				{
//					for( resultlistindex = 0; resultlistindex < resultlist.numstrings; resultlistindex++ )
//					{
//						if( !Q_strcmp( resultlist.strings[resultlistindex], temp ))
//							break;
//					}

//					if( resultlistindex == resultlist.numstrings )
//					{
//						stringlistappend( &resultlist, temp );
//					}
//				}
//			}
//			stringlistfreecontents( &dirlist );
//		}
//	}

//	if( resultlist.numstrings )
//	{
//		stringlistsort( &resultlist );
//		numfiles = resultlist.numstrings;
//		numchars = 0;

//		for( resultlistindex = 0; resultlistindex < resultlist.numstrings; resultlistindex++ )
//			numchars += (int)Q_strlen( resultlist.strings[resultlistindex]) + 1;
//		search = reinterpret_cast<SSearch*>(Mem_Alloc( fs_mempool, sizeof(SSearch) + numchars + numfiles * sizeof( char* )));
//		search->filenames = (char **)((char *)search + sizeof( SSearch ));
//		search->filenamesbuffer = (char *)((char *)search + sizeof( SSearch ) + numfiles * sizeof( char* ));
//		search->numfilenames = (int)numfiles;
//		numfiles = numchars = 0;

//		for( resultlistindex = 0; resultlistindex < resultlist.numstrings; resultlistindex++ )
//		{
//			size_t	textlen;

//			search->filenames[numfiles] = search->filenamesbuffer + numchars;
//			textlen = Q_strlen(resultlist.strings[resultlistindex]) + 1;
//			Q_memcpy( search->filenames[numfiles], resultlist.strings[resultlistindex], textlen );
//			numfiles++;
//			numchars += (int)textlen;
//		}
//	}

//	stringlistfreecontents( &resultlist );

//	Mem_Free( basepath );

	return search;
}

void Fs::FS_InitMemory()
{
//	fs_mempool = Mem_AllocPool( "FileSystem Pool" );

//	// add a path separator to the end of the basedir if it lacks one
//	if( fs_basedir[0] && fs_basedir[Q_strlen(fs_basedir) - 1] != '/' && fs_basedir[Q_strlen(fs_basedir) - 1] != '\\' )
//		Q_strncat( fs_basedir, "/", sizeof( fs_basedir ));

//	fs_searchpaths = NULL;
}

/*
=============================================================================

WADSYSTEM PRIVATE COMMON FUNCTIONS

=============================================================================
*/
// associate extension with wad type
//const SWadType Fs::wad_types =
//{
//{ "pal", TYP_QPAL	}, // palette
//{ "lmp", TYP_QPIC	}, // quake1, hl pic
//{ "fnt", TYP_QFONT	}, // hl qfonts
//{ "mip", TYP_MIPTEX	}, // hl/q1 mip
//{ "raw", TYP_RAW	}, // signed raw data
//{ NULL,  TYP_NONE	}
//};

char Fs::W_TypeFromExt( const char *lumpname )
{
//	const char	*ext = FS_FileExtension( lumpname );
//	const SWadType	*type;

//	// we not known about filetype, so match only by filename
//	if( !Q_strcmp( ext, "*" ) || !Q_strcmp( ext, "" ))
//		return TYP_ANY;

//	for( type = wad_types; type->ext; type++ )
//	{
//		if(!Q_stricmp( ext, type->ext ))
//			return type->type;
//	}
	return TYP_NONE;
}

const char *Fs::W_ExtFromType( char lumptype )
{
//	const SWadType	*type;

//	// we not known aboyt filetype, so match only by filename
//	if( lumptype == TYP_NONE || lumptype == TYP_ANY )
//		return "";

//	for( type = wad_types; type->ext; type++ )
//	{
//		if( lumptype == type->type )
//			return type->ext;
//	}
	return "";
}

SDlumpInfo *Fs::W_FindLump( SWfile *wad, const char *name, const char matchtype )
{
	int	left, right, middle;

//	if( !wad || !wad->lumps || matchtype == TYP_NONE )
//		return NULL;

//	// look for the file (binary search)
//	left = 0;
//	right = wad->numlumps - 1;

//	while( left <= right )
//	{
//		int	diff;

//		middle = (left + right) / 2;
//		diff = Q_stricmp( wad->lumps[middle].name, name );

//		// Found it
//		if( !diff )
//		{
//			if( matchtype == TYP_ANY || matchtype == wad->lumps[middle].type )
//				return &wad->lumps[middle]; // found
//			else break;
//		}

//		// if we're too far in the list
//		if( diff > 0 )
//			right = middle - 1;
//		else left = middle + 1;
//	}

	return NULL;
}

/*
====================
FS_AddFileToWad

Add a file to the list of files contained into a package
====================
*/
SDlumpInfo *Fs::W_AddFileToWad( const char *name, SWfile *wad, int filepos, int packsize, int realsize, char type, char compression )
{
	int		left, right, middle;
	SDlumpInfo	*plump;

//	// look for the slot we should put that file into (binary search)
//	left = 0;
//	right = wad->numlumps - 1;

//	while( left <= right )
//	{
//		int	diff;

//		middle = ( left + right ) / 2;
//		diff = Q_stricmp( wad->lumps[middle].name, name );

//		// If we found the file, there's a problem
//		if( !diff ) MsgDev( D_NOTE, "Wad %s contains the file %s several times\n", wad->filename, name );

//		// If we're too far in the list
//		if( diff > 0 ) right = middle - 1;
//		else left = middle + 1;
//	}

//	// We have to move the right of the list by one slot to free the one we need
//	plump = &wad->lumps[left];
//	Q_memmove( plump + 1, plump, ( wad->numlumps - left ) * sizeof( *plump ));
//	wad->numlumps++;

//	Q_memcpy( plump->name, name, sizeof( plump->name ));
//	plump->filepos = filepos;
//	plump->disksize = realsize;
//	plump->size = packsize;
//	plump->compression = compression;

//	// convert all qmip types to miptex
//	if( type == TYP_QMIP )
//		plump->type = TYP_MIPTEX;
//	else plump->type = type;

//	// check for Quake 'conchars' issues (only lmp loader supposed to read this lame pic)
//	if( !Q_stricmp( plump->name, "conchars" ) && plump->type == TYP_QMIP )
//		plump->type = TYP_QPIC;

	return plump;
}

bool Fs::W_ReadLumpTable( SWfile *wad )
{
	size_t		lat_size;
	SDlumpInfo	*srclumps;
	int		i, k, numlumps;

	// nothing to convert ?
	if( !wad || !wad->numlumps )
		return false;

//	lat_size = wad->numlumps * sizeof( SDlumpInfo );
//	srclumps = (SDlumpInfo *)Mem_Alloc( wad->mempool, lat_size );
//	numlumps = wad->numlumps;
//	wad->numlumps = 0;	// reset it

//	if( read( wad->handle, srclumps, lat_size ) != lat_size )
//	{
//		MsgDev( D_ERROR, "W_ReadLumpTable: %s has corrupted lump allocation table\n", wad->filename );
//		W_Close( wad );
//		return false;
//	}

//	// swap everything
//	for( i = 0; i < numlumps; i++ )
//	{
//		char	name[16];

//		// cleanup lumpname
//		Q_strnlwr( srclumps[i].name, name, sizeof( srclumps[i].name ));

//		// check for '*' symbol issues
//		k = Q_strlen( Q_strrchr( name, '*' ));
//		if( k ) name[Q_strlen( name ) - k] = '!'; // quake1 issues (can't save images that contain '*' symbol)

//		W_AddFileToWad( name, wad, srclumps[i].filepos, srclumps[i].size, srclumps[i].disksize, srclumps[i].type, srclumps[i].compression );
//	}

//	// release source lumps
//	Mem_Free( srclumps );

	return true;
}

quint8 *Fs::W_ReadLump( SWfile *wad, SDlumpInfo *lump, size_t *lumpsizeptr )
{
	quint8	*buf;
	size_t	size = 0;

//	// assume error
//	if( lumpsizeptr ) *lumpsizeptr = 0;

//	// no wads loaded
//	if( !wad || !lump ) return NULL;

//	if( lseek( wad->handle, lump->filepos, SEEK_SET ) == -1 )
//	{
//		MsgDev( D_ERROR, "W_ReadLump: %s is corrupted\n", lump->name );
//		return NULL;
//	}

//	buf = (quint8 *)Mem_Alloc( wad->mempool, lump->disksize );
//	size = read( wad->handle, buf, lump->disksize );
//	if( size < lump->disksize )
//	{
//		MsgDev( D_WARN, "W_ReadLump: %s is probably corrupted\n", lump->name );
//		Mem_Free( buf );
//		return NULL;
//	}

//	if( lumpsizeptr ) *lumpsizeptr = lump->size;
	return buf;
}

/*
=============================================================================

WADSYSTEM PUBLIC BASE FUNCTIONS

=============================================================================
*/
SWfile *Fs::W_Open( const char *filename, const char *mode )
{
	SDwadInfo	header;
	SWfile		*wad = 0;//(SWfile *)Mem_Alloc( fs_mempool, sizeof( SWfile ));
	const char	*comment = "Generated by Xash WadLib. ";

//	if( mode[0] == 'a' ) wad->handle = open( filename, O_RDWR|O_BINARY, 0x666 );
//	else if( mode[0] == 'w' ) wad->handle = open( filename, O_CREAT|O_TRUNC|O_WRONLY|O_BINARY, 0x666 );
//	else if( mode[0] == 'r' ) wad->handle = open( filename, O_RDONLY|O_BINARY, 0x666 );

//	if( wad->handle < 0 )
//	{
//		W_Close( wad );
//		MsgDev( D_ERROR, "W_Open: couldn't open %s\n", filename );
//		return NULL;
//	}

//	// copy wad name
//	Q_strncpy( wad->filename, filename, sizeof( wad->filename ));
//	wad->mempool = Mem_AllocPool( filename );
//	wad->filetime = FS_SysFileTime( filename );

//	// if the file is opened in "write", "append", or "read/write" mode
//	if( mode[0] == 'w' )
//	{
//		SDwadInfo	hdr;

//		wad->numlumps = 0;		// blank wad
//		wad->lumps = NULL;		//
//		wad->mode = O_WRONLY;

//		// save space for header
//		hdr.ident = IDWAD3HEADER;
//		hdr.numlumps = wad->numlumps;
//		hdr.infotableofs = sizeof( SDwadInfo );
//		write( wad->handle, &hdr, sizeof( hdr ));
//		write( wad->handle, comment, Q_strlen( comment ) + 1 );
//		wad->infotableofs = tell( wad->handle );
//	}
//	else if( mode[0] == 'r' || mode[0] == 'a' )
//	{
//		if( mode[0] == 'a' )
//		{
//			lseek( wad->handle, 0, SEEK_SET );
//			wad->mode = O_APPEND;
//		}

//		if( read( wad->handle, &header, sizeof( SDwadInfo )) != sizeof( SDwadInfo ))
//		{
//			MsgDev( D_ERROR, "W_Open: %s can't read header\n", filename );
//			W_Close( wad );
//			return NULL;
//		}

//		switch( header.ident )
//		{
//		case IDWAD2HEADER:
//		case IDWAD3HEADER:
//			break; // WAD2, WAD3 allow r\w mode
//		default:
//			MsgDev( D_ERROR, "W_Open: %s unknown wadtype\n", filename );
//			W_Close( wad );
//			return NULL;
//		}

//		wad->numlumps = header.numlumps;
//		if( wad->numlumps >= MAX_FILES_IN_WAD && wad->mode == O_APPEND )
//		{
//			MsgDev( D_WARN, "W_Open: %s is full (%i lumps)\n", wad->numlumps );
//			wad->mode = O_RDONLY; // set read-only mode
//		}
//		wad->infotableofs = header.infotableofs; // save infotableofs position
//		if( lseek( wad->handle, wad->infotableofs, SEEK_SET ) == -1 )
//		{
//			MsgDev( D_ERROR, "W_Open: %s can't find lump allocation table\n", filename );
//			W_Close( wad );
//			return NULL;
//		}

//		// NOTE: lumps table can be reallocated for O_APPEND mode
//		wad->lumps = reinterpret_cast<SDlumpInfo*>(Mem_Alloc( wad->mempool, wad->numlumps * sizeof( SDlumpInfo )));

//		if( wad->mode == O_APPEND )
//		{
//			size_t	lat_size = wad->numlumps * sizeof( SDlumpInfo );

//			if( read( wad->handle, wad->lumps, lat_size ) != lat_size )
//			{
//				MsgDev( D_ERROR, "W_ReadLumpTable: %s has corrupted lump allocation table\n", wad->filename );
//				W_Close( wad );
//				return NULL;
//			}

//			// if we are in append mode - we need started from infotableofs poisition
//			// overwrite lumptable as well, we have her copy in wad->lumps
//			lseek( wad->handle, wad->infotableofs, SEEK_SET );
//		}
//		else
//		{
//			// setup lump allocation table
//			switch( header.ident )
//			{
//			case IDWAD2HEADER:
//			case IDWAD3HEADER:
//				if(!W_ReadLumpTable( wad ))
//					return NULL;
//				break;
//			}
//		}
//	}
	// and leaves the file open
	return wad;
}

void Fs::W_Close( SWfile *wad )
{
	int	ofs;

	if( !wad ) return;

	if( wad->handle >= 0 && ( wad->mode == O_APPEND || wad->mode == O_WRONLY ))
	{
		SDwadInfo	hdr;

		// write the lumpinfo
		ofs = tell( wad->handle );
		write( wad->handle, wad->lumps, wad->numlumps * sizeof( SDlumpInfo ));

		// write the header
		hdr.ident = IDWAD3HEADER;
		hdr.numlumps = wad->numlumps;
		hdr.infotableofs = ofs;

		lseek( wad->handle, 0, SEEK_SET );
		write( wad->handle, &hdr, sizeof( hdr ));
	}

//	Mem_FreePool( &wad->mempool );
	if( wad->handle >= 0 ) close( wad->handle );
//	Mem_Free( wad ); // free himself
}

/*
=============================================================================

FILESYSTEM IMPLEMENTATION

=============================================================================
*/
quint8 *Fs::W_LoadFile( const char *path, int *lumpsizeptr, bool gamedironly )
{
	SSearchPath	*search;
	int		index;

	search = FS_FindFile( path, &index, gamedironly );
	if( search && search->wad )
		return W_ReadLump( search->wad, &search->wad->lumps[index], reinterpret_cast<size_t*>(lumpsizeptr) );
	return NULL;
}
