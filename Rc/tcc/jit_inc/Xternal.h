/*|| https://wake.tools ||                                          | license | Apache License Version 2.0 |
  | --------------------------------------------------------------------------------------------------+-+++|>
  +-  Xternal.h               
  |---------------------------------------------------------------------------------------------------+-+++|>
  +-  Utility functions basics for common usage       
  |          
  |-----|--------|---------------------|-----------------------------|--------------------------------+-+++|>
*/    
#ifndef HDEF_Xternal
#define HDEF_Xternal
///
///--------------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------------
///==|TYPES|==
///============================================================================================
///--------------------------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __TINYC__ 
#define DYNAMIC_LINK
#endif

#ifdef  USE_ATTRIBUTE_CALL_TYPE
#define  CallType_C	  	__attribute__((cdecl))
#define  CallType_Std 	__attribute__((stdcall))
#else
#define  CallType_C	  	__cdecl
#define  CallType_Std 	__stdcall
#endif

#ifndef fn
	#ifdef __cplusplus
	#define fn     extern "C" CallType_C
	#define fn_std extern "C" CallType_Std
	#else
	#define fn 	   CallType_C
	#define fn_std CallType_Std
	#endif
#endif

#ifdef DYNAMIC_EXPORT
	#define export __declspec(dllexport)
#elif  defined(DYNAMIC_LINK)
	#define export __declspec(dllimport)
#else
	 #define export
#endif

#ifndef inl
#define inl static inline
#endif

#define ASSERT_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#if  defined(_WIN64)
#define SYS_64
#else
#define SYS_32
#endif

#define _min(a, b) ((a < b) ? a : b)
#define _max(a, b) ((a > b) ? a : b)
#define _concat_(a, b) a##b
#define _concat(a, b) _concat_(a, b)

#define _stringize_(A) #A
#define _stringize(A) _stringize_(A)

#ifndef NULL_PTR
	#define NULL_PTR ((void*)0)
#endif
#ifndef NULL
	#define NULL 0
#endif
#ifndef NUL
	#define NUL '\0'
#endif
/*
#ifndef u8
	#define s8
#else
	#define s8 u8
#endif
#define s16 u
#define s32 U
typedef uint8_t  s8_t;
#ifdef __cplusplus
typedef char16_t s16_t;
typedef char32_t s32_t;
#else
typedef uint16_t s16_t;
typedef uint32_t s32_t;
#endif
typedef uint8_t* text_t;
typedef uint8_t  UTF8_t;
typedef uint16_t UTF16_t;
typedef uint32_t UTF32_t;
*/
typedef unsigned int uint;
typedef float  float32_t;
typedef double float64_t;
typedef char   byte_t;

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
#ifndef OUT
#define OUT
#endif

/*
typedef void 		(CallType_C *PtrFunc)(void);
typedef void* 		(CallType_C *PtrFuncRAny)(void);
typedef void* 		(CallType_C *PtrFuncRPAny)(void*);
typedef uint32_t 	(CallType_C *PtrFuncRInt)(void);
typedef bool 		(CallType_C *PtrFuncRBoolPAny)(void*);
*/

typedef struct
std_array_t{
	uint8_t** data;
	size_t size;
	size_t max;
}std_array_t;

//#pragma GCC unroll n
#define Pragma_Unroll_2  _Pragma("unroll 2")
#define Pragma_Unroll_4  _Pragma("unroll 4")
#define Pragma_Unroll_6  _Pragma("unroll 6")
#define Pragma_Unroll_8  _Pragma("unroll 8")
#define Pragma_Unroll_16 _Pragma("unroll 16")
///
///--------------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------------
///==|VIEW|== 
///============================================================================================
///--------------------------------------------------------------------------------------------
#include <string.h> //strlen

typedef struct View {
	byte_t* header;
	size_t  size;
	uint8_t bytes; //Size in bytes (first bit to 1, if writable)
	size_t  start;
}View;


typedef struct view_t {
	byte_t* data;
	size_t  size;
	uint8_t bytes; //Size in bytes (first bit to 1, if writable) ---> (never rewritable)? or UTF8 vs UTF16 stride?
}view_t, mview_t;//mview_t is same as view_t but ensured that is allocated with malloc (can be freed)

//Predefined
static const View _UTF16_t = { .bytes=2 };
static const View _UTF8_t  = { .bytes=1 };

#define _v(_view)(view_t){.size=_view.size,.data=_view.data }
#define _s(_view)_view.size,_view.data
#define D(_str)(view_t){.data=(byte_t*)_str, .size=(strlen(_str)}
#define S(_str)(view_t){.data=(byte_t*)_str, .size=(sizeof(_str)-1)}

#define V(_type, _ptr)(view_t){.size=(sizeof(_type)), .data=(byte_t*)_ptr}
/*
inl view_t view_from_String(view_t from, char c){

}*/
//
inl view_t view_from_lastchar(view_t from, char c){
view_t r = {.data= from.data, .size=from.size};
for(uint i = from.size-1; i!=(uint)-1;i--){
	if(from.data[i]==c){
		i++;//exclude char
		r.data=&r.data[i];
		r.size=r.size-i;
		break;
	}
}
return r;
}
inl view_t view_from_char(view_t from, char c){
view_t r = {.data= from.data, .size=from.size};
for(uint i = 0; i<from.size;i++){
	if(from.data[i]==c){
		i++;//exclude char
		r.data=&r.data[i];
		r.size=i;
		break;
	}
}
return r;
}
inl view_t view_to_char(view_t from, char c){
view_t r = {.data= from.data, .size=from.size};
for(uint i = 0; i<from.size;i++){
	if(from.data[i]==c){
		r.size=i;
		break;
	}
}
return r;
}
inl view_t view_to_lastchar(view_t from, char c){
view_t r = {.data= from.data, .size=from.size};
for(int i = from.size-1; i>=0;i--){
	if(from.data[i]==c){
		r.size=i;
		break;
	}
}
return r;
}
inl view_t view_to_lastchar_inc(view_t from, char c){
view_t r = {.data= from.data, .size=from.size};
for(int i = from.size-1; i>=0;i--){
	if(from.data[i]==c){
		r.size=i+1;
		break;
	}
}
return r;
}
//
inl view_t view_substr(view_t from,size_t start, size_t end){
if(end>from.size)end=from.size;
if(start>end)start=end;
view_t r = {.data=(from.data+start), .size=(end-start)};
return r;
}
//
inl view_t char_view(char* str){
return (view_t){.data=str, .size=strlen(str)};
}
inl view_t char_view_full(char* str){
return (view_t){.data=str, .size=strlen(str)+1};
}
//
inl int view_match(view_t v, char* str){
	int i=0;
	char c;
	while((c = str[i] )!=0 && i<v.size){
		if(c != v.data[i]) break;
		i++;
	}
	int size = strlen(str);
	if(i == size){
		//found
		return  0;
		//return  v.size-size;

	}else{
		//not found
		return i-size;
	}
}
//
/*
#define VLA(_name, _max)                                     		\
	byte_t _name##_vla[_max+ sizeof(fptr_t)];                 	 	\
	view_t _name = {.header=(fptr_t*)_name##_vla,	                \
					.data=(byte_t*)&_name##_vla[sizeof(fptr_t)] ,   \
					.size=_max,                          			\
					};      */

#define Wstr_V(_str) {.header=(View*)&_UTF16_t,.data=(byte_t*)_str,.size=wcslen_(_str)}

#define Vla_V(_name, _max)                                     		\
	byte_t _name##_vla[_max+ sizeof(fptr_t)];                 	 	\
	view_t _name = {.header=(fptr_t*)_name##_vla,	                \
					.data=(byte_t*)&_name##_vla[sizeof(fptr_t)] ,   \
					.size=_max,                          			\
					};           
///
///--------------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------------
///==|IO|==
///============================================================================================
///--------------------------------------------------------------------------------------------
#ifdef CONSOLE_OUTPUT
   #undef USE_ERR_PRINTF
   extern void*  console_main;
   fn void AddToConsole(void* cls, const char* format, ...);
   #ifndef _CR_
   #define _CR_ ""
   #define _DL_ ">"
   #endif
#else
   #ifndef _CR_
   #define _CR_ "\n"
   #define _DL_ ":"
   #endif
#endif

fn void _Sleep(uint32_t _nMillisecond);

fn void _exit_FAILURE();
fn void _exit_SUCCESS();

#ifdef D_Debug
   #define dbg_printf _printf
#else
   #define dbg_printf
#endif

fn int _printf(const char *_format, ...);
//typedef  int (*fptr_printf) (const char* _format, ...);
//export extern fptr_printf _printf;
//#define _printf _printf_;

