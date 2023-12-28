#ifndef SHADER_DRAW_H
#define SHADER_DRAW_H
#include "jit.h"
#include "sokol_gfx.h"
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

#ifndef NUM_TYPE
#define NUM_TYPE
typedef float num;
#endif

#define DefaultVsParam(_vs_params, _shp)\
_vs_params.iMouse[0]=       _j->shp.iMouse.x;\
_vs_params.iMouse[1]=       _j->shp.iMouse.y;\
_vs_params.iResolution[0]=  _j->shp.iResolution.x;\
_vs_params.iResolution[1]=  _j->shp.iResolution.y;\
_vs_params.iResolution[2]=  _j->shp.iResolution.z;\
_vs_params.iTime=           _j->shp.iTime;\
_vs_params.iTimeDelta=      _j->shp.iTimeDelta;


// Quaternion
typedef struct quaternion {
         num x;
         num y;
         num z;
         num w;
}quaternion;
inl quaternion quaternion_new() {
   return (quaternion){
      .x = 0,
      .y = 0,
      .z = 0,
      .w = 1,
   };
}
inl quaternion quaternion_roll(quaternion _, num _angle)
{
   num _rw = cos(0.5 * _angle) ;
   num _rz = sin(0.5 * _angle);

   num _tx = (_.x * _rw)  - (_.y * _rz);
   num _tz = (_.z * _rw)  + (_.w * _rz);

   return (quaternion){
      .y    = (_.y * _rw)  + (_.x * _rz),
      .w    = (_.w * _rw)  - (_.z * _rz),
      .x = _tx,
      .z = _tz,
   };
}
inl quaternion quaternion_yaw(quaternion _, num _angle)
{
   num _rw =  cos(0.5 * _angle) ;
   num _ry =  sin(0.5 * _angle);

   num _tx = (_.x * _rw) + (_.z * _ry);
   num _ty = (_.y * _rw) + (_.w * _ry);

   return (quaternion){
      .z = (_.z * _rw) - (_.x * _ry),
      .w = (_.w * _rw) - (_.y * _ry),
      .x = _tx,
      .y = _ty,
   };
}
inl quaternion quaternion_pitch(quaternion _, num _angle)
{
   num _rw =  cos(0.5 * _angle);
   num _rx =  sin(0.5 * _angle);

   num _tx  = (_.x * _rw) + (_.w * _rx);
   num _ty  = (_.y * _rw) - (_.z * _rx);

   return (quaternion){
      .z           = (_.z * _rw) + (_.y * _rx),
      .w            = (_.w * _rw) - (_.x * _rx),
      .x = _tx,
      .y = _ty,
   };
}
inl quaternion quaternion_combine(quaternion _, quaternion _o)
{
   num _tx = (_.x * _o.w) + (_.w * _o.x) + (_.z * _o.y) - (_.y * _o.z);
   num _ty = (_.y * _o.w) + (_.w * _o.y) + (_.x * _o.z) - (_.z * _o.x);
   num _tz = (_.z * _o.w) + (_.w * _o.z) + (_.y * _o.x) - (_.x * _o.y);

   return (quaternion){
      .w    = (_.w * _o.w) - (_.x * _o.x) - (_.y * _o.y) - (_.z * _o.z),
      .x    = _tx,
      .y    = _ty,
      .z    = _tz,
   };
}
inl quaternion quaternion_inverse(quaternion _) {
   return quaternion_combine(_, (quaternion){.x = 0,.y = 0,.z = -1,.w = 1,});
}

//// angle
inl num loopAngle(num _angle) {
   if (_angle < -3.14159265)
      _angle += 6.28318531;
   else
   if (_angle >  3.14159265)
      _angle -= 6.28318531;

   return _angle;
}
inl num diffAngle(num _nAngle1, num _nAngle2) {
   num _nTurn = _nAngle1 + 3.14159265 - _nAngle2;
   if (_nTurn >  3.14159265)
      _nTurn -= 6.28318531;
   _nTurn = _nTurn - 3.14159265;
   if (_nTurn < 0) {
      _nTurn *= -1;
   }
   if (_nTurn >  3.14159265){
      _nTurn = _nTurn - 6.28318531;
   }
   return _nTurn;
}
inl num lawOfCosines(num a, num b, num c) {
   num val = (a*a + b*b - c*c) / (2 * a * b);
   //if(val> 1)val=1;
   //if(val< -1)val=-1;
   return acos(val);
}


