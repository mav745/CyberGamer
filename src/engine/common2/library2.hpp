#ifndef LIBRARY_H
#define LIBRARY_H

#include "xash_types.hpp"

#include <windows.h>
#include <winnt.h>
#include <minwindef.h>

class CLibrary
{
public:
	CLibrary(const QString dllname, int build_ordinals_table) {}
	CLibrary(const QString dllname, int build_ordinals_table, bool directpath) {}
	~CLibrary() {}

	struct MEMORYMODULE
	{
		PIMAGE_NT_HEADERS	headers;
		quint8		*codeBase;
		void		**modules;
		int		numModules;
		int		initialized;
	};

	void *instance;

	// Protection flags for memory pages (Executable, Readable, Writeable)
	int ProtectionFlags[2][2][2];

	void CopySections( const quint8 *data, PIMAGE_NT_HEADERS old_headers, MEMORYMODULE *module );
	void FreeSections( PIMAGE_NT_HEADERS old_headers, MEMORYMODULE *module );
	void FinalizeSections( MEMORYMODULE *module );
	void PerformBaseRelocation( MEMORYMODULE *module, quint32 delta );
	FARPROC MemoryGetProcAddress( void *module, const char *name );
	int BuildImportTable( MEMORYMODULE *module );
	void MemoryFreeLibrary( void *hInstance );
	void *MemoryLoadLibrary( const char *name );
	void FsGetString( SFile *f, char *str );
	void FreeNameFuncGlobals( SDllUser *hInst );
	char *GetMSVCName( const char *in_name );
	bool LibraryLoadSymbols( SDllUser *hInst );
	void *Com_LoadLibraryExt( const char *dllname, int build_ordinals_table, bool directpath );
	void *Com_LoadLibrary( const char *dllname, int build_ordinals_table );
	void *Com_GetProcAddress( void *hInstance, const char *name );
	void Com_FreeLibrary( void *hInstance );
	quint32 Com_FunctionFromName( void *hInstance, const char *pName );
	const char *Com_NameForFunction( void *hInstance, quint32 function );

};

#endif //LIBRARY_H