#ifdef USE_ERR_PRINTF
   fn int err_printf(const char *_format, ...);
   #define err_print(...) err_printf(_CR_"E|" _DL_" " __VA_ARGS__);
#else
   #define err_print(...) _printf(_CR_"E|" _DL_" " __VA_ARGS__);
#endif

#define _print(...)        _printf(_CR_"O" _DL_" " __VA_ARGS__);
#define dbg_print(...)  dbg_printf(_CR_"D" _DL_" "  __VA_ARGS__);
#define warn_print(...)    _printf(_CR_"W" _DL_" " __VA_ARGS__);
#define pass_print(...)    _printf(_CR_"P" _DL_" " __VA_ARGS__);
#define info_print(...)    _printf(_CR_"I" _DL_" " __VA_ARGS__);
#define trace_print(...)   _printf(_CR_"T" _DL_" " __VA_ARGS__);
#define log_print(...)     _printf(_CR_"L" _DL_" " __VA_ARGS__);
#define action_print(...)  _printf(_CR_"A" _DL_" " __VA_ARGS__);
#define _printv(view)      _printf(_CR_"V["#view"]" _DL_" %.*s",view.size,view.data);
/*
#define err_printl(cons,...)     _printf(_CR_"E"_DL_" "cons, __VA_ARGS__);
#define _printl(cons,...)         _printf(_CR_"O"_DL_" "cons, __VA_ARGS__);
#define dbg_printl(cons,...)     dbg_printf(_CR_"D"_DL_" "cons,  __VA_ARGS__);
#define warn_printl(cons,...)    _printf(_CR_"W"_DL_" "cons, __VA_ARGS__);
#define pass_printl(cons,...)    _printf(_CR_"P"_DL_" "cons, __VA_ARGS__);
#define info_printl(cons,...)    _printf(_CR_"I"_DL_" "cons, __VA_ARGS__);
#define trace_printl(cons,...)   _printf(_CR_"T"_DL_" "cons, __VA_ARGS__);
#define log_printl(cons,...)     _printf(_CR_"L"_DL_" "cons, __VA_ARGS__);
#define action_printl(cons,...)  _printf(_CR_"A"_DL_" "cons, __VA_ARGS__);
*/
//TODO
#undef sprintf
#include <stdio.h>
#define _sprintf sprintf
//fn int sprintf(char *str, const char *string,...);
//!
//!---IMPL
#ifdef XTERNAL_IMPL
   ///--------------------------------------------------------------------------------------------
   ///--------------------------------------------------------------------------------------------
   ///==!IO!== IMPL
   ///============================================================================================
   ///--------------------------------------------------------------------------------------------
   #include <stdlib.h>
   #include <stdio.h>
   #include <stdarg.h> //va_list //va_start //va_end
   ///
   #ifdef D_Platform_Windows
      //#include <windows.h>
      fn_std void Sleep(uint32_t _millisecond); //-lKernel32
   #endif

   fn void _Sleep(uint32_t _millisecond){
      #ifdef D_Platform_Windows
         Sleep(_millisecond); //-lKernel32
      #endif
   }

   fn void _exit_FAILURE(){
      //abort();
      exit(EXIT_FAILURE);//GDB: Place a breakpoint here to get the callstack
   }
   fn void _exit_SUCCESS(){
      exit(EXIT_SUCCESS);
   }
   //inline __cdecl  int gzx_printf(const char *format, ...){

	//int vprintf ( const char * format, va_list arg );
   fn int _printf(const char*_format, ...){
       va_list _arg;
       va_start( _arg, _format );

   #ifdef DbgIn_ImGUI

      #define MAX_BUF 4096
      static char buffer[MAX_BUF];
      vsnprintf (buffer,MAX_BUF,_format, _arg);
      AddToConsole(console_main, buffer);
      va_end( _arg );
      return 1;
   #else
		
        int _ret = vprintf(  _format, _arg );
        va_end( _arg );

        fflush(stdout);
        return _ret;
   #endif
   }
 //   fptr_printf _printf = _printf_;
 
   /*
   fn int _printfl(void* console, const char*_format, ...){
       va_list _arg;
       va_start( _arg, _format );

   #ifdef DbgIn_ImGUI

      #define MAX_BUF 4096
      static char buffer[MAX_BUF];
      vsnprintf (buffer,MAX_BUF,_format, _arg);
      AddToConsole(console, buffer);
      va_end( _arg );
      return 1;
   #else
        int _ret = vprintf(  _format, _arg );
        va_end( _arg );

        fflush(stdout);
        return _ret;
   #endif
   }*/
	
   #ifdef USE_ERR_PRINTF
   fn int err_printf(const char*_format, ...){
		va_list _arg;
		va_start( _arg, _format );

		int _ret = vfprintf(stderr, _format, _arg );
		va_end( _arg );
		fflush(stderr);
      return _ret;
   }
   #endif

   #ifdef D_Platform_Android
      #include <android/log.h>
      #define  gzx_android_log_print  __android_log_print
   #endif

#endif//!------
///
///--------------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------------
///==|DEBUG|==
///============================================================================================
///--------------------------------------------------------------------------------------------

//Debug assert
fn void GDB_BreakAssert();
fn void GDB_Send_AddSymbolFile(const char* _path);
fn void GDB_Send_AddSymbolFile_Adr(const char* _path, void* _text_adress);
fn void _assertErr(char* err);
fn void _assert_failed(const char *file, int line, const char* msg);
fn void _registerSignals();
#include <setjmp.h>
#define _setjmp_point() setjmp( jumpsignal );
extern jmp_buf jumpsignal;

#ifdef D_Debug
   #ifndef DBG_NO_FATAL
      #define GZ_Crash() DbgCrash()
   #else
      #define GZ_Crash()
   #endif
      #define dbg_assert(expr, msg) if (!(expr)) _assert_failed(__FILE__, __LINE__,  msg)
      #define dbg_assert_(expr) if (!(expr)) _assert_failed(__FILE__, __LINE__, "assert error")

#else
  #define GZ_Crash()
  #define dbg_assert(expr, msg)
  #define dbg_assert_(expr)
#endif

//!
//!---IMPL
#ifdef XTERNAL_IMPL
   ///--------------------------------------------------------------------------------------------
   ///--------------------------------------------------------------------------------------------
   ///==!DEBUG!== IMPL
   ///============================================================================================
   ///--------------------------------------------------------------------------------------------
   
   int exe_err = 0;
   #include <signal.h>
   jmp_buf jumpsignal;


   fn void stop_assert(){};
   

   //GDB will automaticly break here (with Cwc compiler)
   fn void GDB_Func_Break(){} //raise(SIGTRAP)? void __debugbreak();?
   fn void GDB_Func_ExecuteCmds(){}
   
   

   fn void _assertErr(char* err){
      err_print("Assertion Error: %s",err);
      //show_errors();
      stop_assert();
      GDB_Func_Break();
      abort();
      //exit(EXIT_FAILURE);
   }

   fn void GDB_BreakAssert(){
       fprintf(stderr,"\n");
       fprintf(stderr,"Cmd[GDB]:break stop_assert\n");
      // fprintf(stderr,"Cmd[GDB]:break _wassert\n");
       fflush(stderr);
       GDB_Func_ExecuteCmds();
   }
   
   fn void _assert_failed(const char *file, int line, const char* msg){
      err_print("Assertion fail: %s:%d ( %s )", file, line, msg);
      #ifndef DBG_NO_FATAL
          stop_assert();
         _exit_FAILURE();
      #endif
   }
   
   fn void GDB_Send_AddSymbolFile_Adr(const char* _path, void* _text_adress){
      //err_printl("Cmd[GDB]:add-symbol-file \"%s\" 0x%p\n", _path, _text_adress);
      fprintf(stderr,"\n");
      fprintf(stderr,"Cmd[GDB]:add-symbol-file \"%s\" 0x%p\n", _path, _text_adress);
      fflush(stderr);
      GDB_Func_ExecuteCmds();
   }
   fn void GDB_Send_AddSymbolFile(const char* _path){
      //err_printl("Cmd[GDB]:add-symbol-file \"%s\" 0x%p\n", _path, _text_adress);
      fprintf(stderr,"\n");
      fprintf(stderr,"Cmd[GDB]:symbol-file \"%s\" 0x%p\n", _path);
      fflush(stderr);
      GDB_Func_ExecuteCmds();
   }


   fn void signalHandler(int signum) {
      _printf("\n - Interrupt signal received: ");
      // cleanup and close up stuff here
      switch (signum) {
      case SIGTERM:
         err_print("SIGTERM, termination request, sent to the program ");
         break;
      case SIGSEGV:
         err_print("SIGSEGV, invalid memory access (segmentation fault) ");
         break;
      case SIGINT:
         err_print("SIGINT, external interrupt, usually initiated by the user ");
         break;
      case SIGILL:
         err_print("SIGILL, invalid program image, such as invalid instruction ");
         break;
      case SIGABRT:
         err_print("SIGABRT, abnormal termination condition, as is e.g. initiated by std::abort()");
         break;
      case SIGFPE:
         err_print("SIGFPE, erroneous arithmetic operation such as divide by zero");
         break;
      default:
         err_print("UNKNOW");
         break;
      }
      exe_err = signum;
      longjmp(jumpsignal, 1);
      //exit(signum);
   }

   fn void _registerSignals() {

      //set breakpoint on assert
      // GDB_BreakAssert();

      /* //No sigaction on Windows !?
      int *foo = NULL;
         struct sigaction sa;
         memset(&sa, 0, sizeof(struct sigaction));
         sigemptyset(&sa.sa_mask);
         sa.sa_sigaction = segfault_sigaction;
         sa.sa_flags   = SA_SIGINFO;
         sigaction(SIGSEGV, NULL, NULL);
      */
      for (int i = 1; i < 32; i++) {
         signal(i, signalHandler);
      }
   }
   

