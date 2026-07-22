#include <cmath>
#include "Move.h"
#include "WallDist.h"
#include "Smoke.h"
#include "../../scripts/_animation.fos"
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#ifndef ARMOR_PERK_RATTLING
#define ARMOR_PERK_RATTLING (6)
#endif

static const Item* GetEquippedUtility(const Critter& cr)
{
	for(ItemVecIt it = cr.InvItems.begin(), end = cr.InvItems.end(); it != end; ++it)
	{
		const Item* item = *it;
		if(item && item->Proto && item->AccCritter.Slot == SLOT_UTILITY && item->Proto->Slot == SLOT_UTILITY)
			return item;
	}
	return NULL;
}

static int GetUtilityParamBonus(const Critter& cr, uint param)
{
	const Item* item = GetEquippedUtility(cr);
	if(item && !FLAG(item->Data.BrokenFlags, BI_BROKEN) && item->Proto->Utility_Param == (int)param)
		return item->Proto->Utility_Value;
	return 0;
}

// TODO: multihex bonus to look distance (once multihex works properly)

bool isCompiler=false;
volatile long Init=0;
CRITICAL_SECTION LookInitLocker;
CRITICAL_SECTION LookDistsLocker;
CRITICAL_SECTION LookSmokeLocker;

extern uint8* Lookup;
extern void InitLookup();
extern void FinishLookup();

extern void InitDists();
extern WallDist* GetProtoDists(Map& map);
extern void FinishDists();
extern bool IsMoving(Critter& cr);

void InitLook() // generation
{
	LookLockGuard guard(LookInitLocker);
	if(Init) return;
	InitLookup();
	InitDists();
	InterlockedExchange(&Init, 1);
}

int __stdcall DllMain(void* module, unsigned long reason, void* reserved)
{
	switch(reason)
	{
	case 1: // Process attach
		InitializeCriticalSection(&LookInitLocker);
		InitializeCriticalSection(&LookDistsLocker);
		InitializeCriticalSection(&LookSmokeLocker);
		break;
	case 2: // Thread attach
		break;
	case 3: // Thread detach
		break;
	case 0: // Process detach
		{
			if(!isCompiler)
			{
				FinishLookup();
				FinishDists();
			}
			DeleteCriticalSection(&LookSmokeLocker);
			DeleteCriticalSection(&LookDistsLocker);
			DeleteCriticalSection(&LookInitLocker);
		}
		break;
	}
	return 1;
}

FONLINE_DLL_ENTRY(compiler)
{
	isCompiler=compiler;
}

// The server checks for this marker before allowing native callbacks from this
// DLL to execute concurrently. Old binaries without the marker remain safe by
// falling back to the serialized path.
EXPORT bool ConcurrentExecutionSafe()
{
	return true;
}

int CheckOccluder(uint16 hx, uint16 hy, uint16 tx, uint16 ty, Map& map)
{
    int dx = (int)tx - (int)hx;
    int dy = (int)ty - (int)hy;
    if ((ABS(dx)>=MAX_TRACE) || (ABS(dy)>=MAX_TRACE)) return 0;
    int idx = (SIZE_LIN*(dy+MAX_TRACE)+dx+MAX_TRACE)*MAX_ARR;
    uint16 maxX = map.Proto->Header.MaxHexX;
    uint16 maxY = map.Proto->Header.MaxHexY;
    for (uint i=0;i<OCCLUDER_DIST;i++)
    {
        Move(hx,hy,Lookup[idx++]);
        if ((hx>=maxX) || (hy>=maxY)) return 0; // this include uint16(-1), it's not nice, but practical

		// you can shoot thru
		if(map.IsHexRaked(hx, hy))
		{
			// but not pass - that means occluder
			if(!map.IsHexPassed(hx, hy))
                return BONUS_OCCLUDER;// optimize it that way for now
			else
				continue; // clean
		} // you can't shoot through it
		else
			return 100; // shouldn't even occur (trace check first)
	}
    return 0;
}

