#ifndef CHECK_LOOK_DEFINES_H
#define CHECK_LOOK_DEFINES_H

#include <windows.h>
#include "../fonline2238.h"
#include "../../scripts/ITEMPID.H"

extern CRITICAL_SECTION LookInitLocker;
extern CRITICAL_SECTION LookDistsLocker;
extern CRITICAL_SECTION LookSmokeLocker;

class LookLockGuard
{
private:
	CRITICAL_SECTION* Locker;
	LookLockGuard(const LookLockGuard&);
	LookLockGuard& operator=(const LookLockGuard&);

public:
	explicit LookLockGuard(CRITICAL_SECTION& locker): Locker(&locker) { EnterCriticalSection(Locker); }
	~LookLockGuard() { LeaveCriticalSection(Locker); }
};

#define MAX_PROTO_MAPS		(30000)

#define PI_FLOAT       (3.14159265f)
#define PIBY2_FLOAT    (1.5707963f)
#define BIAS_FLOAT     (0.02f)
#define ABS(__val)     ((__val)>0?(__val):(-(__val)))
#define MIN(__x,__y)   (__x<__y?__x:__y)

#define MAX_ARR (200)
#define SIZE_LIN (201)
#define MAX_TRACE (100)


#define MODE_EXT_NO_WALL_CHECK    (0x00000002) // applies only when sneaking
#define MODE_EXT_LOOK_ADMIN       (0x00008000)
#define MODE_EXT_LOOK_INVISIBLE   (0x00010000)
#define MODE_EXT_GOD              (0x00020000)

#define COND_LIFE                   (1) // ������� ���

#define _CritHasMode(cr,mode) (cr.Params[mode]>0)
#define _CritHasExtMode(cr, mode) ((cr.Params[MODE_EXT]&(mode))!=0)
#define _CritHasPerk(cr,perk) (cr.Params[perk]>0)

#define MAP_DATA_ACTIVE_COUNTDOWN   (8)

#define BONUS_OCCLUDER				(60)
#define BONUS_WALL					(50)
#define BONUS_STEALTH_BOY			(30)

#define BONUS_ARMOR_METAL			(-72)
#define BONUS_ARMOR_COMBAT			(-36)

#define BONUS_WEAPON_RIFLE			(-50)
#define BONUS_WEAPON_HEAVY			(-150)

#define BONUS_ACTIVE_EXPLOSIVES		(-72)
#define BONUS_RUNNING				(-60)

// smoke cloud (see Smoke.cpp and scripts/smoke_hexes.fos)
#define SMOKE_HIDE_BEHIND_DIST		(10)   // targets up to this many hexes past the smoke are hidden; farther ones visible again
#define SMOKE_STILL_MS				(1000) // standing this long inside smoke -> invisible from outside

//#define VOLUME #(proto)		(proto.Volume == 0 ? 50 : proto.Volume)
#define OCCLUDER_DIST (2) // 2 only, no reason to be more overkill

#define MAX_WALLS_DIST	(5)

#endif // CHECK_LOOK_DEFINES_H
