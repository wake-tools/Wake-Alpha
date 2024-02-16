

//#define TCC_NOJIT //compile code instead
//#define TCC_IMGUI //add imgui symbole

#include "jit.h"
#include "FileNod.h"



#ifndef TCC_NOJIT
	#include "dll_load.inc"
	#include "tcc.h"
#else
	#include "tcc.h"
#endif

#ifndef TCC_NOJIT

#include <math.h>
#include <time.h>


enum sg_backend;
enum sg_backend sg_query_backend(void);

extern vec2 test_func1(float _time);


void add_std_func(TCCState* s){
   //!ADD some missing func
   tcc_add_symbol(s, "sinf", sinf);
   tcc_add_symbol(s, "expf", expf);
   tcc_add_symbol(s, "logf", logf);
   tcc_add_symbol(s, "cosf", cosf);
   tcc_add_symbol(s, "tanf", tanf);
   tcc_add_symbol(s, "acosf", acosf);
   tcc_add_symbol(s, "atanf", atanf);
   tcc_add_symbol(s, "sqrtf", sqrtf);
   tcc_add_symbol(s, "atan2f", atan2f);
   
   /////////
   tcc_add_symbol(s, "x_malloc", x_malloc);
   tcc_add_symbol(s, "_free", _free);
   tcc_add_symbol(s, "_assert_failed", _assert_failed);
   tcc_add_symbol(s, "_init_mem", _init_mem);
   tcc_add_symbol(s, "_realloc_mem", _realloc_mem);
   tcc_add_symbol(s, "_insert_mem", _insert_mem);
   tcc_add_symbol(s, "_close_mem", _close_mem);
   ////////

   //FILENOD//
   tcc_add_symbol(s, "FileNod_ini", FileNod_ini);
   tcc_add_symbol(s, "FileNod_IsModified", FileNod_IsModified);
   tcc_add_symbol(s, "FileNod_Reload", FileNod_Reload);
   tcc_add_symbol(s, "FileNod_LoadFile", FileNod_LoadFile);
   tcc_add_symbol(s, "FileNod_IsModified_CheckForReload", FileNod_IsModified_CheckForReload);
   tcc_add_symbol(s, "FileNod_Parse_JSON", FileNod_Parse_JSON);
   tcc_add_symbol(s, "FileNod_Parse_PNG", FileNod_Parse_PNG);
   tcc_add_symbol(s, "FileNod_Write", FileNod_Write);
   tcc_add_symbol(s, "FileNod_JitCompile", FileNod_JitCompile);
   tcc_add_symbol(s, "FileNod_Close", FileNod_Close);
   tcc_add_symbol(s, "FileNod_ToJSON", FileNod_ToJSON);
   
   tcc_add_symbol(s, "EMPTY_DATA_", EMPTY_DATA_);
   tcc_add_symbol(s, "_printf", _printf);
   ///////////////
   tcc_add_symbol(s, "path_read_dir", path_read_dir);
   ///////////////
   
    tcc_add_symbol(s, "sg_query_backend", sg_query_backend);  //Required to compile shader//
    //
    tcc_add_symbol(s, "get_module_shader", get_module_shader);
    tcc_add_symbol(s, "get_module_nod", get_module_nod);
    
    tcc_add_symbol(s, "test_func1", test_func1);
    ///////////////
}




// int (*fptr_tcc_add_symbol)(TCCState *s, const char *name, const void *val);

void jit_add_symbol(TccPrg* _prg, const char* name, const void* val){
	tcc_add_symbol(_prg->s, name, val);
}



void error_func(void* _prg, const char* msg) {
	TccPrg* _ = (TccPrg*)_prg;
	if(_->error_f==0){
		err_print("E[1]:TCC Error: %s, %s\n", msg);
	}else{
		_->error_f(_, msg);
	}
	_->valid = false;
}

void* jit_get_symbol(TccPrg* _prg, const char* name){
	void* _symb = tcc_get_symbol(_prg->s, name);
    if (!_symb){
		err_print("TCC Error, request func not found: %s\n", name); //TODO better way to show name
		error_func(_prg, "JIT_Invalid");_prg->valid=false;return 0;
	}
	return _symb;
}


  //extern DWtype __fixsfdi (SFtype) __attribute__((pcs("aapcs")));
  extern long long int __fixsfdi(float);; //float to int (long)
