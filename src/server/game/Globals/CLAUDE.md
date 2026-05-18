# game/Globals/

Global registries + lookups. Referenced by nearly every handler.

## Files

- `ObjectMgr.cpp/.h` — singleton. Holds **all** static templates loaded from DB:
  - `_creatureTemplateStore`, `_creatureAddonStore`, `_creatureModelStore`.
  - `_gameObjectTemplateStore`, `_gameObjectAddonStore`.
  - `_itemTemplateStore`, `_questTemplates`, `_pageTextStore`, `_npcTextStore`.
  - `_areaTriggerStore`, `_gossipMenuStore`, `_pointsOfInterestStore`.
  - `_playerInfo[race][class]` — starting pos/spells/items.
  - Equipment, broadcast text, vendor lists, trainer lists, taxi paths.
- `ObjectAccessor.cpp/.h` — resolves `ObjectGuid` → live object pointer across maps:
  - `GetPlayer(WorldObject const&, ObjectGuid)`, `FindPlayer(ObjectGuid)`.
  - `GetCreature`, `GetGameObject`, `GetCorpse`, `GetUnit`, `GetWorldObject`.
- `AreaTriggerDataStore.cpp/.h` — area trigger spawn templates.
- `CharacterTemplateDataStore.cpp/.h` — pre-baked character templates (e.g. boost characters).
- `ConversationDataStore.cpp/.h` — conversation entry definitions.

## Usage from handlers

Handlers consult `ObjectMgr` for static data (e.g. quest template), `ObjectAccessor` for live targets (e.g. target unit for a spell).

## HermesProxy

- `ObjectGuid` is 128-bit in 3.4.3 (vs 64-bit in 3.3.5a). Encoding: `high uint64 | low uint64` with type/subtype/realmId packed in high. See `Entities/Object/ObjectGuid.cpp` for layout.
- Many query responses (`SMSG_QUERY_*_RESPONSE`) draw from `ObjectMgr` stores — wire shape differs from 3.3.5a.
