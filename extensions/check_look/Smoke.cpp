// Smoke cloud registry + LOS helpers for check_look().
// Scripts (smoke_hexes.fos) own the smoke items and mirror every add/remove here;
// this file only answers "how much smoke sits on this line" style questions.
// Registry access is protected because visibility checks run on multiple logic
// workers while smoke scripts may add/remove entries.

#include <windows.h>
#include <map>
#include <set>
#include <vector>
#include "Move.h"
#include "Smoke.h"

using namespace std;

extern uint8* Lookup;
extern volatile long Init;
extern void InitLook();
extern int TickDiff;

struct SmokeCorridorEntry
{
	set<uint> hexes;      // line hexes + their 6 neighbors
	uint      expiryTick; // timeGetTime() based
};

typedef map<uint, int> SmokeHexMap; // key = (hy<<16)|hx, value = overlap count

struct SmokeMapState
{
	SmokeHexMap                hexes;
	vector<SmokeCorridorEntry> corridors;
};

// typedefs also dodge the std::map vs "Map& map" parameter name clash below
typedef map<uint, SmokeMapState> SmokeMapReg; // key = map id
static SmokeMapReg SmokeMaps;
static volatile long AnySmoke = 0;

#define HEX_KEY( hx, hy ) ( ( uint( hy ) << 16 ) | uint( hx ) )

static bool HexHasSmoke( SmokeMapState& state, uint16 hx, uint16 hy )
{
	return state.hexes.find( HEX_KEY( hx, hy ) ) != state.hexes.end();
}

static bool HexInLiveCorridor( SmokeMapState& state, uint16 hx, uint16 hy, uint tick )
{
	uint key = HEX_KEY( hx, hy );
	for( size_t i = 0; i < state.corridors.size(); i++ )
	{
		if( state.corridors[ i ].expiryTick > tick && state.corridors[ i ].hexes.count( key ) )
			return true;
	}
	return false;
}

static void PruneCorridors( SmokeMapState& state, uint tick )
{
	for( size_t i = 0; i < state.corridors.size(); )
	{
		if( state.corridors[ i ].expiryTick <= tick )
			state.corridors.erase( state.corridors.begin() + i );
		else
			i++;
	}
}

// Smoke on the observer's hex or any of its 6 neighbors
static bool HexTouchesSmoke( SmokeMapState& state, uint16 hx, uint16 hy )
{
	if( HexHasSmoke( state, hx, hy ) )
		return true;
	for( char dir = 0; dir < 6; dir++ )
	{
		uint16 nx = hx, ny = hy;
		Move( nx, ny, dir );
		if( HexHasSmoke( state, nx, ny ) )
			return true;
	}
	return false;
}

bool HasAnySmoke()
{
	return InterlockedCompareExchange(&AnySmoke, 0, 0) != 0;
}

bool MapHasSmoke( uint mapId )
{
	LookLockGuard guard(LookSmokeLocker);
	SmokeMapReg::iterator it = SmokeMaps.find( mapId );
	return it != SmokeMaps.end() && !it->second.hexes.empty();
}

uint MsSinceMove( Critter& cr )
{
	int ms = (int)timeGetTime() - TickDiff - (int)cr.PrevHexTick;
	return ms > 0 ? (uint)ms : 0;
}

// Walks the LOS line like TraceWall (same Lookup index math) and fills ctx
void ComputeSmokeCtx( Map& map, uint16 cx, uint16 cy, uint16 ox, uint16 oy, int dist, SmokeCtx& ctx )
{
	LookLockGuard guard(LookSmokeLocker);
	ctx.effCount = 0;
	ctx.distBeyond = 0x7FFFFFFF;
	ctx.crIn = ctx.oppIn = ctx.crAdjacent = ctx.corridorAtObserver = ctx.corridorAtOpp = false;

	SmokeMapReg::iterator it = SmokeMaps.find( map.Data.MapId );
	if( it == SmokeMaps.end() || it->second.hexes.empty() )
		return;
	SmokeMapState& state = it->second;

	uint tick = timeGetTime();
	ctx.crIn = HexHasSmoke( state, cx, cy );
	ctx.oppIn = HexHasSmoke( state, ox, oy );
	ctx.crAdjacent = ctx.crIn || HexTouchesSmoke( state, cx, cy );
	ctx.corridorAtObserver = HexInLiveCorridor( state, cx, cy, tick );
	ctx.corridorAtOpp = HexInLiveCorridor( state, ox, oy, tick );

	int dx = (int)ox - (int)cx;
	int dy = (int)oy - (int)cy;
	if( ( ABS( dx ) >= MAX_TRACE ) || ( ABS( dy ) >= MAX_TRACE ) )
		return;
	if( !Init )
		InitLook();

	int idx = ( SIZE_LIN * ( dy + MAX_TRACE ) + dx + MAX_TRACE ) * MAX_ARR;
	uint16 maxX = map.Proto->Header.MaxHexX;
	uint16 maxY = map.Proto->Header.MaxHexY;
	uint16 hx = cx, hy = cy;
	int lastSmokeStep = 0;
	for( int i = 0; i < dist; i++ )
	{
		Move( hx, hy, Lookup[ idx++ ] );
		if( ( hx >= maxX ) || ( hy >= maxY ) )
			return;
		if( HexHasSmoke( state, hx, hy ) && !HexInLiveCorridor( state, hx, hy, tick ) )
		{
			ctx.effCount++;
			lastSmokeStep = i + 1;
		}
	}
	if( ctx.effCount > 0 )
		ctx.distBeyond = dist - lastSmokeStep;
}

