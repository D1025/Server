FOnline: Aftertimes Mapper

***

Changes in FOnline: Aftertimes Mapper

 - new interface (Set ScreenWidth = 1090 or higher in client's FOnline.cfg) with re-arranged tabs and submenus
 - zoom % information
 - tile grid (press G to turn it on/off)
 - autowall [wip] (press A and drag mouse to check it out)

***

To run a mapper, specify the server and client path in the Mapper.cfg file.

To load a saved map, type "~<map name>" in the console.
TO save a map, type "^<map name> [/text] [/nopack]". "/text" to save it in text format, "/nopack" to save it in unpacked version.

Other commands, preceded with "*":
 new		create a new map;
 unload		unload the current map;
 scripts	list of scripts;
 size <w> <h>	set a new map size;
 dupl		search for items with the same pids, placed on the same hexes;
 scroll		search for scroll blockes, around which there are less than two other scroll blockers;
 pidpos <pid>	search for all items with a given pid;
 hex <hx> <hy>	search for all objects on a given hex.

To run a script function, use "#". Executed function must be of prototype "string FuncName(string)", string argument is passed from a console, and function results will be displayed in the message box. Default module is "main", to execute a function from another module use "@" (for example, "#module@MyFunc").
Example:
In console, "#MyFunc Hello wo";
In script, "string MyFunc(string my) { return my + "rld!"; }";
Result in the message box: "Result: Hello world!".

Scripts are placed in the "/data" directory. List of loaded scripts is in scripts.cfg. The main script is mapper_main.fos, which contains all reserved functions, and basic list of API functions.

The mapper does not support FO and FO2 map formats, as well as older FOnline map formats.

On the main panel, there is a range of buttons that group the objects by their type.
Below these buttons, there's a panel with buttons toggling the display of items in the map view.
On the right, there are buttons that determine which objects will be selected by clicking and dragging the mouse.
Other buttons:
Fast - displays list of often used items;
Ign - Ignore, shows ignored objects that are not rendered on the map;
Inv - Inventory, shows objects inside a container or inside critters' inventory;
Lst - List, shows all loaded maps and allows to switch between them.

Upon selecting an object, an option window will appear allowing to edit some of it's properties (marked green; white are not editable).
To add an object to Ignored objects list, click it with Ctrl on the object selection panel.
To add an object to the container or inventory, click it with Alt on the object selection panel.
To put an object in a critter's slot, click the item in it's inventory with Shift.

To apply a change in the object properties window to a range of selected objects (of the same type), click To All button.

To remove selected object(s), press Del.

To change direction of a critter, use middle mouse button.

To add a range of objects to already selected objects, hold Ctrl.

To change zoom, use mouse wheel.

To speed up scrolling of objects in the panel, use the following:
Shift - one page,
Ctrl - 100 elements,
Alt - 1000 elements.

To play animation of a selected critter, use "@" command with codes of desired animations. For instance, to play animations of movement, and then using, type "@abal" (case is ignored, as well as whitespaces). If no critter is selected, all critters on the map will play the animation. For information on animation codes, see http://modguide.nma-fallout.com/#Graphics011

To move a critter, hold down Shift and click the desired position.

Hotkeys:
F1: Enable/Disable display of items.
F2: Enable/Disable display of sceneries.
F3: Enable/Disable display of walls.
F4: Enable/Disable display of critter.
F5: Enable/Disable display of tiles.
F6: Enable/Disable display of frequently used objects.
F7: Hide/Show the main panel.
Shift + F7: Fix the position of the main panel (enabled by default).
F8: Enable/Disable scrolling with mouse.
F9: Hide/Show the object properties window.
Shift + F9: Fix the position of the object properties window (disabled by default).
Shift + F10: Set rain.
F11: Full screen/window toggle.
Shift + Escape: Exit the mapper.
Del: Delete the selected objects.
Ctrl + X: Cut objects.
Ctrl + C: Copy objects.
Ctrl + V: Paste objects.
Ctrl + A: Select all.
Ctrl + S: Enable/Disable ignoring of scroll blockers.
Ctrl + B: Show impassable hexes. Red are impassable, and not shootable through. Green are impassable, shootable through.
Ctrl + M: Display NPC information over their heads (this has few modes of display).
Ctrl + L: Save the log to a txt file.
Tab: Change the type of objects selection (diamond or rectangle).
+: Change time by 1 hour.
-: Change time by -1 hour.

Special Entires:
0 - default;
240 - starting position for a player who logged for a first time;
241 - starting position for players on a map with NoLogOff flag.
242 - replication.
243 - car entry point.
245 - vertibird entry point.
246 - boat entry point.

Additional functions in this Mapper:

#cmc - display list of avaiable maps
#ClearTiles - delete stack of tiles from the same coords on current map
#AddParam - adds parameter to marked critter
#ChangeParams - changes parameters for marked critter
#FixWorldEntires - to descr
#FixMapEntires - to descr
#PortMap - adds faction parameters to all critters on map
#goto [x] [y] - moves to and mark hex from coords
#getsize - returns size of current map
#FixLockers - fixes maps with broken lockers and doors, after conversion from Fallout and Fallout 2 map formats.
#CleanTech - removes white scrollblockers and entire hexes from current map.
	Usage: #CleanTech [argument] Arguments: afterconv (removes ENT and white S hexes from map), 3853 (removes ENT hexes only), 4012 (removes white S hexes only)
	2049 (removes EG hexes only), greengrids (removes green exit grids), browngrids (removes brown exit grids).
#ReplaceHex - replaces marked tech hexes ONLY. Use another tech hex's PID as argument for the replacement. Usage: #ReplaceHex [replacebyTechPID]
#SignSpawners - signs spawner scripts to not-scripted lockers on map or reworks current spawners setting up the new Val4 to it.
	Usage: #SignSpawners [argument] Arguments: unguardedtown, guardedtown (runtime town spawners), privatedungeon (onetime spawners), publicdungeon (runtime spawners), encounter (onetime spawners for encounters).
#FixLockersOnAllMaps - as above, but it will load, rework and save all maps in .\data\maps subdirectory.
#MakeMobsDynamics - it will look for critters signed to mob or map_dungeon script module and sign them to mob_dynamic module, init_static_mob function with ST_NPC_ROLE = 203.
#MakeGuards - signs script for guards to critters on map. If there're selected critters then only to them, in other case this function will look for unscripted critters on map.
				Additionally it will set random bag from range 11-19 to critters.
	Usage: #MakeGuards [prefix argument]
	When prefix argument is empty then common_guard module will be signed to critters.
	With defined prefix (f.ex. #MakeGuards hub) critters will be signed to hub_guard module.
#MakeNPCUnlootable - signs npc_unlootable module to critters. If there're selected critters then only to them, in other case this function will look for unscripted critters on map.
#SetBagsToGuards - will look for guards scripted with following modules: common_guard, vaul_guard, navarro_guard, junktown_guard, hub_guard, frisco_guard, la_ady_guard...
					and set bags to them.
	Usage: #SetBagsToGuards [kind]
	Kinds: sgmix, sglow, sgmed, sghi, ew
#Switch - reversing sequence if setted objects at selected hex.

Hotkeys:
Left Alt + C with marked critter: Allow to choose ST_OVERRIDE_CRT value.
C - show descriptions for containers.
D - show descriptions for doors.
F - show descriptions for forcefields.