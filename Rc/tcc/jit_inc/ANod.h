/*|| https://wake.tools ||                                          | license | Apache License Version 2.0 |
  | --------------------------------------------------------------------------------------------------+-+++|>
  +-  ANod.h
  |---------------------------------------------------------------------------------------------------+-+++|>
  +-  Nodes of Nodes
  |
  |-----|--------|---------------------|-----------------------------|--------------------------------+-+++|>
*/
#include "Xternal.h"
//
#ifndef HDEF_Nod
#define HDEF_Nod
//
typedef struct {
	Dictionary dic; //must be first (Dictionary_print)
	std_array_t* mem;
	//enum NOD_TYPE type; //used?
	//  public Dictionary<string, Nod> sub;

} Nod;
//////////////////////
//////////----------//

//////
inl bool Nod_let_str(Nod* _this, view_t find, view_t set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_let_str( &_this->dic, find, set_to);
}
inl bool Nod_set_str(Nod* _this, view_t find, view_t set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_set_str( &_this->dic, find, set_to);
}
inl view_t Nod_get_str(Nod* _this, view_t find){
	return Dictionary_get( &_this->dic, find);
}
//////
inl bool Nod_let_str_c(Nod* _this, view_t find, char* set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_let_str_char( &_this->dic, find, set_to);
}
inl bool Nod_set_str_c(Nod* _this, view_t find, char* set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_set_str_char( &_this->dic, find, set_to);
}
inl char* Nod_get_str_c(Nod* _this, view_t find){
	return Dictionary_get( &_this->dic, find).data;
}
//////
inl bool Nod_let_dec(Nod* _this, view_t find, int64_t set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_let_dec(&_this->dic, find, set_to);
}
inl bool Nod_set_dec(Nod* _this, view_t find, int64_t set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_set_dec(&_this->dic, find, set_to);
}
inl int64_t Nod_get_dec(Nod* _this, view_t find){
	return Dictionary_get_dec(&_this->dic, find);
}
//////
inl bool Nod_let_int(Nod* _this, view_t find, int64_t set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_let_int(&_this->dic, find, set_to);
}
inl bool Nod_set_int(Nod* _this, view_t find, int64_t set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_set_int(&_this->dic, find, set_to);
}
inl int64_t Nod_get_int(Nod* _this, view_t find){
	return Dictionary_get_int(&_this->dic, find);
}
//////
inl bool Nod_let_bool(Nod* _this, view_t find, bool set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_let_bool(&_this->dic, find, set_to);
}
inl bool Nod_set_bool(Nod* _this, view_t find, bool set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_set_bool(&_this->dic, find, set_to);
}
inl bool Nod_get_bool(Nod* _this, view_t find){
	return Dictionary_get_bool(&_this->dic, find);
}
//////
inl bool Nod_set_null(Nod* _this, view_t find){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_set_null(&_this->dic, find);
}
inl bool Nod_get_null(Nod* _this, view_t find){
	return Dictionary_get_null(&_this->dic, find);
}
//////
inl bool Nod_let_ptr(Nod* _this, view_t find, void* set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_let_ptr(&_this->dic, find,set_to);
}
inl bool Nod_set_ptr(Nod* _this, view_t find, void* set_to){
   dbg_assert(_this != 0, "Nod_set null ref");
	return Dictionary_set_ptr(&_this->dic, find,set_to);
}
inl void* Nod_get_ptr(Nod* _this, view_t find){
	return Dictionary_get_ptr(&_this->dic, find);
}
//////
//////

//----------//////////
//////////////////////


