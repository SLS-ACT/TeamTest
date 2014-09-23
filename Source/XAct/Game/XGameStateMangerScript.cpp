/*
** Lua binding: XGameStateManger
** Generated automatically by tolua++-1.0.92 on 09/03/14 22:30:22.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_XGameStateManger_open (lua_State* tolua_S);

#include "XGameState.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"XGameStateManger");
 tolua_usertype(tolua_S,"XGameSateBase");
 tolua_usertype(tolua_S,"XMap");
}

/* method: ChangeState of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_ChangeState00
static int tolua_XGameStateManger_XGameStateManger_ChangeState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byState = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  {
   XGameStateManger::ChangeState(byState);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangeState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetGameState of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_GetGameState00
static int tolua_XGameStateManger_XGameStateManger_GetGameState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byState = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  {
   XGameSateBase* tolua_ret = (XGameSateBase*)  XGameStateManger::GetGameState(byState);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XGameSateBase");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetGameState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCurrState of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_GetCurrState00
static int tolua_XGameStateManger_XGameStateManger_GetCurrState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned char tolua_ret = (unsigned char)  XGameStateManger::GetCurrState();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCurrState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnJump of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_OnJump00
static int tolua_XGameStateManger_XGameStateManger_OnJump00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XGameStateManger::OnJump();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnJump'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetShowJoystick of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_SetShowJoystick00
static int tolua_XGameStateManger_XGameStateManger_SetShowJoystick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byType = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  bool bIsShow = ((bool)  tolua_toboolean(tolua_S,3,0));
  {
   XGameStateManger::SetShowJoystick(byType,bIsShow);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetShowJoystick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCityTouch of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_SetCityTouch00
static int tolua_XGameStateManger_XGameStateManger_SetCityTouch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byTouch = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  {
   XGameStateManger::SetCityTouch(byTouch);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCityTouch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetJoyStickStates of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_SetJoyStickStates00
static int tolua_XGameStateManger_XGameStateManger_SetJoyStickStates00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byType = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  bool bActive = ((bool)  tolua_toboolean(tolua_S,3,0));
  {
   XGameStateManger::SetJoyStickStates(byType,bActive);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetJoyStickStates'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: UseSkillByDir of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_UseSkillByDir00
static int tolua_XGameStateManger_XGameStateManger_UseSkillByDir00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byDir = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  {
   XGameStateManger::UseSkillByDir(byDir);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UseSkillByDir'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetActivityRoleID of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_GetActivityRoleID00
static int tolua_XGameStateManger_XGameStateManger_GetActivityRoleID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* szMapInfoPath = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned char byMark = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
  {
   unsigned short tolua_ret = (unsigned short)  XGameStateManger::GetActivityRoleID(szMapInfoPath,byMark);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetActivityRoleID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHEARTBEAT_REQ of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_SetHEARTBEAT_REQ00
static int tolua_XGameStateManger_XGameStateManger_SetHEARTBEAT_REQ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byHEARTBEAT_REQ = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  {
   XGameStateManger::SetHEARTBEAT_REQ(byHEARTBEAT_REQ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHEARTBEAT_REQ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnReLoadGame of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_OnReLoadGame00
static int tolua_XGameStateManger_XGameStateManger_OnReLoadGame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XGameStateManger::OnReLoadGame();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnReLoadGame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCheckWaiting of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_SetCheckWaiting00
static int tolua_XGameStateManger_XGameStateManger_SetCheckWaiting00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byCheck = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  {
   XGameStateManger::SetCheckWaiting(byCheck);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCheckWaiting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCheckWaiting of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_GetCheckWaiting00
static int tolua_XGameStateManger_XGameStateManger_GetCheckWaiting00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned char tolua_ret = (unsigned char)  XGameStateManger::GetCheckWaiting();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCheckWaiting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NetClose of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_NetClose00
static int tolua_XGameStateManger_XGameStateManger_NetClose00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XGameStateManger::NetClose();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NetClose'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnNetReLoadSucc of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_OnNetReLoadSucc00
static int tolua_XGameStateManger_XGameStateManger_OnNetReLoadSucc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XGameStateManger::OnNetReLoadSucc();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnNetReLoadSucc'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnNetReLoadFaild of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_OnNetReLoadFaild00
static int tolua_XGameStateManger_XGameStateManger_OnNetReLoadFaild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XGameStateManger::OnNetReLoadFaild();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnNetReLoadFaild'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCurrMap of class  XGameStateManger */
#ifndef TOLUA_DISABLE_tolua_XGameStateManger_XGameStateManger_GetCurrMap00
static int tolua_XGameStateManger_XGameStateManger_GetCurrMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XGameStateManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XMap* tolua_ret = (XMap*)  XGameStateManger::GetCurrMap();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XMap");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCurrMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_XGameStateManger_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"XGameStateManger","XGameStateManger","",NULL);
  tolua_beginmodule(tolua_S,"XGameStateManger");
   tolua_function(tolua_S,"ChangeState",tolua_XGameStateManger_XGameStateManger_ChangeState00);
   tolua_function(tolua_S,"GetGameState",tolua_XGameStateManger_XGameStateManger_GetGameState00);
   tolua_function(tolua_S,"GetCurrState",tolua_XGameStateManger_XGameStateManger_GetCurrState00);
   tolua_function(tolua_S,"OnJump",tolua_XGameStateManger_XGameStateManger_OnJump00);
   tolua_function(tolua_S,"SetShowJoystick",tolua_XGameStateManger_XGameStateManger_SetShowJoystick00);
   tolua_function(tolua_S,"SetCityTouch",tolua_XGameStateManger_XGameStateManger_SetCityTouch00);
   tolua_function(tolua_S,"SetJoyStickStates",tolua_XGameStateManger_XGameStateManger_SetJoyStickStates00);
   tolua_function(tolua_S,"UseSkillByDir",tolua_XGameStateManger_XGameStateManger_UseSkillByDir00);
   tolua_function(tolua_S,"GetActivityRoleID",tolua_XGameStateManger_XGameStateManger_GetActivityRoleID00);
   tolua_function(tolua_S,"SetHEARTBEAT_REQ",tolua_XGameStateManger_XGameStateManger_SetHEARTBEAT_REQ00);
   tolua_function(tolua_S,"OnReLoadGame",tolua_XGameStateManger_XGameStateManger_OnReLoadGame00);
   tolua_function(tolua_S,"SetCheckWaiting",tolua_XGameStateManger_XGameStateManger_SetCheckWaiting00);
   tolua_function(tolua_S,"GetCheckWaiting",tolua_XGameStateManger_XGameStateManger_GetCheckWaiting00);
   tolua_function(tolua_S,"NetClose",tolua_XGameStateManger_XGameStateManger_NetClose00);
   tolua_function(tolua_S,"OnNetReLoadSucc",tolua_XGameStateManger_XGameStateManger_OnNetReLoadSucc00);
   tolua_function(tolua_S,"OnNetReLoadFaild",tolua_XGameStateManger_XGameStateManger_OnNetReLoadFaild00);
   tolua_function(tolua_S,"GetCurrMap",tolua_XGameStateManger_XGameStateManger_GetCurrMap00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_XGameStateManger (lua_State* tolua_S) {
 return tolua_XGameStateManger_open(tolua_S);
};
#endif

