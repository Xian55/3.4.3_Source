# bnetserver/Services/

Battle.net RPC services exposed on `Session`. Each derives from `Service<T>` where `T` is the generated protobuf service.

## Services

- `AuthenticationService` (`bnet.protocol.authentication.AuthenticationServer`):
  - `Logon(LogonRequest)` — validates web credentials (ticket from REST), starts session.
  - `VerifyWebCredentials(VerifyWebCredentialsRequest)` — resumes session via ticket.
  - `GenerateWebCredentials` — issues new ticket.
- `AccountService` — fetch game account list, settings.
- `ConnectionService` (`bnet.protocol.connection.ConnectionService`) — `Connect`, `KeepAlive`, `Echo`. First RPC after TLS handshake.
- `GameUtilitiesService` (`bnet.protocol.game_utilities.GameUtilities`):
  - `GetAllValuesForAttribute` — used by client to fetch realm list (returns realm join tickets).
  - `ProcessClientRequest` — generic command channel (e.g. realm list query, char counts).
- `Service.h` — CRTP base. Forwards `dispatch(method_id, request_bytes)` to typed `Handle*` overrides.
- `ServiceDispatcher.cpp/.h` — static map `service_hash → ServiceBase factory`. Built at startup; `Session` calls it on first frame.

## Hot path for HermesProxy

```
ConnectionService.Connect          (client opens RPC)
Authentication.Logon               (with REST ticket)
GameUtilities.GetAllValuesForAttribute("Command_RealmListRequest_v1_b9", ...) → realm list
GameUtilities.ProcessClientRequest("Command_RealmJoinRequest_v1_b9", realm_address) → join token
```

## Cross-refs

- Realm data source: `../../shared/Realm/Realm.cpp` + DB.
- Generated protobuf stubs: `../../proto/Client/authentication_service.pb.h`, `connection_service.pb.h`, `game_utilities_service.pb.h`.
