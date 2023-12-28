#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "float.h"
#include <math.h>
#ifndef PI
#define PI 3.141592653589793238462643383279502884L
#endif
#ifndef PI_2
#define PI_2 1.570796326794896619231321691639751442098584699687552910487
#define R90 PI_2
#endif
#ifndef PI_4
#define PI_4 0.785398163397448309615660845819875721049292349843776455
#endif

#define IM_COL32_R_SHIFT    0
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    16
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#define IM_COL32(R,G,B,A)    (((ImU32)(A)<<IM_COL32_A_SHIFT) | ((ImU32)(B)<<IM_COL32_B_SHIFT) | ((ImU32)(G)<<IM_COL32_G_SHIFT) | ((ImU32)(R)<<IM_COL32_R_SHIFT))

extern float fmodf(float x, float y);
extern void _print_float(char* name, float val);
jit_t* args;

#define KEY_UP 0x57//W
#define KEY_DOWN 0x53//S
#define KEY_LEFT 0x41
#define KEY_RIGHT 0x44

#ifndef NUM_TYPE
#define NUM_TYPE
typedef float num;
#endif

typedef ImVec2 Point;

typedef struct DrawCtx{
   ImDrawList* ctx;
   Point origin;
   Point camera;
   Point hsize;
   Point mouse;
   num zoom;
}DrawCtx;

//////////////////
///== GRAPH == ///
//////////////////
inl void drawSquare(DrawCtx*_, Point _pt, char* _name) {

   //ImDrawFlags_RoundCornersNone
   //mcRef.graphics.lineStyle(2, 0xE30202, 100, true, "none");
   num size = 5.0;
   ImDrawList_AddRect(_->ctx,   (ImVec2){   (_->origin.x+(_pt.x-size)*_->zoom),
                                 (_->origin.y+(_pt.y-size)*_->zoom)},
                        (ImVec2){   (_->origin.x+(_pt.x+size)*_->zoom),
                                 (_->origin.y+(_pt.y+size)*_->zoom)},
   IM_COL32(255, 0, 0, 128), 0, ImDrawFlags_RoundCornersNone, 3.0);
   
   ImDrawList_AddText_Vec2(_->ctx, (ImVec2){(_->origin.x+(_pt.x-size)*_->zoom),
                                 (_->origin.y+(_pt.y+size*1.5)*_->zoom)}, IM_COL32(255, 0, 0, 128), _name, 0);
}
inl void drawSquare_(DrawCtx*_, vec3 _pt, char* _name) {
   drawSquare(_, (Point){_pt.x, _pt.y},_name);
}


inl void drawRectFilled(DrawCtx*_, vec3 _pt, num _size_x, num _size_y, ImU32 _col) {

   ImDrawList_AddRectFilled(_->ctx,   (ImVec2){   (_->origin.x+(_pt.x-_size_x)*_->zoom),
                                 (_->origin.y+(_pt.y-_size_x)*_->zoom)},
                        (ImVec2){   (_->origin.x+(_pt.x+_size_y)*_->zoom),
                                 (_->origin.y+(_pt.y+_size_y)*_->zoom)},
  _col , 0, 0);
}



inl void drawCircle(DrawCtx*_, vec3 _pt, num _size, char* _name, ImU32 _col, num _th) {
  int circle_segments=20;// for(num s = _size; _size>0; s-=5){circle_segments++;}

 ImDrawList_AddCircle(_->ctx,  (ImVec2){   _->origin.x + _pt.x*_->zoom , 
                                          _->origin.y + _pt.y*_->zoom }, 
                                          _size*_->zoom,   
                                _col, circle_segments, _th);  
                                
}
//Primary Color
inl void moveto_(DrawCtx*_, vec3 _pt) {
   ImDrawList_PathClear(_->ctx);
   ImDrawList_PathLineTo(_->ctx, (ImVec2){   (_->origin.x+(_pt.x)*_->zoom),
                                 (_->origin.y+(_pt.y)*_->zoom)});
}
inl void lineto_(DrawCtx*_, vec3 _pt) {
   ImDrawList_PathLineTo(_->ctx, (ImVec2){   (_->origin.x+(_pt.x)*_->zoom),
                                 (_->origin.y+(_pt.y)*_->zoom)});
}
//Primary Color
inl void moveto(DrawCtx*_, Point _pt) {
   ImDrawList_PathClear(_->ctx);
   ImDrawList_PathLineTo(_->ctx, (ImVec2){   (_->origin.x+(_pt.x)*_->zoom),
                                 (_->origin.y+(_pt.y)*_->zoom)});
}
inl void lineto(DrawCtx*_, Point _pt) {
   ImDrawList_PathLineTo(_->ctx, (ImVec2){   (_->origin.x+(_pt.x)*_->zoom),
                                 (_->origin.y+(_pt.y)*_->zoom)});
}
inl void stopDraw_Shape(DrawCtx*_) {//ImU32 ImU32
   ImDrawList_PathFillConvex(_->ctx, IM_COL32(128,128,128,128));
}
inl void stopDraw_Lines(DrawCtx*_, ImU32 _col, num _thickness, ImDrawFlags _flags) {
   ImDrawList_PathStroke(_->ctx, IM_COL32(255,50,50,200), _flags, _thickness);
}
inl void stopDraw_dbg_Lines(DrawCtx*_) {
   ImDrawList_PathStroke(_->ctx, IM_COL32(255,50,50,200), 0, 2.0);
}
inl void drawLine(DrawCtx*_, vec3 _pt1, vec3 _pt2 ) {
   moveto_(_,_pt1);
   lineto_(_,_pt2);
   stopDraw_Lines(_,IM_COL32(255,50,50,200), 0, 2.0);
}


