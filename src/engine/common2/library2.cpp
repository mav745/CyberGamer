#include "library2.hpp"

#ifndef IMAGE_SIZEOF_BASE_RELOCATION
// Vista SDKs no longer define IMAGE_SIZEOF_BASE_RELOCATION!?
# define IMAGE_SIZEOF_BASE_RELOCATION (sizeof(IMAGE_BASE_RELOCATION))
#endif



// Protection flags for memory pages (Executable, Readable, Writeable)
//int ProtectionFlags[2][2][2] =
//{
//{
//{ PAGE_NOACCESS, PAGE_WRITECOPY },		// not executable
//{ PAGE_READONLY, PAGE_READWRITE },
//},
//{
//{ PAGE_EXECUTE, PAGE_EXECUTE_WRITECOPY },	// executable
//{ PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE },
//},
//};

typedef bool (WINAPI *DllEntryProc)( HINSTANCE hinstDLL, quint32 fdwReason, LPVOID lpReserved );

#define GET_HEADER_DICTIONARY( module, idx )	&(module)->headers->OptionalHeader.DataDirectory[idx]
#define CALCULATE_ADDRESS( base, offset )	(((quint32)(base)) + (offset))

void CLibrary::CopySections( const quint8 *data, PIMAGE_NT_HEADERS old_headers, MEMORYMODULE *module )
{
	int i, size;
	quint8 *dest;
	quint8 *codeBase = module->codeBase;
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION( module->headers );

	for( i = 0; i < module->headers->FileHeader.NumberOfSections; i++, section++ )
	{
		if( section->SizeOfRawData == 0 )
		{
			// section doesn't contain data in the dll itself, but may define
			// uninitialized data
			size = old_headers->OptionalHeader.SectionAlignment;

			if( size > 0 )
			{
				dest = (quint8 *)VirtualAlloc((quint8 *)CALCULATE_ADDRESS(codeBase, section->VirtualAddress), size, MEM_COMMIT, PAGE_READWRITE );
				section->Misc.PhysicalAddress = (quint32)dest;
//				Q_memset( dest, 0, size );
			}
			// section is empty
			continue;
		}

		// commit memory block and copy data from dll
		dest = (quint8 *)VirtualAlloc((quint8 *)CALCULATE_ADDRESS(codeBase, section->VirtualAddress), section->SizeOfRawData, MEM_COMMIT, PAGE_READWRITE );
//		Q_memcpy( dest, (quint8 *)CALCULATE_ADDRESS(data, section->PointerToRawData), section->SizeOfRawData );
		section->Misc.PhysicalAddress = (quint32)dest;
	}
}

void CLibrary::FreeSections( PIMAGE_NT_HEADERS old_headers, MEMORYMODULE *module )
{
	int	i, size;
	quint8	*codeBase = module->codeBase;
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(module->headers);

	for( i = 0; i < module->headers->FileHeader.NumberOfSections; i++, section++ )
	{
		if( section->SizeOfRawData == 0 )
		{
			size = old_headers->OptionalHeader.SectionAlignment;
			if( size > 0 )
			{
				VirtualFree( codeBase + section->VirtualAddress, size, MEM_DECOMMIT );
				section->Misc.PhysicalAddress = 0;
			}
			continue;
		}

		VirtualFree( codeBase + section->VirtualAddress, section->SizeOfRawData, MEM_DECOMMIT );
		section->Misc.PhysicalAddress = 0;
	}
}