inl num mul(vec3 pt1, vec3 pt2){
   return pt1.x*pt2.x+pt1.y*pt2.y+pt1.z*pt2.z;
}

inl vec3 substract(vec3 pt1, vec3 pt2){
   return (vec3){
      .x=pt1.x-pt2.x,
      .y=pt1.y-pt2.y,
      .z=pt1.z-pt2.z,
   };
}

inl vec3 add(vec3 pt1, vec3 pt2){
   return (vec3){
      .x=pt1.x+pt2.x,
      .y=pt1.y+pt2.y,
      .z=pt1.z+pt2.z,
   };
}


inl vec3 cross(vec3 _, vec3 _o)
{
   num _tx  =    (_.z * _o.y )   - (_.y * _o.z);
   num _ty  =    (_.x * _o.z )    - (_.z * _o.x);
   return(vec3){
      .z =    (  _.y * _o.x )    - (  _.x * _o.y),
      .x = _tx,
      .y = _ty,
   };
}

inl vec3 rev_cross(vec3 _, vec3 _o)
{
   num _ty =    (_o.x * _.z ) - (_o.z * _.x);
   num _tx =    (_o.z * _.y ) - (_o.y * _.z);
   return(vec3){
      .z =      (_o.y * _.x ) - (_o.x * _.y),
      .x = _tx,
      .y = _ty,
   };
}

inl vec3 rotate(vec3 _, quaternion _q)
{
   num _tx  = _.x;
   num _ty  = _.y;
   num _tz =  _.z;

   _=cross(_,(vec3){.x=_q.x,.y=_q.y,.z=_q.z});

   _.x += _tx * _q.w;
   _.y += _ty * _q.w;
   _.z += _tz * _q.w;

   _=cross(_,(vec3){.x=_q.x,.y=_q.y,.z=_q.z});

   _.x = 2 * _.x + _tx;
   _.y = 2 * _.y + _ty;
   _.z = 2 * _.z + _tz;

   return _;
}
//
inl vec3 move_loop2d(vec3 pt, vec3 ptMov, num _bound){
   num x = pt.x+ptMov.x;
   num y = pt.y+ptMov.y;
   if( ptMov.x < 0)
   { //left dir
      if(x<0)x+=_bound;
   }else
   { //right dir
      if(x>_bound)x-=_bound;
   }
   if( ptMov.y < 0)
   { //left dir
      if(y<0)y+=_bound;
   }else
   { //right dir
      if(y>_bound)y-=_bound;
   }

   return (vec3){
      .x=x,
      .y=y,
      .z=pt.z,
   };
}

//////////
inl vec3 middle2d(vec3 _pt1, vec3 _pt2) {
   return (vec3){
      .x = (_pt1.x + _pt2.x)/2,
      .y = (_pt1.y + _pt2.y)/2,
      .z = _pt1.z,
   };
}
inl vec3 middle(vec3 _pt1, vec3 _pt2) {
   return (vec3){
      .x = (_pt1.x + _pt2.x)/2,
      .y = (_pt1.y + _pt2.y)/2,
      .z = (_pt1.z + _pt2.z)/2,
   };
}
inl vec3 proportion(vec3 _pt2, vec3 _pt3, num _prop) {
   return (vec3){
   .x = _pt3.x + ((_pt2.x - _pt3.x)*_prop),
   .y = _pt3.y + ((_pt2.y - _pt3.y)*_prop),
   .z = _pt3.z + ((_pt2.z - _pt3.z)*_prop),
   };
}
inl num fbarycentre(num a, num b, num c, num t) {
   num t2 = 1-t;
   return t2*t2*a + 2*t2*t*b + t*t*c;
}
inl vec2 barycentre2v( num _nDist , vec2 _pt1, vec2 _pt2, vec2  _pt3 ) {
   return (vec2){
      .x = fbarycentre(_pt1.x, _pt2.x, _pt3.x, _nDist),
      .y = fbarycentre(_pt1.y, _pt2.y, _pt3.y, _nDist),
   };
}

