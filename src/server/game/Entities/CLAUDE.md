# game/Entities/

Game object hierarchy + UpdateData wire format source.

## Class tree

```
Object
 └── WorldObject (has position, map ptr)
      ├── Unit
      │    ├── Player        (Player/Player.cpp — the giant)
      │    ├── Creature      (Creature/)
      │    │    └── Pet      (Pet/)
      │    ├── Totem         (Totem/)
      │    └── TempSummon    (in Creature/)
      ├── GameObject         (GameObject/)
      ├── DynamicObject      (DynamicObject/)
      ├── Corpse             (Corpse/)
      ├── AreaTrigger        (AreaTrigger/)
      ├── Conversation       (Conversation/)
      ├── SceneObject        (SceneObject/)
      └── (sub-class) Vehicle is Unit-like via Vehicle/
Item (in Item/, NOT a WorldObject)
Transport (Transport/) — special WorldObject
```

## Object base (`Object/Object.cpp/.h`)

- Holds `_objectGuid` (`ObjectGuid` 128-bit), `_objectType` flags.
- **Field storage** = `UpdateFieldHolder` indexed by descriptor enum. Modern WoW replaces `m_uint32Values[]` with typed update fields.
- `BuildCreateUpdateBlockForPlayer`, `BuildValuesUpdate`, `BuildOutOfRangeUpdateBlock` → emit `SMSG_UPDATE_OBJECT` payload. **Source of all object-state wire data.**
- `BuildMovementUpdate` → emits `MovementInfo` segment.

## Player (`Player/Player.cpp/.h`)

Largest single class. Owns:
- Inventory (`_items[INVENTORY_END]`).
- Spellbook, talents, glyphs.
- Quest log (`m_QuestStatus`).
- Achievement progress (via `AchievementMgr`).
- Reputation (`ReputationMgr`).
- Social (`SocialMgr` peer).
- Pet, group, guild, instance lock state.
- Action bar, taxi nodes known.
- Honor, currency, conquest.

Helpers in `Player/`: `CollectionMgr.cpp` (toys/mounts/pets), `SocialMgr.cpp`, `CinematicMgr.cpp`, `SceneMgr.cpp`, `RestMgr.cpp`, `PlayerTaxi.cpp`, `TradeData.cpp`, `KillRewarder.cpp`.

## HermesProxy

`SMSG_UPDATE_OBJECT` wire format **differs heavily** from 3.3.5a:
- 3.4.3 uses descriptor-based UpdateFields, bit-packed mask.
- 3.3.5a uses simple `m_uint32Values[]` mask array.
- See `Object::BuildValuesUpdate` and `*UpdateFields.h` headers (under `Entities/<Type>/`).

## Cross-refs

- Movement append: `../Movement/`.
- Spell aura on Unit: `../Spells/Auras/`.
- Object resolve: `../Globals/ObjectAccessor.cpp`.