void CLibrary::FinalizeSections( MEMORYMODULE *module )
{
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION( module->headers );
	int	i;

	// loop through all sections and change access flags
	for( i = 0; i < module->headers->FileHeader.NumberOfSections; i++, section++ )
	{
		quint32	protect, oldProtect, size;
		int	executable = (section->Characteristics & IMAGE_SCN_MEM_EXECUTE) != 0;
		int	readable = (section->Characteristics & IMAGE_SCN_MEM_READ) != 0;
		int	writeable = (section->Characteristics & IMAGE_SCN_MEM_WRITE) != 0;

		if( section->Characteristics & IMAGE_SCN_MEM_DISCARDABLE )
		{
			// section is not needed any more and can safely be freed
			VirtualFree((LPVOID)section->Misc.PhysicalAddress, section->SizeOfRawData, MEM_DECOMMIT);
			continue;
		}

		// determine protection flags based on characteristics
		protect = ProtectionFlags[executable][readable][writeable];
		if( section->Characteristics & IMAGE_SCN_MEM_NOT_CACHED )
			protect |= PAGE_NOCACHE;

		// determine size of region
		size = section->SizeOfRawData;

		if( size == 0 )
		{
			if( section->Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA )
				size = module->headers->OptionalHeader.SizeOfInitializedData;
			else if( section->Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA )
				size = module->headers->OptionalHeader.SizeOfUninitializedData;
		}

		if( size > 0 )
		{
			// change memory access flags
//			if( !VirtualProtect((LPVOID)section->Misc.PhysicalAddress, size, protect, &oldProtect ))
//				Sys_Error( "Com_FinalizeSections: error protecting memory page\n" );
		}
	}
}

void CLibrary::PerformBaseRelocation( MEMORYMODULE *module, quint32 delta )
{
	quint32	i;
	quint8	*codeBase = module->codeBase;
	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY( module, IMAGE_DIRECTORY_ENTRY_BASERELOC );

	if( directory->Size > 0 )
	{
		PIMAGE_BASE_RELOCATION relocation = (PIMAGE_BASE_RELOCATION)CALCULATE_ADDRESS( codeBase, directory->VirtualAddress );
		for( ; relocation->VirtualAddress > 0; )
		{
			quint8	*dest = (quint8 *)CALCULATE_ADDRESS( codeBase, relocation->VirtualAddress );
			quint16	*relInfo = (quint16 *)((quint8 *)relocation + IMAGE_SIZEOF_BASE_RELOCATION );

			for( i = 0; i<((relocation->SizeOfBlock-IMAGE_SIZEOF_BASE_RELOCATION) / 2); i++, relInfo++ )
			{
				quint32	*patchAddrHL;
				int	type, offset;

				// the upper 4 bits define the type of relocation
				type = *relInfo >> 12;
				// the lower 12 bits define the offset
				offset = *relInfo & 0xfff;

				switch( type )
				{
				case IMAGE_REL_BASED_ABSOLUTE:
					// skip relocation
					break;
				case IMAGE_REL_BASED_HIGHLOW:
					// change complete 32 bit address
					patchAddrHL = (quint32 *)CALCULATE_ADDRESS( dest, offset );
					*patchAddrHL += delta;
					break;
				default:
//					MsgDev( D_ERROR, "PerformBaseRelocation: unknown relocation: %d\n", type );
					break;
				}
			}

			// advance to next relocation block
			relocation = (PIMAGE_BASE_RELOCATION)CALCULATE_ADDRESS( relocation, relocation->SizeOfBlock );
		}
	}
}

