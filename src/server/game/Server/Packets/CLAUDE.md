# game/Server/Packets/

125 packet definition files. **Primary reference for 3.4.3 wire format.**

## Convention

Each subsystem has `<Name>Packets.h` + `.cpp`:
- `class FooRequest final : public ClientPacket { void Read() override; ... fields }` — incoming.
- `class FooResponse final : public ServerPacket { WorldPacket const* Write() override; ... fields }` — outgoing.
- Constructor of `ClientPacket` takes `WorldPacket&&`, opcode passed.
- Constructor of `ServerPacket` takes opcode + reserve size.

`AllPackets.h` aggregates every `*Packets.h` for `Opcodes.cpp` to instantiate.

## Files grouped (for HermesProxy lookup)

### Auth/Session
- `AuthenticationPackets.h` — `AuthChallenge`, `AuthSession`, `AuthResponse`, `AuthContinuedSession`, `ConnectToFailed`, `Ping`/`Pong`, `EnterEncryptedMode`. **CRITICAL.**
- `BattlenetPackets.h` — Bnet bridge over world socket (rare).
- `SystemPackets.h` — server time, MOTD.
- `AddonPackets.h` — addon enum, public keys.

### Character lifecycle
- `CharacterPackets.h` — enum, create, delete, login, logout, character cache responses.
- `ClientConfigPackets.h` — account data tags.
- `MiscPackets.h` — catch-all.

### Movement / Combat / Spells / Items
- `MovementPackets.h` — `MoveUpdate*`, `MoveSet*`, `TransferPending`, `TransferAborted`, knockback. Per-tick traffic.
- `CombatPackets.h`, `CombatLogPackets.h`, `CombatLogPacketsCommon.h` — attack swings, log entries.
- `SpellPackets.h` — `CastSpell`, `SpellGo`, `SpellStart`, `AuraUpdate`, cooldowns.
- `ItemPackets.h`, `ItemPacketsCommon.h` — inventory, use, swap, refund.
- `BankPackets.h` — bank slot ops.
- `EquipmentSetPackets.h` — gear sets.

### Social / Group
- `ChatPackets.h` — say/yell/whisper/channel, emotes.
- `ChannelPackets.h` — channel join/notify.
- `PartyPackets.h` — group ops.
- `GuildPackets.h` — roster, motd, ranks.
- `SocialPackets.h` — friends, ignores.
- `WhoPackets.h` — `/who` query.
- `InspectPackets.h` — inspect.
- `DuelPackets.h` — duel start/end.
- `TradePackets.h` — trade slots.

### Pets / Vehicles / Totems
- `PetPackets.h`, `PetitionPackets.h`, `TotemPackets.h`, `VehiclePackets.h`.

### World content
- `QuestPackets.h` — quest log, give, complete.
- `QueryPackets.h` — creature/gameobject/page-text/realm queries.
- `LootPackets.h` — loot view, roll, master.
- `MailPackets.h` — inbox, send, attach.
- `AreaTriggerPackets.h`, `GameObjectPackets.h`, `SceneObjectPackets.h`, `ScenePackets.h`.
- `WorldStatePackets.h` — UI flags (e.g. battleground scores).
- `TaxiPackets.h` — flight master.
- `InstancePackets.h` — instance lock info, reset.
- `BattlegroundPackets.h` — BG queue, status, score.
- `ArenaPackets.h` — arena team.
- `LFGPackets.h` — dungeon finder.
- `CalendarPackets.h` — events, invites.
- `AuctionHousePackets.h` — list, bid, mail outcome.

### Progression / Collections
- `AchievementPackets.h` — earned, criteria.
- `ReputationPackets.h` — faction standing.
- `TalentPackets.h` — spec, talents, glyphs.
- `BattlePetPackets.h`, `CollectionPackets.h`, `ToyPackets.h` — collection-screen (post-Cata only).
- `PerksProgramPacketsCommon.h` — modern store.
- `TokenPackets.h` — WoW tokens.

### Misc
- `ReferAFriendPackets.h`, `ScenarioPackets.h`, `SkillPackets.h`, `TicketPackets.h`, `WardenPackets.h`, `HotfixPackets.h`, `CraftingPacketsCommon.h`.

### Foundation
- `Packet.cpp/.h` — `ClientPacket`/`ServerPacket` base. NOT here; in `../Packet.cpp`.
- `PacketUtilities.cpp/.h` — sanitizers (string length caps, regex validation).
- `AllPackets.h` — aggregator.

## HermesProxy lookup pattern

For a 3.3.5a opcode you want to relay:
1. Grep its name in `../Protocol/Opcodes.h` → get 3.4.3 hex.
2. Grep that opcode in `Opcodes.cpp` → handler binding line names a `*Handler.cpp` + handler method.
3. Look at the bound `ClientPacket` class → `.cpp` `Read()` shows wire shape.
