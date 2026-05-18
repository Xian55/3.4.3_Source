# bnetserver/

Battle.net daemon. Handles login + realm selection. **First stop for HermesProxy.**

## Entry

- `Main.cpp` — boot. Reads `bnetserver.conf.dist`. Starts `LoginRESTService` (HTTPS) + `SessionManager` (TLS RPC). Loads DB.
- `bnetserver.cert.pem` / `bnetserver.key.pem` — TLS cert + key. Self-signed by default; replace for prod.
- `bnetserver.conf.dist` — config template (ports, DB conn, cert paths, SRP version).
- `resource.h`, `bnetserver.rc`, `bnetserver.ico` — Win resource.

## Subdirs

- `REST/` — HTTP login endpoint (port `LoginREST.Port`, default 8081 over TLS).
- `Server/` — post-REST TLS+protobuf Battle.net session.
- `Services/` — RPC service handlers dispatched on `Session`.

## Flow

1. Client hits REST: `GET /bnetserver/login/` → form, `POST` → SRP6 challenge → ticket.
2. Client opens TLS to `Battle.net.Port` (default 1119), sends protobuf frames.
3. `SessionManager` accepts → `Session::Start()` reads first protobuf header → routes to a `Service`.
4. `Authentication.Logon` verifies ticket → `GameUtilities` serves `RealmListTicket` + realms.
5. Client picks realm → gets address+token for worldserver.

## Cross-refs

- Proto wire defs: `../proto/Login/Login.proto`, `../proto/RealmList/RealmList.proto`, `../proto/Client/*.proto`.
- SRP6 impl: `../../common/Cryptography/Authentication/SRP6.cpp`.
- Realm DB row: `../shared/Realm/`.
