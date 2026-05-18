# bnetserver/REST/

HTTPS login endpoint. **First wire step for HermesProxy.** Form-based SRP6 auth → returns login ticket.

## Files

- `LoginRESTService.cpp/.h` — HTTPS server. Routes:
  - `GET  /bnetserver/login/`   → returns `FormInputs` (account/password fields) as JSON/protobuf.
  - `POST /bnetserver/login/`   → consumes `LoginForm` (account+password), runs SRP6, returns `LoginResult { ticket }`.
  - `GET  /bnetserver/portal`   → returns Bnet RPC endpoint (host:port) for next step.
- `LoginHttpSession.cpp/.h` — per-connection HTTP session (extends `Trinity::Net::Http::Session`).

## SRP versions

`SrpVersion::v1` (legacy) vs `v2`. Hash `SrpHashFunction::Sha256` or `Sha512`. Configured via `LoginREST.SRPVersion` + `SRPHash`. Salt + verifier from DB.

## Ticket format

`TC-` prefix + base64 random. Stored in `LoginTickets` table with account_id + expiry. Consumed later by `Authentication.Logon` and `Authentication.VerifyWebCredentials` (see `../Services/AuthenticationService.cpp`).

## Cross-refs

- HTTP framework: `../../shared/Networking/Http/`.
- Proto messages: `../../proto/Login/Login.proto` (`FormInputs`, `LoginForm`, `SrpLoginChallenge`, `LoginResult`).
- SRP impl: `../../../common/Cryptography/Authentication/SRP6.cpp`.
