# game/Movement/

Position updates + server-side splines + pathfinding.

## Files

- `MovementDefines.h` — `MovementFlags`, `MovementFlagsExtra`, `UnitMoveType` (walk/run/swim/fly speeds), spline flags.
- `MotionMaster.cpp/.h` — per-Unit FSM owning a stack of `MovementGenerator` (idle, chase, follow, flee, point, random, path, taxi, etc).
- `MovementGenerator.cpp/.h` — base. Children in `MovementGenerators/`.
- `MovementGenerators/` — concrete: `IdleMovementGenerator`, `ChaseMovementGenerator`, `WaypointMovementGenerator`, `PointMovementGenerator`, `FleeingMovementGenerator`, `HomeMovementGenerator`, `FlightPathMovementGenerator`, etc.
- `AbstractPursuer.cpp/.h` — base for chase/follow shared bits.
- `PathGenerator.cpp/.h` — Recast/Detour pathfinding wrapper. Builds polyline given start/end on a navmesh.
- `Spline/` — server-recorded spline replay; sync'd to client via spline opcodes.
- `Waypoints/` — DB-loaded creature patrol paths.

## Wire interaction

- **Incoming**: `Handlers/MovementHandler.cpp` consumes `CMSG_MOVE_*` packets. Sets `MovementInfo` on `Unit`. Relays to nearby players via `MSG_MOVE_*` broadcast.
- **Outgoing**: spline updates emit `SMSG_MONSTER_MOVE`, `SMSG_FLIGHT_SPLINE_SYNC`, `SMSG_MOVE_*_FORCE`. Packets defined in `../Server/Packets/MovementPackets.h`.
- **Anti-cheat**: server validates movement deltas, falls. Mismatch → kick / `Warden` flag.

## HermesProxy

- High-volume wire path. Per-tick client position packets.
- `MovementInfo` serialization order differs between 3.3.5a and 3.4.3 — see `MovementPackets.cpp` `WriteMovementInfo`.
- Spline opcodes added in modern WoW for smoother client replication.

## Cross-refs

- Map grid scan: `../Maps/Map.cpp` (`Map::UpdateObjectVisibility`).
- Path mesh: `../Movement/MovementGenerators/` + `../../../common/Nav/`.
