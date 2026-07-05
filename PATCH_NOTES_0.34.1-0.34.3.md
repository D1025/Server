# Patch Notes 0.0.34

## New Worldmap Event: Wasteland Threat
- A new recurring worldmap boss event can now appear in the wasteland.
- The event spawns as a visible **Wasteland Threat** location and announces its zone in global chat.
- Boss events can appear in desert, mountain, city, and coastal arenas.
- Each event contains one major boss and a supporting group of minions.
- Defeating the boss leaves a **New Era Holotape** as the main reward.
- Event locations clean themselves up after the fight, with a short grace period for looting.

### Boss Variants
- **The Abomination**: a huge deathclaw-style melee boss with long reach, self-healing, and deathclaw minions.
- **Warlord Kaine**: a ranged Enclave gunner using minigun pressure, suppressive fire, grenade barrages, and Enclave support troops.
- **The Harbinger**: a glowing horror with energy novas, radiation pressure, poison casts, self-healing, and ghoul swarms.

## NPC Combat AI
- NPCs are more tactical in combat.
- Humanoid NPCs can now heal wounded allies during fights if they are nearby, capable, and not already busy.
- NPCs try to move out of the firing line of friendly deployed heavy weapons.
- Ranged NPCs now prefer more sensible engagement distances instead of constantly hugging targets at point blank.
- Hurt ranged NPCs are more likely to fall back toward safer range.
- Many NPC prototypes received combat tuning, including higher weapon skills and relevant perks such as ranged damage, toughness, Man of Steel, Sharpshooter, and Heave Ho.

## Combat Mechanics
- Added **Guarded Stance** weapon perk.
- Guarded Stance gives **+10 Armor Class** while a weapon with this perk is held in the active hand.
- Added wall-slam damage for knockback effects: being stopped early by a wall or obstacle can now deal extra impact damage.
- If a knocked-back target slams into another living character, the impact damage is split between both characters.
- Swipe attacks now cover a proper forward arc instead of a narrow straight line, making long-reach swipe weapons more reliable.
- Bludgeoning damage has been adjusted into a hybrid model: normal weapon damage is reduced normally, while the melee-damage component is added after DT, DR, and AC.
- Unarmed-style weapons such as Brass Knuckles, Spiked Knuckles, Power Fist, and Mega Power Fist are now handled more consistently as unarmed-style attacks.

## Weapon Updates
- Guarded Stance was added to: **Club**, **Sledgehammer**, **Spear**, **Crowbar**, **Super Sledge**, **Sharpened Spear**, **Sharpened Pole**, and **Louisville Slugger**.
- Several melee and unarmed weapons received damage buffs:
  - **Knife**: 20 -> 24
  - **Sledgehammer**: 0 -> 40 on melee modes
  - **Crowbar**: 28 -> 36
  - **Brass Knuckles**: 20 -> 24
  - **Super Sledge**: 0 -> 70 on melee modes
  - **Spiked Knuckles**: 30 -> 36
  - **Power Fist**: 48 -> 60
  - **Combat Knife**: 30 -> 38
  - **Sharpened Pole**: 6 -> 30 on thrust
  - **Shiv**: 8 -> 18
  - **Wrench**: 8 -> 24
- **Super Sledge** also gained Knockback in addition to Bludgeoning and Guarded Stance.

## Power Armor
- Improved Power Armor item handling.
- Power Armor can no longer be moved into invalid inventory slots or swapped out in unintended ways.
- Dropping worn Power Armor to the ground still exits the suit.
- The hidden Power Armor stash slot now behaves more consistently on both client and server.
- These changes should reduce invalid item move errors related to Power Armor.