#define Nod_alloc(name, memory) Nod* name=_calloc(1, Nod);_track_mem(memory, name, 0, 0);Nod_ini(name,memory);
#define Nod_new(name, memory) Nod name={};Nod_ini(&name,memory);
inl void Nod_ini(Nod* _this, std_array_t* _mem){
	Dictionary_ini(&_this->dic, _mem);
	_this->mem=_mem;
}
/*
///////
inl Nod* Nod_set_struct(Nod* _this, view_t _fullkey, OUT view_t* lastkey ){
	//Navigate throught nodes
	int i = 0;
	int start = 0;
	Idx_t* e = 0;
	Nod* curr_nod = _this;
	
	do{ 
	//if(_fullkey.data[i]=='.' || _fullkey.data[i]=='['  || _fullkey.data[i]==']' ){ //TODO Not the last one (if not end by \0)
	if(_fullkey.data[i]=='.' || _fullkey.data[i]=='['  || _fullkey.data[i]==']' || _fullkey.size == i){
		*lastkey = (view_t){.data=&_fullkey.data[start], .size=(size_t)i-start};
		
		if(lastkey->size>0 &&  _fullkey.size!=i){ //Not the last one

			e = Dictionary_set_elem(&curr_nod->dic, (*lastkey));
			if(!Dictionary_elem_is_ptr(e)){
				//Not a ptr type? Create new alloc:
				Nod_alloc(new_anod, curr_nod->mem);
				Dictionary_modified_ptr(&curr_nod->dic, e, new_anod);
			}
			curr_nod = (Nod*)e->xptr;
		}
		start=i+1;
	}
	i++;}while(_fullkey.data[i-1]!=0);
	
	return curr_nod;
}*/
/*
inl Nod* Nod_set(Nod* _this, view_t _fullkey, view_t _val){
	view_t lastkey={};
	Nod* last_nod = Nod_set_struct(_this, _fullkey, &lastkey);
	//info_print("lastkey: %.*s", _s(lastkey));

	Dictionary_set(&last_nod->dic, lastkey, _val); 
	return last_nod;
}*/
///////

///////
inl Nod* Nod_get_struct(Nod* _this, view_t _fullkey, OUT view_t* lastkey ){
	//Navigate throught nodes
	int i = 0;
	int start = 0;
	Idx_t* e = 0;
	Nod* curr_nod = _this;
	
	do{ 
	if(_fullkey.data[i]=='.' || _fullkey.data[i]=='['  || _fullkey.data[i]==']' || _fullkey.size == i){
		*lastkey = (view_t){.data=&_fullkey.data[start], .size=(size_t)i-start};
		
		if(lastkey->size>0){ 

			e = Dictionary_set_elem(&curr_nod->dic, (*lastkey));
			if(!Dictionary_elem_is_ptr(e)){
				//Not a ptr type? Create new alloc:
				Nod_alloc(new_anod, curr_nod->mem);
				Dictionary_modified_ptr(&curr_nod->dic, e, new_anod);
			}
			curr_nod = (Nod*)e->xptr;
		}
		start=i+1;
	}
	i++;}while(_fullkey.data[i-1]!=0);
	
	return curr_nod;
}
inl Nod* Nod_get(Nod* _this, view_t _fullkey){
   dbg_assert(_this != 0, "Nod_get null ref");
   if(_fullkey.data==0)return 0; //TODO or assert?
	view_t lastkey={};
	Nod* last_nod = Nod_get_struct(_this, _fullkey, &lastkey);
	//info_print("lastkey: %.*s", _s(lastkey));
	return last_nod;
}
///////
inl size_t Nod_size(Nod* _this){
	return _this->dic.list.size;
}
////////
inl view_t Nod_ExtactBracket(view_t src, char s, char e) {
	
	int start_idx = -1;
	for(int i=0; i<src.size; i++){
		if(src.data[i] == s){start_idx=i;break;}
	}
	if(start_idx==-1)return (view_t){}; //Not found
	
	start_idx++;
	view_t result = (view_t){.data=&src.data[start_idx] };
	
	int end_idx=start_idx;
	int deep = 0;
	while (end_idx < src.size) {char curr=src.data[end_idx];
		if (curr == s) {deep++;}
		if (curr == e) {
			if (deep == 0) {
				result.size=end_idx-start_idx;
				return result;
			}
			deep--;
		}
		end_idx++;
	}
	err_print("JSON parsing: Bracket mismatch");
	return (view_t){};
 }
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
inl Nod* Nod_Exact_SubBracket(Nod* _this, view_t json);
inl Nod* Nod_Exact_SubArray(Nod* _this, view_t json);
#include "stdio.h"
inl int64_t view2int(view_t v){
    int ret = 0;
	char sign=1;
	int i=0;
	if(v.data[0]=='-'){i++;sign=-1;}
    for(;i<v.size;i++){
        ret = ret * 10 +(v.data[i]-'0');
    }
    return ret*sign;
}
inl int64_t view2dec_dot(view_t v, int dot){

    int64_t base = 0;
	char sign=1;
	int i=0;
	if(v.data[0]=='-'){i++;sign=-1;}
	if(dot-i>ELEM_FLAG_DEC_EXP_SIZE_LEFT || (dot>(ELEM_FLAG_DEC_EXP_SIZE_LEFT-1)&&v.data[i]=='9')){ //Max value guard
		return 9*ELEM_FLAG_DEC_MAX *sign;//Set a maximum value
	}
	
    for(;i<v.size && ((i-dot)<=ELEM_FLAG_DEC_EXP_SIZE);i++){
		if(v.data[i] != '.'){
			base = base * 10 +(v.data[i]-'0');
		}
    }

	int64_t exp = 1;
	for(int j=0;j<(v.size-dot-1-(v.size-i));j++){ //Truncate to ELEM_FLAG_DEC_EXP_SIZE
		exp*=10;
	}
    return (base*(ELEM_FLAG_DEC_EXP/exp) )*sign;
}


