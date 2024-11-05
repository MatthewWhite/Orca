#ifndef HASH_H
#define HASH_H

#pragma once

#include <cstdint>

const uint64_t FNV1A_OFFSET_64 = 0xcbf29ce484222325;
const uint64_t FNV1A_PRIME_64 = 0x100000001B3;

constexpr uint64_t _fnv1a64(const char* str, uint64_t hashValue)
{
	return str[0] == 0 ? hashValue : _fnv1a64(&str[1], (hashValue ^ (uint64_t)str[0]) * FNV1A_PRIME_64);
}

constexpr uint64_t fnv1a64(const char* str)
{
	return _fnv1a64(str, FNV1A_OFFSET_64);
}

constexpr uint64_t hash(const char* str)
{
	return _fnv1a64(str, FNV1A_OFFSET_64);
}

#endif