#define INCLUDE_GLOBAL_VARIABLES
#include "../fonline2238.h"

#ifndef __SERVER
#ifndef __CLIENT
#pragma error ("PANIC: no defines in server/client parameters")
#endif
#endif

#ifdef __SERVER
#include <strstream>
#include <string>
#include <windows.h>
#include <Mmsystem.h>
#endif

#define BONUS_WEAPON_MAX_AP						(105)
#define BONUS_WEAPON_MAX_RANGE                  (106)
#define BONUS_ARMOR_NORMAL_DT					(100)
#define BONUS_ARMOR_LASER_DT					(101)
#define BONUS_ARMOR_FIRE_DT						(102)
#define BONUS_ARMOR_PLASMA_DT					(103)
#define BONUS_ARMOR_EXPLODE_DT					(104)
#define BONUS_ARMOR_NORMAL_DR					(105)
#define BONUS_ARMOR_LASER_DR					(106)
#define BONUS_ARMOR_FIRE_DR						(107)
#define BONUS_ARMOR_PLASMA_DR					(108)
#define BONUS_ARMOR_EXPLODE_DR					(109)
#define BONUS_ARMOR_CRIT_CHANCE					(110)
#define BONUS_ARMOR_CRIT_POWER					(111)
#define BONUS_ARMOR_MAX_AP						(112)
#define BONUS_ARMOR_RAD_RES						(113)
#define BONUS_ARMOR_POISON_RES					(114)
#define BONUS_ARMOR_STRENGTH					(115)
#define BONUS_ARMOR_PERCEPTION					(116)
#define BONUS_ARMOR_ENDURANCE					(117)
#define BONUS_ARMOR_CHARISMA					(118)
#define BONUS_ARMOR_INTELLIGENCE				(119)
#define BONUS_ARMOR_AGILITY						(120)
#define BONUS_ARMOR_LUCK						(121)
#define BONUS_ARMOR_CARRY_WEIGHT				(122)
#define BONUS_ARMOR_HEALING_RATE				(123)

// Slot/parameters allowing
EXPORT bool allowSlot_Hand1(uint8, Item&, Critter&, Critter& toCr);

// Parameters Get behavior
EXPORT int getParam_Strength(CritterMutual& cr, uint);
EXPORT int getParam_Perception(CritterMutual& cr, uint);
EXPORT int getParam_Endurance(CritterMutual& cr, uint);
EXPORT int getParam_Charisma(CritterMutual& cr, uint);
EXPORT int getParam_Intellegence(CritterMutual& cr, uint);
EXPORT int getParam_Agility(CritterMutual& cr, uint);
EXPORT int getParam_Luck(CritterMutual& cr, uint);
EXPORT int getParam_Level(CritterMutual& cr, uint);
EXPORT int getParam_Hp(CritterMutual& cr, uint);
EXPORT int getParam_MaxLife(CritterMutual& cr, uint);
EXPORT int getParam_MaxAp(CritterMutual& cr, uint);
EXPORT int getParam_Ap(CritterMutual& cr, uint);
EXPORT int getParam_MaxMoveAp(CritterMutual& cr, uint);
EXPORT int getParam_MoveAp(CritterMutual& cr, uint);
EXPORT int getParam_MaxWeight(CritterMutual& cr, uint);
EXPORT int getParam_Sequence(CritterMutual& cr, uint);
EXPORT int getParam_MeleeDmg(CritterMutual& cr, uint);
EXPORT int getParam_HealingRate(CritterMutual& cr, uint);
EXPORT int getParam_CriticalChance(CritterMutual& cr, uint);
EXPORT int getParam_CriticalPower(CritterMutual& cr, uint);
EXPORT int getParam_MaxCritical(CritterMutual& cr, uint);
EXPORT int getParam_Ac(CritterMutual& cr, uint);
EXPORT int getParam_DamageResistance(CritterMutual& cr, uint index);
EXPORT int getParam_DamageThreshold(CritterMutual& cr, uint index);
EXPORT int getParam_RadiationResist(CritterMutual& cr, uint);
EXPORT int getParam_PoisonResist(CritterMutual& cr, uint);
EXPORT int getParam_Reputation(CritterMutual& cr, uint index);
EXPORT int getParam_Timeout(CritterMutual& cr, uint index);
//EXPORT void changedParam_Reputation(CritterMutual& cr, uint index, int oldValue);

EXPORT int Critter_GetAC(CritterMutual& cr, bool head);
EXPORT int Critter_GetDT(CritterMutual& cr, uint dmg_type, bool head);
EXPORT int Critter_GetDR(CritterMutual& cr, uint dmg_type, bool head);

// Extended methods
EXPORT bool Critter_IsInjured(CritterMutual& cr);
EXPORT bool Critter_IsDmgEye(CritterMutual& cr);
EXPORT bool Critter_IsDmgLeg(CritterMutual& cr);
EXPORT bool Critter_IsDmgTwoLeg(CritterMutual& cr);
EXPORT bool Critter_IsDmgArm(CritterMutual& cr);
EXPORT bool Critter_IsDmgTwoArm(CritterMutual& cr);
EXPORT bool Critter_IsAddicted(CritterMutual& cr);
EXPORT bool Critter_IsOverweight(CritterMutual& cr);
EXPORT bool Item_Weapon_IsHtHAttack(Item& item, uint8 mode);
EXPORT bool Item_Weapon_IsGunAttack(Item& item, uint8 mode);
EXPORT bool Item_Weapon_IsRangedAttack(Item& item, uint8 mode);
EXPORT void Item_Weapon_SetMode(Item& item, uint8 mode);

// Callbacks
uint GetUseApCost(CritterMutual& cr, Item& item, uint8 mode);
uint GetAttackDistantion(CritterMutual& cr, Item& item, uint8 mode);