inl num distance2v( vec2 _pt1, vec2 _pt2 ) {
   num x  = _pt1.x - _pt2.x;
   num y  = _pt1.y - _pt2.y;
   return sqrt(x*x+ y*y);
}
inl num distance( vec3 _pt1, vec3 _pt2 ) {//distance2d?
   num x  = _pt1.x - _pt2.x;
   num y  = _pt1.y - _pt2.y;
   return sqrt(x*x+ y*y);
}
inl vec3 posS( vec3 _pt, num _angle, num _distance) {//posS
   return (vec3){
      .x = _pt.x + sin(_angle) * _distance,
      .y = _pt.y + cos(_angle) * _distance,
      .z = _pt.z,
   };
}
inl vec3 posC( vec3 _pt, num _angle, num _distance) {
   return (vec3){
      .x = _pt.x + cos(_angle) * _distance,
      .y = _pt.y + sin(_angle) * _distance,
      .z = _pt.z,
   };
}
inl vec2 posS2v( vec2 _pt, num _angle, num _distance) {
   return (vec2){
      .x = _pt.x + sin(_angle) * _distance,
      .y = _pt.y + cos(_angle) * _distance,
   };
}
inl vec2 posC2v( vec2 _pt, num _angle, num _distance) {
   return (vec2){
      .x = _pt.x + cos(_angle) * _distance,
      .y = _pt.y + sin(_angle) * _distance,
   };
}

//
inl num angle2v(vec2 _pt1, vec2 _pt2) {
   return atan2(_pt1.x - _pt2.x,_pt1.y - _pt2.y);
}
inl num angle(vec3 _pt1, vec3 _pt2) {
   return atan2(_pt1.x - _pt2.x,_pt1.y - _pt2.y);
}
inl num angle90(vec3 _pt1, vec3 _pt2, num _dir) {
   return atan2(_pt1.x - _pt2.x,_pt1.y - _pt2.y)+ R90*_dir;
}
//get angles of triangles, usefull for bones
inl vec2 tangles(vec2 _origin, vec2 _traget, vec3 _triside) //_triside = length of ech tri size, z is the dist
{
   num dist= distance2v(_origin, _traget);
   num d1 = angle2v(_origin, _traget);
   num d2 = lawOfCosines(_triside.z, _triside.x, _triside.y);
   num a1 = d1+d2;
   num a2 = lawOfCosines(_triside.x, _triside.y, _triside.z);
   return (vec2){a1, a2};
}
inl num tangle_P(vec2 _origin, vec2 _traget, vec3 _triside) //_triside = length of ech tri size, z is the dist
{
   num dist= distance2v(_origin, _traget);
   num d1 = angle2v(_origin, _traget);
   num d2 = lawOfCosines(_triside.z, _triside.x, _triside.y);
   num a1 = d1+d2;
   return a1;
}
inl num tangle(vec3 _origin, vec3 _traget, vec3 _triside) //_triside = length of ech tri size, z is the dist
{
   num dist= distance(_origin, _traget);
   num d1 = angle(_origin, _traget);
   num d2 = lawOfCosines(_triside.z, _triside.x, _triside.y);
   num a1 = d1+d2;
   return a1;
}
////
#define vt _->vertex[_->v]
static void draw_tri2(drawlist_t* _, vec3 _p1, vec3 _pT, vec3 _p2, vec3 _pdir1, vec3 _pdir2, num _dim){
   //TODO
   //dbg_assert(_->t+2<sizeof(gbl_tx),"tx overflow");
   //dbg_assert(_->v+4<sizeof(gbl_vertex),"vertex overflow");

   vec3 _pTM = proportion(_pdir1, _pdir2, 0.5);
   vec3 _pB = proportion(_p1, _p2, 0.5);

   //_->tx[_->t]=(tx_t){_->v+0, _->v+1, _->v+2};_->t++;
   _->tx[_->t]=(tx_t){_->v+0, _->v+1, _->v+3};_->t++;
   _->tx[_->t]=(tx_t){_->v+3, _->v+1, _->v+2};_->t++;

   vt.id.a=1;vt.pt=(vec4){_p1.x,_p1.y,_p1.z, 1.0};vt.depth=(vec2){0,_dim};vt.lookat=(vec4){_pdir1.x,_pdir1.y,_pdir1.z,     1.0};_->v++;//0
   vt.id.a=1;vt.pt=(vec4){_pT.x,_pT.y,_pT.z, 1.0};vt.depth=(vec2){0,_dim};vt.lookat=(vec4){_pTM.x,   _pTM.y,   _pTM.z,         1.0};_->v++;//1
   vt.id.a=1;vt.pt=(vec4){_p2.x,_p2.y,_p2.z, 1.0};vt.depth=(vec2){0,_dim};vt.lookat=(vec4){_pdir2.x,_pdir2.y,_pdir2.z,    1.0};_->v++;//2
   //2 sided tri
   vt.id.a=1;vt.pt=(vec4){_pB.x,_pB.y,_pB.z,  1.0};vt.depth=(vec2){1,_dim};vt.lookat=(vec4){_pT.x,   _pT.y,   _pT.z,         1.0};_->v++;//3
}

