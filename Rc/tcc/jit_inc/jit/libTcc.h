#ifndef LIBTCC_H
#define LIBTCC_H
#ifdef __cplusplus
extern "C" {
#endif

struct TCCState;
typedef struct TCCState TCCState;

/* create a new TCC compilation context */
typedef TCCState* (*fptr_tcc_new)();
extern fptr_tcc_new tcc_new;

/* free a TCC compilation context */
typedef void (*fptr_tcc_delete)(TCCState *s);
extern fptr_tcc_delete 
			tcc_delete;

/* set CONFIG_TCCDIR at runtime */
typedef void (*fptr_tcc_set_lib_path)(TCCState *s, const char *path);
extern fptr_tcc_set_lib_path 
			tcc_set_lib_path;

/* set error/warning display callback */
typedef void (*fptr_tcc_set_error_func)(TCCState *s, void *error_opaque,
    void (*error_func)(void *opaque, const char *msg));
extern fptr_tcc_set_error_func 
			tcc_set_error_func;

/* set options as from command line (multiple supported) */
typedef void (*fptr_tcc_set_options)(TCCState *s, const char *str);
extern fptr_tcc_set_options 
			tcc_set_options;
/*****************************/
/* preprocessor */

/* add include path */
typedef int (*fptr_tcc_add_include_path)(TCCState *s, const char *pathname);
extern fptr_tcc_add_include_path 
			tcc_add_include_path;
			
/* add in system include path */
typedef int (*fptr_tcc_add_sysinclude_path)(TCCState *s, const char *pathname);
extern fptr_tcc_add_sysinclude_path 
			tcc_add_sysinclude_path;

/* define preprocessor symbol 'sym'. Can put optional value */
typedef void (*fptr_tcc_define_symbol)(TCCState *s, const char *sym, const char *value);
extern fptr_tcc_define_symbol 
			tcc_define_symbol;
			
/* undefine preprocess symbol 'sym' */
typedef void (*fptr_tcc_undefine_symbol)(TCCState *s, const char *sym);
extern fptr_tcc_undefine_symbol 
			tcc_undefine_symbol;
			
/*****************************/
/* compiling */

/* add a file (C file, dll, object, library, ld script). Return -1 if error. */
typedef int (*fptr_tcc_add_file)(TCCState *s, const char *filename);
extern fptr_tcc_add_file 
			tcc_add_file;

/* compile a string containing a C source. Return -1 if error. */
typedef int (*fptr_tcc_compile_string)(TCCState *s, const char *buf);
extern fptr_tcc_compile_string 
			tcc_compile_string;
			
/*****************************/
/* linking commands */

/* set output type. MUST BE CALLED before any compilation */
typedef int (*fptr_tcc_set_output_type)(TCCState *s, int output_type);
extern fptr_tcc_set_output_type 
			tcc_set_output_type;
			
#define TCC_OUTPUT_MEMORY   1 /* output will be run in memory (default) */
#define TCC_OUTPUT_EXE      2 /* executable file */
#define TCC_OUTPUT_DLL      3 /* dynamic library */
#define TCC_OUTPUT_OBJ      4 /* object file */
#define TCC_OUTPUT_PREPROCESS 5 /* only preprocess (used internally) */

/* equivalent to -Lpath option */
typedef int (*fptr_tcc_add_library_path)(TCCState *s, const char *pathname);
extern fptr_tcc_add_library_path 
			tcc_add_library_path;

/* the library name is the same as the argument of the '-l' option */
typedef int (*fptr_tcc_add_library)(TCCState *s, const char *libraryname);
extern fptr_tcc_add_library 
			tcc_add_library;
			
/* add a symbol to the compiled program */
typedef int (*fptr_tcc_add_symbol)(TCCState *s, const char *name, const void *val);
extern fptr_tcc_add_symbol 
			tcc_add_symbol;
			
/* output an executable, library or object file. DO NOT call
   tcc_relocate() before. */
typedef int (*fptr_tcc_output_file)(TCCState *s, const char *filename);
extern fptr_tcc_output_file 
			tcc_output_file;
			
/* link and run main() function and return its value. DO NOT call
   tcc_relocate() before. */
typedef int (*fptr_tcc_run)(TCCState *s, int argc, char **argv);
extern fptr_tcc_run 
			tcc_run;
			
/* do all relocations (needed before using tcc_get_symbol()) */
typedef int (*fptr_tcc_relocate)(TCCState *s1, void *ptr);
extern fptr_tcc_relocate 
			tcc_relocate;
			
/* possible values for 'ptr':
   - TCC_RELOCATE_AUTO : Allocate and manage memory internally
   - NULL              : return required memory size for the step below
   - memory address    : copy code to memory passed by the caller
   returns -1 if error. */
#define TCC_RELOCATE_AUTO (void*)1

/* return symbol value or NULL if not found */
typedef void* (*fptr_tcc_get_symbol)(TCCState *s, const char *name);
extern fptr_tcc_get_symbol 
			tcc_get_symbol;
			
#ifdef __cplusplus
}
#endif
#endif
