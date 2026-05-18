# worldserver/

Game daemon. Accepts world sockets, runs World loop.

## Files

- `Main.cpp` — boot. Reads `worldserver.conf.dist`. Connects DBs (login/char/world). Loads `World` singleton. Starts `WorldSocketMgr` on `WorldServerPort` (default 8085). Runs game loop tick. Stops on SIGINT or `World::IsStopped()`.
- `worldserver.conf.dist` — config template. Massive; controls every game knob.
- `resource.h`, `worldserver.rc`, `worldserver.ico` — Win resource.

## Subdirs

- `CommandLine/` — CLI argument parsing.
- `RemoteAccess/` — RA cli (telnet-like) for in-game GM commands over TCP.
- `TCSoap/` — SOAP server (gSOAP) for external command exec.
- `PrecompiledHeaders/` — PCH stub.

## Hot links

- World loop: `../game/World/World.cpp` (`World::Update`).
- Socket accept: `../game/Server/WorldSocketMgr.cpp`.
- Per-client session: `../game/Server/WorldSession.cpp`.
- DB pool: `../database/Database/`.

## HermesProxy note

This binary owns the TCP port the client connects to AFTER realm select. `WorldServerPort` published to bnetserver via `Realm.address`. AES key sourced from bnetserver session key (carried via `RealmJoinTicket`).
