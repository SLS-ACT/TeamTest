/*
** Lua binding: XCityState
** Generated automatically by tolua++-1.0.92 on 12/19/13 02:17:48.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_XCityState_open (lua_State* tolua_S);

#include "XGameState.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"XGameSateBase");
 tolua_usertype(tolua_S,"XMap");
 tolua_usertype(tolua_S,"XCityState");
 tolua_usertype(tolua_S,"pub;ic XGameSateBase");
}

/* method: ToCityState of class  XCityState */
#ifndef TOLUA_DISABLE_tolua_XCityState_XCityState_ToCityState00
static int tolua_XCityState_XCityState_ToCityState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XCityState",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"XGameSateBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XGameSateBase* pState = ((XGameSateBase*)  tolua_tousertype(tolua_S,2,0));
  {
   XCityState* tolua_ret = (XCityState*)  XCityState::ToCityState(pState);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XCityState");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ToCityState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMap of class  XCityState */
#ifndef TOLUA_DISABLE_tolua_XCityState_XCityState_GetMap00
static int tolua_XCityState_XCityState_GetMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XCityState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XCityState* self = (XCityState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMap'", NULL);
#endif
  {
   XMap* tolua_ret = (XMap*)  self->GetMap();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XMap");
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

/* method: SetMapId of class  XCityState */
#ifndef TOLUA_DISABLE_tolua_XCityState_XCityState_SetMapId00
static int tolua_XCityState_XCityState_SetMapId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XCityState",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XCityState* self = (XCityState*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wMapId = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMapId'", NULL);
#endif
  {
   self->SetMapId(wMapId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMapId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMapId of class  XCityState */
#ifndef TOLUA_DISABLE_tolua_XCityState_XCityState_GetMapId00
static int tolua_XCityState_XCityState_GetMapId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XCityState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XCityState* self = (XCityState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMapId'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->GetMapId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMapId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_XCityState_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"XCityState","XCityState","pub;ic XGameSateBase",NULL);
  tolua_beginmodule(tolua_S,"XCityState");
   tolua_function(tolua_S,"ToCityState",tolua_XCityState_XCityState_ToCityState00);
   tolua_function(tolua_S,"GetMap",tolua_XCityState_XCityState_GetMap00);
   tolua_function(tolua_S,"SetMapId",tolua_XCityState_XCityState_SetMapId00);
   tolua_function(tolua_S,"GetMapId",tolua_XCityState_XCityState_GetMapId00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_XCityState (lua_State* tolua_S) {
 return tolua_XCityState_open(tolua_S);
};
#endif