#endif//!------
///
///--------------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------------
///==|MEMORY|==
///============================================================================================
///--------------------------------------------------------------------------------------------
fn void* xx_malloc   (size_t size);
fn void*  x_malloc   (size_t num, size_t size);
fn void*  x_calloc   (size_t num, size_t size);
fn void*  x_mallocpy (void* src, size_t num, size_t size);
fn void*   _realloc  (void* ptr, size_t size);
fn void    _free     (void* ptr);

#define _malloc(num, type) (type*)x_malloc(num, sizeof(type))
#define _calloc(num, type) (type*)x_calloc(num, sizeof(type))

#ifdef D_NO_INTRINSIC
   GzFunc void* x_memmove ( void* destination, const void * source, size_t num );
   GzFunc void* x_memset ( void* ptr, int value, size_t num );                   //intrinsic
   GzFunc void* x_memcpy ( void* destination, const void * source, size_t num );     //intrinsic
   #define _memmove x_memmove
   #define _memset  x_memset
   #define _memcpy  x_memcpy
#else
   //#include <stdlib.h>
   #include <string.h>
   #define _memmove memmove
   #define _memset  memset
   #define _memcpy  memcpy
#endif

#define _mallocpy(src, num, type) (type*)x_mallocpy((type*)src, num, sizeof(type))

fn std_array_t _init_mem(size_t size);
fn bool _close_mem(std_array_t* mem_array);
fn int _end(std_array_t* mem_array);

fn size_t _track_mem(std_array_t* base, void* mem_ptr_new, void* mem_prt_last, size_t mem_idx_last);
fn void* _realloc_mem(void* data, size_t new_size, size_t copy_size, std_array_t* base, void* mem_prt_last, size_t* mem_idx_last);
fn void _insert_mem(uint8_t** data, size_t* size, size_t* limit, int typesize, size_t idx, int trail, std_array_t* base, void* mem_prt_last, size_t* mem_idx_last);

fn void _release_mem_ptr(std_array_t* pool, void** mem_prt);
fn void _release_mem_idx(std_array_t* pool, void* mem_prt, size_t mem_idx);
//!
//!---IMPL
#ifdef XTERNAL_IMPL
   ///--------------------------------------------------------------------------------------------
   ///--------------------------------------------------------------------------------------------
   ///==!MEMORY!== IMPL
   ///============================================================================================
   ///--------------------------------------------------------------------------------------------
   #include <stdlib.h>
   #include <string.h>

   #ifdef D_MemoryCheck
   uint nFPtr_Instance_Counted = 0;
   static uint nAlloc_Count = 0;
   static uint nAllocated = 0;
   static uint nFreed = 0;
   static void Alloc_Add(){
      nAlloc_Count++;
      nAllocated++;
   }
   static  void Alloc_Sub(){
      nAlloc_Count--;
      nFreed++;
   }
   #endif

   fn void* xx_malloc(size_t size){
      #ifdef D_MemoryCheck
      Alloc_Add();
      #endif

      void* ret = malloc(size);

      #ifdef D_Debug
      if(!ret) {
         err_print("Unable to malloc: size[%d]", size);
      }
      #endif

      return ret;
   };
   fn void* x_malloc(size_t num, size_t size){
      return xx_malloc(num * size);
   };
   fn void* x_calloc(size_t num, size_t size){
      #ifdef D_MemoryCheck
      Alloc_Add();
      #endif

      #ifndef D_NO_Calloc
         void* ret = calloc(num, size);
      #else

      void* ret = malloc(size*num);
      memset(ret, 0, size*num);

      #endif

      #ifdef D_Debug
      if(!ret) {
         err_print("Unable to calloc: num[%d] size[%d]", num, size);
      }
      #endif

      return ret;
   };
   fn void* x_mallocpy(void* src, size_t num, size_t size){
      #ifdef D_MemoryCheck
      Alloc_Add();
      #endif
      void* ptr =  malloc(num * size);
      return _memcpy(ptr,src, num * size);
   };
   fn void  _free(void* ptr){
      #ifdef D_MemoryCheck
      if(ptr!=0){
      Alloc_Sub();
      }
      #endif
      #ifndef D_TestIf_UsingFreedMemory
      free(ptr);
      #endif
   };
   fn void* _realloc(void* ptr, size_t size){
      #ifdef D_MemoryCheck
      if(ptr == 0){Alloc_Add();} //ptr==0 => malloc
      #endif
      return realloc(ptr, size);
   };

   #ifdef D_NO_INTRINSIC
   fn void* x_memset (void* ptr, int value, size_t num ){return memset(ptr,value,num);};
   fn void* x_memmove(void* destination, const void * source, size_t num ){return memmove(destination,source,num);};
   fn void* x_memcpy (void* destination, const void * source, size_t num ){return memcpy(destination,source,num);};
   #endif


   fn std_array_t _init_mem(size_t ini_size){
      std_array_t pool={.max=ini_size};
      pool.max = ini_size;
      pool.data = _malloc(ini_size,uint8_t*);
      return pool;
   }

   fn void _release_mem_idx(std_array_t* pool, void* mem_prt, size_t mem_idx){

      if(mem_prt != 0 && mem_idx<pool->size && pool->data[mem_idx]==mem_prt){
         _free(pool->data[mem_idx]);
         pool->data[mem_idx]=0;
      }
   }

   fn void _release_mem_ptr(std_array_t* pool, void** mem_prt){
      if(*mem_prt==0)return;

      for(int i=0; i<pool->size;i++){
         if( pool->data[i]==*mem_prt){
            //info_print("FREE: %p", pool->data[i]);
            _free(pool->data[i]);
            pool->data[i]=0;
            *mem_prt=0;
            return;
         }
      }
   }

   fn size_t _track_mem(std_array_t* pool, void* mem_ptr_new, void*    mem_prt_last, size_t mem_idx_last){
      //size_t idx = pool->size; pool->size++;

      if(pool->size>=pool->max){
         //realloc?
         pool->max=(pool->max+1)*2;
         uint8_t** newdata = _malloc(pool->max,uint8_t*);
         _memcpy(newdata, pool->data, (pool->size)*sizeof(uint8_t*));
         _free(pool->data);
         pool->data=newdata;
      }

      //free last one if already assigned and not already freed
      if(mem_prt_last != 0 && mem_idx_last<pool->size && pool->data[mem_idx_last]==mem_prt_last){
         _free(pool->data[mem_idx_last]);
         pool->data[mem_idx_last]=0;
      }
      //////////////
      pool->data[pool->size]=mem_ptr_new;
      pool->size++;
      return pool->size-1;
   }

   fn void* _realloc_mem(void* data, size_t new_size, size_t copy_size, std_array_t* base, void* mem_prt_last, size_t* mem_idx_last){
         void* newdata=xx_malloc(new_size);
         _memcpy(newdata, data, copy_size);
         *mem_idx_last = _track_mem(base,newdata,mem_prt_last,*mem_idx_last);
         return newdata;
   }

   fn void _insert_mem(uint8_t** data, size_t* size, size_t* limit, int typesize, size_t idx, int trail, std_array_t* base, void* mem_prt_last, size_t* mem_idx_last){

      if(idx>(*size)){(*size)=idx;}
      if((*size+trail) >= *limit){
         *limit = (*size+trail)*2;
         *data=_realloc_mem(*data,(*limit)*typesize,(*size)*typesize,base,mem_prt_last,mem_idx_last);
      }
      _memmove(&(*data)[(idx+trail+1)*typesize], &(*data)[idx*typesize], ((*size)-idx)*typesize);
      _memset(&(*data)[(idx+1)*typesize],0,trail*typesize);
      (*size)+=trail+1;
   }

   fn bool _close_mem(std_array_t* pool){
      for(size_t i =0; i<pool->size;i++){
         //if(pool->data[i]!=0){
            _free(pool->data[i]);
         //}
      }
      pool->size=0;
      pool->max=0;
      _free(pool->data);
      return true;
   }

   fn void GDB_Func_Break();
   fn int _end(std_array_t* pool){
      if(pool!=0){
         _close_mem(pool);
      }
      int ret = 0;
      #ifdef D_MemoryCheck
      info_print(" -- ====== MEM CHECK ======= --");
      if(nAlloc_Count == 0){
         pass_print(" -- No Memory Leak -- [Allocated: %d]", nAllocated);
      }else{
         warn_print("Has Memory leak! Leaked: %d [Allocated: %d, nFreed: %d]", nAlloc_Count, nAllocated, nFreed);
         ret = 1;
         GDB_Func_Break();
      }
      info_print(" -- Total FPtr instance counted -- [Counted: %d]", nFPtr_Instance_Counted);
      #endif
      return ret;
   }
