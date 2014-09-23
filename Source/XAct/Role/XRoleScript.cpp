/*
** Lua binding: XRole
** Generated automatically by tolua++-1.0.92 on 08/29/14 14:14:24.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_XRole_open (lua_State* tolua_S);

#include "XRole.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"XEffect");
 tolua_usertype(tolua_S,"XRole");
 tolua_usertype(tolua_S,"XHeadEffect");
}

/* method: Create of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_Create00
static int tolua_XRole_XRole_Create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XRole* tolua_ret = (XRole*)  XRole::Create();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XRole");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Release of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_Release00
static int tolua_XRole_XRole_Release00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Release'", NULL);
#endif
  {
   self->Release();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Release'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Cleanup of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_Cleanup00
static int tolua_XRole_XRole_Cleanup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Cleanup'", NULL);
#endif
  {
   self->Cleanup();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Cleanup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAITarget of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetAITarget00
static int tolua_XRole_XRole_SetAITarget00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  XRole* pTarget = ((XRole*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAITarget'", NULL);
#endif
  {
   self->SetAITarget(pTarget);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAITarget'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MoveTo of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_MoveTo00
static int tolua_XRole_XRole_MoveTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  float fX = ((float)  tolua_tonumber(tolua_S,2,0));
  float fY = ((float)  tolua_tonumber(tolua_S,3,0));
  bool bRun = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MoveTo'", NULL);
#endif
  {
   self->MoveTo(fX,fY,bRun);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MoveTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPosition of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetPosition00
static int tolua_XRole_XRole_SetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  float fX = ((float)  tolua_tonumber(tolua_S,2,0));
  float fY = ((float)  tolua_tonumber(tolua_S,3,0));
  float fH = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosition'", NULL);
#endif
  {
   self->SetPosition(fX,fY,fH);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCamp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetCamp00
static int tolua_XRole_XRole_SetCamp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char tolua_var_1 = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCamp'", NULL);
#endif
  {
   self->SetCamp(tolua_var_1);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCamp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Wait of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_Wait00
static int tolua_XRole_XRole_Wait00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  bool bForce = ((bool)  tolua_toboolean(tolua_S,2,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Wait'", NULL);
#endif
  {
   self->Wait(bForce);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Wait'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LoadAction of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_LoadAction00
static int tolua_XRole_XRole_LoadAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byActType = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadAction'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->LoadAction(byActType);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRoleID of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetRoleID00
static int tolua_XRole_XRole_SetRoleID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wRoleID = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRoleID'", NULL);
#endif
  {
   self->SetRoleID(wRoleID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRoleID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRoleId of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetRoleId00
static int tolua_XRole_XRole_GetRoleId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRoleId'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->GetRoleId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRoleId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTypeA of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetTypeA00
static int tolua_XRole_XRole_SetTypeA00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byTypeA = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTypeA'", NULL);
#endif
  {
   self->SetTypeA(byTypeA);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTypeA'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTypeB of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetTypeB00
static int tolua_XRole_XRole_SetTypeB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byTypeB = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTypeB'", NULL);
#endif
  {
   self->SetTypeB(byTypeB);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTypeB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTypeC of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetTypeC00
static int tolua_XRole_XRole_SetTypeC00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byTypeC = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTypeC'", NULL);
#endif
  {
   self->SetTypeC(byTypeC);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTypeC'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTypeA of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetTypeA00
static int tolua_XRole_XRole_GetTypeA00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTypeA'", NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->GetTypeA();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTypeA'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTypeB of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetTypeB00
static int tolua_XRole_XRole_GetTypeB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTypeB'", NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->GetTypeB();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTypeB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTypeC of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetTypeC00
static int tolua_XRole_XRole_GetTypeC00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTypeC'", NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->GetTypeC();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTypeC'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosX of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetPosX00
static int tolua_XRole_XRole_GetPosX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosX'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetPosX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPosX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosY of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetPosY00
static int tolua_XRole_XRole_GetPosY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosY'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetPosY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPosY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCDTimer of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetCDTimer00
static int tolua_XRole_XRole_GetCDTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char bySklPos = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCDTimer'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->GetCDTimer(bySklPos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCDTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetLevel of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetLevel00
static int tolua_XRole_XRole_SetLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byRoleLvl = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetLevel'", NULL);
#endif
  {
   self->SetLevel(byRoleLvl);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLevel of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetLevel00
static int tolua_XRole_XRole_GetLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLevel'", NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->GetLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetHp00
static int tolua_XRole_XRole_SetHp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwHp = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHp'", NULL);
#endif
  {
   self->SetHp(dwHp);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetHp00
static int tolua_XRole_XRole_GetHp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHp'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->GetHp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMaxHp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetMaxHp00
static int tolua_XRole_XRole_SetMaxHp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwMaxHp = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMaxHp'", NULL);
#endif
  {
   self->SetMaxHp(dwMaxHp);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMaxHp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMaxHp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetMaxHp00
static int tolua_XRole_XRole_GetMaxHp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMaxHp'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->GetMaxHp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMaxHp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetMp00
static int tolua_XRole_XRole_SetMp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwMp = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMp'", NULL);
#endif
  {
   self->SetMp(dwMp);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetMp00
static int tolua_XRole_XRole_GetMp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMp'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->GetMp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMaxMp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetMaxMp00
static int tolua_XRole_XRole_SetMaxMp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwMaxMp = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMaxMp'", NULL);
#endif
  {
   self->SetMaxMp(dwMaxMp);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMaxMp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMaxMp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetMaxMp00
static int tolua_XRole_XRole_GetMaxMp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMaxMp'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->GetMaxMp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMaxMp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAttPhy of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetAttPhy00
static int tolua_XRole_XRole_SetAttPhy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwAttPhy = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAttPhy'", NULL);
#endif
  {
   self->SetAttPhy(dwAttPhy);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAttPhy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAttPhy of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetAttPhy00
static int tolua_XRole_XRole_GetAttPhy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAttPhy'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->GetAttPhy();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAttPhy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDefPhy of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetDefPhy00
static int tolua_XRole_XRole_SetDefPhy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwDefPhy = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDefPhy'", NULL);
#endif
  {
   self->SetDefPhy(dwDefPhy);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDefPhy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDefPhy of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetDefPhy00
static int tolua_XRole_XRole_GetDefPhy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDefPhy'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->GetDefPhy();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDefPhy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAttSkl of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetAttSkl00
static int tolua_XRole_XRole_SetAttSkl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwAttSkl = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAttSkl'", NULL);
#endif
  {
   self->SetAttSkl(dwAttSkl);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAttSkl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAttSkl of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetAttSkl00
static int tolua_XRole_XRole_GetAttSkl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAttSkl'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->GetAttSkl();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAttSkl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDefSkl of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetDefSkl00
static int tolua_XRole_XRole_SetDefSkl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwDefSkl = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDefSkl'", NULL);
#endif
  {
   self->SetDefSkl(dwDefSkl);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDefSkl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDefSkl of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetDefSkl00
static int tolua_XRole_XRole_GetDefSkl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDefSkl'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->GetDefSkl();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDefSkl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCritRate of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetCritRate00
static int tolua_XRole_XRole_SetCritRate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  float fCritRate = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCritRate'", NULL);
#endif
  {
   self->SetCritRate(fCritRate);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCritRate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCritRate of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetCritRate00
static int tolua_XRole_XRole_GetCritRate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCritRate'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetCritRate();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCritRate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCritRateCoefficient of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetCritRateCoefficient00
static int tolua_XRole_XRole_SetCritRateCoefficient00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwCritRateCoefficient = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCritRateCoefficient'", NULL);
#endif
  {
   self->SetCritRateCoefficient(dwCritRateCoefficient);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCritRateCoefficient'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCritRateCoefficient of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetCritRateCoefficient00
static int tolua_XRole_XRole_GetCritRateCoefficient00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCritRateCoefficient'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->GetCritRateCoefficient();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCritRateCoefficient'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCritRateMax of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetCritRateMax00
static int tolua_XRole_XRole_SetCritRateMax00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  float fCritRateMax = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCritRateMax'", NULL);
#endif
  {
   self->SetCritRateMax(fCritRateMax);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCritRateMax'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCritRateMax of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetCritRateMax00
static int tolua_XRole_XRole_GetCritRateMax00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCritRateMax'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetCritRateMax();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCritRateMax'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCritRateMin of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetCritRateMin00
static int tolua_XRole_XRole_SetCritRateMin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  float fCritRateMin = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCritRateMin'", NULL);
#endif
  {
   self->SetCritRateMin(fCritRateMin);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCritRateMin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCritRateMin of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetCritRateMin00
static int tolua_XRole_XRole_GetCritRateMin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCritRateMin'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetCritRateMin();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCritRateMin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCriMul of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetCriMul00
static int tolua_XRole_XRole_SetCriMul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  float fCriMul = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCriMul'", NULL);
#endif
  {
   self->SetCriMul(fCriMul);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCriMul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCriMul of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetCriMul00
static int tolua_XRole_XRole_GetCriMul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCriMul'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetCriMul();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCriMul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCritLvl of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetCritLvl00
static int tolua_XRole_XRole_SetCritLvl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wCriMul = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCritLvl'", NULL);
#endif
  {
   self->SetCritLvl(wCriMul);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCritLvl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCritLvl of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetCritLvl00
static int tolua_XRole_XRole_GetCritLvl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCritLvl'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->GetCritLvl();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCritLvl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSkillLvl of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetSkillLvl00
static int tolua_XRole_XRole_SetSkillLvl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char bySklPos = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  unsigned char bySklLvl = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSkillLvl'", NULL);
#endif
  {
   self->SetSkillLvl(bySklPos,bySklLvl);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSkillLvl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSkillLvl of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetSkillLvl00
static int tolua_XRole_XRole_GetSkillLvl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char bySklPos = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSkillLvl'", NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->GetSkillLvl(bySklPos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSkillLvl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetReMp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetReMp00
static int tolua_XRole_XRole_SetReMp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wReMp = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetReMp'", NULL);
#endif
  {
   self->SetReMp(wReMp);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetReMp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetReMp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetReMp00
static int tolua_XRole_XRole_GetReMp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetReMp'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->GetReMp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetReMp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRoleName of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetRoleName00
static int tolua_XRole_XRole_SetRoleName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  const char* szRoleName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRoleName'", NULL);
#endif
  {
   self->SetRoleName(szRoleName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRoleName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRoleNameByUnicode of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetRoleNameByUnicode00
static int tolua_XRole_XRole_SetRoleNameByUnicode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  const char* szRoleName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRoleNameByUnicode'", NULL);
#endif
  {
   self->SetRoleNameByUnicode(szRoleName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRoleNameByUnicode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetEffectId of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetEffectId00
static int tolua_XRole_XRole_SetEffectId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wEffectId = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetEffectId'", NULL);
#endif
  {
   self->SetEffectId(wEffectId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetEffectId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRoleHeadEffect of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetRoleHeadEffect00
static int tolua_XRole_XRole_GetRoleHeadEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRoleHeadEffect'", NULL);
#endif
  {
   XHeadEffect* tolua_ret = (XHeadEffect*)  self->GetRoleHeadEffect();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XHeadEffect");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRoleHeadEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRoleName of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetRoleName00
static int tolua_XRole_XRole_GetRoleName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRoleName'", NULL);
#endif
  {
   char* tolua_ret = (char*)  self->GetRoleName();
   //tolua_pushstring(tolua_S,(const char*)tolua_ret);
   int nLen = OGDP::xstr_wcslen((const unsigned short*)tolua_ret) + 1;
   lua_pushlstring(tolua_S,(const char*)tolua_ret, (nLen << 1));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRoleName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetIZFinishTick of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetIZFinishTick00
static int tolua_XRole_XRole_GetIZFinishTick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetIZFinishTick'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetIZFinishTick();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetIZFinishTick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetComboMaxNum of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetComboMaxNum00
static int tolua_XRole_XRole_GetComboMaxNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetComboMaxNum'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetComboMaxNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetComboMaxNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetComboTimer of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetComboTimer00
static int tolua_XRole_XRole_SetComboTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wComboTimer = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetComboTimer'", NULL);
#endif
  {
   self->SetComboTimer(wComboTimer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetComboTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Revive of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_Revive00
static int tolua_XRole_XRole_Revive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Revive'", NULL);
#endif
  {
   self->Revive();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Revive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnLevelUp of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_OnLevelUp00
static int tolua_XRole_XRole_OnLevelUp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnLevelUp'", NULL);
#endif
  {
   self->OnLevelUp();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnLevelUp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSKillPosByDir of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetSKillPosByDir00
static int tolua_XRole_XRole_GetSKillPosByDir00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byDir = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  {
   unsigned char tolua_ret = (unsigned char)  XRole::GetSKillPosByDir(byDir);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSKillPosByDir'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSKillPosByDir of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_setSKillPosByDir00
static int tolua_XRole_XRole_setSKillPosByDir00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byDir = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  unsigned char byPos = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
  {
   XRole::setSKillPosByDir(byDir,byPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSKillPosByDir'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: UsePlayerAI of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_UsePlayerAI00
static int tolua_XRole_XRole_UsePlayerAI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'UsePlayerAI'", NULL);
#endif
  {
   self->UsePlayerAI();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UsePlayerAI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ResetPAI of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_ResetPAI00
static int tolua_XRole_XRole_ResetPAI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ResetPAI'", NULL);
#endif
  {
   self->ResetPAI();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ResetPAI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRoleElf of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetRoleElf00
static int tolua_XRole_XRole_SetRoleElf00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byType = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  XRole* pElf = ((XRole*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRoleElf'", NULL);
#endif
  {
   self->SetRoleElf(byType,pElf);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRoleElf'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: UseSklAfterRunTo of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_UseSklAfterRunTo00
static int tolua_XRole_XRole_UseSklAfterRunTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wPosX = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
  unsigned short wPosY = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  unsigned char byType = ((unsigned char)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'UseSklAfterRunTo'", NULL);
#endif
  {
   self->UseSklAfterRunTo(wPosX,wPosY,byType);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UseSklAfterRunTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ChangeAct of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_ChangeAct00
static int tolua_XRole_XRole_ChangeAct00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byActID = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChangeAct'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ChangeAct(byActID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangeAct'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFlipX of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetFlipX00
static int tolua_XRole_XRole_SetFlipX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  bool fFlipX = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFlipX'", NULL);
#endif
  {
   self->SetFlipX(fFlipX);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFlipX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Dead of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_Dead00
static int tolua_XRole_XRole_Dead00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Dead'", NULL);
#endif
  {
   self->Dead();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Dead'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Disappear of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_Disappear00
static int tolua_XRole_XRole_Disappear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Disappear'", NULL);
#endif
  {
   self->Disappear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Disappear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnHurt of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_OnHurt00
static int tolua_XRole_XRole_OnHurt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"XEffect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  XEffect* pEffect = ((XEffect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnHurt'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnHurt(pEffect);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnHurt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AIEnabled of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_AIEnabled00
static int tolua_XRole_XRole_AIEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  bool bEnabled = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AIEnabled'", NULL);
#endif
  {
   self->AIEnabled(bEnabled);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AIEnabled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDeadAlphaOffSet of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetDeadAlphaOffSet00
static int tolua_XRole_XRole_SetDeadAlphaOffSet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned char byOffSet = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  {
   XRole::SetDeadAlphaOffSet(byOffSet);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDeadAlphaOffSet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAI of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetAI00
static int tolua_XRole_XRole_SetAI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wAI = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
  bool bUseAI = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAI'", NULL);
#endif
  {
   self->SetAI(wAI,bUseAI);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAIID of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetAIID00
static int tolua_XRole_XRole_GetAIID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAIID'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->GetAIID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAIID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetElemType of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetElemType00
static int tolua_XRole_XRole_SetElemType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byType = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetElemType'", NULL);
#endif
  {
   self->SetElemType(byType);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetElemType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetElemVal of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetElemVal00
static int tolua_XRole_XRole_SetElemVal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byType = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  unsigned short wVal = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetElemVal'", NULL);
#endif
  {
   self->SetElemVal(byType,wVal);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetElemVal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSpdScale of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetSpdScale00
static int tolua_XRole_XRole_SetSpdScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wPerc = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSpdScale'", NULL);
#endif
  {
   self->SetSpdScale(wPerc);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSpdScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSpdScale of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetSpdScale00
static int tolua_XRole_XRole_GetSpdScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpdScale'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->GetSpdScale();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSpdScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRoleScale of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetRoleScale00
static int tolua_XRole_XRole_SetRoleScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  float fScale = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRoleScale'", NULL);
#endif
  {
   self->SetRoleScale(fScale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRoleScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFightNum of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetFightNum00
static int tolua_XRole_XRole_SetFightNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwFightNum = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFightNum'", NULL);
#endif
  {
   self->SetFightNum(dwFightNum);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFightNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFightNum of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_GetFightNum00
static int tolua_XRole_XRole_GetFightNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFightNum'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->GetFightNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFightNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DelRoleEquipEffect of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_DelRoleEquipEffect00
static int tolua_XRole_XRole_DelRoleEquipEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DelRoleEquipEffect'", NULL);
#endif
  {
   self->DelRoleEquipEffect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DelRoleEquipEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetEquipEffectId of class  XRole */
