# common/Cryptography/

Crypto primitives. **HermesProxy must mirror these to relay encrypted traffic.**

## Files

### Authentication subdir (`Authentication/`)
- `SRP6.cpp/.h` — Secure Remote Password v6 for REST login.
  - Supports `SrpVersion::v1`/`v2`, hash `Sha256`/`Sha512`.
  - `MakeRegistrationData` (creates verifier), `GetSessionKey` (server-side proof check).
  - **HermesProxy**: 3.3.5a client speaks SRP6 over `realmd` plain TCP (CMD_AUTH_LOGON_CHALLENGE/PROOF); 3.4.3 expects SRP6 over HTTPS REST. Proxy translates.
- `WorldPacketCrypt.cpp/.h` — **AES-GCM** session crypt for world socket.
  - Two streams: `_clientDecrypt` (in), `_serverEncrypt` (out).
  - `Init(AES::Key)` from session key derived after AuthSession.
  - `EncryptSend(data, len, Tag&)` produces 12-byte tag per packet.
  - `DecryptRecv(data, len, Tag&)` verifies tag.
  - Counters `_clientCounter`, `_serverCounter` increment per packet (used as nonce).
  - **3.3.5a uses ARC4 with HMAC-SHA1 key**; proxy must rekey at boundary.
- `AuthDefines.h` — `SessionKey` (40-byte for SRP), `AccountTypes`, `AES::Key` typedefs.

### Top-level
- `AES.cpp/.h` — `Trinity::Crypto::AES` GCM-128 wrapper. `Key` = 16 bytes. `Tag` = 12 bytes.
- `ARC4.cpp/.h` — legacy ARC4 (still used internally for some addon/CASC checks).
- `RSA.cpp/.h` — RSA-2048/4096 for warden, hotfix sigs.
- `Ed25519.cpp/.h` — modern signature scheme. Used in `../../server/shared/Secrets/`.
- `Argon2.cpp/.h` — password KDF (alt to SRP for newer accounts).
- `BigNumber.cpp/.h` — OpenSSL `BIGNUM` wrapper. Foundation for SRP exponentiation.
- `CryptoConstants.h`, `CryptoGenerics.h`, `CryptoHash.h` — typed wrappers (SHA1/SHA256/SHA512/MD5/HMAC).
- `CryptoRandom.cpp/.h` — `Trinity::Crypto::GetRandomBytes`.
- `HMAC.h` — templated HMAC over hash type.
- `OpenSSLCrypto.cpp/.h` — global init/teardown.
- `SessionKeyGenerator.h` — KDF for deriving sub-keys from session key (e.g. addon channel key).
- `TOTP.cpp/.h` — RFC 6238 authenticator code check.

## HermesProxy critical path

1. **Login**: receive 3.3.5a `CMD_AUTH_LOGON_CHALLENGE` → compute SRP6 against server. Reply with `CMD_AUTH_LOGON_PROOF`. Internally, the proxy does the REST SRP6 dance with 3.4.3 server using same salt/verifier.
2. **Realmlist**: 3.3.5a `CMD_REALM_LIST` request → relay to `GameUtilitiesService.GetAllValuesForAttribute` on 3.4.3 bnetserver. Translate response back.
3. **World**: 3.3.5a uses ARC4 with derived session key; 3.4.3 uses AES-GCM with `Tag[12]` + counter. Proxy reframes every packet.

## Cross-refs

- World socket: `../../server/game/Server/WorldSocket.cpp` consumes `WorldPacketCrypt`.
- REST entry: `../../server/bnetserver/REST/LoginRESTService.cpp` consumes `SRP6`.