// Generic stuff
//int  GetNightPersonBonus();
uint GetAimApCost(int hitLocation);
uint GetAimHit(int hitLocation);
uint GetMultihex(CritterMutual& cr);
int GetArmoredDR(CritterMutual& cr, int dmgType, const Item* armor);
int GetArmoredDT(CritterMutual& cr, int dmgType, const Item* armor);
const Item* GetHeadArmor(CritterMutual& cr);
bool IsRunning(CritterMutual& cr);
int GetArmorDR(CritterMutual& cr, int dmgType, const Item* armor);
int GetArmorDT(CritterMutual& cr, int dmgType, const Item* armor);

// utils.dll mixing for client
#ifdef __CLIENT
extern void RegisterAngelScriptExtensions();
EXPORT bool GetNameTextInfo( CritterCl& cr, int& x, int& y );
#endif

/************************************************************************/
/* Initialization                                                       */
/************************************************************************/

int __stdcall DllMain(void* module, unsigned long reason, void* reserved)
{
	// In this function all global variables is NOT initialized, use DllMainEx instead
	return 1;
}

FONLINE_DLL_ENTRY(compiler)
{
	// bool compiler - true if script compiled using ASCompiler, false if script compiled in server
	// In this function all global variables is initialized, if compiled not by compiler

	#ifdef __CLIENT
	RegisterAngelScriptExtensions();
	#endif

	if(compiler) return;

	// Register callbacks
	FOnline->GetUseApCost = &GetUseApCost;
	FOnline->GetAttackDistantion = &GetAttackDistantion;

	// Register script global vars
	memset(&GlobalVars, 0, sizeof(GlobalVars));
	for(uint i = 0; i < ASEngine->GetGlobalPropertyCount(); i++)
	{
		const char* name;
		void* ptr;
		if(ASEngine->GetGlobalPropertyByIndex(i,&name,NULL,NULL,NULL,NULL,&ptr) < 0) continue;

#define REGISTER_GLOBAL_VAR(type, gvar) else if(!strcmp(#gvar, name)) GlobalVars.gvar = (type*)ptr
		REGISTER_GLOBAL_VAR(int , CurX);
		REGISTER_GLOBAL_VAR(int , CurY);
		REGISTER_GLOBAL_VAR(uint, HitAimEyes);
		REGISTER_GLOBAL_VAR(uint, HitAimHead);
		REGISTER_GLOBAL_VAR(uint, HitAimGroin);
		REGISTER_GLOBAL_VAR(uint, HitAimTorso);
		REGISTER_GLOBAL_VAR(uint, HitAimArms);
		REGISTER_GLOBAL_VAR(uint, HitAimLegs);
	}
}

/************************************************************************/
/* Slot/parameters allowing                                             */
/************************************************************************/

EXPORT bool allowSlot_Hand1(uint8, Item&, Critter&, Critter& toCr)
{
	return toCr.Params[PE_AWARENESS] != 0;
}

/************************************************************************/
/* Parameters Get behaviors                                             */
/************************************************************************/

int checkBonus(Item& it, int bonusType)
{
	if(it.Data.ScriptValues[0]==bonusType) return it.Data.ScriptValues[5];
	if(it.Data.ScriptValues[1]==bonusType) return it.Data.ScriptValues[6];
	if(it.Data.ScriptValues[2]==bonusType) return it.Data.ScriptValues[7];
	if(it.Data.ScriptValues[3]==bonusType) return it.Data.ScriptValues[8];
	if(it.Data.ScriptValues[4]==bonusType) return it.Data.ScriptValues[9];
	return 0;
}

int checkBonus(const Item* it, int bonusType)
{
	if(it->Data.ScriptValues[0]==bonusType) return it->Data.ScriptValues[5];
	if(it->Data.ScriptValues[1]==bonusType) return it->Data.ScriptValues[6];
	if(it->Data.ScriptValues[2]==bonusType) return it->Data.ScriptValues[7];
	if(it->Data.ScriptValues[3]==bonusType) return it->Data.ScriptValues[8];
	if(it->Data.ScriptValues[4]==bonusType) return it->Data.ScriptValues[9];
	return 0;
}

EXPORT int getParam_Strength(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_STRENGTH] + cr.Params[ST_STRENGTH_EXT];

	const Item* armor=cr.ItemSlotArmor;
	if(checkBonus(armor, BONUS_ARMOR_STRENGTH)!=0) val++;

	return CLAMP(val, 1, 30);
}

EXPORT int getParam_Perception(CritterMutual& cr, uint)
{
	int val =  cr.Params[ST_PERCEPTION] + cr.Params[ST_PERCEPTION_EXT];
	//if(cr.Params[TRAIT_NIGHT_PERSON]) val += GetNightPersonBonus();

	if (cr.Params[DAMAGE_EYE])
		val=1;

	const Item* armor=cr.ItemSlotArmor;
	if(checkBonus(armor, BONUS_ARMOR_PERCEPTION)!=0) val++;

	return CLAMP(val, 1, 30);
}

EXPORT int getParam_Endurance(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_ENDURANCE] + cr.Params[ST_ENDURANCE_EXT];

	const Item* armor=cr.ItemSlotArmor;
	if(checkBonus(armor, BONUS_ARMOR_ENDURANCE)!=0) val++;

	return CLAMP(val, 1, 30);
}

EXPORT int getParam_Charisma(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_CHARISMA] + cr.Params[ST_CHARISMA_EXT];

	const Item* armor=cr.ItemSlotArmor;
	if(checkBonus(armor, BONUS_ARMOR_CHARISMA)!=0) val++;

	return CLAMP(val, 1, 30);
}