fn TccPrg* Tcc_Compile(std_array_t* _mem, view_t _code, char* _main, void* _main_arg, char** _argv, int _argc,addsymb_fptr _addsymb_f,complete_fptr _complete_f,error_fptr _error_f, File* _file){
	
	info_print("!!========== JIT Compile ==========!!");
	
	TccPrg* _prg = _malloc(1, TccPrg);
	_prg->mem=_mem; _track_mem(_mem, _prg, 0, 0);
	_prg->addsymb_f=_addsymb_f;
	_prg->complete_f=_complete_f;
	_prg->error_f=_error_f;
   _prg->file = _file;
	
	//info_print("_code end: %c", _code.data[_code.size]);
	//_code must end with '\0'
	//info_print("size: %d", _code.size);
	//info_print("bool: %d", _code.data[0] );
	dbg_assert(_code.data[_code.size] == 0, "Please use a valid C string");\
	
	TCCState* s = tcc_new(); _prg->s=s;_prg->valid=true;
	if (!s){err_print("Could not create tcc state"); 
		_prg->valid=false; return _prg;
	}
	
	/*
	tcc_add_include_path(s,".");
	tcc_add_include_path(s,"Rc/tcc/include/");
	tcc_add_include_path(s,"Rc/tcc/lib_inc/");
	tcc_add_include_path(s,"Rc/tcc/jit_inc/");

	tcc_add_library_path(s,"Rc/tcc/lib/");
	*/
	for(int i = 0; i< _argc; i++){
		switch((*_argv)[0]){
			case 'I':
				tcc_add_include_path(s, &(*_argv)[2] );
            //TODO print arg path?
				//info_print("-I %s",  &(*_argv)[2] );
			break;
			case 'L':
				tcc_add_library_path(s, &(*_argv)[2] );
				//info_print("-L %s",  &(*_argv)[2] );
			break;
		}
		_argv++;
	}
	// MUST BE CALLED before any compilation //
	tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
	tcc_set_error_func(s, _prg, error_func); //to capture any warnings and errors.
	
	if (tcc_compile_string(s, _code.data) == -1){
        _prg->valid=false;return _prg;
	}
	
	/*
	//TODO Callback
	// as a test, we add symbols that the compiled program can use.
    // You may also open a dll with tcc_add_dll() and use symbols from that
    tcc_add_symbol(s, "add", adds);
    tcc_add_symbol(s, "hello", hellos);
	//tcc_add_symbol(s, "__fixsfdi", __fixsfdi); //need lib!?
	tcc_add_symbol(s, "fmodf", fmodf);
	tcc_add_symbol(s, "_print_float", _print_float);
	*/
   add_std_func(s);

   
	_prg->addsymb_f(_prg);
	

	
	// relocate the code //
    if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0){
		err_print("JIT_Invalid: TCC_RELOCATE_AUTO"); _prg->valid=false;return _prg;
	}
	/*
	//TODO Entry
	// get entry symbol //
	int (*func)(int);
    func = tcc_get_symbol(s, "jit_main");
    if (!func){	err_print("JIT_Invalid: jit_main");_prg->valid=false;return _prg;}
	
	_prg->main=(void(*)(void))func;
	*/
	if(_prg->valid){
		_prg->complete_f(_prg);
	}else{
		err_print("JIT INCOMPLETE");
	}
	
    // run the code //
 //   func(32);
    //func(32);
//	((int(*)(int))_prg->main)(32);
	 
	//	err_print("Okay!!");
	//	exit(0);
	//    tcc_delete(s);
	return _prg;
}

fn void Tcc_Release(TccPrg* _this){
	 tcc_delete(_this->s);
	 _release_mem_ptr(_this->mem, (void**)&_this);
}

fn void jit_create(){
	dll_load("tcc/libtcc_32.dll");
}

#else
//#include "canvas.c" //todo beter way
void jit_add_symbol(TccPrg* _prg, const char* name, const void* val){};
void* jit_get_symbol(TccPrg* _prg, const char* name){return 0;};
fn TccPrg* Tcc_Compile(std_array_t* _mem, view_t _code, char* _main, void* _main_arg, char** _argv, int _argc, 	addsymb_fptr addsymb_f,complete_fptr complete_f,error_fptr error_f,File* _file){return 0;};
fn void Tcc_Release(TccPrg* _this){};
fn void jit_create(){}
#endif