///--------------------------------------------------------------------------------------------
///==|IMGUI  OBJECTS|==
///============================================================================================

/// JSON VIEW
static int display_nod(Dictionary* _this, view_t _key, int _id){
   if(_this==0) return _id;
   igTableNextRow(0,0);
   igTableNextColumn();
   
   if(_this->list.size==0){
      igPushStyleColor_U32(ImGuiCol_Text, IM_COL32(150, 150, 100, 255));
   }else{
      igPushStyleColor_U32(ImGuiCol_Text, IM_COL32(250, 200, 20, 255));
   }

 	bool open = 
   igTreeNodeEx_Str(_key.data, ImGuiTreeNodeFlags_SpanFullWidth);
      igTableNextColumn();
      
      char nb_itm[15] = {};//Reini each time
      _sprintf((char*)&nb_itm, "[%d]", _this->list.size);
      igTextDisabled(nb_itm);
      igTableNextColumn();		
      
   igPopStyleColor(1); 
      
   if (open){
      for(size_t i=0;i<_this->list.size;i++){_id++;
         Idx_t* e = &_this->list.data[i];
         if(e!=0){   
            char* key = &_this->str.data[ e->kidx ]; 
            view_t gkey = (view_t){.data=key, .size=strlen(key)};
            view_t str_val = Dictionary_get(_this, gkey); //TODO optimize

            if(Dictionary_elem_is_ptr(e)){		
               igSetNextItemOpen(true, ImGuiCond_FirstUseEver);
               _id = display_nod((Dictionary*)e->xptr, gkey,_id);
				}else{
								
               igTableNextRow(0,0);
               igTableNextColumn();

               igTreeNodeEx_Str(gkey.data, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
               igTableNextColumn();
									      
               igPushID_Int(_id); // assign unique id
                  igSetNextItemWidth(-FLT_MIN);		 
                  //////
                  switch(e->flags){
                     case ELEM_FLAG_STR:
                        igInputText("", str_val.data,  str_val.size, 0,0,0);
                     break;
                     case ELEM_FLAG_INT:{
                        static const ImS64  s64_one_int = 1;
                        igInputScalar("",   ImGuiDataType_S64,  &e->xctm, &s64_one_int, 0,0,0);
                              
                     break;}
                     case ELEM_FLAG_DEC:{
                        #define max_buff_size 30
                        char buf[max_buff_size] = {};//Reini each time
                        _sprintf((char*)&buf, "%jd.%jd", Dictionary_elem_get_dec_base(e),Dictionary_elem_get_dec_fract(e));
                        for(int i = max_buff_size; i>0;i--){
                           char c = buf[i];
                           if(c=='.' || (c>='1' && c<='9')){break;}
                           buf[i]='\0';
                        }
                     
                        static const ImS64  s64_one_dec = ELEM_FLAG_DEC_EXP;
                        igInputScalar("",   ImGuiDataType_S64,  &e->xctm, &s64_one_dec, NULL, buf, 0);
                     break;}
                     case ELEM_FLAG_BOOL:{
                        static const ImS64  s64_max_int = 1;
                        static const ImS64  s64_min_int = 0;
                        char* bool_val = (char*)"false";
                        if(e->xctm != 0){
                           bool_val = (char*)"true";
                        }
                        igInputScalar("",ImGuiDataType_U64, &e->xctm, &s64_min_int, &s64_max_int, bool_val, 0);
                        
                     break;}
                     case ELEM_FLAG_NULL:{
                        static const ImS64  s64_max_int = 0;
                        static const ImS64  s64_min_int = 0;
                        char* null_val = (char*)"null";
                        igSliderScalar("",ImGuiDataType_U64, &e->xctm, &s64_min_int, &s64_max_int, null_val, 0);
                     break;}
                  }
                  /////
               igPopID();
            }
         }
      }
   igTreePop();}
    
   return _id;
}

/// JSON VIEW
static void display_table_nod(Nod* _nod, view_t _name){
   const float TEXT_BASE_WIDTH = 10;// ImGui::CalcTextSize("A").x;
   const float TEXT_BASE_HEIGHT = igGetTextLineHeightWithSpacing();

   static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
 
   if (igBeginTable("json_tbl", 4, flags, (ImVec2){}, 0)){
      // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
      igTableSetupColumn("Name", ImGuiTableColumnFlags_NoHide|ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 30.0f,0);
      igTableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, TEXT_BASE_WIDTH * 30.0f,0);
      igTableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 5.0f,0);
      igTableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 8.0f,0);
      igTableHeadersRow();
      
      if(_nod!=0){
         igSetNextItemOpen(true, ImGuiCond_FirstUseEver);
         display_nod(&_nod->dic, _name,0); 
      }
   igEndTable();}
}