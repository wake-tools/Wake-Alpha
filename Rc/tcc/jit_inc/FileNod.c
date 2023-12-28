/*|| https://wake.tools ||                                          | license | Apache License Version 2.0 |
  | --------------------------------------------------------------------------------------------------+-+++|>
  +-  FileNod.c              
  |---------------------------------------------------------------------------------------------------+-+++|>
  +-  File Loader for json data in a dictionary Nod & utilities       
  |          
  |-----|--------|---------------------|-----------------------------|--------------------------------+-+++|>
*/
#include "FileNod.h"
#include "jit/tcc.h"
//
fn void FileNod_ini(FileNod* _this, std_array_t* mem){
	String_ini(&_this->key_path, mem);
	_this->mem=mem;	
}
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600 //GetFileInformationByHandleEx
#endif
//
//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
//
fn int64_t FileNod_GetTime_Modification(FileNod* _this){
	#ifdef D_Platform_Windows
	struct __stat64  attrib;
    stat64(_this->file.fullpath.data, &attrib);
	return attrib.st_mtime * 1000000000; //TODO see .tv_sec (+ attrib.st_mtime.tv_sec)
	//https://gitlab.gnome.org/GNOME/glib/-/merge_requests/1329
	//https://github.com/OSGeo/gdal/issues/3139
	#else
	return 0;
	#endif
}
//
fn bool fnod_read_mem(File* _file, std_array_t* _mem){
	if(!file_read(_file))return false;
	_track_mem(_mem, _file->content.data, 0, 0);
	return true;
}
//
fn bool fnod_read(FileNod* _this){ //TODO call fnod_read_mem
	//
	if(!file_read(&_this->file))return false;
	//_this->file = file_load(_this->file);
	info_print("size: %d", _this->file.content.size);
	_track_mem(_this->mem, _this->file.content.data, 0, 0);
	//_track_mem(_this->mem, _this->file.used_path, 0, 0);
	//_printv(_this->file.fullpath);
	///////////
	return true;
}
//
fn bool FileNod_LoadFile(FileNod* _this, char* _path){
	_release_mem_ptr(_this->mem, (void**)&_this->file.content.data);
	void** old_path = (void**)&_path;
   //info_print("-Xe_LoadFile");
	//_this->file = Xe_LoadFile(_this->file.used_path);
	_this->file = Xe_LoadFile(_path, true);
	
	_release_mem_ptr(_this->mem, old_path);
	
	info_print("LoadFile _path: %s", _path);
	info_print("LoadFile size: %d", _this->file.content.size);
	
	
	_track_mem(_this->mem, _this->file.content.data, 0, 0);
	_track_mem(_this->mem, _this->file.used_path, 0, 0);
	
	_this->data = (view_t){ .size=_this->file.content.size, .data=_this->file.content.data};
		
	return true;
}

//
fn bool FileNod_Parse_JSON(FileNod* _this){
	///////////
	_this->type = FILENODE_JSON;
	Nod_ini(&_this->jsonnod, _this->mem);
	view_t json = (view_t){ .size=_this->file.content.size, .data=_this->file.content.data};
	Nod_Parse_JSON(&_this->jsonnod, json );
   //
	if(!_this->keep_mem_open){
		_release_mem_ptr(_this->mem, (void**)&_this->file.content.data);
	}
	return true;
}
fn bool FileNod_Parse_JSON_To_Nod(FileNod* _this, Nod* _nod){
	///////////
	_this->type = FILENODE_JSON;
	view_t json = (view_t){ .size=_this->file.content.size, .data=_this->file.content.data};
	Nod_Parse_JSON(_nod, json );
   //
	if(!_this->keep_mem_open){
		_release_mem_ptr(_this->mem, (void**)&_this->file.content.data);
	}
	return true;
}
//
#include "stb/stb_image.h"
fn bool FileNod_Parse_PNG(FileNod* _this){
	_this->type = FILENODE_PNG;
   view_t png = (view_t){ .size=_this->file.content.size, .data=_this->file.content.data};

   int x, y, c;
   _this->dim.d=4;
   _this->datatype = (void*)stbi_load_from_memory(png.data, png.size, &x, &y, &c,  _this->dim.d);
   _this->dim.w=x;
   _this->dim.h=y;
   _this->dim.c=c;
  //logo_texture = (unsigned long long)ImGui_ImplOpenGL2_LoadTexture(data, x, y);
   return true;
}
#include "stb/stb_image_write.h"
fn bool FileNod_Write_PNG(FileNod* _this){
   info_print("WRITE PNG: %s, %d, h:%d, d:%d, stride: %d",_this->file.fullpath.data, _this->dim.w, _this->dim.h, _this->dim.d, _this->dim.w*_this->dim.d);
   stbi_write_png(_this->file.fullpath.data, _this->dim.w, _this->dim.h,  _this->dim.d, _this->datatype, _this->dim.w*_this->dim.d);
   return true;
}

fn bool FileNod_Write(FileNod* _this){
   switch(_this->type){
		case FILENODE_DATA:{
	
		break;}
		
		case FILENODE_JSON:{
		break;}

      case FILENODE_PNG:{
			FileNod_Write_PNG(_this);
		break;}
		
	}
   return true;
}

