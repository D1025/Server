# Patch 0.2.1

Patch 0.2.1 focuses on fixes for recently introduced systems, consistent damage calculations, and balance adjustments.

## Combat and Damage

- Unified explosive damage between direct hits and hex-targeted shots.
- Direct hits with explosive weapons can no longer deal critical damage.
- `Demolition Expert` now correctly increases explosive damage by 25%.
- Unified explosion radius calculations for rockets, 40mm grenades, the `Blast` upgrade, and `Demolition Expert`.
- Hex-targeted explosive shots now correctly account for the weapon mode, ammunition, upgrades, and attacker bonuses.
- `Wall Slam` damage is now included in the standard dealt and received damage message.
- When a knocked-back target collides with another character, the second victim receives a separate damage message.
- `Overload` now converts consumed `Persistent Damage` into flat electrical damage applied after DT, DR, and critical-hit modifiers.

## Limbs and Healing

- Each limb now has the same base maximum HP as the character's main HP pool instead of half that value.
- `Iron Limbs` increases maximum limb HP by 50%.
- Standardized maximum limb HP calculations for players, NPCs, mercenaries, militia, and arena characters.
- Natural `Healing Rate` recovery now works when the main HP pool is full but at least one uncrippled limb is damaged.
- `Vampirism` now heals the head in addition to the arms and legs.
- Fixed Super Mutant healing limits so Strength is correctly included in maximum HP calculations.

## Perks and Weapons

- `In Your Face!` now reduces incoming damage from nearby enemies by 40%.
- Knockback from `And Stay Back` now works only within half of the shotgun's maximum range.
- Reduced the `Combat Shotgun` range from 15 to 12 hexes.
- Reduced the `HK CAWS` range from 15 to 12 hexes.

## Chems and Utility Items

- `Psycho` reduces AP regeneration by 10% while active.
- `Buffout` reduces AP regeneration by 10% while active.
- `Nuka-Cola` grants +10% AP regeneration and +1 Agility at the cost of 2 maximum AP.
- The AP regeneration effect of the `Adrenaline Regulator` has been disabled. The item remains in the game and will receive a new effect in the future.
- `Spectacles` are now a utility item that grants +5 FoV.

## Classes and Controls

- Deathclaws and Super Mutants can no longer pick up or equip Power Armor. Attempting to do so displays a message and leaves the armor on the ground.
- A class ability waiting for a target is now cancelled when the cursor changes to an attack or another action.
- Fixed an issue that allowed a regular attack and a previously activated class ability to execute at the same time.

## Interface

- Replaced the `Damage Resistance` entry on the character sheet and character creation screen with `Move Speed (%)`.
- `Move Speed` uses the same calculation as the `~ms` command: `0%` is the base speed, positive values indicate faster movement, and negative values indicate slower movement.
- Updated descriptions for the affected perks, chems, and items.
