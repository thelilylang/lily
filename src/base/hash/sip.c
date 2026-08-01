/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <base/hash/sip.h>
#include <base/platform.h>

#include <string.h>

// The state is made of `Usize` words, so everything below is expressed in
// terms of their width rather than a hardcoded 64.
#define SIP_WORD_BITS (sizeof(Usize) * 8)

// NOTE: The `% SIP_WORD_BITS` is what keeps a rotation by a whole word - which
// is the identity, and which the round below asks for on 64 bits - from
// shifting by the width of the type, which is undefined.
#define ROTATE_LEFT(value, bits)             \
    (((value) << ((bits) % SIP_WORD_BITS)) | \
     ((value) >> ((SIP_WORD_BITS - (bits)) % SIP_WORD_BITS)))

// Rotation constants of a round. A 32-bit `Usize` cannot use the 64-bit ones:
// half of them are wider than the word. These are SipHash's and HalfSipHash's
// respectively, which is the same round applied to each width.
#ifdef PLATFORM_64
#define SIP_ROTATE_V1_A 13
#define SIP_ROTATE_V3_A 16
#define SIP_ROTATE_V0 32
#define SIP_ROTATE_V1_B 17
#define SIP_ROTATE_V3_B 21
#define SIP_ROTATE_V2 32
#else
#define SIP_ROTATE_V1_A 5
#define SIP_ROTATE_V3_A 8
#define SIP_ROTATE_V0 16
#define SIP_ROTATE_V1_B 13
#define SIP_ROTATE_V3_B 7
#define SIP_ROTATE_V2 16
#endif

typedef struct
{
    Usize v0;
    Usize v1;
    Usize v2;
    Usize v3;
} SipHashState;

static void
mix__SipHashState(SipHashState *self);

static void
final__SipHashState(SipHashState *self, Usize len);

void
mix__SipHashState(SipHashState *self)
{
    self->v0 += self->v1;
    self->v2 += self->v3;
    self->v1 = ROTATE_LEFT(self->v1, SIP_ROTATE_V1_A);
    self->v3 = ROTATE_LEFT(self->v3, SIP_ROTATE_V3_A);
    self->v1 ^= self->v0;
    self->v3 ^= self->v2;
    self->v0 = ROTATE_LEFT(self->v0, SIP_ROTATE_V0);
    self->v2 += self->v1;
    self->v0 += self->v3;
    self->v1 = ROTATE_LEFT(self->v1, SIP_ROTATE_V1_B);
    self->v3 = ROTATE_LEFT(self->v3, SIP_ROTATE_V3_B);
    self->v1 ^= self->v2;
    self->v3 ^= self->v0;
    self->v2 = ROTATE_LEFT(self->v2, SIP_ROTATE_V2);
}

void
final__SipHashState(SipHashState *self, Usize len)
{
    self->v2 ^= 0xFF;

    for (int i = 0; i < 4; ++i) {
        mix__SipHashState(self);
    }

    self->v0 ^= len;

    for (int i = 0; i < 4; ++i) {
        mix__SipHashState(self);
    }
}

Usize
hash_sip(const void *key, Usize key_len, const Usize k0, const Usize k1)
{
    SipHashState state = {
#ifdef PLATFORM_64
        .v0 = k0 ^ 0x736f6d6570736575ULL,
#else
        .v0 = k0 ^ 0x736f6d65,
#endif
#ifdef PLATFORM_64
        .v1 = k1 ^ 0x646f72616e646f6dULL,
#else
        .v1 = k1 ^ 0x646f7261,
#endif
#ifdef PLATFORM_64
        .v2 = k0 ^ 0x6c7967656e657261ULL,
#else
        .v2 = k0 ^ 0x6e657261,
#endif
#ifdef PLATFORM_64
        .v3 = k1 ^ 0x7465646279746573ULL,
#else
        .v3 = k1 ^ 0x79746573,
#endif
    };

    // NOTE: The key is read a word at a time, not a `Uint64` at a time. Where
    // `Usize` is 32 bits the latter dropped the upper half of every block on
    // its way into the state, so the hash never saw half of the key.
    const Uint8 *key_bytes = (const Uint8 *)key;
    const Uint8 *end = key_bytes + key_len - (key_len % sizeof(Usize));

    while (key_bytes < end) {
        Usize block;

        // A key is not guaranteed to be aligned for a `Usize`, which reading
        // one through a cast would require.
        memcpy(&block, key_bytes, sizeof(Usize));

        state.v3 ^= block;
        for (int i = 0; i < 2; ++i) {
            mix__SipHashState(&state);
        }
        state.v0 ^= block;
        key_bytes += sizeof(Usize);
    }

    Usize last_block = 0;

    memcpy(&last_block, key_bytes, key_len % sizeof(Usize));

    state.v3 ^= last_block;
    for (int i = 0; i < 2; ++i) {
        mix__SipHashState(&state);
    }
    state.v0 ^= last_block;

    final__SipHashState(&state, key_len);

    return state.v0 ^ state.v1 ^ state.v2 ^ state.v3;
}
