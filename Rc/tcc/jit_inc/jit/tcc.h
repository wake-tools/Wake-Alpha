
#include "Xternal.h"

struct TCCState;
struct TccPrg;
typedef struct TCCState TCCState;
typedef struct TccPrg TccPrg;



typedef void (*addsymb_fptr)(TccPrg* _prg);
typedef void (*complete_fptr)(TccPrg* _prg);
typedef void (*error_fptr)(TccPrg* _prg, const char* msg);

typedef struct TccPrg {
	TCCState *s;
	std_array_t* mem;
	void (*main)(void);
	bool valid;
	addsymb_fptr addsymb_f;
	complete_fptr complete_f;
	error_fptr error_f;
   File* file;
}TccPrg;



fn void Tcc_Run(TccPrg* _this);
fn TccPrg* Tcc_Compile(std_array_t* _mem, view_t _code, char* _main, void* _main_arg, char** _argv, int _argc,
	addsymb_fptr addsymb_f,
	complete_fptr complete_f,
	error_fptr error_f,
   File* _file
);
fn void Tcc_Release(TccPrg* _prg);
bool dll_load( char* dll);