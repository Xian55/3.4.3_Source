# dep/ — vendored third-party

DO NOT MODIFY. All libs vendored at fixed versions. Patches in `../Patches/` if upstream tweaks needed.

## Inventory

See `PackageList.txt` for canonical version list.

- `boost/` — only headers referenced by CMake (system boost linked).
- `openssl/`, `openssl_ed25519/` — TLS, crypto primitives.
- `protobuf/` — bnet RPC + RealmList.
- `mysql/` — DB client.
- `fmt/` — formatting.
- `rapidjson/` — JSON.
- `recastnavigation/` — navmesh/path.
- `g3dlite/` — math/geom.
- `CascLib/` — Blizzard storage reader (client data extract).
- `argon2/` — password hashing.
- `efsw/` — file watcher.
- `gsoap/` — SOAP for `worldserver` TCSoap RA cli.
- `jemalloc/`, `SFMT/`, `short_alloc/`, `utf8cpp/`, `valgrind/`, `zlib/`, `threads/`, `readline/` — misc.

## Rule

If a dep needs a fix, patch in `../Patches/` and add CMake apply step. Never edit `dep/` directly.