inl void Nod_Extact_Val_Special(Nod* _this, view_t key, view_t val) {
	//special val: BOOL, DECIMAL, INTEGER, NULL

	//Trim end
	for(int i=0;i<val.size;i++){
		if(val.data[i] <= 32){
			val.size=i;
		}
	}
	//
	if(compareV(val, S("false"))==0){
		Dictionary_set_bool(&_this->dic, key, false);return;
	}
	if(compareV(val, S("true"))==0){
		Dictionary_set_bool(&_this->dic, key, true);return;
	}
	if(compareV(val, S("null"))==0){
		Dictionary_set_null(&_this->dic, key);return;
	}
	if((val.data[0]>='0'&&val.data[0]<='9') || (val.data[0]=='-'&&val.data[1]>='0'&&val.data[1]<='9')){
		//It's a number
		int dot = 0;
		for(int i=0;i<val.size;i++){
			if(val.data[i]=='.'){
				dot=i;break;
			}
		}
		if(dot){
			//DECIMAL
			Dictionary_set_dec(&_this->dic, key, view2dec_dot(val,dot));return;
		}else{
			//INTEGER
			Dictionary_set_int(&_this->dic, key, view2int(val));return;
		}
	}
	//UNKWOW => to string
	Dictionary_set_str(&_this->dic, key, val);
}

inl Nod* Nod_Let_NewNodElem(Nod* _this, view_t key){

	int32_t idx = Dictionary_find_idx(&_this->dic,key);
	if(idx < 0){
		idx=Dictionary_add(&_this->dic, key);
		
		Idx_t* e = &_this->dic.list.data[idx];

		Nod_alloc(new_anod, _this->mem);
		Dictionary_modified_ptr(&_this->dic, e, new_anod);
		return new_anod;
	};
	return (Nod*)Dictionary_elem_get_ptr(&_this->dic.list.data[idx]);
}

inl void Nod_Extact_Val(Nod* _this, view_t key, view_t val) {
	
	for(int i=0; i < val.size; i++){
		char c = val.data[i];if(c > 32){ //Valid char
		if(c=='{'){
			Nod_Exact_SubBracket(Nod_Let_NewNodElem(_this, key), val);
			break;
		}else
		if(c=='['){
			Nod_Exact_SubArray(Nod_Let_NewNodElem(_this, key), val);
			break;
		}else 
		if(c==34){//quote
			for(int j=i+1; j < val.size; j++){
				if(val.data[j]==34){i++;//end quote
					val.data=&val.data[i];
					val.size=j-i;
					Dictionary_set_str(&_this->dic, key, val);
					break;
				}
			}
			break;
		}else{
			//special val: BOOL, DECIMAL, INTEGER, NULL
			val.size=val.size-i;
			val.data=&val.data[i];
			Nod_Extact_Val_Special(_this, key, val);
			break;
		}
	}}
}