int TraceWall(uint16 hx, uint16 hy, uint16 tx, uint16 ty, Map& map, int dist)
{
    // if (dist==0) dist=GetDistantion(hx,hy,tx,ty); // not needed here; enable if and when needed
    int dx = (int)tx - (int)hx;
    int dy = (int)ty - (int)hy;
    if ((ABS(dx)>=MAX_TRACE) || (ABS(dy)>=MAX_TRACE)) return 0;
    int idx = (SIZE_LIN*(dy+MAX_TRACE)+dx+MAX_TRACE)*MAX_ARR;
    uint16 maxX = map.Proto->Header.MaxHexX;
    uint16 maxY = map.Proto->Header.MaxHexY;
    for (int i=0;i<dist;i++)
    {
        Move(hx,hy,Lookup[idx++]);
        if ((hx>=maxX) || (hy>=maxY)) return i; // this include uint16(-1), it's not nice, but practical
        if(!map.IsHexRaked(hx, hy)) return i;
	}
    return dist;
}

EXPORT bool check_look(Map& map, Critter& cr, Critter& opponent)
{
	if(!Init) InitLook();
	if(_CritHasExtMode(opponent,MODE_EXT_GOD)) return false;
	if(opponent.Params[ST_CLASS_ID] == CLASS_DEATHCLAW &&
	   opponent.Params[ST_CLASS_EFFECT] == CLASS_EFFECT_DEATHCLAW_UNDERGROUND) return false;

	if((uint)opponent.Params[TO_GOD_MODE] > FOnline->FullSecond) return false;

	if(!cr.CritterIsNpc)
	{
		if(_CritHasExtMode(cr,MODE_EXT_LOOK_ADMIN)) return true;
		if(cr.MapId != map.Data.MapId && map.Data.UserData[MAP_DATA_ACTIVE_COUNTDOWN]!=0) return false;
	}
	if(_CritHasExtMode(opponent,MODE_EXT_LOOK_INVISIBLE) && _CritHasMode(opponent, MODE_HIDE))
        return false; // 100% invis for admins

	if(_CritHasExtMode(opponent,MODE_EXT_LOOK_ALWAYS_VISIBLE) && !_CritHasMode(opponent, MODE_HIDE))
        return true;

	uint16 cx = cr.HexX;
	uint16 cy = cr.HexY;
	uint16 ox = opponent.HexX;
	uint16 oy = opponent.HexY;

	int dist = GetDistantion(cx, cy, ox, oy);
	if(dist>60) return false;

	const Item* utility = GetEquippedUtility(cr);
	int utilityReveal = utility && !FLAG(utility->Data.BrokenFlags, BI_BROKEN) && utility->Data.ScriptValues[1] == 1 ? utility->Proto->MagicPower : 0;
	if(dist<=utilityReveal || dist<=cr.ItemSlotExt->Proto->MagicPower || dist<=cr.ItemSlotMain->Proto->MagicPower) return true;

	// smoke cloud rules (Smoke.cpp, synced from smoke_hexes.fos). Placed before
	// the LookMinimum return so smoke works inside the always-visible range too;
	// dist<=1 stays a hard visibility floor. Teammates ignore smoke entirely.
	if(HasAnySmoke() && MapHasSmoke(map.Data.MapId))
	{
		bool teammates = (cr.Params[ST_TEAM_ID] == opponent.Params[ST_TEAM_ID] && cr.Params[ST_TEAM_ID] >= 200);

		// active IR goggles in the utility slot: the observer sees through
		// smoke as if it wasn't there (works from inside the cloud too)
		const Item* irUtility = GetEquippedUtility(cr);
		bool irActive = irUtility != NULL && irUtility->Proto->ProtoId == PID_IR_GOGGLES
			&& !FLAG(irUtility->Data.BrokenFlags, BI_BROKEN) && irUtility->Data.ScriptValues[1] == 1;

		if(!teammates && !irActive && dist > 1)
		{
			SmokeCtx smoke;
			ComputeSmokeCtx(map, cx, cy, ox, oy, dist, smoke);
			// observer inside the cloud sees nothing beyond 1 hex, unless a shot
			// corridor runs through his position
			if(smoke.crIn && !smoke.corridorAtObserver) return false;
			// observer at the cloud edge sees only 1 hex deep into the smoke
			if(smoke.crAdjacent && smoke.oppIn && smoke.effCount > 1) return false;
			// standing still >=1s inside smoke -> invisible (corridor over the
			// target's hex reveals them)
			if(smoke.oppIn && !smoke.corridorAtOpp && MsSinceMove(opponent) >= SMOKE_STILL_MS) return false;
			// smoke curtain: a target (not in smoke) up to SMOKE_HIDE_BEHIND_DIST
			// hexes past the cloud is hidden; farther targets are unaffected
			if(!smoke.oppIn && smoke.effCount > 0 && smoke.distBeyond <= SMOKE_HIDE_BEHIND_DIST) return false;
		}
	}

	// min range - always visible
	if(dist <= (int)(FOnline->LookMinimum)) return true;

	// dead/unconcious/neg hp - only minimum range
	if(cr.Cond != COND_LIFE) return (dist <= (int)(FOnline->LookMinimum));

	bool isWeaponScoped = cr.ItemSlotMain->Proto->WeaponHasPerk( WEAPON_PERK_SCOPE_RANGE );

    int front_range=(cr.Params[DAMAGE_EYE]!=0)?1:(CLAMP((cr.Params[ST_PERCEPTION]+cr.Params[ST_PERCEPTION_EXT]+GetUtilityParamBonus(cr, ST_PERCEPTION)),1,30));
	if(cr.Params[PE_SHARPSHOOTER]) front_range+=2*cr.Params[PE_SHARPSHOOTER];
    front_range*=3;
    front_range+= cr.Params[ST_BONUS_LOOK];
    if (isWeaponScoped)
    	front_range+=5;
	front_range+=(int)(FOnline->LookNormal);

	if(dist > front_range) return false;

	// max perception range
	int max_range = front_range;

	// transform direction from critter A to critter B into "character coord-space"
	uint8 dir = (uint8)GetDirection(cx, cy, ox, oy);

	dir = cr.Dir>dir?cr.Dir-dir:dir-cr.Dir;

    // adjust distance based on fov (NOT only for sneakers)
    switch(dir)
    {
        case 0:
            max_range -= (max_range* (int)(FOnline->LookDir[0]))/100; // front
            break;
        case 1:
        case 5:
            max_range -= (max_range* (int)(FOnline->LookDir[1]))/100; // frontsides
            break;
        case 2:
        case 4:
            max_range -= (max_range* (int)(FOnline->LookDir[2]))/100; // backsides
            if (isWeaponScoped)
            	max_range -= 10;
            break;
        default:
            max_range -= (max_range* (int)(FOnline->LookDir[3]))/100; // back
    		if (isWeaponScoped)
    			max_range -= 10;
    }

	if(dist > max_range) return false;

	if(_CritHasMode(opponent, MODE_HIDE))
	{
		if(cr.Params[ST_TEAM_ID] == opponent.Params[ST_TEAM_ID] && cr.Params[ST_TEAM_ID] >= 200)
		{
			if (!cr.CritterIsNpc)
			{
				max_range = TraceWall(cx, cy, ox, oy, map, max_range); // in case wall is blocking
				return dist <= max_range; // behind a wall
			}
		}

		// wall gives success
		if (!_CritHasExtMode(cr,MODE_EXT_NO_WALL_CHECK))
		{
			max_range = TraceWall(cx, cy, ox, oy, map, max_range); // in case wall is blocking
			if(dist > max_range) return false;
		}

		int sk = opponent.Params[SK_SNEAK] + opponent.Params[ST_EXT_SNEAK] + GetUtilityParamBonus(opponent, SK_SNEAK);
		uint deathclawHide = ((uint)opponent.Params[ST_CLASS_OPTIONS] >> CLASS_OPTION_2_SHIFT) & CLASS_OPTION_VALUE_MASK;
		if(opponent.Params[ST_CLASS_ID] == CLASS_DEATHCLAW && deathclawHide == CLASS_DEATHCLAW_HIDE_CHAMELEON)
		{
			int agility = CLAMP(opponent.Params[ST_AGILITY] + opponent.Params[ST_AGILITY_EXT] +
			                    GetUtilityParamBonus(opponent, ST_AGILITY), 1, 30);
			sk += min(75, agility * 5);
		}

		// bonuses before clamp

		// 1. next to a wall
		WallDist* wd=GetProtoDists(map);
		if(wd->distances[oy*wd->proto->Header.MaxHexX+ox] <=
			(opponent.Params[PE_GHOST] ? 5 : 1)) sk+=BONUS_WALL;

		// 2. occluder bonus
		if (!_CritHasExtMode(cr,MODE_EXT_NO_WALL_CHECK))
			sk += CheckOccluder(ox, oy, cx, cy, map);

		// 3. stealth boy
		if(opponent.Params[ST_SNEAK_FLAGS]&4) sk+=BONUS_STEALTH_BOY;

		// 4. night TODO?

		// clamp
		sk=CLAMP(sk,-300,600);

		switch(dir)
        {
            case 0:
                sk -= (int)(FOnline->LookSneakDir[0]); // front
                break;
            case 1:
            case 5:
                sk -= (int)(FOnline->LookSneakDir[1]); // frontsides
                break;
            case 2:
            case 4:
                sk -= (int)(FOnline->LookSneakDir[2]); // backsides
                break;
            default: ;
                sk -= (int)(FOnline->LookSneakDir[3]); // back
        }

		// armor penalty is data-driven through armor proto perks
		if(opponent.ItemSlotArmor != nullptr && opponent.ItemSlotArmor->Proto != nullptr
			&& opponent.ItemSlotArmor->Proto->ArmorHasPerk(ARMOR_PERK_RATTLING))
		{
			sk = -10000;
		}

		// weapons penalty
		const ProtoItem* proto=opponent.ItemSlotMain->Proto;
		if(proto->Type==ITEM_TYPE_WEAPON && (FLAG(proto->Flags,ITEM_TWO_HANDS) || proto->Weapon_Anim1==ANIM1_SMG))
		{
			switch(proto->Weapon_Anim1)
			{
			case ANIM1_HEAVY_RIFLE:
			case ANIM1_MINIGUN:
			case ANIM1_ROCKET_LAUNCHER:
			case ANIM1_FLAMER:
				sk+=BONUS_WEAPON_HEAVY;
				break;
			default:
				if(proto->Weapon_Skill[0]<=SK_ENERGY_WEAPONS || proto->Weapon_Skill[0]==SK_RIFLES) sk+=BONUS_WEAPON_RIFLE;
				break;
			}
		}

		const ProtoItem* proto2=opponent.ItemSlotExt->Proto;
		if(proto2->Type==ITEM_TYPE_WEAPON && (FLAG(proto2->Flags,ITEM_TWO_HANDS) || proto2->Weapon_Anim1==ANIM1_SMG))
		{
			switch(proto2->Weapon_Anim1)
			{
			case ANIM1_HEAVY_RIFLE:
			case ANIM1_MINIGUN:
			case ANIM1_ROCKET_LAUNCHER:
			case ANIM1_FLAMER:
				sk+=BONUS_WEAPON_HEAVY;
				break;
			default:
				if(proto->Weapon_Skill[0]<=SK_ENERGY_WEAPONS || proto->Weapon_Skill[0]==SK_RIFLES) sk+=BONUS_WEAPON_RIFLE;
				break;
			}
		}

		// running
		if(IsMoving(opponent) && opponent.IsRuning && !opponent.Params[PE_SILENT_RUNNING]) sk+=BONUS_RUNNING;

		// active explosive held
		if(opponent.Params[ST_SNEAK_FLAGS]&1) sk+=BONUS_ACTIVE_EXPLOSIVES;

		if(sk <= 0)	return true;

        sk/=(int)(FOnline->SneakDivider);
        return front_range >= dist+sk;
	}
	else // opponent doesn't sneak
	{
	  if (!cr.CritterIsNpc)
	  {
			max_range = TraceWall(cx, cy, ox, oy, map, max_range); // in case wall is blocking
			return dist <= max_range; // behind a wall
	  }
	  return true; // max pe range handled before
	}
}