FARPROC CLibrary::MemoryGetProcAddress( void *module, const char *name )
{
	int	idx = -1;
	quint32	i, *nameRef;
	quint16	*ordinal;
	PIMAGE_EXPORT_DIRECTORY exports;
	quint8	*codeBase = ((MEMORYMODULE *)module)->codeBase;
	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY((MEMORYMODULE *)module, IMAGE_DIRECTORY_ENTRY_EXPORT );

	if( directory->Size == 0 )
	{
		// no export table found
		return NULL;
	}

	exports = (PIMAGE_EXPORT_DIRECTORY)CALCULATE_ADDRESS( codeBase, directory->VirtualAddress );

	if( exports->NumberOfNames == 0 || exports->NumberOfFunctions == 0 )
	{
		// DLL doesn't export anything
		return NULL;
	}

	// search function name in list of exported names
	nameRef = (quint32 *)CALCULATE_ADDRESS( codeBase, exports->AddressOfNames );
	ordinal = (quint16 *)CALCULATE_ADDRESS( codeBase, exports->AddressOfNameOrdinals );

	for( i = 0; i < exports->NumberOfNames; i++, nameRef++, ordinal++ )
	{
		// GetProcAddress case insensative ?????
//		if( !Q_stricmp( name, (const char *)CALCULATE_ADDRESS( codeBase, *nameRef )))
//		{
//			idx = *ordinal;
//			break;
//		}
	}

	if( idx == -1 )
	{
		// exported symbol not found
		return NULL;
	}

	if((quint32)idx > exports->NumberOfFunctions )
	{
		// name <-> ordinal number don't match
		return NULL;
	}

	// addressOfFunctions contains the RVAs to the "real" functions
	return (FARPROC)CALCULATE_ADDRESS( codeBase, *(quint32 *)CALCULATE_ADDRESS( codeBase, exports->AddressOfFunctions + (idx * 4)));
}

int CLibrary::BuildImportTable( MEMORYMODULE *module )
{
	int	result=1;
	quint8	*codeBase = module->codeBase;
	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY( module, IMAGE_DIRECTORY_ENTRY_IMPORT );

	if( directory->Size > 0 )
	{
		PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)CALCULATE_ADDRESS( codeBase, directory->VirtualAddress );

		for( ; !IsBadReadPtr( importDesc, sizeof( IMAGE_IMPORT_DESCRIPTOR )) && importDesc->Name; importDesc++ )
		{
			quint32	*thunkRef, *funcRef;
			LPCSTR	libname;
			void	*handle;

			libname = (LPCSTR)CALCULATE_ADDRESS( codeBase, importDesc->Name );
//			handle = Com_LoadLibraryExt( libname, false, true );

//			if( handle == NULL )
//			{
//				MsgDev( D_ERROR, "couldn't load library %s\n", libname );
//				result = 0;
//				break;
//			}

//			module->modules = reinterpret_cast<void**>(Mem_Realloc( host.mempool, module->modules, (module->numModules + 1) * (sizeof( void* ))));
//			module->modules[module->numModules++] = handle;

			if( importDesc->OriginalFirstThunk )
			{
				thunkRef = (quint32 *)CALCULATE_ADDRESS( codeBase, importDesc->OriginalFirstThunk );
				funcRef = (quint32 *)CALCULATE_ADDRESS( codeBase, importDesc->FirstThunk );
			}
			else
			{
				// no hint table
				thunkRef = (quint32 *)CALCULATE_ADDRESS( codeBase, importDesc->FirstThunk );
				funcRef = (quint32 *)CALCULATE_ADDRESS( codeBase, importDesc->FirstThunk );
			}

			for( ; *thunkRef; thunkRef++, funcRef++ )
			{
				if( IMAGE_SNAP_BY_ORDINAL( *thunkRef ))
				{
					LPCSTR	funcName = (LPCSTR)IMAGE_ORDINAL( *thunkRef );
//					*funcRef = (quint32)Com_GetProcAddress( handle, funcName );
				}
				else
				{
					PIMAGE_IMPORT_BY_NAME thunkData = (PIMAGE_IMPORT_BY_NAME)CALCULATE_ADDRESS( codeBase, *thunkRef );
					LPCSTR	funcName = (LPCSTR)&thunkData->Name;
//					*funcRef = (quint32)Com_GetProcAddress( handle, funcName );
				}

				if( *funcRef == 0 )
				{
					result = 0;
					break;
				}
			}
			if( !result ) break;
		}
	}
	return result;
}