#endif//!------
///
///--------------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------------
///==|ARRAY|==
///============================================================================================
///--------------------------------------------------------------------------------------------
export extern const char EMPTY_DATA_[128];

#define EMPTY_STR (char*)&EMPTY_DATA_

#ifndef DEFAULT_arraystack_size
#define DEFAULT_arraystack_size 32
#endif

#define ARRAY_fixed(name_, type_, maxsize_)\
\
typedef struct name_{\
   type_ data[maxsize_];\
   int size;\
}name_;\
\
inl type_* name_##_add(name_* _, type_ _new){\
   if(_->size < maxsize_){\
      _->data[_->size] = _new;\
      _->size++;\
      return &_->data[_->size-1];\
   }\
   dbg_print(#name_ " out of bound: %d, max:", _->size, maxsize_);\
   return 0;\
}\
\
inl bool name_##_sub(name_* _){\
   if(_->size > 0){\
      _->size--;\
      return &_->data[_->size];\
   }\
   return 0;\
}\
\
inl bool name_##_contain(name_* _, type_* _match){\
   for(int i =0; i < _->size; i++){\
      if(_match == &_->data[i]){\
         return true;\
      }\
   }\
   return false;\
}\
\
inl bool name_##_reset(name_* _){\
   _->size = 0;\
   return true;\
}\


#define ARRAY ARRAY_grow
#define ARRAY_grow(name_, type_, maxsize_)\
\
typedef struct name_{\
   type_* data;\
   size_t size;\
   size_t max;\
   \
   std_array_t* mem;\
   size_t last_mem_idx;\
   type_ data_stack[maxsize_];\
}name_;\
\
inl void name_##_ini(name_* _, std_array_t* mem){\
   _->data = (type_*)&_->data_stack;\
   _->max = maxsize_;\
   _->mem=mem;\
   _->size=0;\
}\
\
inl type_* name_##_add(name_* _, type_ _new){\
   dbg_assert(_->data != 0, "Please call ini before usage");\
   if(_->size >= _->max){\
      _->max = (_->max+1)*2;\
      _->data=(type_*)_realloc_mem(_->data, _->max*sizeof(type_),_->size*sizeof(type_), _->mem,_->data, &_->last_mem_idx);\
   }\
   _->data[_->size] = _new;\
   _->size++;\
   return &_->data[_->size-1];\
}\
inl type_* name_##_put(name_* _, type_ _new){/*put without resizing, ie: null ending*/\
   dbg_assert(_->data != 0, "Please call ini before usage");\
   if(_->size >= _->max){\
      _->max = (_->max+1)*2;\
      _->data=(type_*)_realloc_mem(_->data, _->max*sizeof(type_),_->size*sizeof(type_), _->mem,_->data, &_->last_mem_idx);\
   }\
   _->data[_->size] = _new;\
   return &_->data[_->size];\
}\
inl type_* name_##_push(name_* _, view_t _new){\
   dbg_assert(_->data != 0, "Please call ini before usage");\
   if(_->size+_new.size >= _->max){\
      _->max = (_->size+_new.size)*2;\
      _->data=(type_*)_realloc_mem(_->data, _->max*sizeof(type_),_->size*sizeof(type_), _->mem,_->data, &_->last_mem_idx);\
   }\
   _memcpy((void*)&_->data[_->size], _new.data, _new.size); \
   _->size+=_new.size;\
   return &_->data[_->size-_new.size];\
}\
inl type_* name_##_edit_grow(name_* _, view_t _new, size_t _to){\
   dbg_assert(_->data != 0, "Please call ini before usage");\
   if(_to+_new.size>_->size){\
      if(_to+_new.size >= _->max){\
         _->max = (_to+_new.size)*2;\
         _->data=(type_*)_realloc_mem(_->data, _->max*sizeof(type_),_->size*sizeof(type_), _->mem,_->data, &_->last_mem_idx);\
      }\
      _->size=_to+_new.size;\
   }\
   _memcpy((void*)&_->data[_to], _new.data, _new.size); \
   return &_->data[_to];\
}\
inl type_* name_##_edit(name_* _, view_t _new, size_t _to){\
   dbg_assert(_->data != 0, "Please call ini before usage");\
   if(_to+_new.size>_->size){\
      dbg_assert(_->data != 0, "Source larger than destination. Use edit grow instead");\
   }\
   _memcpy((void*)&_->data[_to], _new.data, _new.size); \
   return &_->data[_to];\
}\
\
inl bool name_##_sub(name_* _){\
   if(_->size > 0){\
      _->size--;\
      return &_->data[_->size];\
   }\
   return 0;\
}\
\
inl bool name_##_contain(name_* _, type_* _match){\
   for(size_t i =0; i < _->size; i++){\
      if(_match == &_->data[i]){\
         return true;\
      }\
   }\
   return false;\
}\
inl type_* name_##_insert_trail(name_* _, size_t idx, int trail, type_ _val){\
   dbg_assert(_->data != 0, "Please call ini before usage");\
   _insert_mem((uint8_t**)&_->data, &_->size, &_->max,sizeof(type_), idx, trail, _->mem,_->data, &_->last_mem_idx);\
   _->data[idx] = _val;\
   return &_->data[idx];\
}\
inl type_* name_##_insert(name_* _, size_t idx, type_ _val){\
   dbg_assert(_->data != 0, "Please call ini before usage");\
   _insert_mem((uint8_t**)&_->data, &_->size, &_->max,sizeof(type_), idx, 0, _->mem,_->data, &_->last_mem_idx);\
   _->data[idx] = _val;\
   return &_->data[idx];\
}\
inl type_* name_##_remove_swap(name_* _, size_t idx){\
   dbg_assert(_->data != 0, "Please call ini before usage");\
   _->size--;\
   _->data[idx] = _->data[_->size];\
   return &_->data[idx];\
}\
\
inl type_ name_##_get(name_* _, size_t idx){\
   return _->data[idx];\
}\
\
inl type_* name_##_get_ptr(name_* _, size_t idx){\
   return &_->data[idx];\
}\
inl bool name_##_reset(name_* _){\
   if(_->size >= maxsize_){_release_mem_idx(_->mem,_->data, _->last_mem_idx);} /*if heap alloc, release mem - to validate -*/ \
   name_##_ini(_, _->mem);\
   return true;\
}
/*\ Why?
inl bool name_##_reset(name_* _){\
   _->last_mem_idx = _track_mem(_->mem,0,_->data, _->last_mem_idx);\
   name_##_ini(_, _->mem);\
   if(_->size != maxsize_){_free(_->data);}\
   return true;\
}\*/

///////// Range ///////
typedef struct Range_t {
   uint32_t  begin;
   uint32_t  end;
} Range_t;
ARRAY_grow(RangeArray, Range_t, DEFAULT_arraystack_size); //Default string stack, is 25 enough?

///////// IDX ///////
typedef struct Idx_t { //FIXME use a array with only index to locate the value structure (faster insertion / iteration)
   // Key element //
   uint32_t  kidx;
   // Value data //
   Range_t  vidx;
   int64_t xctm; // custom value, 64 bit var?
   void* xptr; // custom ptr -> enable recursivity
   char flags;   // 1 = str value modified, 10= custom value modified
   ////////////////
} Idx_t;

//#define IdxArray_new(name, memory) IdxArray name={};IdxArray_ini(&name,memory);
ARRAY_grow(IdxArray, Idx_t, DEFAULT_arraystack_size); //Default string stack, is 25 enough?