int GetEngineLook(Critter& cr)
{
	int look=(cr.Params[DAMAGE_EYE]!=0)?1:(CLAMP((cr.Params[ST_PERCEPTION]+cr.Params[ST_PERCEPTION_EXT]+GetUtilityParamBonus(cr, ST_PERCEPTION)),1,30));
    look*=3;
    look+= cr.Params[ST_BONUS_LOOK];
	look+=(int)(FOnline->LookNormal);
    if( look < (int) FOnline->LookMinimum )
        look = FOnline->LookMinimum;
    return look;
}

EXPORT bool check_trap_look(Map& map, Critter& cr, Item& trap)
{
	int dist=GetDistantion(cr.HexX,cr.HexY,trap.AccHex.HexX,trap.AccHex.HexY);
	int perception=CLAMP(cr.Params[ST_PERCEPTION]+cr.Params[ST_PERCEPTION_EXT]+GetUtilityParamBonus(cr, ST_PERCEPTION),1,30);
	int skilldiff=cr.Params[SK_TRAPS] + GetUtilityParamBonus(cr, SK_TRAPS) - trap.TrapGetValue();
	return dist<=perception/2 + skilldiff/50;
}

EXPORT uint Map_WallDistance(Map& map, uint16 hx, uint16 hy) // test
{
	WallDist* wd=GetProtoDists(map);
	return wd->distances[hy*wd->proto->Header.MaxHexX+hx];
}