EXPORT int getParam_Intellegence(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_INTELLECT] + cr.Params[ST_INTELLECT_EXT];
	//if(cr.Params[TRAIT_NIGHT_PERSON]) val += GetNightPersonBonus();

	if (cr.Params[TRAIT_BERSERKER] && ((getParam_Hp(cr, 0)*100)/getParam_MaxLife(cr,0)) > 50)
		val=1;

	const Item* armor=cr.ItemSlotArmor;
	if(checkBonus(armor, BONUS_ARMOR_INTELLIGENCE)!=0) val++;

	return CLAMP(val, 1, 30);
}

EXPORT int getParam_Agility(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_AGILITY] + cr.Params[ST_AGILITY_EXT];

	const Item* armor=cr.ItemSlotArmor;
	if(checkBonus(armor, BONUS_ARMOR_AGILITY)!=0) val++;

	return CLAMP(val,1,30);
}

EXPORT int getParam_Luck(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_LUCK] + cr.Params[ST_LUCK_EXT];

	const Item* armor=cr.ItemSlotArmor;
	if(checkBonus(armor, BONUS_ARMOR_LUCK)!=0) val++;

	return CLAMP(val, 1, 30);
}

EXPORT int getParam_Hp(CritterMutual& cr, uint)
{
	return cr.Params[ST_CURRENT_HP];
}

EXPORT int getParam_HpHead(CritterMutual& cr, uint)
{
	return cr.Params[ST_HP_HEAD];
}

EXPORT int getParam_HpLa(CritterMutual& cr, uint)
{
	return cr.Params[ST_HP_LA];
}

EXPORT int getParam_HpRa(CritterMutual& cr, uint)
{
	return cr.Params[ST_HP_RA];
}

EXPORT int getParam_HpLl(CritterMutual& cr, uint)
{
	return cr.Params[ST_HP_LL];
}

EXPORT int getParam_HpRl(CritterMutual& cr, uint)
{
	return cr.Params[ST_HP_RL];
}

EXPORT int getParam_MaxLife(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_MAX_LIFE] + CLAMP(cr.Params[ST_LEVEL], 0, 30)* getParam_Endurance(cr, 0)/2 + cr.Params[ST_MAX_LIFE_EXT];
	return CLAMP(val, 1, 9999);
}

EXPORT int getParam_MaxAp(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_ACTION_POINTS] + cr.Params[ST_ACTION_POINTS_EXT];
	
	const Item* armor=cr.ItemSlotArmor;
	if(checkBonus(armor, BONUS_ARMOR_MAX_AP)!=0) val++;

	const Item* weapon = cr.ItemSlotMain;
	if (checkBonus(weapon, BONUS_WEAPON_MAX_AP)!=0) val++;

	if (cr.Params[TRAIT_BERSERKER] && ((getParam_Hp(cr, 0)*100)/getParam_MaxLife(cr,0)) <= 50)
		val+=2;

	return CLAMP(val, 1, 9999);
}

EXPORT int getParam_Level(CritterMutual& cr, uint)
{
    return cr.Params[ST_LEVEL];
}

EXPORT int getParam_Ap(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_CURRENT_AP];
	val /= AP_DIVIDER;
	return CLAMP(val, -9999, 9999);
}

EXPORT int getParam_MaxMoveAp(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_MAX_MOVE_AP];
	return CLAMP(val, 0, 9999);
}

EXPORT int getParam_MoveAp(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_MOVE_AP];
	return CLAMP(val, 0, 9999);
}

EXPORT int getParam_MaxWeight(CritterMutual& cr, uint)
{
	return 400000;
	int val = max(cr.Params[ST_CARRY_WEIGHT] + cr.Params[ST_CARRY_WEIGHT_EXT], 0);
	val += CONVERT_GRAMM(25 + getParam_Strength(cr, 0) * (25 - cr.Params[TRAIT_SMALL_FRAME] * 10));
	if(cr.Params[PE_PACK_RAT])
	{
		val*=4;
		val/=3;
	}
	val+=20000;

	const Item* armor=cr.ItemSlotArmor;
	val+=checkBonus(armor, BONUS_ARMOR_CARRY_WEIGHT)*1000;

	return CLAMP(val, 0, 2000000000);
}

EXPORT int getParam_Sequence(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_SEQUENCE] + cr.Params[ST_SEQUENCE_EXT] + getParam_Perception(cr, 0) * 2;
	return CLAMP(val, 0, 9999);
}

EXPORT int getParam_MeleeDmg(CritterMutual& cr, uint)
{
	int strength = getParam_Strength(cr, 0);
	int val = cr.Params[ST_MELEE_DAMAGE] + cr.Params[ST_MELEE_DAMAGE_EXT] + (strength > 6 ? (cr.Params[TRAIT_BRUISER]?2*(strength - 5):(strength - 5)) : 1);
	return CLAMP(val, 1, 9999);
}

EXPORT int getParam_HealingRate(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_HEALING_RATE] + cr.Params[ST_HEALING_RATE_EXT] + 3 * getParam_Charisma(cr, 0);

	if(cr.Params[TRAIT_FAST_METABOLISM]) val += 10;

	const Item* armor=cr.ItemSlotArmor;
	val+=checkBonus(armor, BONUS_ARMOR_HEALING_RATE);

	return CLAMP(val, 0, 9999);
}

EXPORT int getParam_CriticalChance(CritterMutual& cr, uint)
{
	return 0;
}

EXPORT int getParam_CriticalPower(CritterMutual& cr, uint)
{
	return getParam_Luck(cr, 0) * 3 + cr.Params[PE_BETTER_CRITICALS] * 15;
}

EXPORT int getParam_MaxCritical(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_MAX_CRITICAL] + cr.Params[ST_MAX_CRITICAL_EXT];
	return CLAMP(val, -100, 100);
}

