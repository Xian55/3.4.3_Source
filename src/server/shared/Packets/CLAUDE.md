# shared/Packets/

Low-level wire buffer. Foundation for all packet serialization.

## Files

- `ByteBuffer.cpp/.h` — primitive read/write. Append-grow, read cursor.
  - `<<` / `>>` operators for `u/int8/16/32/64`, `float`, `double`, `string`, `bool`, `ObjectGuid`.
  - **Bit-level ops** (3.4.3-style): `WriteBit`, `WriteBits(value, count)`, `ReadBit`, `ReadBits(count)`. Critical for HermesProxy — 3.3.5a is byte-aligned, 3.4.3 packets pack tightly.
  - `FlushBits()` — pads current bit accumulator before writing bytes again.
  - `WriteString` writes length prefix as bits (variable count per packet).
  - Throws `ByteBufferException` on under/overrun.

## Usage downstream

- `WorldPacket` (in `../../game/Server/WorldPacket.h`) extends `ByteBuffer`, adds `opcode` field.
- `ServerPacket::Write()` / `ClientPacket::Read()` in `../../game/Server/Packets/*.h` use bit ops extensively.

## HermesProxy translation note

3.3.5a → 3.4.3 packet rewrites must re-pack values bit-precise; lookup matching `*.cpp` to mirror bit field widths.
