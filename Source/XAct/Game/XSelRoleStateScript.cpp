/*
** Lua binding: XSelRoleState
** Generated automatically by tolua++-1.0.92 on 03/22/14 14:23:47.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_XSelRoleState_open (lua_State* tolua_S);

#include "XGameState.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"XSelRoleState");
 tolua_usertype(tolua_S,"XRole");
 tolua_usertype(tolua_S,"XGameSateBase");
}

/* method: ToSelRoleState of class  XSelRoleState */
#ifndef TOLUA_DISABLE_tolua_XSelRoleState_XSelRoleState_ToSelRoleState00
static int tolua_XSelRoleState_XSelRoleState_ToSelRoleState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XSelRoleState",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"XGameSateBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XGameSateBase* pState = ((XGameSateBase*)  tolua_tousertype(tolua_S,2,0));
  {
   XSelRoleState* tolua_ret = (XSelRoleState*)  XSelRoleState::ToSelRoleState(pState);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XSelRoleState");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ToSelRoleState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMap of class  XSelRoleState */
#ifndef TOLUA_DISABLE_tolua_XSelRoleState_XSelRoleState_GetMap00
static int tolua_XSelRoleState_XSelRoleState_GetMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XSelRoleState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XSelRoleState* self = (XSelRoleState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMap'", NULL);
#endif
  {
   XSelRoleState* tolua_ret = (XSelRoleState*)  self->GetMap();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XSelRoleState");
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

/* method: GetRoleByID of class  XSelRoleState */
#ifndef TOLUA_DISABLE_tolua_XSelRoleState_XSelRoleState_GetRoleByID00
static int tolua_XSelRoleState_XSelRoleState_GetRoleByID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XSelRoleState",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XSelRoleState* self = (XSelRoleState*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byRoleID = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRoleByID'", NULL);
#endif
  {
   XRole* tolua_ret = (XRole*)  self->GetRoleByID(byRoleID);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XRole");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRoleByID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddRole of class  XSelRoleState */
#ifndef TOLUA_DISABLE_tolua_XSelRoleState_XSelRoleState_AddRole00
static int tolua_XSelRoleState_XSelRoleState_AddRole00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XSelRoleState",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XSelRoleState* self = (XSelRoleState*)  tolua_tousertype(tolua_S,1,0);
  XRole* pRole = ((XRole*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddRole'", NULL);
#endif
  {
   self->AddRole(pRole);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddRole'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_XSelRoleState_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"XSelRoleState","XSelRoleState","XGameSateBase",NULL);
  tolua_beginmodule(tolua_S,"XSelRoleState");
   tolua_function(tolua_S,"ToSelRoleState",tolua_XSelRoleState_XSelRoleState_ToSelRoleState00);
   tolua_function(tolua_S,"GetMap",tolua_XSelRoleState_XSelRoleState_GetMap00);
   tolua_function(tolua_S,"GetRoleByID",tolua_XSelRoleState_XSelRoleState_GetRoleByID00);
   tolua_function(tolua_S,"AddRole",tolua_XSelRoleState_XSelRoleState_AddRole00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_XSelRoleState (lua_State* tolua_S) {
 return tolua_XSelRoleState_open(tolua_S);
};
#endif