void CLibrary::MemoryFreeLibrary( void *hInstance )
{
	MEMORYMODULE	*module = (MEMORYMODULE *)hInstance;

	if( module != NULL )
	{
		int	i;

		if( module->initialized != 0 )
		{
			// notify library about detaching from process
			DllEntryProc DllEntry = (DllEntryProc)CALCULATE_ADDRESS( module->codeBase, module->headers->OptionalHeader.AddressOfEntryPoint );
			(*DllEntry)((HINSTANCE)module->codeBase, DLL_PROCESS_DETACH, 0 );
			module->initialized = 0;
		}

		if( module->modules != NULL )
		{
			// free previously opened libraries
			for( i = 0; i < module->numModules; i++ )
			{
				if( module->modules[i] != NULL )
				{
//					Com_FreeLibrary( module->modules[i] );
				}
			}
//			Mem_Free( module->modules ); // Mem_Realloc end
		}

		FreeSections( module->headers, module );

		if( module->codeBase != NULL )
		{
			// release memory of library
			VirtualFree( module->codeBase, 0, MEM_RELEASE );
		}

		HeapFree( GetProcessHeap(), 0, module );
	}
}

void *CLibrary::MemoryLoadLibrary( const char *name )
{
	MEMORYMODULE	*result = NULL;
	PIMAGE_DOS_HEADER	dos_header;
	PIMAGE_NT_HEADERS	old_header;
	quint8		*code, *headers;
	quint32		locationDelta;
	DllEntryProc	DllEntry;
	QString		errorstring;
	bool		successfull;
	void		*data = NULL;

//	data = FS_LoadFile( name, NULL, false );

//	if( !data )
//	{
//		Q_sprintf( errorstring, "couldn't load %s", name );
//		goto library_error;
//	}

//	dos_header = (PIMAGE_DOS_HEADER)data;
//	if( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
//	{
//		Q_sprintf( errorstring, "%s it's not a valid executable file", name );
//		goto library_error;
//	}

//	old_header = (PIMAGE_NT_HEADERS)&((const quint8 *)(data))[dos_header->e_lfanew];
//	if( old_header->Signature != IMAGE_NT_SIGNATURE )
//	{
//		Q_sprintf( errorstring, "%s missing PE header", name );
//		goto library_error;
//	}

//	// reserve memory for image of library
//	code = (quint8 *)VirtualAlloc((LPVOID)(old_header->OptionalHeader.ImageBase), old_header->OptionalHeader.SizeOfImage, MEM_RESERVE, PAGE_READWRITE );

//	if( code == NULL )
//	{
//		// try to allocate memory at arbitrary position
//		code = (quint8 *)VirtualAlloc( NULL, old_header->OptionalHeader.SizeOfImage, MEM_RESERVE, PAGE_READWRITE );
//	}
//	if( code == NULL )
//	{
//		Q_sprintf( errorstring, "%s can't reserve memory", name );
//		goto library_error;
//	}

//	result = (MEMORYMODULE *)HeapAlloc( GetProcessHeap(), 0, sizeof( MEMORYMODULE ));
//	result->codeBase = code;
//	result->numModules = 0;
//	result->modules = NULL;
//	result->initialized = 0;

//	// XXX: is it correct to commit the complete memory region at once?
//	// calling DllEntry raises an exception if we don't...
//	VirtualAlloc( code, old_header->OptionalHeader.SizeOfImage, MEM_COMMIT, PAGE_READWRITE );

//	// commit memory for headers
//	headers = (quint8 *)VirtualAlloc( code, old_header->OptionalHeader.SizeOfHeaders, MEM_COMMIT, PAGE_READWRITE );

//	// copy PE header to code
//	Q_memcpy( headers, dos_header, dos_header->e_lfanew + old_header->OptionalHeader.SizeOfHeaders );
//	result->headers = (PIMAGE_NT_HEADERS)&((const quint8 *)(headers))[dos_header->e_lfanew];

//	// update position
//	result->headers->OptionalHeader.ImageBase = (quint32)code;

//	// copy sections from DLL file block to new memory location
//	CopySections( reinterpret_cast<quint8*>(data), old_header, result );

//	// adjust base address of imported data
//	locationDelta = (quint32)(code - old_header->OptionalHeader.ImageBase);
//	if( locationDelta != 0 ) PerformBaseRelocation( result, locationDelta );

//	// load required dlls and adjust function table of imports
//	if( !BuildImportTable( result ))
//	{
//		Q_sprintf( errorstring, "%s failed to build import table", name );
//		goto library_error;
//	}

//	// mark memory pages depending on section headers and release
//	// sections that are marked as "discardable"
//	FinalizeSections( result );

//	// get entry point of loaded library
//	if( result->headers->OptionalHeader.AddressOfEntryPoint != 0 )
//	{
//		DllEntry = (DllEntryProc)CALCULATE_ADDRESS( code, result->headers->OptionalHeader.AddressOfEntryPoint );
//		if( DllEntry == 0 )
//		{
//			Q_sprintf( errorstring, "%s has no entry point", name );
//			goto library_error;
//		}

//		// notify library about attaching to process
//		successfull = (*DllEntry)((HINSTANCE)code, DLL_PROCESS_ATTACH, 0 );
//		if( !successfull )
//		{
//			Q_sprintf( errorstring, "can't attach library %s", name );
//			goto library_error;
//		}
//		result->initialized = 1;
//	}

//	Mem_Free( data ); // release memory
//	return (void *)result;
//library_error:
//	// cleanup
//	if( data ) Mem_Free( data );
//	MemoryFreeLibrary( result );
//	MsgDev( D_ERROR, "LoadLibrary: %s\n", errorstring );

	return NULL;
}

