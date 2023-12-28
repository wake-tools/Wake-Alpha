#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libWk.h"
#include "Xternal.h"

//// Functions call /////
fptr_itf_dll_test 	 	itf_dll_test =0;
fptr_itf_dll_main 	 	itf_dll_main =0;
HANDLE h_dll=0;

static bool func_error(char* func_name){
	err_print("error loading: %s", func_name);
	return true;
}
//
#define LoadProcAddress(_name, _fname) _name=(fptr_##_name)GetProcAddress(h_dll,_fname);if(_name==0){if(func_error(#_name))return false;}
//
bool dll_load_wk(){
/*
   #if defined(D_x32)
   h_dll = LoadLibraryA("itf/itf.dll");
   info_print("Load itf_32.dll");
   #endif
   #if defined(D_x64)
   h_dll = LoadLibraryA("itf/itf.dll");
   info_print("Load itf_64.dll");
   #endif
*/ 

   info_print("Load itf.dll"); 
   h_dll = LoadLibraryA("itf/itf.dll");

   if(!h_dll){//second try (local dir)
      h_dll = LoadLibraryA("itf.dll");
      GDB_Send_AddSymbolFile("itf.dll");
   }else{
      GDB_Send_AddSymbolFile("itf/itf.dll");
   }
   
   if(h_dll){

  // itf_dll_main=(fptr_itf_dll_main)GetProcAddress(h_dll,"itf_dll_main");
   
      LoadProcAddress(itf_dll_main, "wk_dll_main");

     // LoadProcAddress(itf_dll_test, "wk_test");
      return true;
      //FreeLibrary(h_dll);
   }else{
      err_print("Unable to load itf.dll");
      return false;
   }
  return 0;
}



int testttt( const char* _format, ...){

	_printf(_format);
	return 666;
}

bool dll_run_wk(Nod* _nod){
   if(!itf_dll_main)return false;
   info_print("RUN INTERFACE");
   //printf("result: %d",itf_dll_test(&_printf_));
   //printf("result: %d",itf_dll_test(testttt));
    itf_dll_main(_nod);
    info_print("END");
   return true;
}