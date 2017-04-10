//-----------------------------------------------------------------------------
//MurmurHash2 was written by Austin Appleby, and is placed in the public
//domain. The author hereby disclaims copyright to this source code.

//Note - This code makes a few assumptions about how your machine behaves -
//1. We can read a 4-byte value from any address without crashing
//2. sizeof(int) == 4
//
//And it has a few limitations -
//
//1. It will not work incrementally.
//2. It will not produce the same results on little-endian and big-endian
//   machines.

#include "murmur-hash-2.h"

#define MIX(h,k,m) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

uint32_t murmur_hash_aligned_2(const void * key, uint32_t len, uint32_t seed)
{
	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	const unsigned char* data = (const unsigned char *)key;

	uint32_t h = seed ^ len;

	int align = (uint64_t)data & 3;

	if (align && (len >= 4))
	{
		// pre-load the temp registers
		uint32_t t = 0, d = 0;

		switch (align)
		{
		case 1: t |= data[2] << 16;
		case 2: t |= data[1] << 8;
		case 3: t |= data[0];
		}

		t <<= (8 * align);

		data += 4 - align;
		len -= 4 - align;

		int sl = 8 * (4 - align);
		int sr = 8 * align;

		// mix
		while (len >= 4)
		{
			d = *(uint32_t *)data;
			t = (t >> sr) | (d << sl);

			uint32_t k = t;

			MIX(h, k, m);

			t = d;

			data += 4;
			len -= 4;
		}

		// handle leftover data in temp registers
		d = 0;

		if (len >= align)
		{
			switch (align)
			{
			case 3: d |= data[2] << 16;
			case 2: d |= data[1] << 8;
			case 1: d |= data[0];
			}

			uint32_t k = (t >> sr) | (d << sl);
			MIX(h, k, m);

			data += align;
			len -= align;

			// handle tail bytes
			switch (len)
			{
			case 3: h ^= data[2] << 16;
			case 2: h ^= data[1] << 8;
			case 1: h ^= data[0];
				h *= m;
			};
		}
		else
		{
			switch (len)
			{
			case 3: d |= data[2] << 16;
			case 2: d |= data[1] << 8;
			case 1: d |= data[0];
			case 0: h ^= (t >> sr) | (d << sl);
				h *= m;
			}
		}

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	}
	else
	{
		while (len >= 4)
		{
			uint32_t k = *(uint32_t *)data;

			MIX(h, k, m);

			data += 4;
			len -= 4;
		}

		// handle tail bytes
		switch (len)
		{
		case 3: h ^= data[2] << 16;
		case 2: h ^= data[1] << 8;
		case 1: h ^= data[0];
			h *= m;
		};

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	}
}
