#ifndef LIBTCC_H
#define LIBTCC_H
#ifdef __cplusplus
extern "C" {
#endif

#include "Xternal.h"
#include "FileNod.h"

typedef int (*fptr_itf_dll_test)(fptr_printf);

typedef int (*fptr_itf_dll_main)(Nod* _nod);
extern fptr_itf_dll_main
            itf_dll_main;


#ifdef __cplusplus
}
#endif
#endif
