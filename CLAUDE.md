# 3.4.3 Source (Wrathion)

WoW 3.4.3 (WotLK Classic) private server. TrinityCore-derived fork. ~70% playable per README.

## HermesProxy context

User builds `X:\Programming\HermesProxy` — bridges 3.3.5a client → this 3.4.3 server. Key wire differences:
- **Opcodes**: 16-bit scrambled (e.g. `CMSG_ACCEPT_TRADE = 0x315A`) — NOT linear like 3.3.5a. See `src/server/game/Server/Protocol/Opcodes.h`.
- **Crypt**: AES-GCM 12-byte tag per packet (3.3.5a was ARC4). See `src/common/Cryptography/Authentication/WorldPacketCrypt.h`.
- **Login**: REST → protobuf-over-TLS Battle.net flow (3.3.5a was SRP6 over plain TCP).
- **Multi-socket**: `ConnectionType` REALM=0 + INSTANCE=1 sockets per session.

## Layout

- `src/common/` — utils, crypto, logging, asio glue. Linked by all server binaries.
- `src/server/bnetserver/` — login daemon (REST + protobuf).
- `src/server/worldserver/` — game daemon (opcodes + AES).
- `src/server/shared/` — code shared between bnet+world (Realm, Networking, ByteBuffer).
- `src/server/game/` — game logic library (handlers, packets, entities, spells).
- `src/server/database/` — MySQL connection layer + async pool.
- `src/server/scripts/` — gameplay scripts per continent. Not on wire path.
- `src/server/proto/` — `.proto` files for bnet RPC.
- `src/genrev/` — embed git rev into binaries.
- `dep/` — vendored third-party libs. Don't modify.
- `Databases/`, `Patches/` — SQL dumps + migration patches.

## Build

CMake. Top `CMakeLists.txt`. Toolchain hints in `cmake/`, `PreLoad.cmake`. Generates `bnetserver.exe` + `worldserver.exe`.

## Comm flow (HermesProxy target)

```
client → bnetserver REST  (HTTPS /bnetserver/login/) → ticket
       → bnetserver Session (TLS + protobuf framed)  → Authenticate → RealmList
       → worldserver WorldSocket (TCP + AES-GCM)      → AuthSession → opcode loop
```

## Hot files

- Opcode table: `src/server/game/Server/Protocol/Opcodes.cpp`
- Session dispatch: `src/server/game/Server/WorldSession.cpp`
- Wire socket: `src/server/game/Server/WorldSocket.cpp`
- Packet defs (125 files): `src/server/game/Server/Packets/`
- Handlers (40+ files): `src/server/game/Handlers/`
- Bnet login: `src/server/bnetserver/REST/LoginRESTService.cpp` + `Server/Session.cpp`
- Realm select: `src/server/bnetserver/Services/GameUtilitiesService.cpp`

## Distributed CLAUDE.md trail

Walk `CLAUDE.md` in each subdir for tighter context.