EXPORT void Map_SmokeAdd( Map& map, uint16 hx, uint16 hy )
{
	LookLockGuard guard(LookSmokeLocker);
	SmokeMaps[ map.Data.MapId ].hexes[ HEX_KEY( hx, hy ) ]++;
	InterlockedExchange(&AnySmoke, 1);
}

EXPORT void Map_SmokeRemove( Map& map, uint16 hx, uint16 hy )
{
	LookLockGuard guard(LookSmokeLocker);
	SmokeMapReg::iterator it = SmokeMaps.find( map.Data.MapId );
	if( it == SmokeMaps.end() )
		return;

	SmokeHexMap::iterator hexIt = it->second.hexes.find( HEX_KEY( hx, hy ) );
	if( hexIt != it->second.hexes.end() && --hexIt->second <= 0 )
		it->second.hexes.erase( hexIt );

	if( it->second.hexes.empty() )
	{
		SmokeMaps.erase( it );
		InterlockedExchange(&AnySmoke, SmokeMaps.empty() ? 0 : 1);
	}
}

// Opens a vision corridor along the shot line (line hexes + neighbors).
// Returns true only when the line actually crosses smoke - callers skip
// RefreshVisible orchestration otherwise.
EXPORT bool Map_SmokeCorridor( Map& map, uint16 hx, uint16 hy, uint16 tx, uint16 ty, uint durationMs )
{
	LookLockGuard guard(LookSmokeLocker);
	SmokeMapReg::iterator it = SmokeMaps.find( map.Data.MapId );
	if( it == SmokeMaps.end() || it->second.hexes.empty() )
		return false;
	SmokeMapState& state = it->second;

	uint tick = timeGetTime();
	PruneCorridors( state, tick );

	int dx = (int)tx - (int)hx;
	int dy = (int)ty - (int)hy;
	if( ( ABS( dx ) >= MAX_TRACE ) || ( ABS( dy ) >= MAX_TRACE ) )
		return false;
	if( !Init )
		InitLook();

	SmokeCorridorEntry corridor;
	corridor.expiryTick = tick + durationMs;
	bool crossesSmoke = false;

	int dist = GetDistantion( hx, hy, tx, ty );
	int idx = ( SIZE_LIN * ( dy + MAX_TRACE ) + dx + MAX_TRACE ) * MAX_ARR;
	uint16 maxX = map.Proto->Header.MaxHexX;
	uint16 maxY = map.Proto->Header.MaxHexY;
	uint16 curX = hx, curY = hy;

	// shooter's own hex counts too - someone inside the cloud shooting out
	// opens the lane at their position
	for( int i = 0; i <= dist; i++ )
	{
		if( i > 0 )
		{
			Move( curX, curY, Lookup[ idx++ ] );
			if( ( curX >= maxX ) || ( curY >= maxY ) )
				break;
		}
		if( HexHasSmoke( state, curX, curY ) )
			crossesSmoke = true;
		corridor.hexes.insert( HEX_KEY( curX, curY ) );
		for( char dir = 0; dir < 6; dir++ )
		{
			uint16 nx = curX, ny = curY;
			Move( nx, ny, dir );
			corridor.hexes.insert( HEX_KEY( nx, ny ) );
		}
	}

	if( !crossesSmoke )
		return false;

	state.corridors.push_back( corridor );
	return true;
}

// Cover query for combat.fos ApplyCoverLoss. Corridors deliberately ignored:
// a short vision lane does not remove the physical screening.
EXPORT bool Map_SmokeOnLine( Map& map, uint16 hx, uint16 hy, uint16 tx, uint16 ty )
{
	LookLockGuard guard(LookSmokeLocker);
	SmokeMapReg::iterator it = SmokeMaps.find( map.Data.MapId );
	if( it == SmokeMaps.end() || it->second.hexes.empty() )
		return false;
	SmokeMapState& state = it->second;

	int dx = (int)tx - (int)hx;
	int dy = (int)ty - (int)hy;
	if( ( ABS( dx ) >= MAX_TRACE ) || ( ABS( dy ) >= MAX_TRACE ) )
		return false;
	if( !Init )
		InitLook();

	if( HexHasSmoke( state, hx, hy ) )
		return true;

	int dist = GetDistantion( hx, hy, tx, ty );
	int idx = ( SIZE_LIN * ( dy + MAX_TRACE ) + dx + MAX_TRACE ) * MAX_ARR;
	uint16 maxX = map.Proto->Header.MaxHexX;
	uint16 maxY = map.Proto->Header.MaxHexY;
	uint16 curX = hx, curY = hy;
	for( int i = 0; i < dist; i++ )
	{
		Move( curX, curY, Lookup[ idx++ ] );
		if( ( curX >= maxX ) || ( curY >= maxY ) )
			return false;
		if( HexHasSmoke( state, curX, curY ) )
			return true;
	}
	return false;
}