int GetRunningAc(CritterMutual& cr, bool head)
{
	int val = cr.Params[ST_ARMOR_CLASS] + cr.Params[ST_ARMOR_CLASS_EXT];
	val += getParam_Agility(cr, 0);

	while(cr.Params[PE_HTH_EVADE])
	{
		const Item* weapon=cr.ItemSlotMain;
		if(!weapon->IsWeapon())
			break;
		if(weapon->Proto->Weapon_Skill[0]!=SK_THROWING &&
			weapon->Proto->Weapon_Skill[0]!=SK_UNARMED)
			break; // damn spears
		weapon=cr.ItemSlotExt;
		if(!weapon->IsWeapon())
			break;
		if(weapon->Proto->Weapon_Skill[0]!=SK_THROWING &&
			weapon->Proto->Weapon_Skill[0]!=SK_UNARMED)
			break; // damn spears

		if(cr.Params[PE_HTH_EVADE]) val+=20;
		break;
	}

	const Item* armor = head ? GetHeadArmor(cr) : cr.ItemSlotArmor;
	if(armor->GetId() && armor->IsArmor()) val += armor->Proto->Armor_AC;

	return CLAMP(val, 0, 95);
}

EXPORT int getParam_Ac(CritterMutual& cr, uint)
{
	if(!IsRunning(cr))
	{
		int StandingAC = (GetRunningAc(cr,false));
		return StandingAC; // todo: turn based
	}
	// if(!IsRunning(cr)) return 0; // todo: turn based
	return GetRunningAc(cr,false) + (cr.Params[PE_LIVEWIRE] ? getParam_Agility(cr, 0) : 0);
}

EXPORT int getParam_DamageResistance(CritterMutual& cr, uint index)
{
	int dmgType = index - ST_NORMAL_RESIST + 1;
	return GetArmoredDR(cr, dmgType, cr.ItemSlotArmor);
}

EXPORT int getParam_DamageThreshold(CritterMutual& cr, uint index)
{
	int dmgType = index - ST_NORMAL_ABSORB + 1;
	return GetArmoredDT(cr, dmgType, cr.ItemSlotArmor);
}

EXPORT int getParam_RadiationResist(CritterMutual& cr, uint)
{
	int val = cr.Params[ST_RADIATION_RESISTANCE] + cr.Params[ST_RADIATION_RESISTANCE_EXT] + 20 + getParam_Endurance(cr, 0) * 3;
	if (cr.Params[TRAIT_FAST_METABOLISM]) val -= 30;

	const Item* armor=cr.ItemSlotArmor;
	val+=checkBonus(armor, BONUS_ARMOR_RAD_RES);

	return CLAMP(val, -95, 95);
}

EXPORT int getParam_PoisonResist(CritterMutual& cr, uint)
{
	int endurance = getParam_Endurance(cr, 0);
	int val = cr.Params[ST_POISON_RESISTANCE] + cr.Params[ST_POISON_RESISTANCE_EXT] + 8 + endurance * 2 + (endurance > 10 ? 6 : 0) + (endurance > 15 ? 6 : 0);
	if (cr.Params[TRAIT_FAST_METABOLISM]) val -= 30;

	const Item* armor=cr.ItemSlotArmor;
	val+=checkBonus(armor, BONUS_ARMOR_POISON_RES);

	return CLAMP(val, -95, 95);
}

EXPORT int getParam_Reputation(CritterMutual& cr, uint index)
{
/*#ifdef __SERVER
	if(cr.Params[index] == 0x80000000)
	{
		FOnline->CritterChangeParameter(cr, index);
		cr.Params[index] = 0;
	}
#else*/
	if(cr.Params[index] == 0x80000000) return 0;
//#endif
	return cr.Params[index];
}

EXPORT int getParam_Timeout(CritterMutual& cr, uint index)
{
	return (uint)cr.Params[index] > FOnline->FullSecond ? (uint)cr.Params[index] - FOnline->FullSecond : 0;
}

EXPORT int getParam_SleepinessTimeout(CritterMutual& cr, uint index)
{
	return 0;
	/*
	uint full = FOnline->FullSecond;
	uint stopped=cr.Params[TO_SLEEPY_STOPPED];
	uint cumulated=0;
	if(cr.Params[TO_SLEEPY_CUMULATE]!=0 && full>uint(cr.Params[TO_IMMUNITY_TIME]))
	{
		if(cr.Params[TO_IMMUNITY_TIME]>cr.Params[TO_SLEEPY_CUMULATE]) cumulated=cr.Params[TO_IMMUNITY_TIME];
		else cumulated=cr.Params[TO_SLEEPY_CUMULATE];
	}
	uint sleepiness = cr.Params[index] + ((cumulated!=0)?(full-cumulated):0) + ((stopped!=0 && stopped!=(uint)-1)?(full-stopped):0);
	return sleepiness > FOnline->FullSecond ? sleepiness - FOnline->FullSecond : 0;
	*/
}

/*EXPORT void changedParam_Reputation(CritterMutual& cr, uint index, int oldValue)
{
	if(oldValue == 0x80000000) cr.Params[index] += 0x80000000;
}*/

/************************************************************************/
/* Extended methods                                                     */
/************************************************************************/

EXPORT bool Critter_IsInjured(CritterMutual& cr)
{
	return Critter_IsDmgArm(cr) || Critter_IsDmgLeg(cr) || Critter_IsDmgEye(cr);
}

EXPORT bool Critter_IsDmgEye(CritterMutual& cr)
{
	return cr.Params[DAMAGE_EYE] != 0;
}

EXPORT bool Critter_IsDmgLeg(CritterMutual& cr)
{
	return cr.Params[DAMAGE_RIGHT_LEG] || cr.Params[DAMAGE_LEFT_LEG];
}

EXPORT bool Critter_IsDmgTwoLeg(CritterMutual& cr)
{
	return cr.Params[DAMAGE_RIGHT_LEG] && cr.Params[DAMAGE_LEFT_LEG];
}

EXPORT bool Critter_IsDmgArm(CritterMutual& cr)
{
	return cr.Params[DAMAGE_RIGHT_ARM] || cr.Params[DAMAGE_LEFT_ARM];
}

