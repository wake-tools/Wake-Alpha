#define AUDIO_VEC2(func)\
{\
static float audio_time = 0;\
float step = 1.0/44100.0;\
vec2* buf = (vec2*)_j->audio_stream;\
for(int i=0;i<_j->audio_samples_count;i++){\
   audio_time+=step;\
   vec2 data = func(audio_time);\
   ((vec2*)(_j->audio_stream))[i].x += data.x;\
   ((vec2*)(_j->audio_stream))[i].y += data.y;\
}}