/*
///////// PTR ///////
#define PtrArray_new(name, memory) PtrArray name={};PtrArray_ini(&name,memory);
ARRAY_grow(PtrArray, void*, 25); //Default string stack, is 25 enough?
*/
///////// STRING ///////
#define String_new(name, memory) String name={};String_ini(&name,memory);
ARRAY_grow(String, byte_t, DEFAULT_arraystack_size); //Default string stack, is 25 enough?
/*
inl void String_push(String* _, char* data){
   size_t i = 0;
   char c = 1;
   while(c != 0){
      c = data[i];
      String_add(_, c);
      i++;
   }
}
*/
///////// DICTIONARY /////////
typedef struct Dictionary {
   String str;
   IdxArray  list;
   String val;
   RangeArray empty_zone;
   bool modified; //default false
} Dictionary;

#define Dictionary_new(name, memory) Dictionary name={};Dictionary_ini(&name,memory);
inl void Dictionary_ini(Dictionary* _, std_array_t* base){
   String_ini(&_->str, base);
   String_ini(&_->val, base);
   _->str.size=1;//reserve idx 0 (no id found)
   _->val.size=1;//reserve first char nul terminated (C string compatible) when not set
   IdxArray_ini(&_->list, base);
   RangeArray_ini(&_->empty_zone, base);
}

#define ELEM_FLAG_STR  0
#define ELEM_FLAG_INT  1
#define ELEM_FLAG_DEC  2
#define ELEM_FLAG_BOOL 3
#define ELEM_FLAG_NULL 4
#define ELEM_FLAG_PTR  5

#define ELEM_FLAG_BIT_MODIFIED (1<<3)


#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

#define ELEM_FLAG_DEC_EXP  10000000000 //10^10
#define ELEM_FLAG_DEC_EXP_SIZE  (int)(sizeof(STR(ELEM_FLAG_DEC_EXP))-2) //10^10
#define ELEM_FLAG_DEC_EXP_SIZE_LEFT  (19-(int)(sizeof(STR(ELEM_FLAG_DEC_EXP))-2)) //10^10
#define ELEM_FLAG_DEC_MAX  1000000000000000000 //19 total - 10 = 10^9


inl void Dictionary_modified_str(Dictionary* _, Idx_t* _elem){
   _->modified=true;_elem->flags |= ELEM_FLAG_BIT_MODIFIED;
   _elem->flags = ELEM_FLAG_STR;
}
//
inl void Dictionary_modified_dec(Dictionary* _, Idx_t* _elem, int64_t set_to){
   _->modified=true;_elem->flags |= ELEM_FLAG_BIT_MODIFIED;
   _elem->flags = ELEM_FLAG_DEC;
   _elem->xctm=set_to;
}
inl int64_t Dictionary_elem_get_dec(Idx_t* _elem){
   return (int64_t)_elem->xctm;
}
inl intmax_t Dictionary_elem_get_dec_base(Idx_t* _elem){
   return (int64_t)_elem->xctm/ELEM_FLAG_DEC_EXP;//(10^ELEM_FLAG_DEC_EXP);
}
inl intmax_t Dictionary_elem_get_dec_fract(Idx_t* _elem){
   intmax_t val = (int64_t)_elem->xctm-((Dictionary_elem_get_dec_base(_elem))*ELEM_FLAG_DEC_EXP);
   if(val>=0)return val;
   return val*-1;
}
//
inl void Dictionary_modified_int(Dictionary* _, Idx_t* _elem, int64_t set_to){
   _->modified=true;_elem->flags |= ELEM_FLAG_BIT_MODIFIED;
   _elem->flags = ELEM_FLAG_INT;
   _elem->xctm=set_to;
}
inl int64_t Dictionary_elem_get_int(Idx_t* _elem){
   return (int64_t)_elem->xctm;
}
//
inl void Dictionary_modified_ptr(Dictionary* _, Idx_t* _elem, void *set_to){
   _->modified=true;_elem->flags |= ELEM_FLAG_BIT_MODIFIED;
   _elem->flags = ELEM_FLAG_PTR;
   _elem->xptr=(void*)set_to;//free last ptr (let user manage it)?
}
inl void* Dictionary_elem_get_ptr(Idx_t* _elm){
   return (void*)_elm->xptr;
}
///
inl void Dictionary_modified_bool(Dictionary* _, Idx_t* _elem, bool set_to){
   _->modified=true;_elem->flags |= ELEM_FLAG_BIT_MODIFIED;
   _elem->flags = ELEM_FLAG_BOOL;
   _elem->xctm=(bool)set_to;
}
inl bool Dictionary_elem_get_bool(Idx_t* _elem){
   return (bool)_elem->xctm;
}
///
inl void Dictionary_modified_null(Dictionary* _, Idx_t* _elem){
   _->modified=true;_elem->flags |= ELEM_FLAG_BIT_MODIFIED;
   _elem->flags = ELEM_FLAG_NULL;
   _elem->xctm=0;
}
inl bool Dictionary_elem_get_null(Idx_t* _elem){
   if(_elem->flags==ELEM_FLAG_NULL){
      return true;
   }else{
      return false;
   }
}
///
////////
inl bool Dictionary_elem_is_ptr(Idx_t* _elem){
   //TODO use type because it can be prt & value when we get nod struct (for simplicity)

   //if((_elem->flags & (1<<2)) == (1<<2)){
   if(_elem->xptr!=0){
      return true;
   }
   return false;
}
////////

inl void Dictionary_print_empty_zone(Dictionary* _){
   for(size_t i=0;i<_->empty_zone.size;i++){
      info_print("zone: [%d, %d]", _->empty_zone.data[i].begin, _->empty_zone.data[i].end );
   }
}
inl int compare_char(char l, char r){
   return r-l;
}
/*
inl int compare(char* left, char* right){
   int j=0; char l; char r;
   do{
      l = left[j];
      r = right[j];
      if(l!=r){return compare_char(l,r);} //Basic Ascii sort
      j++;
   }while(l != 0 && r!=0);
   if(l==0&&r==0){
      return 0;
   }
   return compare_char(l,r);
}*/
/*
inl int compareVR(char* left, view_t right){ //TODO bug if view_t size = 0
   int j=0; char l; char r;
   do{
      l = left[j];
      r = right.data[j];
      if(l!=r){return compare_char(l,r);} //Basic Ascii sort
      j++;
   }while(l!=0 && j<right.size);
   if(l==0&&j==right.size){
      return 0;
   }
   return compare_char(l,r);
}

inl int compareVL(view_t left, char* right){ //TODO bug if view_t size = 0
   int j=0; char l; char r;
   do{
      l = left.data[j];
      r = right[j];
      if(l!=r){return compare_char(l,r);} //Basic Ascii sort
      j++;
   }while(r!=0 && j<left.size);
   if(j==left.size&&r==0){
      return 0;
   }
   return compare_char(l,r);
}*/

inl int compareV(view_t left, view_t right){
   char l; char r;
   if(right.size > left.size){
      for(int i=0;i<left.size;i++){
         l = left.data[i];
         r = right.data[i];
         if(l!=r){return compare_char(l,r);} //Basic Ascii sort
      }
      if(right.size==left.size){return 0;}
      return compare_char(0,right.data[left.size]);
   }else{
      for(int i=0;i<right.size;i++){
         l = left.data[i];
         r = right.data[i];
         if(l!=r){return compare_char(l,r);} //Basic Ascii sort
      }
      if(right.size==left.size){return 0;}
      return compare_char(left.data[right.size],0);
   }
}

inl int32_t Dictionary_find_idx(Dictionary* _, view_t find){
/* //Linear search
   for(size_t i=0;i<_->list.size;i++){
      char* elem = &_->str.data[ _->list.data[i] ];
      if(compare(elem, find) ){
         return _->list.data[i];
      }
   }
*/
   uint32_t min = 0;
   uint32_t max = _->list.size;

   uint32_t mdl_idx = min+(max-min)/2;
   do{
      char* elem = &_->str.data[ _->list.data[mdl_idx].kidx ];
      //int v = compare(elem, find);
      //int v = compareVR(elem, find);

      view_t e = (view_t){.data=elem, .size=strlen(elem)};
      int v = compareV(e, find);

      if(v == 0){//found
         return mdl_idx;
      }else if (v < 0) {
         max = mdl_idx;
      }else{
         min = mdl_idx;
      }
      uint32_t last_idx=mdl_idx;
      mdl_idx = min+(max-min)/2;
      if(last_idx == mdl_idx){
         return -(mdl_idx+1); //use reserved idx 0 when not found
      }
   }while(true);
   return -(mdl_idx+1); //use reserved idx 0 when not found
}