/*
---------------------------------------------------------------

		Name for function stuff

---------------------------------------------------------------
*/
void CLibrary::FsGetString( SFile *f, char *str )
{
	char	ch;

//	while(( ch = FS_Getc( f )) != EOF )
//	{
//		*str++ = ch;
//		if( !ch ) break;
//	}
}

void CLibrary::FreeNameFuncGlobals( SDllUser *hInst )
{
	int	i;

	if( !hInst ) return;

//	if( hInst->ordinals ) Mem_Free( hInst->ordinals );
//	if( hInst->funcs ) Mem_Free( hInst->funcs );

//	for( i = 0; i < hInst->num_ordinals; i++ )
//	{
//		if( hInst->names[i] )
//			Mem_Free( hInst->names[i] );
//	}

	hInst->num_ordinals = 0;
	hInst->ordinals = NULL;
	hInst->funcs = NULL;
}

char *CLibrary::GetMSVCName( const char *in_name )
{
	char	*pos, *out_name;

//	if( in_name[0] == '?' )  // is this a MSVC C++ mangled name?
//	{
//		if(( pos = Q_strstr( in_name, "@@" )) != NULL )
//		{
//			int	len = pos - in_name;

//			// strip off the leading '?'
//			out_name = copystring( in_name + 1 );
//			out_name[len-1] = 0; // terminate string at the "@@"
//			return out_name;
//		}
//	}
	return "";//copystring( in_name );
}

