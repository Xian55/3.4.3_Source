# game/Handlers/

`WorldSession::Handle<X>Opcode` impls. One `.cpp` per subsystem. **Wire handler code lives here.**

## Lookup pattern

Each function bound to a CMSG in `../Server/Protocol/Opcodes.cpp`. To find a handler:
1. Grep CMSG name in `Opcodes.cpp` → `&WorldSession::HandleFooOpcode` line.
2. Grep `WorldSession::HandleFooOpcode` here → impl.

## Files (by subsystem)

### Auth/Session
- `AuthHandler.cpp` — `CMSG_AUTH_SESSION`, `CMSG_AUTH_CONTINUED_SESSION`, ping, `CMSG_ENTER_ENCRYPTED_MODE_ACK`. **Entry handler from wire.**
- `BattlenetHandler.cpp` — bnet bridge over world (e.g. `CMSG_BATTLENET_REQUEST`).
- `MiscHandler.cpp` — catch-all: time sync, logout, repop, ready check, server info.

### Character lifecycle
- `CharacterHandler.cpp` — `CMSG_ENUM_CHARACTERS`, `CMSG_CHAR_CREATE`, `CMSG_PLAYER_LOGIN`, char delete/rename/customize. Builds player from DB on login.

### Movement
- `MovementHandler.cpp` — `CMSG_MOVE_*` (start, stop, jump, fall, swim, fly), `CMSG_MOVE_TELEPORT_ACK`, `CMSG_MOVE_FORCE_*_SPEED_CHANGE_ACK`. **High-volume.**

### Combat / Spells
- `CombatHandler.cpp` — attack swing, sheathed.
- `SpellHandler.cpp` — `CMSG_CAST_SPELL`, `CMSG_USE_ITEM`, cancel cast, cancel aura, learn talents.
- `PetHandler.cpp` — pet cast, pet command, abandon, rename.

### Items / Inventory
- `ItemHandler.cpp` — swap, split, sell, buy, repair, refund, transmog, currency.
- `BankHandler.cpp` — bank slot, buy bank slot.
- `LootHandler.cpp` — loot start, autoloot, master loot, roll, release.
- `TradeHandler.cpp` — trade init, set item, accept, cancel.
- `MailHandler.cpp` — send, return, take attachment, get list.

### Social
- `ChatHandler.cpp` — say/yell/whisper/party/raid/guild/officer/channel.
- `ChannelHandler.cpp` — join, leave, kick, password, list.
- `SocialHandler.cpp` — friends/ignore list ops.
- `GroupHandler.cpp` — invite, accept, kick, raid convert, ready check, loot method.
- `GuildHandler.cpp` — guild create/disband, motd, ranks, bank.
- `PetitionsHandler.cpp` — guild/arena charter.
- `InspectHandler.cpp` — `/inspect`.
- `DuelHandler.cpp` — duel propose, accept.
- `ArenaTeamHandler.cpp` — arena team ops.

### World content
- `NPCHandler.cpp` (+ `.h`) — gossip, trainer, vendor, banker, talent reset, taxi master, stable.
- `QuestHandler.cpp` — quest give, accept, complete, push to party, abandon.
- `QueryHandler.cpp` — creature/gameobject/item/quest/page text/realm/name queries.
- `TaxiHandler.cpp` — flight path activation.
- `BattleGroundHandler.cpp` — BG queue join/leave, score request, port accept.
- `LFGHandler.cpp` — dungeon finder.
- `CalendarHandler.cpp` — events, invites, RSVPs.
- `AuctionHouseHandler.cpp` — list items, place bid, cancel.
- `TokenHandler.cpp` — WoW token UI.

### Progression / Collections
- `SkillHandler.cpp` — skill cap, learn.
- `ScenarioHandler.cpp` — scenario events.
- `SceneHandler.cpp` — scenes (cutscene-like).
- `ToyHandler.cpp` — toy box.
- `CollectionsHandler.cpp` — mount/pet collection sync.
- `BattlePetHandler.cpp` — pet battle.

### Misc / System
- `VehicleHandler.cpp` — vehicle enter/exit, control.
- `WardenHandler.cpp` — anti-cheat round-trip (in `../Warden/`).
- `TicketHandler.cpp` — GM ticket.
- `HotfixHandler.cpp` — DB2 hotfix sync.
- `AddonHandler.cpp` (not present standalone; see CharacterHandler addon block).

## HermesProxy

This dir is the canonical source for "what the server actually does with packet X". Cross-ref with `Server/Packets/*.h` for the `Read()` body that hands the handler typed fields.

## Cross-refs

- Binding map: `../Server/Protocol/Opcodes.cpp`.
- Packet types: `../Server/Packets/<Name>Packets.h`.
- Game state: `../Entities/Player/Player.cpp`, `../Globals/ObjectAccessor.cpp`, `../Spells/SpellMgr.cpp`.