inl uint32_t Dictionary_find(Dictionary* _, view_t find){
   int32_t idx = Dictionary_find_idx(_,find);
   if(idx < 0){return 0;}
   return _->list.data[idx].kidx;
}

inl int32_t Find_space(RangeArray* zones, uint32_t req_size){
   for(uint32_t i=0; i< zones->size; i++){
      Range_t range = zones->data[i];
      if(req_size < range.end-range.begin){
         return i;
      }
   }
   return -1;
}

#undef near //#windows.h fix (already defined)
inl int32_t Dictionary_add(Dictionary* _, view_t str){

   int32_t idx = Dictionary_find_idx(_, str);
   if(idx < 0){
      uint32_t near = -(idx+1);
      //if(!(near+1>_->list.size) && compareVL(str,&_->str.data[_->list.data[near].kidx] )<0){
      char* own = &_->str.data[_->list.data[near].kidx];
      view_t vown = (view_t){.data=own,.size=strlen(own)};
      if(!(near+1>_->list.size) && compareV(str, vown)<0){
         near++;
      }
      IdxArray_insert(&_->list, near,  (Idx_t){.kidx=(uint32_t)_->str.size} ); //uint32_t or size_t?
      ////////////////////////
      //String_push(&_->str, char_view_full(str));
      String_push(&_->str, str);String_add(&_->str,'\0');
      return near;
   }else{ //Already exist
      return 0;
   }
}

inl bool Dictionary_set(Dictionary* _, Idx_t* _elm, view_t set_to){

   if(_elm->vidx.end == 0){ //Never set
      // Push only if no empty zone found //
      _elm->vidx.begin = _->val.size;
      //String_push(&_->val, set_to);
      String_push(&_->val, set_to);String_add(&_->val,'\0'); //TODO validate
      _elm->vidx.end   = _->val.size;
      /////////////////////////////////////
   }else{
      //Reassing?
      uint32_t available = _elm->vidx.end  - _elm->vidx.begin;
      //if(set_to.size < available ){ //jsute rewrite to it current space
      if(set_to.size+1 < available ){  //TODO validate +1 //jsute rewrite to it current space

         //String_edit(&_->val, set_to, _elm->vidx.begin);
         String_edit(&_->val, set_to, _elm->vidx.begin);_->val.data[_elm->vidx.begin+set_to.size]='\0';//TODO validate null ending
         //_elm->vidx.end  =  _elm->vidx.begin + set_to.size;
         _elm->vidx.end  =  _elm->vidx.begin + set_to.size+1;//TODO validate +1

      }else{
         //find if fit on another space
         //int32_t sidx = Find_space(&_->empty_zone, set_to.size);
         int32_t sidx = Find_space(&_->empty_zone, set_to.size+1); //TODO validate +1
         if(sidx==-1){//no space avalaible, push to end
            //add empty zone
            //_print("add empty zone: %s", set_to);
            RangeArray_add( &_->empty_zone,  _elm->vidx );
            ////////////////
            _elm->vidx.begin = _->val.size;
            //String_push(&_->val, set_to);
            String_push(&_->val, set_to);String_add(&_->val,'\0'); //TODO validate
            _elm->vidx.end   = _->val.size;

         }else{
            //Found space
            //_print("Found space: %s", set_to);
            _elm->vidx = _->empty_zone.data[sidx];
            //String_edit(&_->val, set_to, _elm->vidx.begin);
            String_edit(&_->val, set_to, _elm->vidx.begin);_->val.data[_elm->vidx.begin+set_to.size]='\0';//TODO validate null ending
            //_elm->vidx.end  =  _elm->vidx.begin + set_to.size;
            _elm->vidx.end  =  _elm->vidx.begin + set_to.size+1;//TODO validate +1

            //remove from list
            RangeArray_remove_swap(&_->empty_zone, sidx);
            ///
         }
      }
   }
   Dictionary_modified_str(_, _elm);
   return true;
}