#ifndef TOLUA_DISABLE_tolua_XRole_XRole_SetEquipEffectId00
static int tolua_XRole_XRole_SetEquipEffectId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XRole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XRole* self = (XRole*)  tolua_tousertype(tolua_S,1,0);
  unsigned short wEffectId = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetEquipEffectId'", NULL);
#endif
  {
   self->SetEquipEffectId(wEffectId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetEquipEffectId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_XRole_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"XHeadEffect","XHeadEffect","",NULL);
  tolua_beginmodule(tolua_S,"XHeadEffect");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"XRole","XRole","",NULL);
  tolua_beginmodule(tolua_S,"XRole");
   tolua_function(tolua_S,"Create",tolua_XRole_XRole_Create00);
   tolua_function(tolua_S,"Release",tolua_XRole_XRole_Release00);
   tolua_function(tolua_S,"Cleanup",tolua_XRole_XRole_Cleanup00);
   tolua_function(tolua_S,"SetAITarget",tolua_XRole_XRole_SetAITarget00);
   tolua_function(tolua_S,"MoveTo",tolua_XRole_XRole_MoveTo00);
   tolua_function(tolua_S,"SetPosition",tolua_XRole_XRole_SetPosition00);
   tolua_function(tolua_S,"SetCamp",tolua_XRole_XRole_SetCamp00);
   tolua_function(tolua_S,"Wait",tolua_XRole_XRole_Wait00);
   tolua_function(tolua_S,"LoadAction",tolua_XRole_XRole_LoadAction00);
   tolua_function(tolua_S,"SetRoleID",tolua_XRole_XRole_SetRoleID00);
   tolua_function(tolua_S,"GetRoleId",tolua_XRole_XRole_GetRoleId00);
   tolua_function(tolua_S,"SetTypeA",tolua_XRole_XRole_SetTypeA00);
   tolua_function(tolua_S,"SetTypeB",tolua_XRole_XRole_SetTypeB00);
   tolua_function(tolua_S,"SetTypeC",tolua_XRole_XRole_SetTypeC00);
   tolua_function(tolua_S,"GetTypeA",tolua_XRole_XRole_GetTypeA00);
   tolua_function(tolua_S,"GetTypeB",tolua_XRole_XRole_GetTypeB00);
   tolua_function(tolua_S,"GetTypeC",tolua_XRole_XRole_GetTypeC00);
   tolua_function(tolua_S,"GetPosX",tolua_XRole_XRole_GetPosX00);
   tolua_function(tolua_S,"GetPosY",tolua_XRole_XRole_GetPosY00);
   tolua_function(tolua_S,"GetCDTimer",tolua_XRole_XRole_GetCDTimer00);
   tolua_function(tolua_S,"SetLevel",tolua_XRole_XRole_SetLevel00);
   tolua_function(tolua_S,"GetLevel",tolua_XRole_XRole_GetLevel00);
   tolua_function(tolua_S,"SetHp",tolua_XRole_XRole_SetHp00);
   tolua_function(tolua_S,"GetHp",tolua_XRole_XRole_GetHp00);
   tolua_function(tolua_S,"SetMaxHp",tolua_XRole_XRole_SetMaxHp00);
   tolua_function(tolua_S,"GetMaxHp",tolua_XRole_XRole_GetMaxHp00);
   tolua_function(tolua_S,"SetMp",tolua_XRole_XRole_SetMp00);
   tolua_function(tolua_S,"GetMp",tolua_XRole_XRole_GetMp00);
   tolua_function(tolua_S,"SetMaxMp",tolua_XRole_XRole_SetMaxMp00);
   tolua_function(tolua_S,"GetMaxMp",tolua_XRole_XRole_GetMaxMp00);
   tolua_function(tolua_S,"SetAttPhy",tolua_XRole_XRole_SetAttPhy00);
   tolua_function(tolua_S,"GetAttPhy",tolua_XRole_XRole_GetAttPhy00);
   tolua_function(tolua_S,"SetDefPhy",tolua_XRole_XRole_SetDefPhy00);
   tolua_function(tolua_S,"GetDefPhy",tolua_XRole_XRole_GetDefPhy00);
   tolua_function(tolua_S,"SetAttSkl",tolua_XRole_XRole_SetAttSkl00);
   tolua_function(tolua_S,"GetAttSkl",tolua_XRole_XRole_GetAttSkl00);
   tolua_function(tolua_S,"SetDefSkl",tolua_XRole_XRole_SetDefSkl00);
   tolua_function(tolua_S,"GetDefSkl",tolua_XRole_XRole_GetDefSkl00);
   tolua_function(tolua_S,"SetCritRate",tolua_XRole_XRole_SetCritRate00);
   tolua_function(tolua_S,"GetCritRate",tolua_XRole_XRole_GetCritRate00);
   tolua_function(tolua_S,"SetCritRateCoefficient",tolua_XRole_XRole_SetCritRateCoefficient00);
   tolua_function(tolua_S,"GetCritRateCoefficient",tolua_XRole_XRole_GetCritRateCoefficient00);
   tolua_function(tolua_S,"SetCritRateMax",tolua_XRole_XRole_SetCritRateMax00);
   tolua_function(tolua_S,"GetCritRateMax",tolua_XRole_XRole_GetCritRateMax00);
   tolua_function(tolua_S,"SetCritRateMin",tolua_XRole_XRole_SetCritRateMin00);
   tolua_function(tolua_S,"GetCritRateMin",tolua_XRole_XRole_GetCritRateMin00);
   tolua_function(tolua_S,"SetCriMul",tolua_XRole_XRole_SetCriMul00);
   tolua_function(tolua_S,"GetCriMul",tolua_XRole_XRole_GetCriMul00);
   tolua_function(tolua_S,"SetCritLvl",tolua_XRole_XRole_SetCritLvl00);
   tolua_function(tolua_S,"GetCritLvl",tolua_XRole_XRole_GetCritLvl00);
   tolua_function(tolua_S,"SetSkillLvl",tolua_XRole_XRole_SetSkillLvl00);
   tolua_function(tolua_S,"GetSkillLvl",tolua_XRole_XRole_GetSkillLvl00);
   tolua_function(tolua_S,"SetReMp",tolua_XRole_XRole_SetReMp00);
   tolua_function(tolua_S,"GetReMp",tolua_XRole_XRole_GetReMp00);
   tolua_function(tolua_S,"SetRoleName",tolua_XRole_XRole_SetRoleName00);
   tolua_function(tolua_S,"SetRoleNameByUnicode",tolua_XRole_XRole_SetRoleNameByUnicode00);
   tolua_function(tolua_S,"SetEffectId",tolua_XRole_XRole_SetEffectId00);
   tolua_function(tolua_S,"GetRoleHeadEffect",tolua_XRole_XRole_GetRoleHeadEffect00);
   tolua_function(tolua_S,"GetRoleName",tolua_XRole_XRole_GetRoleName00);
   tolua_function(tolua_S,"GetIZFinishTick",tolua_XRole_XRole_GetIZFinishTick00);
   tolua_function(tolua_S,"GetComboMaxNum",tolua_XRole_XRole_GetComboMaxNum00);
   tolua_function(tolua_S,"SetComboTimer",tolua_XRole_XRole_SetComboTimer00);
   tolua_function(tolua_S,"Revive",tolua_XRole_XRole_Revive00);
   tolua_function(tolua_S,"OnLevelUp",tolua_XRole_XRole_OnLevelUp00);
   tolua_function(tolua_S,"GetSKillPosByDir",tolua_XRole_XRole_GetSKillPosByDir00);
   tolua_function(tolua_S,"setSKillPosByDir",tolua_XRole_XRole_setSKillPosByDir00);
   tolua_function(tolua_S,"UsePlayerAI",tolua_XRole_XRole_UsePlayerAI00);
   tolua_function(tolua_S,"ResetPAI",tolua_XRole_XRole_ResetPAI00);
   tolua_function(tolua_S,"SetRoleElf",tolua_XRole_XRole_SetRoleElf00);
   tolua_function(tolua_S,"UseSklAfterRunTo",tolua_XRole_XRole_UseSklAfterRunTo00);
   tolua_function(tolua_S,"ChangeAct",tolua_XRole_XRole_ChangeAct00);
   tolua_function(tolua_S,"SetFlipX",tolua_XRole_XRole_SetFlipX00);
   tolua_function(tolua_S,"Dead",tolua_XRole_XRole_Dead00);
   tolua_function(tolua_S,"Disappear",tolua_XRole_XRole_Disappear00);
   tolua_function(tolua_S,"OnHurt",tolua_XRole_XRole_OnHurt00);
   tolua_function(tolua_S,"AIEnabled",tolua_XRole_XRole_AIEnabled00);
   tolua_function(tolua_S,"SetDeadAlphaOffSet",tolua_XRole_XRole_SetDeadAlphaOffSet00);
   tolua_function(tolua_S,"SetAI",tolua_XRole_XRole_SetAI00);
   tolua_function(tolua_S,"GetAIID",tolua_XRole_XRole_GetAIID00);
   tolua_function(tolua_S,"SetElemType",tolua_XRole_XRole_SetElemType00);
   tolua_function(tolua_S,"SetElemVal",tolua_XRole_XRole_SetElemVal00);
   tolua_function(tolua_S,"SetSpdScale",tolua_XRole_XRole_SetSpdScale00);
   tolua_function(tolua_S,"GetSpdScale",tolua_XRole_XRole_GetSpdScale00);
   tolua_function(tolua_S,"SetRoleScale",tolua_XRole_XRole_SetRoleScale00);
   tolua_function(tolua_S,"SetFightNum",tolua_XRole_XRole_SetFightNum00);
   tolua_function(tolua_S,"GetFightNum",tolua_XRole_XRole_GetFightNum00);
   tolua_function(tolua_S,"DelRoleEquipEffect",tolua_XRole_XRole_DelRoleEquipEffect00);
   tolua_function(tolua_S,"SetEquipEffectId",tolua_XRole_XRole_SetEquipEffectId00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_XRole (lua_State* tolua_S) {
 return tolua_XRole_open(tolua_S);
};
#endif