EXPORT bool Critter_IsDmgTwoArm(CritterMutual& cr)
{
	return cr.Params[DAMAGE_RIGHT_ARM] && cr.Params[DAMAGE_LEFT_ARM];
}

EXPORT bool Critter_IsAddicted(CritterMutual& cr)
{
	for(uint i = ADDICTION_BEGIN; i <= ADDICTION_END; i++)
		if(cr.Params[i]) return true;
	return false;
}

EXPORT bool Critter_IsOverweight(CritterMutual& cr)
{
	// Calculate inventory items weight
	uint w = 0;
	for(ItemVecIt it = cr.InvItems.begin(), end = cr.InvItems.end(); it != end; ++it)
		w += (*it)->GetWeight();

	return w > (uint)getParam_MaxWeight(cr, 0);
}

EXPORT int Critter_GetAC(CritterMutual& cr, bool head)
{
	if(!IsRunning(cr))
	{
		int StandingAC = (GetRunningAc(cr,head));
		return StandingAC; // todo: turn based
	}
	
	// if(!IsRunning(cr)) return 0; // todo: turn based
	return GetRunningAc(cr,head);
}

EXPORT int Critter_GetDR(CritterMutual& cr, uint dmgType, bool head)
{
	return GetArmoredDR(cr, dmgType, head ? GetHeadArmor(cr) : cr.ItemSlotArmor);
}

EXPORT int Critter_GetDT(CritterMutual& cr, uint dmgType, bool head)
{
	return GetArmoredDT(cr, dmgType, head ? GetHeadArmor(cr) : cr.ItemSlotArmor);
}

EXPORT int Critter_GetArmorDR(CritterMutual& cr, uint dmgType, bool head)
{
	return GetArmorDR(cr, dmgType, head ? GetHeadArmor(cr) : cr.ItemSlotArmor);
}

EXPORT int Critter_GetArmorDT(CritterMutual& cr, uint dmgType, bool head)
{
	return GetArmorDT(cr, dmgType, head ? GetHeadArmor(cr) : cr.ItemSlotArmor);
}

EXPORT bool Item_Weapon_IsHtHAttack(Item& item, uint8 mode)
{
	if(!item.IsWeapon() || !item.WeapIsUseAviable(mode & 7)) return false;
	int skill = SKILL_OFFSET(item.Proto->Weapon_Skill[mode & 7]);
	return skill == SK_UNARMED;
}

EXPORT bool Item_Weapon_IsGunAttack(Item& item, uint8 mode)
{
	if(!item.IsWeapon() || !item.WeapIsUseAviable(mode & 7)) return false;
	int skill = SKILL_OFFSET(item.Proto->Weapon_Skill[mode & 7]);
	return skill == SK_SMALL_GUNS || skill == SK_BIG_GUNS || skill == SK_ENERGY_WEAPONS || skill == SK_MELEE_WEAPONS;
}

EXPORT bool Item_Weapon_IsRangedAttack(Item& item, uint8 mode)
{
	if(!item.IsWeapon() || !item.WeapIsUseAviable(mode & 7)) return false;
	int skill = SKILL_OFFSET(item.Proto->Weapon_Skill[mode & 7]);
	return skill == SK_SMALL_GUNS || skill == SK_BIG_GUNS || skill == SK_ENERGY_WEAPONS || skill == SK_THROWING || skill == SK_MELEE_WEAPONS;
}


EXPORT void Item_Weapon_SetMode(Item& item, uint8 mode)
{
	item.Data.Rate=mode;
}

#ifdef __CLIENT
EXPORT bool GetNameTextInfo( CritterCl& cr, int& x, int& y )
{
	if( !cr.strTextOnHead.empty() )
		return false;
		
	x = (int) ( (float) ( cr.TextRect.L + (cr.TextRect.R - cr.TextRect.L) / 2 + FOnline->ScrOx ) / FOnline->SpritesZoom - 100.0f );
	y = (int) ( (float) ( cr.TextRect.T + FOnline->ScrOy ) / FOnline->SpritesZoom - 70.0f );
	return true;
}
#endif

/************************************************************************/
/* Callbacks                                                            */
/************************************************************************/

uint GetUseApCost(CritterMutual& cr, Item& item, uint8 mode)
{
	uint8 use = mode & 0xF;
	uint8 aim = mode >> 4;
	int apCost = 1;

	if(use == USE_USE)
	{
		if(TB_BATTLE_TIMEOUT_CHECK(getParam_Timeout(cr, TO_BATTLE)))
			apCost = FOnline->TbApCostUseItem;
		else
			apCost = FOnline->RtApCostUseItem;
	}
	else if(use == USE_RELOAD)
	{
		if (!item.Proto->Weapon_ReloadAp)
			apCost = 3;
		if(TB_BATTLE_TIMEOUT_CHECK(getParam_Timeout(cr, TO_BATTLE)))
			apCost = FOnline->TbApCostReloadWeapon;
		else
			//apCost = FOnline->RtApCostReloadWeapon;
			apCost = item.Proto->Weapon_ReloadAp;
		if(cr.Params[PE_QUICK_POCKETS]) return CLAMP(apCost-3, 0, 10);
		//if(item.IsWeapon() && item.Proto->Weapon_Perk == WEAPON_PERK_FAST_RELOAD) apCost/2; // That one is deleted
	}
	else if(use >= USE_PRIMARY && use <= USE_THIRD && item.IsWeapon())
	{
		int skill = item.Proto->Weapon_Skill[use];
		bool hthAttack = Item_Weapon_IsHtHAttack(item, mode);
		bool rangedAttack = Item_Weapon_IsRangedAttack(item, mode);

		apCost = item.Proto->Weapon_ApCost[use];
		if(aim) apCost += GetAimApCost(aim);
		if(cr.Params[PE_BONUS_RATE_OF_FIRE] !=0) apCost--;
		if(cr.Params[TRAIT_HEAVY_HANDED] && hthAttack) apCost++;
		//if(checkBonus(item, BONUS_WEAPON_AP_COST)!=0) apCost--;
	}

	if(apCost < 1) apCost = 1;
	if(item.GetProtoId()==144) apCost=5;
	return apCost;
}

