# server/shared/

Code shared between `bnetserver` + `worldserver`. Linked as `shared` lib.

## Subdirs

- `Networking/` — async TCP/TLS sockets + HTTP framework. Used by both daemons.
- `Packets/` — `ByteBuffer` low-level read/write primitive. Foundation for `WorldPacket`.
- `Realm/` — `Realm` struct, `RealmHandle` (region|site|index), `RealmList` reader.
- `DataStores/` — `DBC`/`DB2` storage file readers (client static data).
- `JSON/` — wrappers over rapidjson.
- `Dynamic/` — dynamic loadable module support.
- `Secrets/` — `SecretMgr.cpp` — TOTP, encryption keys for stored secrets.

## Top-level files

- `DetourMemoryFunctions.h` — alloc hooks for Recast/Detour navmesh.
- `CMakeLists.txt` — builds `shared` lib.

## HermesProxy relevance

- `Networking/` — proxy must implement same socket framing.
- `Packets/ByteBuffer` — wire read/write primitive; same semantics as 3.3.5a but with bit-packed strings + GUIDs.
- `Realm/` — realm flags, build, region encoding.