//TODO revers _pBL / _pBR
static void draw_quad4(drawlist_t* _, vec3 _pTL, vec3 _pTR, vec3 _pBR, vec3 _pBL, int _type_id)
{
   vec3 _pTM = proportion(_pTL, _pTR,0.5);
   vec3 c1 = proportion(_pTL, _pBR,0.5);//0.4 motif!
   vec3 c2 = proportion(_pTR, _pBL,0.5);//0.4 motif!

   vec3 c = proportion(c1, c2,0.5);
   vec3 _pCe = (vec3){c.x,c.y,c.z};

   float _dim = 50;


   _->tx[_->t]=(tx_t){_->v+0, _->v+1, _->v+4};_->t++;
   _->tx[_->t]=(tx_t){_->v+1, _->v+2, _->v+4};_->t++;
   _->tx[_->t]=(tx_t){_->v+2, _->v+3, _->v+4};_->t++;
   _->tx[_->t]=(tx_t){_->v+3, _->v+0, _->v+4};_->t++;
   
   vt.id.a=1;vt.id.b=_type_id;vt.pt=(vec4){_pTL.x,_pTL.y,_pTL.z, 1.0};vt.depth=(vec2){0.0, _dim};vt.lookat=(vec4){_pBL.x,_pBL.y,_pBL.z, 1.0};_->v++;//0
   vt.id.a=1;vt.id.b=_type_id;vt.pt=(vec4){_pTR.x,_pTR.y,_pTR.z, 1.0};vt.depth=(vec2){0.0, _dim};vt.lookat=(vec4){_pBR.x,_pBR.y,_pBR.z, 1.0};_->v++;//1
   vt.id.a=1;vt.id.b=_type_id;vt.pt=(vec4){_pBR.x,_pBR.y,_pBR.z, 0.0};vt.depth=(vec2){1.0, _dim};vt.lookat=(vec4){_pTR.x,_pTR.y,_pTR.z,-1.0};_->v++;//2
   vt.id.a=1;vt.id.b=_type_id;vt.pt=(vec4){_pBL.x,_pBL.y,_pBL.z, 0.0};vt.depth=(vec2){1.0, _dim};vt.lookat=(vec4){_pTL.x,_pTL.y,_pTL.z,-1.0};_->v++;//3
   vt.id.a=1;vt.id.b=_type_id;vt.pt=(vec4){_pCe.x,_pCe.y,_pCe.z, 0.5};vt.depth=(vec2){0.5, _dim};vt.lookat=(vec4){_pTM.x,_pTM.y,_pTM.z,-1.0};_->v++;//4 TODO Z
}
/////////////////////////
static num zxHRes = 900;
static void draw_quad_m(drawlist_t* _, vec3 p_TL, vec3 p_TR, vec3 p_BL, vec3 p_BR,  int _type_id)
{
   vec3 a = p_TR;
   vec3 b = p_TL;

   vec3 c = p_BR;
   vec3 d = p_BL;

   a.z=1.0-(a.z/(zxHRes/1.0));
   b.z=1.0-(b.z/(zxHRes/1.0));

   c.z=1.0-(c.z/(zxHRes/1.0));
   d.z=1.0-(d.z/(zxHRes/1.0));

   //a.x=a.x/iHRes.x;
   //a.y=a.y/iHRes.y;
   //a.z=(a.z/iHRes.z);
   //a.y*=-1;

   //b.x=b.x/iHRes.x;
   //b.y=b.y/iHRes.y;
   //b.z=(b.z/iHRes.z);
   //b.y*=-1;
     /*
      a.x/=(1.0-a.z);
      a.y/=(1.0-a.z);

      b.x/=(1.0-b.z);
      b.y/=(1.0-b.z);
      */
      a.x/=(a.z);
      a.y/=(a.z);

      b.x/=(b.z);
      b.y/=(b.z);


      c.x/=(c.z);
      c.y/=(c.z);

      d.x/=(d.z);
      d.y/=(d.z);

      //a.y*=-1;
      //b.y*=-1;


   if( a.x > b.x  || c.x > d.x ){
   //   return;
   }

   vec3 ori = cross(substract(a,b), substract(c, b));
   if( ori.z>0 ){
      return;
   }

   vec3 m1 = middle(p_TR, p_BR);
   vec3 m2 = middle(p_TL, p_BL);
   //Head
   draw_quad4(_,   p_TL,       p_TR,
            m1,       m2, _type_id      
        );

   draw_quad4(_,   p_BL ,     p_BR,
            m1,       m2, _type_id    
     );
}
////
static void draw_quad4_2(drawlist_t* _, vec3 _pTL, vec3 _pTR, vec3 _pBR, vec3 _pBL, int _type_id)
{
/*
   vec3 _pTM = proportion(_pTL, _pTR,0.5);
   vec3 _pBM = proportion(_pBL, _pBR,0.5);
   vec3 c1 = proportion(_pTL, _pBR,0.5);//0.4 motif!
   vec3 c2 = proportion(_pTR, _pBL,0.5);//0.4 motif!

   vec3 c = proportion(c1, c2,0.5);
   vec3 _pCe = (vec3){c.x,c.y,c.z};

   float _dim = 50;

*/
   float _dim = 50;
   
   _->tx[_->t]=(tx_t){_->v+0, _->v+1, _->v+3};_->t++;
   _->tx[_->t]=(tx_t){_->v+1, _->v+2, _->v+3};_->t++;
   //_->tx[_->t]=(tx_t){_->v+2, _->v+3, _->v+4};_->t++;
   //_->tx[_->t]=(tx_t){_->v+3, _->v+0, _->v+4};_->t++;
   
   vt.id.a=1;vt.id.b=_type_id;vt.pt=(vec4){_pTL.x,_pTL.y,_pTL.z, 1.0};vt.depth=(vec2){0.0, _dim};vt.lookatV=(vec4){_pTR.x,_pTR.y,_pTR.z, 1.0};vt.lookat=(vec4){_pBL.x,_pBL.y,_pBL.z, 1.0};_->v++;//0
   vt.id.a=1;vt.id.b=_type_id;vt.pt=(vec4){_pTR.x,_pTR.y,_pTR.z, 1.0};vt.depth=(vec2){0.0, _dim};vt.lookatV=(vec4){_pTL.x,_pTL.y,_pTL.z, 1.0};vt.lookat=(vec4){_pBR.x,_pBR.y,_pBR.z, 1.0};_->v++;//1
   vt.id.a=1;vt.id.b=_type_id;vt.pt=(vec4){_pBR.x,_pBR.y,_pBR.z, 0.0};vt.depth=(vec2){1.0, _dim};vt.lookatV=(vec4){_pBL.x,_pBL.y,_pBL.z,-1.0};vt.lookat=(vec4){_pTR.x,_pTR.y,_pTR.z,-1.0};_->v++;//2
   vt.id.a=1;vt.id.b=_type_id;vt.pt=(vec4){_pBL.x,_pBL.y,_pBL.z, 0.0};vt.depth=(vec2){1.0, _dim};vt.lookatV=(vec4){_pBR.x,_pBR.y,_pBR.z,-1.0};vt.lookat=(vec4){_pTL.x,_pTL.y,_pTL.z,-1.0};_->v++;//3
 //  vt.id.a=1;vt.id.b=_type_id;vt.pt=(vec4){_pCe.x,_pCe.y,_pCe.z, 0.5};vt.depth=(vec2){0.5, _dim};vt.lookatV=(vec4){_pBM.x,_pBM.y,_pBM.z,-1.0};vt.lookat=(vec4){_pTM.x,_pTM.y,_pTM.z,-1.0};_->v++;//4 TODO Z
}
static void draw_quad_m2(drawlist_t* _, vec3 p_TL, vec3 p_TR, vec3 p_BL, vec3 p_BR,  int _type_id)
{
   vec3 a = p_TR;
   vec3 b = p_TL;

   vec3 c = p_BR;
   vec3 d = p_BL;

   a.z=1.0-(a.z/(zxHRes/1.0));
   b.z=1.0-(b.z/(zxHRes/1.0));

   c.z=1.0-(c.z/(zxHRes/1.0));
   d.z=1.0-(d.z/(zxHRes/1.0));


      a.x/=(a.z);
      a.y/=(a.z);

      b.x/=(b.z);
      b.y/=(b.z);


      c.x/=(c.z);
      c.y/=(c.z);

      d.x/=(d.z);
      d.y/=(d.z);




   vec3 ori = cross(substract(a,b), substract(c, b));
   if( ori.z>0 ){
   //   return;
   }



   //Head
   draw_quad4_2(_,   p_TR ,     p_TL  ,
           p_BL ,      p_BR , _type_id      
        );

}
static void draw_quad_w(drawlist_t* _, vec3 p_TL, vec3 p_TR, vec3 p_BL, vec3 p_BR, int _type_id)
{
   draw_quad_m(_, p_TR, p_TL, p_BR, p_BL, _type_id);
}

