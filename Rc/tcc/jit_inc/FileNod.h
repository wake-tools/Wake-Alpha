/*|| https://wake.tools ||                                          | license | Apache License Version 2.0 |  
  |---------------------------------------------------------------------------------------------------+-+++|>
  +-  File Loader for json data in a dictionary Nod & utilities       
  |          
  |-----|--------|---------------------|-----------------------------|--------------------------------+-+++|>
*/  
#ifndef HDEF_FileNod
#define HDEF_FileNod
//
#include "Xternal.h"
#include "ANod.h"
//
enum FileNod_Type {FILENODE_DATA,FILENODE_JSON,FILENODE_JIT,FILENODE_JIT_SHADER,FILENODE_PNG};
//
struct FileNod;
typedef struct FileNod FileNod;
typedef void* (*action_fptr)(FileNod*);

typedef struct dim_t{
	int w,h,d,c;
}dim_t;
//
typedef struct FileNod {
	std_array_t* mem;
	//
	String key_path;
	View name;
	File file;
	Nod jsonnod;
	bool keep_mem_open;
	//
	int64_t mod_time;
	double last_valid_mod_time;
	enum FileNod_Type type;
	//
	view_t data;         //original data
	void* datatype;      //extracted_data
	void* valid_datatype;//validated data (under user)
	dim_t dim;
	int32_t cust_id;
   //
	bool module_ini;
	bool no_fileRead;
	action_fptr action_f;

	//
}FileNod;
//

fn void NormalisePath(FileNod* _this, String* str);
fn void GetExt(FileNod* _this, String* name, bool keepdot, bool ret_empty);

#define FileNod_new(name, memory) FileNod name={};FileNod_ini(&name,memory);
fn void FileNod_ini(FileNod* _this, std_array_t* mem);
fn bool FileNod_IsModified(FileNod* _this);
fn bool FileNod_IsModified_CheckForReload(FileNod* _this, double _delta_time, double _freq_mod_time_check);
fn bool FileNod_Reload(FileNod* _this, char* _path);
fn bool FileNod_LoadFile(FileNod* _this, char* _path);
fn bool FileNod_Parse_JSON(FileNod* _this);
fn bool FileNod_Parse_JSON_To_Nod(FileNod* _this, Nod* _nod);
fn bool FileNod_Parse_PNG(FileNod* _this);
fn bool FileNod_Write(FileNod* _this);
fn bool FileNod_JitCompile(FileNod* _this);
fn bool FileNod_Close(FileNod* _this);
fn bool FileNod_ToJSON(FileNod* _this);
//
fn bool fnod_read(FileNod* _this);


typedef int (*jitmain_fptr)(void*);
typedef void* (*_rptr_fptr)();

enum Module_Type {MODULE_UNKNOW, MODULE_LAYOUT,MODULE_SHADER,MODULE_SHADER_VIEW,MODULE_IMGUI};
#define MODULE_NAME_SIZE 25
typedef struct module_t{
   FileNod fnod;
   enum Module_Type type;
  // bool ini;
   bool open;
   bool paused; 
   char name_b[MODULE_NAME_SIZE+1];//+1 NUL ending
   view_t name;
   jitmain_fptr main_func;
}module_t;

#endif
