# proto/

Protobuf wire defs for Battle.net RPC. **Edit `.proto`, never `.pb.cc/.h`** (generated).

## Subdirs

- `Login/` — REST login messages (`Login.proto`). Used between client and `bnetserver/REST/`.
  - `FormInputs`, `LoginForm`, `LoginResult`, `SrpLoginChallenge`, `AuthenticationState` enum.
- `RealmList/` — `RealmList.proto`. Realm enumeration + character counts. Used by `GameUtilitiesService`.
- `Client/` — large catalog of Battle.net service `.proto` (mostly pre-compiled into `.pb.h`; sources `club_*.proto`, `club_listener.proto`, `club_service.proto` visible). Includes:
  - `authentication_service.pb.*`, `connection_service.pb.*`, `game_utilities_service.pb.*` — bnet core RPC.
  - `account_service`, `friends`, `presence`, `report`, `resource`, `user_manager` — peripheral services (many stub-only).
  - `rpc_types.pb.*` — `Header`, `NoData`, `ProcessId`, status codes.
- `api/` (under Client) — extension types.
- `global_extensions/` — protobuf option extensions (custom RPC tags).

## Top-level files

- `ServiceBase.cpp/.h` — base for generated `Service<T>` template. CRTP dispatch.
- `BattlenetRpcErrorCodes.h` — numeric error enum returned in `Header.status` (e.g. `ERROR_BAD_VERSION`, `ERROR_DENIED`, `ERROR_NOT_EXISTS`).

## Build

`CMakeLists.txt` invokes `protoc` to regenerate `.pb.cc/.h` from `.proto`. After editing a `.proto`, rebuild.

## HermesProxy note

Service identity is by **FNV-1a of `service_name`** (e.g. `bnet.protocol.authentication.AuthenticationServer` → fixed hash). Hash table built in `ServiceDispatcher.cpp`. Same hashes the client uses.
