/**********************************************************************
File name: UUID.cpp
This file is part of: Pythonic Engine

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about pyengine please e-mail one of the
authors named in the AUTHORS file.
**********************************************************************/
#include "UUID.hpp"

#include <random>
#include <cstring>
#include <cassert>

#include <endian.h>

namespace PyEngine {

static const UUIDOctetArray nil_uuid = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

std::random_device uint8_rng;
// std::independent_bits_engine<std::random_device, 8, uint8_t> uint8_rng(true_rng);

/* PyEngine::uuid_nil */

UUIDOctets uuid_nil::generate()
{
    UUIDOctets result;
    memset(&result.data[0], 0, 16);
    return result;
}

/* PyEngine::uuid_random */

UUIDOctets uuid_random::generate()
{
    UUIDOctets result;
    for (int i = 0; i < 16; i++) {
        result.data[i] = uint8_rng();
    }

    result.data[UUIDOctets::RFC4122_VERSION_OFFSET] = (result.data[UUIDOctets::RFC4122_VERSION_OFFSET] & (~UUIDOctets::RFC4122_VERSION_MASK)) | (4 << 4);
    result.data[UUIDOctets::RFC4122_VARIANT_OFFSET] = (result.data[UUIDOctets::RFC4122_VARIANT_OFFSET] & (~UUIDOctets::RFC4122_VARIANT_MASK)) | (0b101 << 5);

    return result;
}

/* PyEngine::UUID */

const uint16_t UUID::RFC4122_MASK_VERSION;
const uint16_t UUID::RFC4122_SHIFT_VERSION;

const uint8_t UUID::RFC4122_MASK_VARIANT;
const uint8_t UUID::RFC4122_SHIFT_VARIANT;

const uint16_t UUID::RFC4122_VERSION_TIME;
const uint16_t UUID::RFC4122_VERSION_POSIX_UIDS;
const uint16_t UUID::RFC4122_VERSION_NAME_MD5;
const uint16_t UUID::RFC4122_VERSION_RANDOM;
const uint16_t UUID::RFC4122_VERSION_NAME_SHA1;

UUID::UUID():
    data{uuid_nil::generate()}
{

}

UUID::UUID(const UUIDOctets &ref):
    data{ref}
{

}

UUID::UUID(const UUID &ref):
    data{ref.data._wrapped_octets}
{

}

UUID& UUID::operator=(const UUID &ref)
{
    memcpy(&data, &ref.data, sizeof(data));
    return *this;
}

UUID& UUID::operator=(const UUIDOctetArray &ref)
{
    memcpy(&data.octets, &ref, sizeof(UUIDOctetArray));
    return *this;
}

bool UUID::is_nil() const
{
    return memcmp(&data.octets[0], &nil_uuid[0], sizeof(UUIDOctetArray)) == 0;
}

std::string UUID::to_string() const
{
    constexpr int size = 16*2+4;
    std::string result(size, '\0');
    const int required_size = snprintf(
        &result.front(),
        size+1,
        "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        be32toh(data.rfc4122.time_low),
        be16toh(data.rfc4122.time_mid),
        be16toh(data.rfc4122.time_hi_and_version),
        data.rfc4122.clock_seq_hi_and_reserved,
        data.rfc4122.clock_seq_low,
        data.rfc4122.node[0],
        data.rfc4122.node[1],
        data.rfc4122.node[2],
        data.rfc4122.node[3],
        data.rfc4122.node[4],
        data.rfc4122.node[5]);

    assert(size == required_size);

    return result;
}

/* free functions */

bool parse_uuid(const std::string &str, UUID &dest)
{
    static const std::string urn_prefix = std::string("urn:uuid:");
    int offset = 0;
    if (str.substr(0, urn_prefix.size()) == urn_prefix) {
        offset = urn_prefix.size();
    }

    if (str.size() - offset != 16*2+4) {
        // cannot be the correct size
        return false;
    }

    // the casts should be fine -- afaik, the C++ standard allows
    // uint8_t for %x

    UUIDOctetArray octets;
    int scan_result = sscanf(
        str.c_str()+offset,
        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        (unsigned int*)&octets[0],
        (unsigned int*)&octets[1],
        (unsigned int*)&octets[2],
        (unsigned int*)&octets[3],
        (unsigned int*)&octets[4],
        (unsigned int*)&octets[5],
        (unsigned int*)&octets[6],
        (unsigned int*)&octets[7],
        (unsigned int*)&octets[8],
        (unsigned int*)&octets[9],
        (unsigned int*)&octets[10],
        (unsigned int*)&octets[11],
        (unsigned int*)&octets[12],
        (unsigned int*)&octets[13],
        (unsigned int*)&octets[14],
        (unsigned int*)&octets[15]);

    if (scan_result != 16) {
        return false;
    }

    dest = octets;

    return true;
}

bool operator==(const UUID &a, const UUID &b)
{
    return memcmp(a.octets(), b.octets(), sizeof(UUIDOctetArray)) == 0;
}

}
