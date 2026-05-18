# server/game/

Massive game-logic lib. Linked into `worldserver`. Sub-systems below.

## HermesProxy hot subdirs (client wire)

- **`Server/`** — `WorldSession`, `WorldSocket`, `WorldPacket`, `Opcodes`. THIS is the entry. See `Server/CLAUDE.md`.
- **`Server/Packets/`** — 125 packet definition files. Wire format. See subdir CLAUDE.md.
- **`Server/Protocol/`** — `Opcodes.cpp` opcode→handler binding. Translation table for HermesProxy.
- **`Handlers/`** — `WorldSession::Handle*Opcode` impls. One file per system.
- **`Entities/Player/`** — Player state + `BuildValuesUpdate` (UPDATE_OBJECT wire data).
- **`Movement/`** — position updates, splines. Per-tick wire chatter.
- **`Spells/`** — spell cast packets, aura updates.
- **`Globals/`** — `ObjectAccessor`, `ObjectMgr`, `ObjectGuid` helpers used across handlers.
- **`Cache/`** — `CharacterCache` (name/guid lookups in handlers).
- **`Chat/`** — chat handlers + hyperlink validation.
- **`Accounts/`** — `AccountMgr`, `BattlenetAccountMgr`, `RBAC` (permissions).

## Lower-priority for proxy (still wire-visible)

- `Mails/`, `Guilds/`, `Groups/`, `AuctionHouse/`, `Calendar/`, `LFG`→`DungeonFinding/`, `Battlegrounds/`, `Battlefield/`, `Quests/`, `Achievements/`, `Reputation/`, `Skills/`, `Petitions/`, `Loot/`, `Mails/`, `Pools/`, `Texts/`, `Time/`, `Weather/`, `Maps/`, `Instances/`, `Scenarios/`, `OutdoorPvP/`, `BattlePets/`, `BattlePay/`, `World/`.

## Server-internal (skip for proxy)

- `AI/` — NPC AI.
- `Scripting/` — script registry, plug-in API.
- `Pools/` — spawn pools.
- `Warden/` — anti-cheat checksum.
- `AuctionHouseBot/` — AH NPC bot.
- `Combat/`, `Conditions/`, `DataStores/`, `Events/`, `Globals/`, `Grids/`, `Miscellaneous/`, `Phasing/`, `Storages/`, `Support/` — internal helpers.

## Wire path summary

```
WorldSocket (TCP+AES-GCM) → reads PacketHeader+payload
  → WorldPacket (ByteBuffer + opcode)
    → WorldSession::ReadDataHandler dispatch via OpcodeTable
      → ClientPacket subclass instantiated, Read() called
        → WorldSession::Handle<X>Opcode(packet&)
          → game logic
          → SendPacket(ServerPacket built via Packets/*.h Write())
```
