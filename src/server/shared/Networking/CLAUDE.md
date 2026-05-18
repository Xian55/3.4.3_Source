# shared/Networking/

Async TCP/TLS socket primitives. Boost.Asio backed.

## Files

- `Socket.h` — CRTP base `Socket<T>`. Owns `boost::asio::ip::tcp::socket`, async read loop, write queue, close.
- `SocketMgr.h` — template manager. Owns acceptor + N `NetworkThread<T>`. Load-balances accept across threads.
- `NetworkThread.h` — runs a `boost::asio::io_context` on a worker thread. Holds vector of active sockets.
- `AsyncAcceptor.h` — wraps `tcp::acceptor`. Hands accepted socket to manager.
- `SslSocket.h` — `Socket<T>` variant with `ssl::stream<tcp::socket>` for TLS (bnet + REST).
- `Http/` — REST framework on top of `Socket<T>`:
  - `BaseHttpSocket.cpp/.h` — HTTP parser, request routing.
  - `HttpSocket.h` (plain) / `HttpSslSocket.h` (TLS).
  - `HttpService.cpp/.h` — service template; concrete services subclass (e.g. `LoginRESTService`).
  - `HttpSessionState.h` — per-session token/cookie store.
  - `HttpCommon.h` — verbs, status codes, headers.

## Frame conventions

- WorldSocket: custom binary (see `../../game/Server/WorldSocket.h` `PacketHeader { uint32 Size; uint8 Tag[12]; }`).
- Bnet RPC: protobuf framed (length-prefixed `Header` + payload).
- REST: HTTP/1.1.

## HermesProxy

Reuse `Socket<T>` API mentally for understanding async flow; otherwise interop only via wire format.
