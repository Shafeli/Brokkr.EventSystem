#pragma once
#include <cstdint>

#define CIRCULAR_SHIFT_LEFT_32_BITS(x, r) ((x << r) | (x >> (32 - r)))


class Hash
{
public:

    static uint32_t Murmur3Hash(const void* key, const size_t len, const uint32_t seed)
    {
        const auto data = static_cast<const uint8_t*>(key);
        const int nblocks = len / 4;
        uint32_t hash = seed;

        constexpr uint32_t kC1 = 0xcc9e2d51;
        constexpr uint32_t kC2 = 0x1b873593;

        // Process the data in 4-byte chunks
        const auto blocks = reinterpret_cast<const uint32_t*>(data + nblocks * 4);
        for (int i = -nblocks; i; i++)
        {
            uint32_t k = blocks[i];

            // Mix the data using the Murmur3 mix function
            k *= kC1;
            k = CIRCULAR_SHIFT_LEFT_32_BITS(k, 15);
            k *= kC2;

            hash ^= k;
            hash = CIRCULAR_SHIFT_LEFT_32_BITS(hash, 13);
            hash = hash * 5 + 0xe6546b64;
        }

        // Handle any remaining bytes
        const auto* tail = data + nblocks * 4;
        uint32_t k = 0;

        // the fall-through is intentional and is used to avoid
        // repeating the same code for cases where the length of
        // the input data is not a multiple of 4

        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch (len & 3)
        {
        case 3:
            k ^= tail[2] << 16;
        case 2:  // NOLINT(clang-diagnostic-implicit-fallthrough)
            k ^= tail[1] << 8;
        case 1:  // NOLINT(clang-diagnostic-implicit-fallthrough)
            k ^= tail[0];
            k *= kC1;
            k = CIRCULAR_SHIFT_LEFT_32_BITS(k, 15);
            k *= kC2;
            hash ^= k;
        }

        // Finalize the hash
        hash ^= len;
        hash = FMix(hash);

        return hash;
    }


private:

    static uint32_t FMix(uint32_t hash)
    {
        hash ^= hash >> 16;
        hash *= 0x85ebca6b;
        hash ^= hash >> 13;
        hash *= 0xc2b2ae35;
        hash ^= hash >> 16;
        return hash;
    }

};