bool CLibrary::LibraryLoadSymbols( SDllUser *hInst )
{
	SFile		*f;
	QString		errorstring;
	SDosHeader	dos_header;
	LONG		nt_signature;
	SPeHeader		pe_header;
	SSectionHeader	section_header;
	bool		rdata_found;
	SOptionalHeader	optional_header;
	long		rdata_delta = 0;
	SExportDirectory	export_directory;
	long		name_offset;
	long		exports_offset;
	long		ordinal_offset;
	long		function_offset;
	QString		function_name;
	quint32		*p_Names = NULL;
	int		i, index;

//	// can only be done for loaded libraries
//	if( !hInst ) return false;

//	for( i = 0; i < hInst->num_ordinals; i++ )
//		hInst->names[i] = NULL;

//	f = FS_Open( hInst->shortPath, "rb", false );
//	if( !f )
//	{
//		Q_sprintf( errorstring, "couldn't load %s", hInst->shortPath );
//		goto table_error;
//	}

//	if( FS_Read( f, &dos_header, sizeof( dos_header )) != sizeof( dos_header ))
//	{
//		Q_sprintf( errorstring, "%s has corrupted EXE header", hInst->shortPath );
//		goto table_error;
//	}

//	if( dos_header.e_magic != DOS_SIGNATURE )
//	{
//		Q_sprintf( errorstring, "%s does not have a valid dll signature", hInst->shortPath );
//		goto table_error;
//	}

//	if( FS_Seek( f, dos_header.e_lfanew, SEEK_SET ) == -1 )
//	{
//		Q_sprintf( errorstring, "%s error seeking for new exe header", hInst->shortPath );
//		goto table_error;
//	}

//	if( FS_Read( f, &nt_signature, sizeof( nt_signature )) != sizeof( nt_signature ))
//	{
//		Q_sprintf( errorstring, "%s has corrupted NT header", hInst->shortPath );
//		goto table_error;
//	}

//	if( nt_signature != NT_SIGNATURE )
//	{
//		Q_sprintf( errorstring, "%s does not have a valid NT signature", hInst->shortPath );
//		goto table_error;
//	}

//	if( FS_Read( f, &pe_header, sizeof( pe_header )) != sizeof( pe_header ))
//	{
//		Q_sprintf( errorstring, "%s does not have a valid PE header", hInst->shortPath );
//		goto table_error;
//	}

//	if( !pe_header.SizeOfOptionalHeader )
//	{
//		Q_sprintf( errorstring, "%s does not have an optional header", hInst->shortPath );
//		goto table_error;
//	}

//	if( FS_Read( f, &optional_header, sizeof( optional_header )) != sizeof( optional_header ))
//	{
//		Q_sprintf( errorstring, "%s optional header probably corrupted", hInst->shortPath );
//		goto table_error;
//	}

//	rdata_found = false;

//	for( i = 0; i < pe_header.NumberOfSections; i++ )
//	{
//		if( FS_Read( f, &section_header, sizeof( section_header )) != sizeof( section_header ))
//		{
//			Q_sprintf( errorstring, "%s error during reading section header", hInst->shortPath );
//			goto table_error;
//		}

//		if((( optional_header.DataDirectory[0].VirtualAddress >= section_header.VirtualAddress ) &&
//			(optional_header.DataDirectory[0].VirtualAddress < (section_header.VirtualAddress + section_header.Misc.VirtualSize))))
//		{
//			rdata_found = true;
//			break;
//		}
//	}

//	if( rdata_found )
//	{
//		rdata_delta = section_header.VirtualAddress - section_header.PointerToRawData;
//	}

//	exports_offset = optional_header.DataDirectory[0].VirtualAddress - rdata_delta;

//	if( FS_Seek( f, exports_offset, SEEK_SET ) == -1 )
//	{
//		Q_sprintf( errorstring, "%s does not have a valid exports section", hInst->shortPath );
//		goto table_error;
//	}

//	if( FS_Read( f, &export_directory, sizeof( export_directory )) != sizeof( export_directory ))
//	{
//		Q_sprintf( errorstring, "%s does not have a valid optional header", hInst->shortPath );
//		goto table_error;
//	}

//	hInst->num_ordinals = export_directory.NumberOfNames;	// also number of ordinals

//	if( hInst->num_ordinals > MAX_LIBRARY_EXPORTS )
//	{
//		Q_sprintf( errorstring, "%s too many exports %i", hInst->shortPath, hInst->num_ordinals );
//		hInst->num_ordinals = 0;
//		goto table_error;
//	}

//	ordinal_offset = export_directory.AddressOfNameOrdinals - rdata_delta;

//	if( FS_Seek( f, ordinal_offset, SEEK_SET ) == -1 )
//	{
//		Q_sprintf( errorstring, "%s does not have a valid ordinals section", hInst->shortPath );
//		goto table_error;
//	}

//	hInst->ordinals = reinterpret_cast<quint16*>(Mem_Alloc( host.mempool, hInst->num_ordinals * sizeof( quint16 )));

//	if( FS_Read( f, hInst->ordinals, hInst->num_ordinals * sizeof( quint16 )) != (hInst->num_ordinals * sizeof( quint16 )))
//	{
//		Q_sprintf( errorstring, "%s error during reading ordinals table", hInst->shortPath );
//		goto table_error;
//	}

//	function_offset = export_directory.AddressOfFunctions - rdata_delta;

//	if( FS_Seek( f, function_offset, SEEK_SET ) == -1 )
//	{
//		Q_sprintf( errorstring, "%s does not have a valid export address section", hInst->shortPath );
//		goto table_error;
//	}

//	hInst->funcs = reinterpret_cast<quint32*>(Mem_Alloc( host.mempool, hInst->num_ordinals * sizeof( quint32 )));

//	if( FS_Read( f, hInst->funcs, hInst->num_ordinals * sizeof( quint32 )) != (hInst->num_ordinals * sizeof( quint32 )))
//	{
//		Q_sprintf( errorstring, "%s error during reading export address section", hInst->shortPath );
//		goto table_error;
//	}

//	name_offset = export_directory.AddressOfNames - rdata_delta;

//	if( FS_Seek( f, name_offset, SEEK_SET ) == -1 )
//	{
//		Q_sprintf( errorstring, "%s file does not have a valid names section", hInst->shortPath );
//		goto table_error;
//	}

//	p_Names = reinterpret_cast<quint32*>(Mem_Alloc( host.mempool, hInst->num_ordinals * sizeof( quint32 )));

//	if( FS_Read( f, p_Names, hInst->num_ordinals * sizeof( quint32 )) != (hInst->num_ordinals * sizeof( quint32 )))
//	{
//		Q_sprintf( errorstring, "%s error during reading names table", hInst->shortPath );
//		goto table_error;
//	}

//	for( i = 0; i < hInst->num_ordinals; i++ )
//	{
//		name_offset = p_Names[i] - rdata_delta;

//		if( name_offset != 0 )
//		{
//			if( FS_Seek( f, name_offset, SEEK_SET ) != -1 )
//			{
//				FsGetString( f, function_name );
//				hInst->names[i] = GetMSVCName( function_name );
//			}
//			else break;
//		}
//	}

//	if( i != hInst->num_ordinals )
//	{
//		Q_sprintf( errorstring, "%s error during loading names section", hInst->shortPath );
//		goto table_error;
//	}
//	FS_Close( f );

//	for( i = 0; i < hInst->num_ordinals; i++ )
//	{
//		if( !Q_strcmp( "GiveFnptrsToDll", hInst->names[i] ))	// main entry point for user dlls
//		{
//			void	*fn_offset;

//			index = hInst->ordinals[i];
//			fn_offset = (void *)Com_GetProcAddress( hInst, "GiveFnptrsToDll" );
//			hInst->funcBase = (quint32)(fn_offset) - hInst->funcs[index];
//			break;
//		}
//	}

//	if( p_Names ) Mem_Free( p_Names );
//	return true;
//table_error:
//	// cleanup
//	if( f ) FS_Close( f );
//	if( p_Names ) Mem_Free( p_Names );
//	FreeNameFuncGlobals( hInst );
//	MsgDev( D_ERROR, "LoadLibrary: %s\n", errorstring );

	return false;
}

