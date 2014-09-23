/*
** Lua binding: MaskWord
** Generated automatically by tolua++-1.0.92 on 07/11/14 18:40:39.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_MaskWord_open (lua_State* tolua_S);

#include "MaskWord.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"MaskWord");
}

/* method: InitMaskWord of class  MaskWord */
#ifndef TOLUA_DISABLE_tolua_MaskWord_MaskWord_InitMaskWord00
static int tolua_MaskWord_MaskWord_InitMaskWord00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MaskWord",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* szPath = ((char*)  tolua_tostring(tolua_S,2,0));
  {
   MaskWord::InitMaskWord(szPath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'InitMaskWord'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CheckMaskWord of class  MaskWord */
#ifndef TOLUA_DISABLE_tolua_MaskWord_MaskWord_CheckMaskWord00
static int tolua_MaskWord_MaskWord_CheckMaskWord00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MaskWord",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed char* str = ((signed char*)  tolua_tostring(tolua_S,2,0));
  {
   signed char* tolua_ret = (signed char*)  MaskWord::CheckMaskWord(str);
   //tolua_pushstring(tolua_S,(const char*)tolua_ret);
   int nLen = OGDP::xstr_wcslen((const unsigned short*)tolua_ret) + 1;
   lua_pushlstring(tolua_S,(const char*)tolua_ret, (nLen << 1));

  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CheckMaskWord'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnlyCheck of class  MaskWord */
#ifndef TOLUA_DISABLE_tolua_MaskWord_MaskWord_OnlyCheck00
static int tolua_MaskWord_MaskWord_OnlyCheck00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MaskWord",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed char* str = ((signed char*)  tolua_tostring(tolua_S,2,0));
  {
   unsigned char tolua_ret = (unsigned char)  MaskWord::OnlyCheck(str);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnlyCheck'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_MaskWord_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"MaskWord","MaskWord","",NULL);
  tolua_beginmodule(tolua_S,"MaskWord");
   tolua_function(tolua_S,"InitMaskWord",tolua_MaskWord_MaskWord_InitMaskWord00);
   tolua_function(tolua_S,"CheckMaskWord",tolua_MaskWord_MaskWord_CheckMaskWord00);
   tolua_function(tolua_S,"OnlyCheck",tolua_MaskWord_MaskWord_OnlyCheck00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_MaskWord (lua_State* tolua_S) {
 return tolua_MaskWord_open(tolua_S);
};
#endif

