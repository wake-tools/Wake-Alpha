#ifndef HDEF_JIT
#define HDEF_JIT

#ifdef __TINYC__ 
   #ifndef TCC_NOJIT
   #include <tcclib.h>
   #endif
#endif
//#define D_Debug
#define DBG_NO_FATAL

#include "Xternal.h"
#include "FileNod.h"


#ifndef D_FILE_ID 
   #define D_FILE_ID jit_im_main
#endif
#define JIT_MAIN D_FILE_ID


#include <stdint.h>
/*
typedef struct cvec4{
	float a,r,g,b;
}vec4;
typedef struct pvec4{
	float x,y,z,w;
}vec4;
*/
typedef struct vec2{
	float x,y;
}vec2;
typedef struct vec3{
	float x,y,z;
}vec3;
typedef struct vec4{
   float x,y,z,w;
}vec4;

typedef struct u4v4 {
	//unsigned char s,t,p,q;
	unsigned char r,g,b,a;
	//unsigned char x,y,z,w;
}u4v4;

typedef struct i8v4 {
	int16_t s,t,p,q;
}i8v4;

typedef struct tx_t {
	uint16_t a;
	uint16_t b;
	uint16_t c;
}tx_t;

typedef struct vtx_t{
	vec4 pt;
	//vec4 color;  //w = percent
	vec4 lookat; //w = direction
   vec4 lookatV; //w = direction
	vec2 depth;  //x = gray, y=dim/length/Z
	u4v4 id;

}vtx_t;

    
typedef struct shader_param_t {
   vec4 iMouse;      // mouse pixel coords .xy: current (if MLB down), zw: click
   vec3 iResolution; // viewport resolution (in pixels)
   vec3 iOffset;     // viewport offset (in pixels)
   float iTime;      // shader playback time (in seconds)
   float iTimeDelta; // render time (in seconds)
   float iFrameRate; // shader frame rate
}shader_param_t;

typedef enum jit_event {
    //JIT_PREPARE     =(1<<0),
    EVENT_INI         =(1<<1),
    EVENT_LOOP        =(1<<2),
    EVENT_PREVIEW     =(1<<3),
    EVENT_PAUSED      =(1<<4),
    EVENT_OPEN        =(1<<5),

} jit_event;


typedef enum jit_state {
    JIT_CLOSE,
    JIT_PAUSED,
    JIT_INI,
    JIT_LOOP,
    JIT_DESTROY,
    JIT_ACTION,
} jit_state;

typedef struct drawlist_t {
   uint32_t v;
	uint32_t t;
	uint32_t l;
	vtx_t* vertex;
	tx_t* tx;
	vec3* line;
}drawlist_t;
static inline void drawlist_reset(drawlist_t* _){
	_->v=0;
	_->t=0;
	_->l=0;
}


typedef struct jit_t {

   view_t name;
   shader_param_t shp;
   enum jit_state state;
   enum jit_event event;
   Nod* nod;
   Nod* action;
   
   float* audio_stream;
   int audio_samples_count;
   int audio_channels_count;

   /*
	uint16_t v;
	uint16_t t;
	uint16_t l;
	vtx_t vertex[4096];
	tx_t tx[4096];
	vec3 line[4096];//make it smaller
   */
  // double time;
   uint64_t posxtime;
	
}jit_t;

/*
static inline void vertex_reset(jit_t* _){
	_->v=0;
	_->t=0;
	_->l=0;
}*/
/*
static inline void vertex_reset(drawlist_t* _){
	_->v=0;
	_->t=0;
	_->l=0;
}*/

struct sg_shader;
struct sg_shader get_module_shader(view_t _module_name, view_t _shader_name);
Nod* get_module_nod(view_t _name);


#endif
