#ifndef FS_HPP
#define FS_HPP

#include "xash_types.hpp"

class Fs
{
	quint8		*fs_mempool;
	SSearchPath	*fs_searchpaths;// = NULL;
	SSearchPath	fs_directpath;		// direct path
	char		fs_rootdir[MAX_SYSPATH];	// engine root directory
	char		fs_basedir[MAX_SYSPATH];	// base directory of game
	char		fs_falldir[MAX_SYSPATH];	// game falling directory
	char		fs_gamedir[MAX_SYSPATH];	// game current directory
	char		gs_basedir[MAX_SYSPATH];	// initial dir before loading gameinfo.txt (used for compilers too)
	bool		fs_ext_path;// = false;	// attempt to read\write from ./ or ../ pathes

	int matchpattern( const char *in, const char *pattern, bool caseinsensitive );
	void listdirectory( QStringList list, const char *path );
	SPackFile* FS_AddFileToPack( const char* name, SPack* pack, int offset, int size );
	void FS_CreatePath( char *path );
	void FS_Path_f();
	void FS_ClearPaths_f();
	void FS_FileBase( const char *in, char *out );
	SPack *FS_LoadPackPAK( const char *packfile, int *error );
	bool FS_AddPack_Fullpath( const char *pakfile, bool *already_loaded, bool keep_plain_dirs, int flags );
	bool FS_AddWad_Fullpath( const char *wadfile, bool *already_loaded, bool keep_plain_dirs );
	void FS_AddGameDirectory( const char *dir, int flags );
	void FS_AddGameHierarchy( const char *dir, int flags );
	const char *FS_FileExtension( const char *in );
	const char *FS_FileWithoutPath( const char *in );
	void FS_ExtractFilePath( const char* const path, char* dest );
	void FS_ClearSearchPath();
	int FS_CheckNastyPath( const char *path, bool isgamedir );
	void FS_Rescan();
	void FS_Rescan_f();

	bool FS_ParseVector( char **pfile, float *v, size_t size );
	bool FS_WriteGameInfo( const char *filepath, SGameInfo *GameInfo );
	void FS_CreateDefaultGameInfo( const char *filename );

	bool FS_ParseLiblistGam( const char *filename, const char *gamedir, SGameInfo *GameInfo );
	void FS_ConvertGameInfo( const char *gamedir, const char *gameinfo_path, const char *liblist_path );
	bool FS_ParseGameInfo( const char *gamedir, SGameInfo *GameInfo );
	void FS_LoadGameInfo( const char *rootfolder );
	void FS_Init();
	void FS_AllowDirectPaths( bool enable );
	void FS_Shutdown();
	long FS_SysFileTime( const char *filename );
	SFile* FS_SysOpen( const char* filepath, const char* mode );
	SFile *FS_OpenPackedFile( SPack *pack, int pack_ind );
	bool FS_SysFileExists( const char *path );
	bool FS_SysFolderExists( const char *path );
	SSearchPath *FS_FindFile( const char *name, int* index, bool gamedironly );
	SFile *FS_OpenReadFile( const char *filename, const char *mode, bool gamedironly );
	SFile *FS_Open( const char *filepath, const char *mode, bool gamedironly );
	int FS_Close( SFile *file );
	int FS_Write( SFile *file, const void *data, size_t datasize );
	int FS_Read( SFile *file, void *buffer, size_t buffersize );
	int FS_Print( SFile *file, const char *msg );
	int FS_Printf( SFile *file, const char* format, ... );
	int FS_VPrintf( SFile *file, const char* format, va_list ap );
	int FS_Getc( SFile *file );
	int FS_UnGetc( SFile *file, quint8 c );
	int FS_Gets( SFile *file, quint8 *string, size_t bufsize );
	int FS_Seek( SFile *file, int offset, int whence );
	int FS_Tell( SFile* file );
	bool FS_Eof( SFile* file );
	void FS_Purge( SFile* file );
	quint8 *FS_LoadFile( const char *path, int *filesizeptr, bool gamedironly );
	SFile *FS_OpenFile( const char *path, int *filesizeptr, bool gamedironly );
	bool FS_WriteFile( const char *filename, const void *data, int len );
	void FS_StripExtension( char *path );
	void FS_DefaultExtension( char *path, const char *extension );
	bool FS_FileExists( const char *filename, bool gamedironly );
	const char *FS_GetDiskPath( const char *name, bool gamedironly );
	bool FS_CheckForCrypt( const char *dllname );
	SDllUser *FS_FindLibrary( const char *dllname, bool directpath );
	int FS_FileSize( const char *filename, bool gamedironly );
	int FS_FileLength( SFile *f );
	int FS_FileTime( const char *filename, bool gamedironly );
	bool FS_Rename( const char *oldname, const char *newname );
	bool FS_Delete( const char *path );
	void FS_FileCopy( SFile *pOutput, SFile *pInput, int fileSize );
	SSearch *FS_Search( const char *pattern, int caseinsensitive, int gamedironly );
	void FS_InitMemory();
	SWadType wad_types[6];
	char W_TypeFromExt( const char *lumpname );

	const char *W_ExtFromType( char lumptype );
	SDlumpInfo *W_FindLump( SWfile *wad, const char *name, const char matchtype );
	SDlumpInfo *W_AddFileToWad( const char *name, SWfile *wad, int filepos, int packsize, int realsize, char type, char compression );

	bool W_ReadLumpTable( SWfile *wad );

	quint8 *W_ReadLump( SWfile *wad, SDlumpInfo *lump, size_t *lumpsizeptr );
	SWfile *W_Open( const char *filename, const char *mode );

	void W_Close( SWfile *wad );
	quint8 *W_LoadFile( const char *path, int *lumpsizeptr, bool gamedironly );



};


#endif //FS_HPP
