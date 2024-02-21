
#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif
#ifndef PI2
#define PI2 6.283185307179586476925286766559f
#endif
#ifndef mod
#define mod(val1,val2) fmod(val1,val2) 
#endif
#ifndef sign
#define sign(a) ( ( (a) < 0 )  ?  -1   : ( (a) > 0 ) )
#endif

#define AUDIO_VEC2(func, volume)\
\
static double audio_time = 0;\
double step = 1.0/44100.0;\
vec2* buf = (vec2*)_j->audio_stream;\
for(int i=0;i<_j->audio_samples_count;i++){\
   audio_time+=step;\
   vec2 data = func(audio_time);\
   ((vec2*)(_j->audio_stream))[i].x += data.x*volume;\
   ((vec2*)(_j->audio_stream))[i].y += data.y*volume;\
}

