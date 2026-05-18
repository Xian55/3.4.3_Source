# bnetserver/Server/

TLS Battle.net protobuf RPC layer. After client has REST ticket, opens TLS here.

## Files

- `Session.cpp/.h` — one connection. Reads framed protobuf: `Header` (varint length + service hash + method + token) + payload. Routes via `ServiceDispatcher`. Tracks `_accountId`, `_gameAccountId`, `_clientSecret`. Holds locale, OS, build, IP.
- `SessionManager.cpp/.h` — `AsyncAcceptor` on `Battle.net.Port` (default 1119). Creates `Session` per accept. Inherits `Trinity::Net::SocketMgr<Session>`.
- `SslContext.cpp/.h` — loads `bnetserver.cert.pem` + key, builds `boost::asio::ssl::context`.

## Wire frame

```
[ uint16 header_size ] [ Header(protobuf) ] [ payload(protobuf) ]
```

`Header` carries `service_hash` (Fnv1a of service name), `method_id`, `token`, `object_id`, `status`. Dispatcher matches `service_hash` → `ServiceBase*` registered in `Session`.

## Cross-refs

- Service dispatch: `../Services/ServiceDispatcher.cpp`.
- Service catalog: `../Services/AuthenticationService`, `AccountService`, `ConnectionService`, `GameUtilitiesService`.
- Generated client RPC stubs (which the server consumes): `../../proto/Client/*.pb.h`.
- Frame base type: `../../proto/ServiceBase.cpp`.
