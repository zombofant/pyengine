#ifndef _PYE_MISC_UUID_H

#include <cstdint>
#include <stdexcept>

namespace PyEngine {

typedef uint8_t UUIDOctetArray[16];

struct __attribute__((packed)) UUIDOctets
{
    static constexpr size_t RFC4122_VERSION_OFFSET = 7;
    static constexpr size_t RFC4122_VARIANT_OFFSET = 8;

    static constexpr size_t RFC4122_VERSION_MASK = 0xF0;
    static constexpr size_t RFC4122_VARIANT_MASK = 0xE0;

    UUIDOctetArray data;
};

struct __attribute__((packed)) UUIDRFC4122
{
    uint32_t time_low;
    uint16_t time_mid;
    uint16_t time_hi_and_version;
    uint8_t clock_seq_hi_and_reserved;
    uint8_t clock_seq_low;
    uint8_t node[6];
};

struct uuid_nil
{
    uuid_nil() = delete;

    static UUIDOctets generate();
};

struct uuid_random
{
    uuid_random() = delete;

    static UUIDOctets generate();
};

struct UUID
{
public:
    static const uint16_t RFC4122_MASK_VERSION = 0xF000;
    static const uint16_t RFC4122_SHIFT_VERSION = 12;

    static const uint8_t RFC4122_MASK_VARIANT = 0xE0;
    static const uint8_t RFC4122_SHIFT_VARIANT = 5;

    static const uint16_t RFC4122_VERSION_TIME = 1;
    static const uint16_t RFC4122_VERSION_POSIX_UIDS = 2;
    static const uint16_t RFC4122_VERSION_NAME_MD5 = 3;
    static const uint16_t RFC4122_VERSION_RANDOM = 4;
    static const uint16_t RFC4122_VERSION_NAME_SHA1 = 5;

    enum RFC4122Variant {
        RESERVED_MCS,
        RFC4122_ORIGINAL,
        RESERVED_MICROSOFT_CORP,
        RESERVED_FUTURE
    };

public:
    UUID();
    explicit UUID(const UUIDOctets &ref);
    UUID(const UUID &ref);
    UUID& operator=(const UUID &ref);
    UUID& operator=(const UUIDOctetArray &ref);

private:
    union {
        UUIDOctets _wrapped_octets;
        uint8_t octets[16];
        UUIDRFC4122 rfc4122;
    } data;

public:
    inline const UUIDOctetArray& octets() const
    {
        return data.octets;
    }

    inline const UUIDRFC4122& rfc4122() const
    {
        return data.rfc4122;
    }

    template <typename generator_t>
    inline static UUID with_generator()
    {
        return UUID(generator_t::generate());
    }

    inline uint16_t get_rfc4122_version()
    {
        return (data.rfc4122.time_hi_and_version & RFC4122_MASK_VERSION) >> RFC4122_SHIFT_VERSION;
    }

    inline RFC4122Variant get_rfc4122_variant()
    {
        switch ((data.rfc4122.clock_seq_hi_and_reserved & RFC4122_MASK_VARIANT) >> RFC4122_SHIFT_VARIANT)
        {
        case 0b000:
        case 0b001:
        case 0b010:
        case 0b011:
        {
            return RESERVED_MCS;
        }
        case 0b100:
        case 0b101:
        {
            return RFC4122_ORIGINAL;
        }
        case 0b110:
        {
            return RESERVED_MICROSOFT_CORP;
        }
        case 0b111:
        {
            return RESERVED_FUTURE;
        }
        default:
        {
            throw std::logic_error("cannot happen");
        }
        }
    }

    std::string to_string();
};

bool parse_uuid(const std::string &str, UUID &dest);

bool operator==(const UUID &a, const UUID &b);

inline bool operator!=(const UUID &a, const UUID &b)
{
    return !(a == b);
}

}

#endif