/*
================
Com_LoadLibrary

smart dll loader - can loading dlls from pack or wad files
================
*/
void *CLibrary::Com_LoadLibraryExt( const char *dllname, int build_ordinals_table, bool directpath )
{
	SDllUser *hInst;

//	hInst = FS_FindLibrary( dllname, directpath );
//	if( !hInst ) return NULL; // nothing to load

//	/*if( hInst->custom_loader )
//	{
//		if( hInst->encrypted )
//		{
//			MsgDev( D_ERROR, "Sys_LoadLibrary: couldn't load encrypted library %s\n", dllname );
//			return NULL;
//		}

//		hInst->hInstance = MemoryLoadLibrary( hInst->fullPath );
//	}
//	else */
//	hInst->hInstance = LoadLibrary( hInst->fullPath );

//	if( !hInst->hInstance )
//	{
//		MsgDev( D_NOTE, "Sys_LoadLibrary: Loading %s - failed\n", dllname );
//		Com_FreeLibrary( hInst );
//		return NULL;
//	}

//	// if not set - FunctionFromName and NameForFunction will not working
//	if( build_ordinals_table )
//	{
//		if( !LibraryLoadSymbols( hInst ))
//		{
//			MsgDev( D_NOTE, "Sys_LoadLibrary: Loading %s - failed\n", dllname );
//			Com_FreeLibrary( hInst );
//			return NULL;
//		}
//	}

//	MsgDev( D_NOTE, "Sys_LoadLibrary: Loading %s - ok\n", dllname );

	return hInst;
}

