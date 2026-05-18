# src/

Source root. Three trees.

## Subdirs

- `common/` — shared utility lib. Crypto (SRP6, AES, RSA, Ed25519, HMAC, Argon2), logging, config, threading, containers, asio Socket wrapper, ByteBuffer, BigNumber, navigation (Detour), DBC/db2 datastore readers.
- `server/` — server binaries + libs. See `server/CLAUDE.md`.
- `genrev/` — generates `revision_data.h` from git rev. Embedded in version strings.

## Notes

- `common/` builds first. `server/shared` depends on `common/`. `server/game` depends on `shared` + `common`. Both daemons link `game` lib.
- `common/Cryptography/` has the SRP6 + WorldPacketCrypt — see its CLAUDE.md, critical for HermesProxy.
- Don't add new top-level dirs here; extend existing.
