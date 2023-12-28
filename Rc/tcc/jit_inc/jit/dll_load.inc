#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libTcc.h"
#include "Xternal.h"

//// Functions call /////
fptr_tcc_new 					   tcc_new;
fptr_tcc_delete 				   tcc_delete;
fptr_tcc_set_lib_path 			tcc_set_lib_path;
fptr_tcc_set_error_func 		tcc_set_error_func;
fptr_tcc_set_options 			tcc_set_options;
fptr_tcc_add_include_path 		tcc_add_include_path;
fptr_tcc_add_sysinclude_path 	tcc_add_sysinclude_path;
fptr_tcc_define_symbol 			tcc_define_symbol;
fptr_tcc_undefine_symbol 		tcc_undefine_symbol;
fptr_tcc_add_file 				tcc_add_file;
fptr_tcc_compile_string 	 	tcc_compile_string;
fptr_tcc_set_output_type 	 	tcc_set_output_type;
fptr_tcc_add_library_path 	 	tcc_add_library_path;
fptr_tcc_add_library 	 		tcc_add_library;
fptr_tcc_add_symbol 	 		tcc_add_symbol;
fptr_tcc_output_file 	 		tcc_output_file;
fptr_tcc_run 	 				tcc_run;
fptr_tcc_relocate 	 			tcc_relocate;
fptr_tcc_get_symbol 	 		tcc_get_symbol;
/////////////////////////

static bool func_error(char* func_name){
	err_print("error loading: %s", func_name);
	return true;
}
//
#define LoadProcAddress(_name) _name=(fptr_##_name)GetProcAddress(h_dll,#_name);if(_name==0){if(func_error(#_name))return false;}
//
//TCCState *s;
bool dll_load( char* dll){

  HANDLE h_dll;

 #if defined(D_x32)
	h_dll = LoadLibraryA("Rc/tcc/libtcc_32.dll");/*Explicit Load*/
	info_print("Load libtcc_32.dll");
 #endif
 #if defined(D_x64)
	h_dll = LoadLibraryA("Rc/tcc/libtcc_64.dll");/*Explicit Load*/
	info_print("Load libtcc_64.dll");
 #endif
  if(h_dll){
 	
	LoadProcAddress(tcc_new);
	LoadProcAddress(tcc_delete);
	LoadProcAddress(tcc_set_lib_path);
	LoadProcAddress(tcc_set_error_func);
	LoadProcAddress(tcc_set_options);
	LoadProcAddress(tcc_add_include_path);
	LoadProcAddress(tcc_add_sysinclude_path);
	LoadProcAddress(tcc_define_symbol);
	LoadProcAddress(tcc_undefine_symbol);
	LoadProcAddress(tcc_add_file);
	LoadProcAddress(tcc_compile_string);
	LoadProcAddress(tcc_set_output_type);
	LoadProcAddress(tcc_add_library_path);
	LoadProcAddress(tcc_add_library);
	LoadProcAddress(tcc_add_symbol);
	LoadProcAddress(tcc_output_file);
	LoadProcAddress(tcc_run);
	LoadProcAddress(tcc_relocate);
	LoadProcAddress(tcc_get_symbol);
	
	//test_imgui();
	//test_lib();
	return true;
	
    //FreeLibrary(h_dll);
  }else{
  
	#if defined(D_x32)
		err_print("Unable to load libtcc_32.dll");
	#endif
	#if defined(D_x64)
		err_print("Unable to load libtcc_64.dll");
	#endif
	
    //exit(-1);
	return false;
  }
  return 0;
}