static void draw_rquad4(drawlist_t* _, vec3 p_TL, vec3 p_TR, vec3 p_BL, vec3 p_BR, int _type_id)
{
   draw_quad4(_,   p_TL,   p_TR,
            //p_BR,    p_BL ,  _cTL,_cTR,_cBR,_cBL
            p_BR,    p_BL, _type_id 
        );
}

////////////////////////
/// LINES (Manga style)
///////////////////////

static vec2 line_drawHalf(drawlist_t* _, int _idx, vec3 _pt1, vec3 _pt2, num _width, vec4 _col, num _prev_angle, num _angle, num _next_angle ,vec2 _last_pt, bool _open, num _dim){
  //TODO dbg_assert
  // dbg_assert(_->t+2<sizeof(gbl_tx),"tx overflow");
  // dbg_assert(_->v+4<sizeof(gbl_vertex),"vertex overflow");

   _->tx[_->t]=(tx_t){_->v+0, _->v+1, _->v+2};_->t++;
   _->tx[_->t]=(tx_t){_->v+0, _->v+2, _->v+3};_->t++;

   vec2 _pw1;
   if(_prev_angle < -90){//not disabled
      _pw1 =   posS2v((vec2){_pt1.x,_pt1.y}, _angle+R90, _width);
   }else{
      _pw1 = _last_pt;
   }

   vec2 _pw2;
   _pw2=posS2v((vec2){_pt2.x,_pt2.y}, _angle+R90, _width);

   if(_open){
      _pw2=posS2v((vec2){_pt2.x,_pt2.y}, _angle+R90, _width);
   }else{
      _pw2 = posS2v((vec2){_pt2.x,_pt2.y}, _angle+diffAngle(_angle, _next_angle)/2, _width);
   }

   vec2 pt_dist =  posS2v((vec2){_pt2.x,_pt2.y}, _angle+diffAngle(_angle, _next_angle)/2 , _width);

   //_pw2 = posR((Point){_pt2.x,_pt2.y},  (_angle2 - _angle1)/2, _width);
   //_pw2 = posS_P((Point){_pt2.x,_pt2.y}, _angle+diffAngle(_angle, _next_angle)/2, _width);

   //fSetCapR
   //ID3 = Lines
   vt.id.a=3;vt.pt=(vec4){_pw1.x,_pw1.y,_pt1.z, 1.0};vt.depth=(vec2){0,_dim};vt.lookat=(vec4){_pt1.x,_pt1.y,_pt1.z, 1.0};_->v++;//0
   vt.id.a=3;vt.pt=(vec4){_pw2.x,_pw2.y,_pt2.z, 1.0};vt.depth=(vec2){0,_dim};vt.lookat=(vec4){_pt2.x,_pt2.y,_pt2.z, 1.0};_->v++;//1
   vt.id.a=3;vt.pt=(vec4){_pt2.x,_pt2.y,_pt2.z, 0.0};vt.depth=(vec2){1,_dim};vt.lookat=(vec4){_pw2.x,_pw2.y,_pt2.z,-1.0};_->v++;//2
   vt.id.a=3;vt.pt=(vec4){_pt1.x,_pt1.y,_pt1.z, 0.0};vt.depth=(vec2){1,_dim};vt.lookat=(vec4){_pw1.x,_pw1.y,_pt1.z,-1.0};_->v++;//3

   return _pw2;
}
#undef vt