inl bool Dictionary_let_str(Dictionary* _, view_t find, view_t set_to){
   int32_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);;Idx_t* _elm = &_->list.data[lidx];
   Dictionary_set(_, _elm, set_to);}
   return true;
}
inl bool Dictionary_set_str(Dictionary* _, view_t find, view_t set_to){
   int32_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);};Idx_t* _elm = &_->list.data[lidx];
   Dictionary_set(_, _elm, set_to);
   return true;
}
//////
inl bool Dictionary_let_str_char(Dictionary* _, view_t find, char* set_to){
   view_t v = char_view(set_to);
   int32_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);;Idx_t* _elm = &_->list.data[lidx];
   Dictionary_set(_, _elm, v);}
   return true;
}
inl bool Dictionary_set_str_char(Dictionary* _, view_t find, char* set_to){
   view_t v = char_view(set_to);
   int32_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);};Idx_t* _elm = &_->list.data[lidx];
   Dictionary_set(_, _elm, v);
   return true;
}
//////
inl bool Dictionary_let_dec(Dictionary* _, view_t find, int64_t set_to){
   int64_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);;Idx_t* _elm = &_->list.data[lidx];
   Dictionary_modified_dec(_, _elm, set_to);}
   return true;
}
inl bool Dictionary_set_dec(Dictionary* _, view_t find, int64_t set_to){
   int64_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);};Idx_t* _elm = &_->list.data[lidx];
   Dictionary_modified_dec(_, _elm, set_to);
   return true;
}
inl int64_t Dictionary_get_dec(Dictionary* _, view_t find){
   int32_t idx = Dictionary_find_idx(_,find);if(idx < 0){return 0;}Idx_t* _elm = &_->list.data[idx];
   return Dictionary_elem_get_dec(_elm);
}
//////
inl bool Dictionary_let_int(Dictionary* _, view_t find, int64_t set_to){
   int64_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);;Idx_t* _elm = &_->list.data[lidx];
   Dictionary_modified_int(_, _elm, set_to);}
   return true;
}
inl bool Dictionary_set_int(Dictionary* _, view_t find, int64_t set_to){
   int64_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);};Idx_t* _elm = &_->list.data[lidx];
   Dictionary_modified_int(_, _elm, set_to);
   return true;
}
inl int64_t Dictionary_get_int(Dictionary* _, view_t find){
   int32_t idx = Dictionary_find_idx(_,find);if(idx < 0){return -1;}Idx_t* _elm = &_->list.data[idx];
   return Dictionary_elem_get_int(_elm);
}
//////
inl bool Dictionary_let_bool(Dictionary* _, view_t find, bool set_to){
   int32_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);;Idx_t* _elm = &_->list.data[lidx];
   Dictionary_modified_bool(_, _elm, set_to);}
   return true;
}
inl bool Dictionary_set_bool(Dictionary* _, view_t find, bool set_to){
   int32_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);};Idx_t* _elm = &_->list.data[lidx];
   Dictionary_modified_bool(_, _elm, set_to);
   return true;
}
inl bool Dictionary_get_bool(Dictionary* _, view_t find){
   int32_t idx = Dictionary_find_idx(_,find);if(idx < 0){return 0;}Idx_t* _elm = &_->list.data[idx];
   return Dictionary_elem_get_bool(_elm);
}
//////
inl bool Dictionary_set_null(Dictionary* _, view_t find){
   int64_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);};Idx_t* _elm = &_->list.data[lidx];
   Dictionary_modified_null(_, _elm);
   return true;
}
inl bool Dictionary_get_null(Dictionary* _, view_t find){
   int32_t idx = Dictionary_find_idx(_,find);if(idx < 0){return 0;}Idx_t* _elm = &_->list.data[idx];
   return Dictionary_elem_get_null(_elm);
}
//////
inl bool Dictionary_let_ptr(Dictionary* _, view_t find, void* set_to){
   int32_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);;Idx_t* _elm = &_->list.data[lidx];
   Dictionary_modified_ptr(_, _elm, set_to);}
   return true;
}
inl bool Dictionary_set_ptr(Dictionary* _, view_t find, void* set_to){
   int32_t lidx = Dictionary_find_idx(_,find);if(lidx < 0){lidx=Dictionary_add(_, find);};Idx_t* _elm = &_->list.data[lidx];
   Dictionary_modified_ptr(_, _elm, set_to);
   return true;
}
inl void* Dictionary_get_ptr(Dictionary* _, view_t find){
   int32_t idx = Dictionary_find_idx(_,find);if(idx < 0){return 0;}
   Idx_t* _elm = &_->list.data[idx];
   return Dictionary_elem_get_ptr(_elm);
}
//////
//!
inl Idx_t* Dictionary_get_elem(Dictionary* _, view_t find){
   int32_t idx = Dictionary_find_idx(_,find);if(idx < 0){return 0;}
   return &_->list.data[idx];
}
inl Idx_t* Dictionary_set_elem(Dictionary* _, view_t find){
   int32_t idx = Dictionary_find_idx(_,find);
   if(idx < 0){idx=Dictionary_add(_, find);};
   return &_->list.data[idx];  //never return 0
}
//!
inl view_t Dictionary_get(Dictionary* _, view_t find){

   int32_t idx = Dictionary_find_idx(_,find);
   if(idx < 0){return (view_t){.data=EMPTY_STR, .size=0};}//Return empty str ptr?

   return (view_t){.data=&_->val.data[ _->list.data[idx].vidx.begin ], .size=(_->list.data[idx].vidx.end-_->list.data[idx].vidx.begin)};
}
/////
static const char* SPACE = "                                                      ";
inl void Dictionary_print_indent(Dictionary* _, int _indent){
   if(_==0) return;
   for(size_t i=0;i<_->list.size;i++){
      Idx_t* e = &_->list.data[i];
      if(e!=0){
         char* key = &_->str.data[ e->kidx ];
         view_t gkey = (view_t){.data=key, .size=strlen(key)};

         view_t str_val = Dictionary_get(_, gkey); //TODO optimize

         info_print(":%.*s%s[%.*s]", _indent, SPACE, key, _s(str_val) );
         if(Dictionary_elem_is_ptr(e)){
            Dictionary_print_indent((Dictionary*)e->xptr, _indent+2);
         }
      }
   }
}
inl void Dictionary_print(Dictionary* _){Dictionary_print_indent(_, 0);}
////
inl int Dictionary_to_JSON_indent(Dictionary* _, view_t _buff, int pos, int _indent, int _num_space_indent){
   if(_==0) return -1;if(pos<0)return -1;
   for(size_t i=0;i<_->list.size;i++){
      Idx_t* e = &_->list.data[i];
      if(e!=0){
         char* key = &_->str.data[ e->kidx ];
         view_t gkey = (view_t){.data=key, .size=strlen(key)};

         view_t str_val = Dictionary_get(_, gkey); //TODO optimize

         if(pos + gkey.size + str_val.size + _indent +10 > _buff.size) //+10 is a brainless number for: separator, quotes ...
         {
            err_print("Error JSON creation buffer overflow, max is %d", _buff.size);
            return -1;
         }

         if(Dictionary_elem_is_ptr(e)){ //ELEM_FLAG_PTR? because we can combine both?
            pos += _sprintf(&_buff.data[pos],"%.*s\"%s\": {\n", _indent, SPACE, key, _s(str_val));
            pos = Dictionary_to_JSON_indent((Dictionary*)e->xptr, _buff, pos, _indent+_num_space_indent, _num_space_indent);
            pos += _sprintf(&_buff.data[pos],"}");
         }else{
            switch(e->flags){
               case ELEM_FLAG_STR:
                  pos += _sprintf(&_buff.data[pos],"%.*s\"%s\": \"%.*s\"", _indent, SPACE, key, _s(str_val));
               break;
               case ELEM_FLAG_INT:
                  pos += _sprintf(&_buff.data[pos],"%.*s\"%s\": %i", _indent, SPACE, key, Dictionary_elem_get_int(e));
               break;
               case ELEM_FLAG_DEC:
                  pos += _sprintf(&_buff.data[pos],"%.*s\"%s\": %jd.%jd", _indent, SPACE, key, Dictionary_elem_get_dec_base(e),Dictionary_elem_get_dec_fract(e));
               break;
               case ELEM_FLAG_BOOL:
                  if(Dictionary_elem_get_bool(e)){
                     pos += _sprintf(&_buff.data[pos],"%.*s\"%s\": true", _indent, SPACE, key);
                  }else{
                     pos += _sprintf(&_buff.data[pos],"%.*s\"%s\": false", _indent, SPACE, key);
                  }
               break;
               case ELEM_FLAG_NULL:
                  pos += _sprintf(&_buff.data[pos],"%.*s\"%s\": null", _indent, SPACE, key);
               break;
            }
         }
         if(i!=_->list.size-1){
            pos += _sprintf(&_buff.data[pos],",\n");
         }else{
            pos += _sprintf(&_buff.data[pos],"\n");
         }
      }
   }
   return pos;
}
inl int Dictionary_to_JSON(Dictionary* _, view_t _buff, int _num_space_indent){
   _buff.data[0]='{';_buff.data[1]='\n';
   int pos = Dictionary_to_JSON_indent(_, _buff, 2,0, _num_space_indent);
   if(pos>0){
      _buff.data[pos]='\n';_buff.data[pos+1]='}';_buff.data[pos+2]=0;
      return pos+2;
   }else{
      _sprintf(&_buff.data[0],"{\"Error\":\"Buffer Overflow:[%d]\"}\n",_buff.size);
      return -1;
   }
}
///////////////
inl int Dictionary_Size_of_elements_calc(Dictionary* _, int num, int _indent, int _num_space_indent){
   if(_==0) return 0;
   for(size_t i=0;i<_->list.size;i++){
      Idx_t* e = &_->list.data[i];
      //
      char* key = &_->str.data[ e->kidx ];
      view_t gkey = (view_t){.data=key, .size=strlen(key)};
      view_t str_val = Dictionary_get(_, gkey); //TODO optimize & Dictionary_print
      num += gkey.size+str_val.size+ _indent + 10; //+10 is a brainless number for: separator, quotes ... 
      //
      if(e!=0 && Dictionary_elem_is_ptr(e)){
         num=Dictionary_Size_of_elements_calc((Dictionary*)e->xptr,num, _indent+_num_space_indent, _num_space_indent);
      }
   }
   return num;
 //  return num+_->list.size;
}
inl int Dictionary_Size_of_elements(Dictionary* _, int _indent){return Dictionary_Size_of_elements_calc(_,0,0,_indent);}
//
inl int Dictionary_GetNumbers_of_elements_calc(Dictionary* _, int num){
   if(_==0) return 0;
   for(size_t i=0;i<_->list.size;i++){
      Idx_t* e = &_->list.data[i];
      if(e!=0 && Dictionary_elem_is_ptr(e)){
         num=Dictionary_GetNumbers_of_elements_calc((Dictionary*)e->xptr,num);
      }
   }
   return num+_->list.size;
}
inl int Dictionary_GetNumbers_of_elements(Dictionary* _){return Dictionary_GetNumbers_of_elements_calc(_,0);}
///////////////
//warning, must be a malloc allocated view_t
inl void mview_add_char(mview_t* _, int _idx, char _c){
   if(_idx>=_->size){
      //Realloc
      char* new_data = _malloc(_->size*2, char);
      _memcpy(new_data,_->data, _->size);
      _free(_->data);
      _->data=new_data;
      _->size*=2;
   }
   _->data[_idx]=_c;
}

//!
//!---IMPL
#ifdef XTERNAL_IMPL
   ///--------------------------------------------------------------------------------------------
   ///--------------------------------------------------------------------------------------------
   ///==!ARRAY!== IMPL
   ///============================================================================================
   ///--------------------------------------------------------------------------------------------
   const char EMPTY_DATA_[128]={0};
#endif//!------
///
///--------------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------------
///==|FILES|==
///============================================================================================
///--------------------------------------------------------------------------------------------
//!File.inc//
typedef struct File{
   char* used_path;

   view_t content; //need to be freed

   //char*  data;
   //size_t size;

   view_t fullpath; //need to be freed
   view_t fullname;
   view_t fullfolder;

   view_t name;
   view_t ext;
   view_t extd;
   view_t folder;
   view_t drive;

}File;
File Xe_LoadFile(const char* _sFullPath, bool _create);
void Xe_FreeFile(File* file);
File* file_read(File* _file);
fn File* file_parse(File*  f, view_t _src);
view_t* normalize_path(view_t* _str);
fn bool fnod_read_mem(File* _file, std_array_t* _mem);

typedef void (*file_fptr)(char* _base_dir, char* _dir, char* _name);
fn size_t path_read_dir(char* _dir, file_fptr _callback);
//!-----------!//

