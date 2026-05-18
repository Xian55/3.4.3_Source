# src/server/

Server-side code. Two binaries.

## Binaries

- `bnetserver/` — Battle.net daemon. Login (REST) + Bnet RPC (TLS+protobuf). Hands client a ticket + realm address.
- `worldserver/` — Game daemon. Accepts world sockets (AES-GCM), runs game loop.

## Libs (linked into above)

- `shared/` — Realm struct, Networking sockets, ByteBuffer, Packets shared layer, JSON, Secrets, DataStores readers.
- `game/` — Game logic. Handlers, packets, entities, spells, maps, etc. Linked by worldserver.
- `database/` — MySQL pool + async queries + Updater for migrations.
- `proto/` — `.proto` defs + generated stubs. Linked by both binaries.
- `scripts/` — Gameplay scripts compiled into worldserver. Not on wire path.

## Auth/comm flow (HermesProxy focus)

```
1. Client → HTTPS POST  /bnetserver/login/         → REST (LoginRESTService.cpp)
                                                     SRP6 (v1/v2 via Sha256/Sha512) handled here
                                                     returns login_ticket "TC-...."
2. Client → TLS+protobuf to bnetserver Session    → Authentication.Logon
                                                     ConnectionService keepalive
                                                     GameUtilitiesService → RealmList + char counts
3. Client → TCP to worldserver WorldSocket       → AuthSession (AES-GCM init)
                                                     opcode loop dispatched by WorldSession
```

## Entry points

- `bnetserver/Main.cpp` — bootstraps SessionManager + LoginRESTService.
- `worldserver/Main.cpp` — bootstraps World singleton + WorldSocketMgr.
