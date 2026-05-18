# shared/Realm/

Realm struct + handle. Shared between bnetserver (publishes realm list) and worldserver (registers itself).

## Files

- `Realm.h` — `Realm` struct: `Id` (`RealmHandle{region,site,index}`), `Build`, `ExternalAddress`, `LocalAddress`, `LocalSubnetMask`, `Port`, `Name`, `NormalizedName`, `Type` (PvP/PvE/RP/RPPvP), `Flags` (RealmFlags), `Timezone`, `AllowedSecurityLevel`, `PopulationLevel`.
- `RealmFlags` enum: NONE, VERSION_MISMATCH, OFFLINE, SPECIFYBUILD, RECOMMENDED, NEW, FULL.
- `RealmHandle::RealmAddress()` — packs region|site|realm into uint32 (region<<24 | site<<16 | realm).
- `RealmList.cpp/.h` (if present) — bnetserver loads from `realmlist` table, refreshes on interval. Publishes via `GameUtilities` RPC.

## Wire shape (over GameUtilities RPC)

Realm sent as attribute blobs in `RealmListUpdates` response. Each entry has flags, name, address (region:site:realm packed), build, etc.

## HermesProxy

Need to translate packed `RealmAddress` (3.4.3) → 3.3.5a's flat `realm id` + provide synthetic ExternalAddress:Port pair as 3.3.5a-style realmlist response.

## DB

Source: `realmlist` table in `auth` DB.