//!
//!---IMPL
#ifdef XTERNAL_IMPL
   ///--------------------------------------------------------------------------------------------
   ///--------------------------------------------------------------------------------------------
   ///==!FILES!== IMPL
   ///============================================================================================
   ///--------------------------------------------------------------------------------------------
   #include <errno.h>

   inl view_t* trim_right(view_t* _str){
      while(_str->size>0){
         char c = _str->data[_str->size-1];
         if(c>32)break;
         _str->size--;
      }
      return _str;
   }

   view_t* normalize_path(view_t* _str){ //str must be writable


      trim_right(_str);//TODO trim left?


      //_str->size = strlen(_str->data)+1;
      if(_str->data[0] == 34){//quote
         info_print("=====  quote ====");
         //_printv((*_str));
         //info_print("SIZE %d",strlen(_str->data));
         //info_print("f[_str] %.*s| %d",_str->size,_str->data, _str->size);
         //info_print("=>: %s|", _str->data);

         _str->data++;
         _str->size--;

         //_str->data[_str->size]=0;
         /*
            while(_str->size>0){
                  if(_str->data[_str->size] == 34){//endqote
                     break;
                  }
                  _str->size--;
            }*/

            if(_str->data[_str->size-1] == 34){
               _str->size--;
               _str->data[_str->size]=0;
            }
            //info_print("SIZE %d",strlen(_str->data));
            //info_print("r[_str] %.*s| %d",_str->size,_str->data, _str->size);
            //info_print("=>: %s|", _str->data);
            //info_print("=====  end ====");
      }


      {//change \ to /
      int i = 0;
      while(_str->data[i]!=0){
         if(_str->data[i]=='\\'){
            _str->data[i]='/';
         }
      i++;}
      }

      {//remove multiple // but keep first one (if we have network path //network -> begin to 1)
      int i=1,j=1;char last=0;
      while(_str->data[i]!=0){
         if(!(_str->data[i]=='/'&&last=='/')){
            _str->data[j]=_str->data[i];
            j++;
         }
         last=_str->data[i];
      i++;}
      _str->size = j;
      }
      if(_str->size==0) return _str;
      {//Remove dot from relative path, ex: ./folder/ to /folder/
         if(_str->size> 1 && _str->data[0]=='.' &&  _str->data[1]=='/'){
            *_str=view_substr(*_str,1,-1);
         }
      }
      if(_str->size<=1) return _str;
      {//Remove single slash from relative path, ex: /folder/ to folder/
         if(_str->data[0]=='/' && _str->data[1]!='/'){
            *_str=view_substr(*_str,1,-1);
         }
      }

      //Remove extra folder ex: entity/../interface.exe to interface.exe
      view_t dest=*_str;
      size_t lstart = 0;
      size_t cursor = 0;
      char l=0;
      char ll=0;
      char lll=0;
      char llll=0;
      for(int i=0; i<_str->size; i++){ //start at 1 to prenvent ../folder/../../folder2
         char a = _str->data[i];
         if( l=='/' && ll=='.' && lll=='.' &&  llll=='/'){ //move folder back
            int ini = cursor;
            int j = cursor-4;
            while(j>0){   j--;
               if(dest.data[j]=='/'){
                  //cursor=j+1;
                  j++;
                  break;
               }
            }
            cursor=j;
            while(cursor+2<dest.size && dest.data[cursor]=='.' && dest.data[cursor+1]=='.' && dest.data[cursor+2]=='/'){
               cursor+=3;
            }
            if(cursor>ini){
               cursor=ini;
            }
         }
         llll=lll;
         lll=ll;
         ll=l;
         l=a;
         if(cursor!=i){
            dest.data[cursor]=_str->data[i];
         }
         cursor++;
      }
      dest.data[cursor]=0;

      return _str;
   }


   fn File* file_parse(File*  f, view_t _src){//note: _src is altered
      f->used_path=_src.data;

      f->fullpath= *normalize_path(&_src);
      _src.data[f->fullpath.size]='\0';//src is altered

      f->fullname = view_from_lastchar(f->fullpath, '/');
      f->name = view_to_lastchar(f->fullname, '.');
      f->ext = view_substr(f->fullname,f->name.size+1,-1);
      f->extd = view_substr(f->fullname,f->name.size,-1);
      f->fullfolder = view_substr(f->fullpath, 0, f->fullpath.size-f->fullname.size);
      f->folder = f->fullfolder;
      f->drive;

      if(f->fullfolder.size>2){
         if(f->fullfolder.data[1]==':'){ //have a drive specified
            f->folder = view_substr(f->fullfolder,3,-1);
            f->drive =  view_substr(f->fullfolder,0,3);
         }
         if(f->fullfolder.data[1]=='/'){ //have a network specified
            f->folder = view_substr(f->fullfolder,2,-1);
            f->drive =  view_substr(f->fullfolder,0,2);
         }
      }

      /*
      _printv(f->fullpath);
      _printv(f->fullname);
      _printv(f->fullfolder);
      _printv(f->folder);
      _printv(f->drive);
      _printv(f->name);
      _printv(f->ext);
      _printv(f->extd);*/
      return f;
   }

   File* file_read(File* _file)
   {
      char* _sFullPath = _file->fullpath.data;
      if(_sFullPath == 0){
         err_print("Error: No file to load.");
         return _file;
      }

      FILE*  f = fopen((char*)_sFullPath, "rb+");
      unsigned char *result;

      if (f != NULL){
         long lSize;
         fseek (f , 0 , SEEK_END);
         lSize = ftell (f);
         rewind (f);
         size_t _nSize = lSize;

         uint8_t* _aData = _malloc(_nSize+1, uint8_t); //+1 null ending
         fread(_aData, 1, _nSize, f);
         _aData[_nSize]='\0';// null ending

         fclose(f);

         _file->content.data=(char*)_aData;
         _file->content.size=_nSize;
         //return  (File){.data=(char*)_aData, .size=_nSize, .path=_mallocpy(_sFullPath, strlen(_sFullPath)+1, char)};
         return _file;
      }else{
         err_print("Error file_read: [%s] (%s)", _sFullPath, strerror(errno));
         return 0;
      }

      return _file;
   }


   inl File Xe_File_Ini(char* _data, char* _path, size_t _size)
   {

   //if nor already ini

      size_t pfullpath_size =  strlen(_path);
      char*  pfullpath=_mallocpy(_path, (pfullpath_size+1), char);

      File f = (File){ .content=(view_t){ .data=(char*)_data, .size=_size} };
      file_parse(&f, (view_t){.data=pfullpath, .size=pfullpath_size});


      return  f;
   }
   
   #include <sys/stat.h>//mkdir
   File Xe_LoadFile(const char* _sFullPath, bool _create)
   {
      if(_sFullPath == 0){
         err_print("Error: No file to load.");
         return (File){};
      }

      FILE*  f = fopen((char*)_sFullPath, "rb+");
      if (_create && f == NULL){
         ////////////////////////////
         f = fopen((char*)_sFullPath, "wb+");
         if(f == NULL){
            info_print("check for create directory structure: %s", _sFullPath);
            //check for create directory structure
            char* end=_sFullPath;
            while((*end)!='\0'){
               if((*end)=='/' || (*end)=='\\'){
                  char save = (*end);
                  (*end)='\0';
                  info_print("mkdir: %s", _sFullPath);
                  mkdir(_sFullPath);
                  (*end)= save;
               }
               end++;
            }
            //Retry
            f = fopen((char*)_sFullPath, "wb+");
         }
      }
      
      unsigned char *result;
      if (f != NULL){
         long lSize;
         fseek (f , 0 , SEEK_END);
         lSize = ftell (f);
         rewind (f);
         size_t _nSize = lSize;

         uint8_t* _aData = _malloc(_nSize+1, uint8_t); //+1 null ending
         fread(_aData, 1, _nSize, f);
         _aData[_nSize]='\0';// null ending
         //info_print("---------------LoadFile size: %d",_nSize);
         if(_nSize==0){
            //info_print("-ERRORRRR");
         }

         fclose(f);
         //return  (File){.data=(char*)_aData, .size=_nSize, .path=_mallocpy(_sFullPath, strlen(_sFullPath)+1, char)};
         return Xe_File_Ini((char*)_aData, (char*)_sFullPath, _nSize);
      }else{
         err_print("Error Loading File: [%s] (%s)", _sFullPath, strerror(errno));
      }

      return (File){};
   }

   void Xe_FreeFile(File* file){
      if(!file) return;
      _free(file->content.data);
      _free(file->used_path);
   }
   
   #include <dirent.h> 

   //// DIR ////
   fn size_t path_read_dir(char* _dir, file_fptr _callback){
      int count=0;
      DIR *d;
      struct dirent* dir;
      d = opendir(_dir);
      if (d) {
         while ((dir = readdir(d)) != NULL) {
            char* f = dir->d_name; 
            if(!(f[0]==0  || (f[0]=='.' && f[1]==0) || (f[0]=='.' && f[1]=='.'))){ //remove . && .. folder
               count++;
               _callback(_dir,"",f);
            }
         }
      }
      return count;
   }

#endif//!------

///////////////////////
#endif //HDEF_Xternal//
///////////////////////
///--------------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------------
///==|MATH|==
///============================================================================================
///--------------------------------------------------------------------------------------------
#undef MIN
#undef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
