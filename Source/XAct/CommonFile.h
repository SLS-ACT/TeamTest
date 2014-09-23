#ifndef __COMMONFILE_H__
#define __COMMONFILE_H__

// Table
#define XFILE_TEXSOUR			("Table/Texture.xls")
#define XFILE_SPRCUT			("Table/TexCut.xls")
#define XFILE_XANI				("Table/TexAni.xls")
#define XFILE_ROLET				("Table/Role.xls")
#define XFILE_EXPVALUE			("Table/RoleLevel.xls")
#define XFILE_NPCT				("Table/Npc.xls")
#define XFILE_AI				("Table/AI.xls")
#define XFILE_SKILL				("Table/Skill.xls")
#define XFILE_SKILLLVL			("Table/SkillLvl.xls")
#define XFILE_CAMERA			("Table/Camera.xls")
#define XFILE_MAP				("Table/Map.xls")
#define XFILE_EQUIP				("Table/Equip.xls")
#define XFILE_EQUIPATTR			("Table/EquipAttr.xls")
#define XFILE_ITEM				("Table/Item.xls")
#define XFILE_ITEMLOGIC			("Table/ItemLogic.xls")
#define XFILE_GEMATTR			("Table/GemAttr.xls")
#define XFILE_IZCHAPTER			("Table/IZChapter.xls")
#define XFILE_INSTZONES			("Table/InstZones.xls")
#define XFILE_IZTARGET			("Table/IZTarget.xls")
#define XFILE_ELF				("Table/Elf.xls")
#define XFILE_ELFATTR			("Table/ElfAttr.xls")
#define XFILE_ELFATTREXT		("Table/ElfAttrExt.xls")
#define XFILE_AWARD				("Table/Award.xls")
#define XFILE_SHOP				("Table/Shop.xls")
#define XFILE_WHEEL				("Table/Wheel.xls")
#define XFILE_ELFEGG			("Table/ElfEgg.xls")
#define XFILE_MISSION			("Table/Mission.xls")
// º«Óï
#define XFILE_TALK_KO			("Table/Talk_Ko.xls")
#define XFILE_SVRLANG_KO		("Table/SvrLang_Ko.xls")
#define XFILE_SOUND_KO			("Table/Music_Ko.xls")

// ÖÐÎÄ
#define XFILE_TALK				("Table/Talk.xls")
#define XFILE_SVRLANG			("Table/SvrLang.xls")
#define XFILE_SOUND				("Table/Music.xls")

#define XFILE_VIP				("Table/Vip.xls")
#define XFILE_EVENT				("Table/Events.xls")
#define XFILE_EVENT_KO			("Table/Events_Ko.xls")
#define XTABLE_ALCHEMY			("Table/Alchemy.xls")
#define XTABLE_TITLE			("Table/Title.xls")
#define XTABLE_VIPGIFT			("Table/VipGift.xls")
#define XTABLE_BOX				("Table/Box.xls")
#define XTABLE_BOXITEM			("Table/BoxItem.xls")
#define XTABLE_BUFF				("Table/Buff.xls")
#define XTABLE_CONFIG			("Table/Config.xls")
#define XTABLE_ELFSTAR			("Table/ElfStar.xls")
#define XTABLE_SIGNIN			("Table/SignIn.xls")
#define XTABLE_DROP				("Table/Drop.xls")
#define XTABLE_SYNTHESUS		("Table/Synthesis.xls")
#define XTABLE_TALENT			("Table/Talent.xls")
#define XTABLE_MISSDAILY		("Table/MissDaily.xls")

// ¾ÞÈË
#define XTABLE_CHARGE			("Table/Charge.xls")
#define XTABLE_CHARGE_HM		("Table/Charge_hm.xls")

// ´¥¿Ø
#define XTABLE_CHARGE_GOOGLE	("Table/Charge_google.xls")


#define XTABLE_RISKERGAMES		("Table/RiskerGames.xls")

// Xml
#define XFILE_ANIMATION			("Table/Animation.xml")
#define XFILE_EFFECTANI			("Table/EffectAnimation.xml")
#define XFILE_ROLE				("Table/Role.xml")
#define XFILE_EFFECT			("Table/Effect.xml")
#define XFILE_PROTOCOL			("Update/Script/Protocol/XProtocol.xml")

// Script
#define XFILE_GAMESCRIPT		("Script/GameInit.lua")
#define XFILE_GAMESCRIPT_KAKAO	("Script_kakao/GameInit_kakao.lua")

// Path create in c++
#define XRES_JPG
#ifdef XRES_JPG
#	define XPATH_TEXCUT			("TexCut/XTex%05d.jpg")
#else
#	define XPATH_TEXCUT			("TexCut/XTex%05d.png")
#endif
#define XPATH_TEXANI			("XAni8b/XAni%05d.xa8")
#define XPATH_MAPINFO			("Table/Map/MapInfo%05d.xls")

#endif // __COMMONFILE_H__