//// SubBracket ---
inl void Nod_Extact_Elements_SubBracket_KeyVal(Nod* _this, view_t elem) {

	bool inside_quote = false;
	view_t key = (view_t){};
	for(int i = 0; i < elem.size; i++){
		if(elem.data[i] == 34){//quote
			if(!inside_quote){
				key.data=&elem.data[i+1];
			}else{
				key.size=(&elem.data[i] - key.data);
			}
			inside_quote=!inside_quote;
		}
		if(!inside_quote && elem.data[i] == ':'){i++;
			view_t val = (view_t){.data=&elem.data[i],.size=elem.size-i};
			Nod_Extact_Val(_this, key, val);
			break;
		}
	}
}
inl Nod* Nod_Extact_Elements_SubBracket(Nod* _this, view_t src) {
	int deep = 0;int s = 0;int idx = 0;bool inside_quote = false;
	
	for ( idx = 0; idx < src.size;idx++) {char curr=src.data[idx];
		if (curr == 34 ) {inside_quote = !inside_quote;} //Double quote switch "
		if (!inside_quote && deep == 0 && curr==',') {
			view_t elem = (view_t){.data=&src.data[s],.size=(size_t)idx-s};
			Nod_Extact_Elements_SubBracket_KeyVal(_this, elem);
			s = idx+1;
		}
		if (curr == '{' || curr == '[') {deep++;}
		if (curr == '}' || curr == ']') {deep--;}
	}
	if (deep == 0 ) {
		view_t elem = (view_t){.data=&src.data[s],.size=(size_t)idx-s};
		Nod_Extact_Elements_SubBracket_KeyVal(_this, elem);
	}
return 0;
}

inl Nod* Nod_Exact_SubBracket(Nod* _this, view_t json) {
	view_t result = Nod_ExtactBracket(json, '{','}');
	 Nod_Extact_Elements_SubBracket(_this, result);
	return 0;
}
//// --- SubBracket

//// SubArray ---
inl void Nod_Extact_Elements_SubArray_KeyVal(Nod* _this, view_t elem, int idx) {

	char str_num[11];//max (-)2147483647...
	_sprintf(str_num,"%d",idx);

	bool inside_quote = false;
	view_t key = (view_t){.data=str_num,.size=strlen(str_num)};

	Nod_Extact_Val(_this, key, elem);
}
inl Nod* Nod_Extact_Elements_SubArray(Nod* _this, view_t src) {
	int deep = 0;int s = 0;int idx = 0;int num = 0;bool inside_quote = false;
	
	for ( idx = 0; idx < src.size;idx++) {char curr=src.data[idx];
		if (curr == 34 ) {inside_quote = !inside_quote;} //Double quote switch "
		if (!inside_quote && deep == 0 && curr==',') {
			view_t elem = (view_t){.data=&src.data[s],.size=(size_t)idx-s};
			Nod_Extact_Elements_SubArray_KeyVal(_this, elem,num);
			s = idx+1;num++;
		}
		if (curr == '{' || curr == '[') {deep++;}
		if (curr == '}' || curr == ']') {deep--;}
	}
	if (deep == 0 ) {
		view_t elem = (view_t){.data=&src.data[s],.size=(size_t)idx-s};
		Nod_Extact_Elements_SubArray_KeyVal(_this, elem,num);
	}
return 0;
}
inl Nod* Nod_Exact_SubArray(Nod* _this, view_t json) {
	view_t result = Nod_ExtactBracket(json, '[',']');
	 Nod_Extact_Elements_SubArray(_this, result);
	return 0;
}
//// --- SubArray

 
inl Nod* Nod_Parse_JSON(Nod* _this, view_t json) {
	Nod* array = Nod_Exact_SubBracket(_this, json); //\r', ' '? or juste remove it? //Todo better/faster cleaning (How to get line #?)
	//parsed = true;
	return array;
}

#endif