////////////////////////////////////
void jit_addsymb(TccPrg* _);
void jit_error(TccPrg* _prg, const char* msg);
void jit_run(TccPrg* _);
void jit_complete(TccPrg* _);
////////////////////////////////////
/*
//TODO unsafe
#ifndef SOKOL_GFX_INCLUDED
typedef struct sg_shader   { uint32_t id; } sg_shader;
void sg_destroy_shader(struct sg_shader shd_id);
#endif
void sg_activate_current_imgui_associate_ctx();
*/
void invalidate_all_subshader(view_t _module_name);
void sg_destroy_imgui_sh_ctx(uint32_t _shid);
module_t* getm_shview(view_t _name);
fn bool FileNod_JitCompile(FileNod* _this)
{
   #ifndef NO_SHADER
	if(_this->type == FILENODE_JIT_SHADER){
      //_this->cust_id=-1;//force reload
      invalidate_all_subshader(_this->file.name);
	}
   #endif
   
	void* old_datatype = 0;
	void* old_valid_datatype =  0;
	if(_this->type == FILENODE_JIT || _this->type == FILENODE_JIT_SHADER){
		old_datatype =  _this->datatype;
		old_valid_datatype =  _this->valid_datatype;
	}
	
	#ifndef TCC_NOJIT
		//TODO error if action_f is null
		TccPrg* prg = (TccPrg*)_this->action_f(_this);
		//
		_this->datatype = prg;	
		if(prg!=0 && prg->valid){
         _this->module_ini=false;
         
         #ifndef NO_SHADER
         if(_this->type == FILENODE_JIT_SHADER){
            //also de-ini associate imgui shaderview
            module_t* m= getm_shview(_this->file.name);
            m->fnod.module_ini=false;
         }
         #endif
         
			_this->valid_datatype = prg;
			//
			if(old_valid_datatype != 0){
            //TODO
				Tcc_Release(old_valid_datatype);
			}
		}else{ 
			if(old_datatype != 0){
				Tcc_Release(old_datatype);
			}
		}
      //
		if(!_this->keep_mem_open){
			_release_mem_ptr(_this->mem, (void**)&_this->file.content.data);
		}
	#endif
   _this->type = FILENODE_JIT; //Change FILENODE_JIT_SHADER to FILENODE_JIT ?
	return true;
}
//
fn bool FileNod_Reload(FileNod* _this, char* path)
{	

   if(!path)path=_this->file.used_path;
   //info_print("RELOAD: ", path);
	if(!_this->no_fileRead)FileNod_LoadFile(_this, path);	
	//
   switch(_this->type){
		case FILENODE_DATA:{
	
		break;}
		
		case FILENODE_JSON:{
			view_t json = (view_t){ .size=_this->file.content.size, .data=_this->file.content.data};
			Nod_Parse_JSON(&_this->jsonnod, json );
		break;}
		
		case FILENODE_JIT:
		case FILENODE_JIT_SHADER:
		{
			FileNod_JitCompile(_this);
		break;}
      
      case FILENODE_PNG:{
			FileNod_Parse_PNG(_this);
		break;}
		
	}
	info_print("Reloaded: %s", _this->file.fullpath.data);
	return true;
}
//
fn bool FileNod_ToJSON(FileNod* _this){
	info_print("--------------");
	info_print("---- TO JSON ---- [%s]", _this->file.fullpath);
	//
	FILE *f  = fopen(_this->file.fullpath.data,"w");
	if(f != NULL){
		int indent = 2;
		int mem_size = Dictionary_Size_of_elements(&_this->jsonnod.dic, (indent+1)); //TODO +1 (wrong calculation)
      
		char buff[mem_size];
		view_t vbuff=(view_t){.data=buff, .size=mem_size};
		//
		int real_size = Dictionary_to_JSON(&_this->jsonnod.dic, vbuff,indent);
		//info_print("Buffer %s", buff);
      if(real_size>mem_size){
         err_print("used more memory than allocated: %d / %d", real_size, mem_size);
      }
		//info_print("used_size %d, real_size %d", mem_size, real_size);
      
		fprintf(f, "%s", buff);
		fclose(f);
	}else{
		err_print("Error opening writable file");
		return false;
	}
   //
	_this->mod_time = FileNod_GetTime_Modification(_this);
	return true;
}

//
fn bool FileNod_Close(FileNod* _this){
   switch(_this->type){
		case FILENODE_PNG:{
         if( _this->datatype)stbi_image_free(_this->datatype);
		break;}
   }

	_release_mem_ptr(_this->mem, (void**)&_this->file.content.data);
	_release_mem_ptr(_this->mem, (void**)&_this->file.fullpath.data);
	//Xe_FreeFile(&_this->file);
	return true;
}

fn bool FileNod_IsModified(FileNod* _this){
	int64_t curr_mod_time = FileNod_GetTime_Modification(_this);
	//if(_this->mod_time!=0 && curr_mod_time != _this->mod_time){
	if( curr_mod_time != _this->mod_time){
		_this->mod_time=curr_mod_time;
		return true;
	}
	//info_print("mod_time: %lld", mod_time);
	return false;
}
//
fn bool FileNod_IsModified_CanReload(FileNod* _this, double _delta_time, double _freq_mod_time_check){
	if(_delta_time > _this->last_valid_mod_time + _freq_mod_time_check ){
		//info_print("check: %s", _this->file.fullpath.data);
		_this->last_valid_mod_time = _delta_time;
		return FileNod_IsModified(_this);
	}
	return false;
}
//
fn bool FileNod_IsModified_CheckForReload(FileNod* _this, double _delta_time, double _freq_mod_time_check){
	if(!_this)return false;
	if(FileNod_IsModified_CanReload(_this, _delta_time, _freq_mod_time_check)){
		_Sleep(1);	//Give time to not reload too fast (give strange result of reading 0)
		FileNod_Reload(_this,_this->file.used_path);
		return true;
	}
	return false;
}


