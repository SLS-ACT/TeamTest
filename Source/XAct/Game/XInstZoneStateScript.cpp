/*
** Lua binding: XInstZoneState
** Generated automatically by tolua++-1.0.92 on 12/19/13 02:17:48.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_XInstZoneState_open (lua_State* tolua_S);

#include "XGameState.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"XInstZoneState");
 tolua_usertype(tolua_S,"XIZMap");
 tolua_usertype(tolua_S,"XGameSateBase");
}

/* method: ToIZState of class  XInstZoneState */
#ifndef TOLUA_DISABLE_tolua_XInstZoneState_XInstZoneState_ToIZState00
static int tolua_XInstZoneState_XInstZoneState_ToIZState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XInstZoneState",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"XGameSateBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XGameSateBase* pState = ((XGameSateBase*)  tolua_tousertype(tolua_S,2,0));
  {
   XInstZoneState* tolua_ret = (XInstZoneState*)  XInstZoneState::ToIZState(pState);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XInstZoneState");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ToIZState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMap of class  XInstZoneState */
#ifndef TOLUA_DISABLE_tolua_XInstZoneState_XInstZoneState_GetMap00
static int tolua_XInstZoneState_XInstZoneState_GetMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XInstZoneState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XInstZoneState* self = (XInstZoneState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMap'", NULL);
#endif
  {
   XIZMap* tolua_ret = (XIZMap*)  self->GetMap();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XIZMap");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetIZId of class  XInstZoneState */
#ifndef TOLUA_DISABLE_tolua_XInstZoneState_XInstZoneState_SetIZId00
static int tolua_XInstZoneState_XInstZoneState_SetIZId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XInstZoneState",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XInstZoneState* self = (XInstZoneState*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wIZId = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetIZId'", NULL);
#endif
  {
   self->SetIZId(wIZId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetIZId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetIZId of class  XInstZoneState */
#ifndef TOLUA_DISABLE_tolua_XInstZoneState_XInstZoneState_GetIZId00
static int tolua_XInstZoneState_XInstZoneState_GetIZId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XInstZoneState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XInstZoneState* self = (XInstZoneState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetIZId'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->GetIZId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetIZId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_XInstZoneState_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"XInstZoneState","XInstZoneState","XGameSateBase",NULL);
  tolua_beginmodule(tolua_S,"XInstZoneState");
   tolua_function(tolua_S,"ToIZState",tolua_XInstZoneState_XInstZoneState_ToIZState00);
   tolua_function(tolua_S,"GetMap",tolua_XInstZoneState_XInstZoneState_GetMap00);
   tolua_function(tolua_S,"SetIZId",tolua_XInstZoneState_XInstZoneState_SetIZId00);
   tolua_function(tolua_S,"GetIZId",tolua_XInstZoneState_XInstZoneState_GetIZId00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_XInstZoneState (lua_State* tolua_S) {
 return tolua_XInstZoneState_open(tolua_S);
};
#endif

