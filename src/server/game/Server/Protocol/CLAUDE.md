# game/Server/Protocol/

Opcode table + packet logging. **Translation reference for HermesProxy.**

## Files

- `Opcodes.h` — defines:
  - `enum OpcodeMisc { MAX_OPCODE=0x3FFF, NUM_OPCODE_HANDLERS=0x4000, UNKNOWN_OPCODE=0xFFFF, NULL_OPCODE=0xBADD }`.
  - `enum OpcodeClient : uint16` — every CMSG with its 16-bit value (e.g. `CMSG_ACCEPT_TRADE = 0x315A`). Values are scrambled, NOT sequential. Build-specific.
  - `enum OpcodeServer : uint16` — every SMSG with its 16-bit value.
  - `enum ConnectionType { REALM=0, INSTANCE=1, DEFAULT=-1 }`.
  - `OpcodeTable` class — two `_internalTable*[NUM_OPCODE_HANDLERS]` arrays.
- `Opcodes.cpp` — `opcodeTable` global. `OpcodeTable::Initialize()` calls thousands of `DEFINE_HANDLER(CMSG_X, STATUS_*, PROCESS_*, &WorldSession::HandleXOpcode)` and `DEFINE_SERVER_OPCODE_HANDLER(SMSG_X, STATUS_*, PROCESS_*, CONNECTION_TYPE_*)`. **This file is the canonical opcode→handler map.**
- `PacketLog.cpp/.h` — PCAP-style dump of all in/out packets if enabled in config. File format compatible with WPP/WPE viewers.

## SessionStatus values

- `STATUS_AUTHED` — only when logged in (no player yet).
- `STATUS_LOGGEDIN` — full player active.
- `STATUS_TRANSFER` — during map transfer.
- `STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT` — relogin grace.
- `STATUS_NEVER` — never accept (debug/dead opcode).
- `STATUS_UNHANDLED` — accept but log + drop.

## PacketProcessing

- `PROCESS_INPLACE` — handle on socket thread (cheap, no game state).
- `PROCESS_THREADUNSAFE` — queue to map thread (touches Player/Map state).

## HermesProxy

- Build opcode translation tables from this file (`OpcodeClient`/`OpcodeServer` enums) keyed by 3.4.3 hex → 3.3.5a name.
- 3.3.5a opcode values are flat (`CMSG_LOGOUT_REQUEST=0x004B`); 3.4.3 are scrambled per build.