uint GetAttackDistantion(CritterMutual& cr, Item& item, uint8 mode)
{
	uint8 use = mode & 0xF;
	int dist = item.Proto->Weapon_MaxDist[use];
	int strength = getParam_Strength(cr, 0);
	if(item.Proto->Weapon_Skill[use] == SKILL_OFFSET(SK_THROWING))
	{
		dist = min(dist, 3 * min(10, strength));
		//dist+=3*(cr.Params[PE_HEAVE_HO]+cr.Params[PE_HEAVE_HO_II]);
		dist+=6*(cr.Params[PE_HEAVE_HO]+cr.Params[PE_HEAVE_HO_II]);
	}
	if(Item_Weapon_IsHtHAttack(item, mode) && cr.Params[MODE_RANGE_HTH]) dist++;
	dist += GetMultihex(cr);
	dist += checkBonus(item, BONUS_WEAPON_MAX_RANGE);
	if(item.Proto->Weapon_Perk == WEAPON_PERK_SCOPE_RANGE && cr.Params[PE_SHARPSHOOTER])
		dist+=5;
	if(dist < 0) dist = 0;
	return dist;
}



/************************************************************************/
/* Generic stuff                                                        */
/************************************************************************/

/*
int GetNightPersonBonus()
{
	if(FOnline->Hour < 6 || FOnline->Hour > 18) return 1;
	if(FOnline->Hour == 6 && FOnline->Minute == 0) return 1;
	if(FOnline->Hour == 18 && FOnline->Minute > 0) return 1;
	return -1;
}
*/

uint GetAimApCost(int hitLocation)
{
	switch(hitLocation)
	{
	case HIT_LOCATION_TORSO:     return FOnline->ApCostAimTorso;
	case HIT_LOCATION_EYES:      return FOnline->ApCostAimEyes;
	case HIT_LOCATION_HEAD:      return FOnline->ApCostAimHead;
	case HIT_LOCATION_LEFT_ARM:
	case HIT_LOCATION_RIGHT_ARM: return FOnline->ApCostAimArms;
	case HIT_LOCATION_GROIN:     return FOnline->ApCostAimGroin;
	case HIT_LOCATION_RIGHT_LEG:
	case HIT_LOCATION_LEFT_LEG:  return FOnline->ApCostAimLegs;
	case HIT_LOCATION_NONE:
	case HIT_LOCATION_UNCALLED:
	default: break;
	}
	return 0;
}

uint GetAimHit(int hitLocation)
{
	switch(hitLocation)
	{
	case HIT_LOCATION_TORSO:     return *GlobalVars.HitAimTorso;
	case HIT_LOCATION_EYES:      return *GlobalVars.HitAimEyes;
	case HIT_LOCATION_HEAD:      return *GlobalVars.HitAimHead;
	case HIT_LOCATION_LEFT_ARM:
	case HIT_LOCATION_RIGHT_ARM: return *GlobalVars.HitAimArms;
	case HIT_LOCATION_GROIN:     return *GlobalVars.HitAimGroin;
	case HIT_LOCATION_RIGHT_LEG:
	case HIT_LOCATION_LEFT_LEG:  return *GlobalVars.HitAimLegs;
	case HIT_LOCATION_NONE:
	case HIT_LOCATION_UNCALLED:
	default: break;
	}
	return 0;
}

uint GetMultihex(CritterMutual& cr)
{
	int mh = cr.Multihex;
	if(mh < 0) mh = FOnline->CritterTypes[cr.BaseType].Multihex;
	return CLAMP(mh, 0, MAX_HEX_OFFSET);
}

/************************************************************************/
/* Support functions                                                    */
/************************************************************************/

int GetRawDR(CritterMutual& cr, int dmgType)
{
	int val = 0;
	switch(dmgType)
	{
	case DAMAGE_NORMAL:   val = cr.Params[ST_NORMAL_RESIST]  + cr.Params[ST_NORMAL_RESIST_EXT];  break;
	case DAMAGE_LASER:    val = cr.Params[ST_LASER_RESIST]   + cr.Params[ST_LASER_RESIST_EXT];   break;
	case DAMAGE_FIRE:     val = cr.Params[ST_FIRE_RESIST]    + cr.Params[ST_FIRE_RESIST_EXT];    break;
	case DAMAGE_PLASMA:   val = cr.Params[ST_PLASMA_RESIST]  + cr.Params[ST_PLASMA_RESIST_EXT];  break;
	case DAMAGE_ELECTR:   val = cr.Params[ST_ELECTRO_RESIST] + cr.Params[ST_ELECTRO_RESIST_EXT]; break;
	case DAMAGE_EMP:      val = cr.Params[ST_EMP_RESIST]     + cr.Params[ST_EMP_RESIST_EXT];     break;
	case DAMAGE_EXPLODE:  val = cr.Params[ST_EXPLODE_RESIST] + cr.Params[ST_EXPLODE_RESIST_EXT]; break;
	case DAMAGE_UNCALLED:
	default: break;
	}

	if(cr.Params[PE_ADRENALINE_RUSH]) // Adrenaline rush perk
	{
		int max_life=getParam_MaxLife(cr,0);
		if(cr.Params[ST_CURRENT_HP] <= max_life/4) val+=15;
		else if(cr.Params[ST_CURRENT_HP] <= max_life/2) val+=10;
		else if(cr.Params[ST_CURRENT_HP] <= 3*max_life/4) val+=5;
	}

	return val;
}