void *CLibrary::Com_LoadLibrary( const char *dllname, int build_ordinals_table )
{
	return 0;//Com_LoadLibraryExt( dllname, build_ordinals_table, false );
}

void *CLibrary::Com_GetProcAddress( void *hInstance, const char *name )
{
	SDllUser *hInst = (SDllUser *)hInstance;

	if( !hInst || !hInst->hInstance )
		return NULL;

	//if( hInst->custom_loader )
	//	return (void *)MemoryGetProcAddress( hInst->hInstance, name );
	return GetProcAddress( reinterpret_cast<HMODULE>(hInst->hInstance), name );
}

void CLibrary::Com_FreeLibrary( void *hInstance )
{
	SDllUser *hInst = (SDllUser *)hInstance;

	if( !hInst || !hInst->hInstance )
		return; // already freed

//	if( host.state == HOST_CRASHED )
//	{
//		// we need to hold down all modules, while MSVC can find error
//		MsgDev( D_NOTE, "Sys_FreeLibrary: hold %s for debugging\n", hInst->dllName );
//		return;
//	}
//	else MsgDev( D_NOTE, "Sys_FreeLibrary: Unloading %s\n", hInst->dllName );

	if( hInst->custom_loader )
		MemoryFreeLibrary( hInst->hInstance );
	else FreeLibrary( reinterpret_cast<HMODULE>(hInst->hInstance) );

	hInst->hInstance = NULL;

	if( hInst->num_ordinals )
		FreeNameFuncGlobals( hInst );
//	Mem_Free( hInst );	// done
}

quint32 CLibrary::Com_FunctionFromName( void *hInstance, const char *pName )
{
	SDllUser	*hInst = (SDllUser *)hInstance;
	int		i, index;

	if( !hInst || !hInst->hInstance )
		return 0;

	for( i = 0; i < hInst->num_ordinals; i++ )
	{
//		if( !Q_strcmp( pName, hInst->names[i] ))
//		{
//			index = hInst->ordinals[i];
//			return hInst->funcs[index] + hInst->funcBase;
//		}
	}
	// couldn't find the function name to return address
	return 0;
}

const char *CLibrary::Com_NameForFunction( void *hInstance, quint32 function )
{
	SDllUser	*hInst = (SDllUser *)hInstance;
	int		i, index;

	if( !hInst || !hInst->hInstance )
		return NULL;

	for( i = 0; i < hInst->num_ordinals; i++ )
	{
		index = hInst->ordinals[i];

		if(( function - hInst->funcBase ) == hInst->funcs[index] )
			return hInst->names[i];
	}
	// couldn't find the function address to return name
	return NULL;
}
