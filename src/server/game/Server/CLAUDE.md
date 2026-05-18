# game/Server/

World socket + session. **THIS is the worldserver wire entry point.**

## Files

- `WorldSocket.cpp/.h` — TCP socket. Handles AES-GCM crypt, auth handshake, fragment reassembly, send queue.
  - `PacketHeader { uint32 Size; uint8 Tag[12]; }` — header on every packet (12-byte AES-GCM tag).
  - `IncomingPacketHeader` adds `uint16 EncryptedOpcode`.
  - `EncryptablePacket` — `WorldPacket` + encrypt-flag, queued via `MPSCQueue`.
  - First send: `SMSG_AUTH_CHALLENGE` with server seed + 16-byte challenge.
  - Client replies: `CMSG_AUTH_SESSION` (build, account, client proof) → server derives AES key.
  - Multi-socket: `CONNECTION_TYPE_REALM` for main, `CONNECTION_TYPE_INSTANCE` for map (each gets own `WorldSocket` sharing session key).
- `WorldSocketMgr.cpp/.h` — `SocketMgr<WorldSocket>`. Listens on `WorldServerPort`. Two sub-pools for realm vs instance sockets.
- `WorldSession.cpp/.h` — per-account session (survives socket reconnects briefly). Owns `Player`, `OpcodeTable` dispatch loop in `Update()`. Holds `_realmSocket`, `_instanceSocket`.
- `WorldPacket.h` — `ByteBuffer` + `_opcode` (uint32). Standard wire object.
- `Packet.cpp/.h` — `ServerPacket` / `ClientPacket` base classes. Children in `Packets/`.
- `Packets/` — 125 packet defs. See subdir CLAUDE.md.
- `Protocol/Opcodes.cpp/.h`, `PacketLog.cpp/.h` — opcode table + PCAP-style dump.

## HermesProxy critical detail

- Handshake derives AES key from **session key + build seeds** (search `SessionKeyGenerator`/`Sha256` in WorldSocket.cpp). 3.3.5a used ARC4 with different KDF — proxy must rekey at boundary.
- After handshake, every packet payload is `AES-GCM(plaintext)` with `Tag[12]` in the header and `Size` covering ciphertext.
- `EncryptedOpcode` is encrypted; only readable after `DecryptRecv`.

## Cross-refs

- Crypt: `../../../common/Cryptography/Authentication/WorldPacketCrypt.h` (AES-GCM streams).
- Dispatch table: `Protocol/Opcodes.cpp` (`opcodeTable` singleton).
- Per-packet types: `Packets/*.h`.
- Handler impls: `../Handlers/*Handler.cpp`.