int GetRawDT(CritterMutual& cr, int dmgType)
{

	int val = 0;
	switch(dmgType)
	{
	case DAMAGE_NORMAL:   val = cr.Params[ST_NORMAL_ABSORB]  + cr.Params[ST_NORMAL_ABSORB_EXT];  break;
	case DAMAGE_LASER:    val = cr.Params[ST_LASER_ABSORB]   + cr.Params[ST_LASER_ABSORB_EXT];   break;
	case DAMAGE_FIRE:     val = cr.Params[ST_FIRE_ABSORB]    + cr.Params[ST_FIRE_ABSORB_EXT];    break;
	case DAMAGE_PLASMA:   val = cr.Params[ST_PLASMA_ABSORB]  + cr.Params[ST_PLASMA_ABSORB_EXT];  break;
	case DAMAGE_ELECTR:   val = cr.Params[ST_ELECTRO_ABSORB] + cr.Params[ST_ELECTRO_ABSORB_EXT]; break;
	case DAMAGE_EMP:      val = cr.Params[ST_EMP_ABSORB]     + cr.Params[ST_EMP_ABSORB_EXT];     break;
	case DAMAGE_EXPLODE:  val = cr.Params[ST_EXPLODE_ABSORB] + cr.Params[ST_EXPLODE_ABSORB_EXT]; break;
	case DAMAGE_UNCALLED:
	default: break;
	}

	if(cr.Params[PE_ADRENALINE_RUSH]) // Adrenaline rush perk
	{
		int max_life=getParam_MaxLife(cr,0);
		if(cr.Params[ST_CURRENT_HP] <= max_life/4) val+=3;
		else if(cr.Params[ST_CURRENT_HP] <= max_life/2) val+=2;
		else if(cr.Params[ST_CURRENT_HP] <= 3*max_life/4) val+=1;
	}

	return val;
}

int GetArmoredDR(CritterMutual& cr, int dmgType, const Item* armor)
{ 
	int val = 0;
	int drVal = 0;

	switch(dmgType)
	{
	case DAMAGE_NORMAL:   val = GetRawDR(cr,dmgType); drVal = armor->Proto->Armor_DRNormal + checkBonus(armor, BONUS_ARMOR_NORMAL_DR);  break;
	case DAMAGE_LASER:    val = GetRawDR(cr,dmgType); drVal = armor->Proto->Armor_DRLaser + checkBonus(armor, BONUS_ARMOR_LASER_DR);   break;
	case DAMAGE_FIRE:     val = GetRawDR(cr,dmgType); drVal = armor->Proto->Armor_DRFire + checkBonus(armor, BONUS_ARMOR_FIRE_DR);    break;
	case DAMAGE_PLASMA:   val = GetRawDR(cr,dmgType); drVal = armor->Proto->Armor_DRPlasma + checkBonus(armor, BONUS_ARMOR_PLASMA_DR);  break;
	case DAMAGE_ELECTR:   val = GetRawDR(cr,dmgType); drVal = armor->Proto->Armor_DRElectr;  break;
	case DAMAGE_EMP:      val = GetRawDR(cr,dmgType); drVal = armor->Proto->Armor_DREmp;     break;
	case DAMAGE_EXPLODE:  val = GetRawDR(cr,dmgType); drVal = armor->Proto->Armor_DRExplode + checkBonus(armor, BONUS_ARMOR_EXPLODE_DR); break;
	case DAMAGE_UNCALLED:
	default: break;
	}

	if(armor->GetId() && armor->IsArmor())
	{
		if(FLAG(cr.Params[MODE_EXT],MODE_EXT_NO_DETERIORATION))
			val += drVal;
		else
			val += (armor->GetDeteriorationProc()!=100)?drVal:0;
	}

	if(dmgType == DAMAGE_EMP) return CLAMP(val, 0, 999);
	return CLAMP(val, 0, 90);
}

int GetArmoredDT(CritterMutual& cr, int dmgType, const Item* armor)
{
	int val = 0;
	int dtVal = 0;
	switch(dmgType)
	{
	case DAMAGE_NORMAL:   val = GetRawDT(cr,dmgType); dtVal = armor->Proto->Armor_DTNormal + checkBonus(armor, BONUS_ARMOR_NORMAL_DT);  break;
	case DAMAGE_LASER:    val = GetRawDT(cr,dmgType); dtVal = armor->Proto->Armor_DTLaser + checkBonus(armor, BONUS_ARMOR_LASER_DT);   break;
	case DAMAGE_FIRE:     val = GetRawDT(cr,dmgType); dtVal = armor->Proto->Armor_DTFire + checkBonus(armor, BONUS_ARMOR_FIRE_DT);    break;
	case DAMAGE_PLASMA:   val = GetRawDT(cr,dmgType); dtVal = armor->Proto->Armor_DTPlasma + checkBonus(armor, BONUS_ARMOR_PLASMA_DT);  break;
	case DAMAGE_ELECTR:   val = GetRawDT(cr,dmgType); dtVal = armor->Proto->Armor_DTElectr;  break;
	case DAMAGE_EMP:      val = GetRawDT(cr,dmgType); dtVal = armor->Proto->Armor_DTEmp;     break;
	case DAMAGE_EXPLODE:  val = GetRawDT(cr,dmgType); dtVal = armor->Proto->Armor_DTExplode + checkBonus(armor, BONUS_ARMOR_EXPLODE_DT); break;
	case DAMAGE_UNCALLED:
	default: break;
	}

	if(armor->GetId() && armor->IsArmor())
	{
		if(FLAG(cr.Params[MODE_EXT],MODE_EXT_NO_DETERIORATION))
			val += dtVal;
		else
			val += (armor->GetDeteriorationProc()!=100)?dtVal:0;
	}

	return CLAMP(val, 0, 999);
}

const Item* GetHeadArmor(CritterMutual& cr)
{
	for(ItemVecIt it=cr.InvItems.begin(),end=cr.InvItems.end();it!=end;++it)
	{
		if((*it)->AccCritter.Slot==SLOT_HEAD) return *it;
	}
	return cr.DefItemSlotArmor;
}