static void line_add(drawlist_t* _, vec3 _pt){
  // dbg_assert(_->l+1<sizeof((jit_t*)0)->line,"line overflow");
   _->line[_->l]=_pt;_->l++;
}
static void line_end(drawlist_t* _, vec4 _col,  num _width, bool _close){
   //_close=true;//TODO?
   if(_->l<=1)return;
   num a_ = -100;
   num a = -100;
   num _a = -100;
   vec2 _last1;
   vec2 _last2;

   if(_close ){
      dbg_assert(_->l+1<sizeof((drawlist_t*)0)->line,"line overflow");
      _->line[_->l]=_->line[0];_->l++;
      _->line[_->l]=_->line[1];_->l++;
   }else{
      _->l++;
   }

   for(int i=1; i< _->l-1; i++){

      vec3 _pt1 = _->line[i-1];
      vec3 _pt2 = _->line[i];
      vec3 _pt3 = _->line[i+1];

      a_ = a;
      a = angle((vec3){_pt2.x, _pt2.y}, (vec3){_pt1.x, _pt1.y});
      _a = angle((vec3){_pt2.x, _pt2.y}, (vec3){_pt3.x, _pt3.y});

      _last1 = line_drawHalf(_, i,_pt1, _pt2, _width,    _col,  a_, a, _a, _last1, (i==_->l-2), -1.0);
      //_last2 = line_drawHalf(_, _pt1, _pt2, _width*-1, _col, a_, a, _a, _last2, (i==_->l-2));
   }
   //last
   //_oLine2_->oHLine1.oPtBL =   _oLine1_->oHLine1.oPtBR;
   //_oLine2_->oHLine2.oPtBL =   _oLine1_->oHLine2.oPtBR;

   _->l=0;
}


#endif //SHADER_DRAW_H
