# Utility — PID-y i efekty

Ten plik opisuje wszystkie aktualne przedmioty utility. Źródłem listy jest
`Pids_Utilities` w `scripts/pids_groups.fos`; parametry pochodzą z prototypów
w `proto/items/` i z obsługujących je skryptów. Lista zawiera obecnie `19`
przedmiotów podzielonych na grupy `Pids_Utility_Electronic`, `Pids_Utility_Passive`,
`Pids_Utility_Bobbleheads` i `Pids_Utility_Books`;
Back Pack jest dopisany bezpośrednio do listy zbiorczej.

## Wspólne reguły

- Każdy utility używa `SLOT_UTILITY` (slot `4`) i ma `Stackable=0`: przedmioty
  nie łączą się w stacki w ekwipunku.
- Postać może mieć założony tylko jeden utility. Założenie następnego zastępuje
  poprzedni.
- Aktywowane efekty utility współdzielą `TO_UTILITY_COOLDOWN`, wyświetlany jako
  `Utility` na standardowej liście cooldownów. Podczas jego działania można zdjąć
  obecne utility, ale nie można założyć ani zamienić go na inne.
- Pasywne bonusy do SPECIAL i skilli działają tylko, gdy przedmiot jest założony
  i nie jest zepsuty.
- Utility nie jest pancerzem: nie daje AC, DT, DR ani perków pancerza.
- Inni gracze widzą nazwę założonego utility wyłącznie w linii Awareness po
  najechaniu na postać.
- Trait `Armor Training` (`TRAIT_ARMOR_TRAINING`, parametr `558`) całkowicie
  blokuje zakładanie przedmiotów do `SLOT_UTILITY`. Walidacja działa po stronie
  klienta i serwera. Jeżeli istniejąca postać ma utility założony w chwili
  logowania, przedmiot jest automatycznie przenoszony do inventory.

## Urządzenia aktywowane ręcznie

| PID | Przedmiot | Efekt |
|---:|---|---|
| 54 | Stealth Boy | Po użyciu w slocie utility włącza efekt Stealth Boy: w obliczeniach wykrywania przeciwnik dostaje `+30` do trudności wykrycia postaci. Pełne naładowanie to `1200` realnych sekund; podczas działania traci `1` ładunek na sekundę. Ponowne użycie wyłącza efekt, a zdjęcie/zastąpienie przedmiotu także go wyłącza. |
| 59 | Motion Sensor | Po użyciu w slocie utility działa przez maksymalnie `600` realnych sekund i traci `1` ładunek na sekundę. Aktywny sensor wykorzystuje `RevealSneakers=15`: serwer automatycznie uznaje cele w zasięgu `15` heksów za widoczne, z wyjątkiem specjalnych trybów pełnej niewidzialności. Gdy w konfiguracji klienta włączony jest wskaźnik sensora, rysuje granicę tego zasięgu. Science pokazuje pozostały czas, a Repair otwiera ekran ładowania. Small Energy Cell przywraca `12` sekund, Micro Fusion Cell `30` sekund, maksymalnie do `600`. Zdjęcie lub zastąpienie przedmiotu wyłącza urządzenie. |

## Plecak

| PID | Przedmiot | Efekt |
|---:|---|---|
| 90 | Back Pack | Założony plecak daje `+50 Carry Weight`. Nie jest kontenerem. |

## Bobbleheady SPECIAL

| PID | Przedmiot | Efekt podczas założenia |
|---:|---|---|
| 25585 | Bobblehead - Strength | `+1 Strength` |
| 25586 | Bobblehead - Perception | `+1 Perception` |
| 25587 | Bobblehead - Endurance | `+1 Endurance` |
| 25588 | Bobblehead - Charisma | `+1 Charisma` |
| 25589 | Bobblehead - Intelligence | `+1 Intelligence` |
| 25590 | Bobblehead - Agility | `+1 Agility` |
| 25591 | Bobblehead - Luck | `+1 Luck` |

Wszystkie siedem tymczasowo używa grafiki przedmiotu Small Statuette:
`art/items/bigbox2.frm` na mapie i `art/inven/beny.frm` w inventory.

## Pozostałe pasywne bonusy

| PID | Przedmiot | Efekt podczas założenia |
|---:|---|---|
| 570 | Goggles | `+15 Traps` |
| 309 | Talisman | Raz na minutę chroni przed ciosem, który obniżyłby HP poniżej `1`: pozostawia postać na `1 HP` i daje `3` sekundy nieśmiertelności. Uruchamia wspólny cooldown `Utility`. |
| 25592 | Riot Shield | Bez broni lub podczas używania jednoręcznej broni zawsze zapewnia jeden poziom coveru przeciw burstom. Nie sumuje się ponad globalny limit `COVER_MAX=1`. Grafika jest placeholderem. |

## Książki przerobione na utility

Książki poniżej nie są czytane ani konsumowane. Nie zapisują też liczby przeczytań;
działają wyłącznie jako założony utility.

| PID | Przedmiot | Efekt podczas założenia |
|---:|---|---|
| 73 | Big Book of Science | `+30 Science` |
| 76 | Deans Electronics | `+30 Repair` |
| 80 | First Aid Book | `+30 First Aid` |
| 86 | Scout Handbook | `+30 Outdoorsman` |
| 102 | Guns and Bullets | `+30 Pistols` |
| 22050 | Tales of Junktown Jerky Vendor | `+30 Barter` |

## Dystrybucja i prezentacja

- Generowane kontenery PvE z `scripts/spawner_pve.fos` mają `5%` szansy na
  dodanie jednego losowego przedmiotu z pełnej, 19-elementowej listy utility.
- Podróżujący NPC z `scripts/npc_travel.fos` wykonują losowanie
  `Random(0, 100) <= 15`, a więc mają około `15,8%` szansy na jeden z 12 utility:
  Stealth Boy, Motion Sensor, Back Pack, Talisman, Riot Shield albo jeden z siedmiu bobbleheadów SPECIAL.
  Wylosowany przedmiot jest od razu zakładany.
- Opis przedmiotu w inventory pokazuje pasywny bonus `Utility_Param` /
  `Utility_Value`. Dla Stealth Boya i Motion Sensora pokazuje także aktualny
  poziom naładowania oraz pojemność maksymalną.
- Stealth Boy i Motion Sensor nie zmieniają już PID-u na osobny „aktywny”
  wariant. Stan włączenia i licznik czasu są przechowywane na tym samym
  egzemplarzu przedmiotu; zdjęcie lub bezpośrednia zamiana utility czyści efekt.

## Implementacja

- Prototypy: `proto/items/utility.fopro`, `proto/items/misc.fopro`,
  `proto/items/container.fopro`.
- Crafting backpacka: wpis `188` w `text/engl/FOCRAFT.MSG`.
- Blokada stackowalnych przedmiotów w slocie: `scripts/critter_item_movement.fos`.
- Blokada slotu dla `Armor Training` i przenoszenie założonego utility przy
  logowaniu: `scripts/critter_item_movement.fos` oraz `scripts/main.fos`.
- Stealth Boy i Motion Sensor: `scripts/item_stealth.fos`.
- Talisman i Riot Shield: `scripts/combat.fos`.
- Pasywne bonusy: `extensions/parameters/parameters.cpp`.
- Wykrywanie Motion Sensora oraz uwzględnianie bonusów Perception, Sneak i Traps:
  `extensions/check_look/check_look.cpp`.
- Losowanie utility: `scripts/spawner_pve.fos` i `scripts/npc_travel.fos`.
- Opisy w inventory i linia Awareness: `scripts/client_interface.fos`.
