# SDL3 baseline before the raylib migration

Date: 2026-11-23  
Reference commit: `79551d506869b05c49ee1e44e40cc5e4d7421f9b`  
Target: Linux x86-64, SDL3 desktop build

## Automated verification

| Check | Result |
|---|---|
| CMake build | Passed |
| Executable generated | `build/game` |
| Executable format | Linux x86-64 ELF |
| Headless SDL startup | Remained running for 3 seconds with the dummy video driver |
| Diff formatting | Passed |

The headless startup confirms initialization and entry into the main loop. It does not validate graphics, mouse input, keyboard input, or a complete battle.

## Reference configuration

- Window: 1280 × 720.
- Hero: Archangel, 100 HP, 60 MP.
- Enemy: Leviathan, 100 HP, 70 MP.
- Hero skills: Holy Strike, Divine Spear, Healing Blessing and Fourth Skill.
- Initial inventory: 2 Potions, 2 Mana Potions and 1 Elixir.
- Bullet hell controls: WASD and arrow keys.
- Bullet hell patterns: 10.
- Visual effect duration: 700 ms.
- Battle log limit: 4 entries.

## Manual baseline checklist

Run:

```bash
./build/game
```

Record each item as `PASS`, `FAIL` or `NOT TESTED` before replacing SDL3.

| Flow | Status | Expected behavior |
|---|---|---|
| Game startup | NOT TESTED | Window opens at 1280 × 720 with hero and enemy panels |
| Skill menu | NOT TESTED | SKILL opens the skill list and BACK returns to actions |
| Skill with mana | NOT TESTED | Skill executes once, consumes the correct mana and advances the turn |
| Skill without mana | NOT TESTED | Disabled or rejected without consuming the turn |
| Healing skill | NOT TESTED | Restores HP without exceeding maximum HP |
| Item menu | NOT TESTED | ITEM opens the inventory and BACK returns to actions |
| Valid item | NOT TESTED | Applies its effect and reduces its quantity by one |
| Item with no useful effect | NOT TESTED | Cannot be consumed |
| Defend | NOT TESTED | Activates defense and advances to the enemy turn |
| Continue | NOT TESTED | Starts the enemy action or bullet hell phase |
| Bullet hell movement | NOT TESTED | WASD and arrows move the soul inside the arena |
| Bullet collision | NOT TESTED | Collision adds damage and activates temporary invincibility |
| Bullet pattern end | NOT TESTED | Damage resolves and the game returns to actions or finishes |
| Victory | NOT TESTED | Victory indicator and final controls are shown |
| Defeat | NOT TESTED | Defeat indicator and final controls are shown |
| Restart | NOT TESTED | Battle, UI and arena return to their initial state |
| Exit button | NOT TESTED | Main loop exits cleanly |
| Window close button | NOT TESTED | Main loop exits cleanly |

## Known observations before migration

- The font path is fixed to `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf`.
- Skill and item button objects are rebuilt during drawing.
- Visual effect expiration changes state inside the drawing method.
- Final `battleMessage` victory/defeat suffixes appear reversed in three code paths, while the turn indicator uses the correct condition.
- Text rendering creates and destroys a surface and texture for every draw call.
- The battle uses random bullet pattern selection, so visual comparisons require recording which pattern ran.

These observations belong to the SDL3 reference and should not be attributed to raylib if they remain after migration.
