/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#define ROTATE_LEFT(value, bits) \
    (((value) << (bits)) | ((value) >> (64 - (bits))))

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
    self->v1 = ROTATE_LEFT(self->v1, 13);
    self->v3 = ROTATE_LEFT(self->v3, 16);
    self->v1 ^= self->v0;
    self->v3 ^= self->v2;
    self->v0 = ROTATE_LEFT(self->v0, 32);
    self->v2 += self->v1;
    self->v0 += self->v3;
    self->v1 = ROTATE_LEFT(self->v1, 17);
    self->v3 = ROTATE_LEFT(self->v3, 21);
    self->v1 ^= self->v2;
    self->v3 ^= self->v0;
    self->v2 = ROTATE_LEFT(self->v2, 32);
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

    const Uint8 *key_bytes = (const Uint8 *)key;
    const Uint8 *end = key_bytes + key_len - (key_len % sizeof(uint64_t));
    const Uint64 *blocks = (const Uint64 *)key_bytes;

    while (key_bytes < end) {
        state.v3 ^= *blocks;
        for (int i = 0; i < 2; ++i) {
            mix__SipHashState(&state);
        }
        state.v0 ^= *blocks;
        blocks++;
        key_bytes += sizeof(uint64_t);
    }

    Uint64 last_block = 0;

    memcpy(&last_block, key_bytes, key_len % sizeof(uint64_t));

    state.v3 ^= last_block;
    for (int i = 0; i < 2; ++i) {
        mix__SipHashState(&state);
    }
    state.v0 ^= last_block;

    final__SipHashState(&state, key_len);

    return state.v0 ^ state.v1 ^ state.v2 ^ state.v3;
}