int GetArmorDR(CritterMutual& cr, int dmgType, const Item* armor)
{
	int val = 0;
	int drVal = 0;
	switch(dmgType)
	{
	case DAMAGE_NORMAL:   drVal = armor->Proto->Armor_DRNormal + checkBonus(armor, BONUS_ARMOR_NORMAL_DR);  break;
	case DAMAGE_LASER:    drVal = armor->Proto->Armor_DRLaser + checkBonus(armor, BONUS_ARMOR_LASER_DR);   break;
	case DAMAGE_FIRE:     drVal = armor->Proto->Armor_DRFire + checkBonus(armor, BONUS_ARMOR_FIRE_DR);    break;
	case DAMAGE_PLASMA:   drVal = armor->Proto->Armor_DRPlasma + checkBonus(armor, BONUS_ARMOR_PLASMA_DR);  break;
	case DAMAGE_ELECTR:   drVal = armor->Proto->Armor_DRElectr;  break;
	case DAMAGE_EMP:      drVal = armor->Proto->Armor_DREmp;     break;
	case DAMAGE_EXPLODE:  drVal = armor->Proto->Armor_DRExplode + checkBonus(armor, BONUS_ARMOR_EXPLODE_DR); break;
	case DAMAGE_UNCALLED:
	default: break;
	}

	if(armor->GetId() && armor->IsArmor())
	{
		if(FLAG(cr.Params[MODE_EXT],MODE_EXT_NO_DETERIORATION))
			val += drVal;
		else
			val += (armor->GetDeteriorationProc()!=100)?drVal:0;
	}

	if(dmgType == DAMAGE_EMP) return CLAMP(val, 0, 999);
	return CLAMP(val, 0, 90);
}

int GetArmorDT(CritterMutual& cr, int dmgType, const Item* armor)
{
	int val = 0;
	int dtVal = 0;
	switch(dmgType)
	{
	case DAMAGE_NORMAL:   dtVal = armor->Proto->Armor_DTNormal + checkBonus(armor, BONUS_ARMOR_NORMAL_DT);  break;
	case DAMAGE_LASER:    dtVal = armor->Proto->Armor_DTLaser + checkBonus(armor, BONUS_ARMOR_LASER_DT);   break;
	case DAMAGE_FIRE:     dtVal = armor->Proto->Armor_DTFire + checkBonus(armor, BONUS_ARMOR_FIRE_DT);    break;
	case DAMAGE_PLASMA:   dtVal = armor->Proto->Armor_DTPlasma + checkBonus(armor, BONUS_ARMOR_PLASMA_DT);  break;
	case DAMAGE_ELECTR:   dtVal = armor->Proto->Armor_DTElectr;  break;
	case DAMAGE_EMP:      dtVal = armor->Proto->Armor_DTEmp;     break;
	case DAMAGE_EXPLODE:  dtVal = armor->Proto->Armor_DTExplode + checkBonus(armor, BONUS_ARMOR_EXPLODE_DT); break;
	case DAMAGE_UNCALLED:
	default: break;
	}

	if(armor->GetId() && armor->IsArmor())
	{
		if(FLAG(cr.Params[MODE_EXT],MODE_EXT_NO_DETERIORATION))
			val += dtVal;
		else
			val += (armor->GetDeteriorationProc()!=100)?dtVal:0;
	}

	return CLAMP(val, 0, 999);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifdef __SERVER

int WalkTime[MAX_CRIT_TYPES];
int TickDiff=0;

EXPORT bool InitWalkProcessing(Critter& cr)
{
	TickDiff=(int)timeGetTime()-cr.WaitEndTick;

	// read critter types
	FILE* f=NULL;
#ifdef _MSC_VER
	fopen_s(&f, "./data/CritterTypes.cfg", "r");
#else
	f=fopen("./data/CritterTypes.cfg", "r");
#endif
	if(!f)
	{
		printf("Terrible error, see InitWalkProcessing in parameters.dll.\n");
		return false;
	}
	char line[2048];
	bool found=false;

	while(!feof(f))
	{
		fgets(line,2048,f);
		int id=0;
		int walk=0;
		//@ Id Name    Alias MH  3d  Walk  Run  Aim Armor Rotate  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z   Walk  Run   Walk steps Sound name
		istrstream str(line);

		string svalue;
		int value;

		str >> svalue;
		if(str.fail() || svalue!="@") continue;

		str >> id;
		if(str.fail()) continue;

		str >> svalue;
		if(str.fail()) continue;
		str >> svalue;
		if(str.fail()) continue;
		bool fail=false;
		for(int j=0;j<33;j++)
		{
			str >> value;
			if(str.fail())
			{
				fail=true;
				break;
			}
		}
		if(fail) continue;

		str >> walk;
		if(str.fail()) continue;
		WalkTime[id]=walk;
		found=true;
	}
	fclose(f);
	if(!found)
	{
		Log("Error: no crtypes read, see InitWalkProcessing in parameters.dll.");
		return false;
	}
	return true;
}

EXPORT bool Critter_IsMoving(Critter& cr)
{
	return (int)timeGetTime()-TickDiff-(int)cr.PrevHexTick<WalkTime[cr.BaseType];
}

#endif

bool IsRunning(CritterMutual& cr)
{
	if(getParam_Timeout(cr,TO_BATTLE)>100000) return cr.Params[ST_TURN_BASED_AC]>0;
#ifdef __SERVER
	return Critter_IsMoving(cr) && (cr.IsRuning || !FOnline->CritterTypes[cr.BaseType].CanRun);
#endif
#ifdef __CLIENT
	return cr.MoveSteps.size() && (cr.IsRuning || !FOnline->CritterTypes[cr.BaseType].CanRun);
#endif
}