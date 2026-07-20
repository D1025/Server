// Smoke cloud state for line-of-sight math (see Smoke.cpp).
// Registry is synced from scripts/smoke_hexes.fos via the exported Map_Smoke* functions.

// Everything check_look() needs to know about smoke between two critters
struct SmokeCtx
{
	int  effCount;           // smoke hexes on the LOS line outside live corridors (target hex included)
	int  distBeyond;         // hexes between the last smoke hex on the line and the target (0 if target in smoke, huge if no smoke)
	bool crIn;               // observer stands in smoke
	bool oppIn;              // target stands in smoke
	bool crAdjacent;         // observer in smoke or next to it
	bool corridorAtObserver; // live shot corridor covers observer's hex
	bool corridorAtOpp;      // live shot corridor covers target's hex
};

bool HasAnySmoke();
bool MapHasSmoke( uint mapId );
void ComputeSmokeCtx( Map& map, uint16 cx, uint16 cy, uint16 ox, uint16 oy, int dist, SmokeCtx& ctx );
uint MsSinceMove( Critter& cr );
