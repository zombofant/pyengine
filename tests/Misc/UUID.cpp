#include <catch.hpp>

#include <CEngine/Misc/UUID.hpp>

using namespace PyEngine;

TEST_CASE("Misc/UUID/nil",
          "Test nil initialization of uuids")
{
    UUID test = UUID::with_generator<uuid_nil>();
    for (int i = 0; i < 16; i++) {
        CHECK(test.octets()[i] == 0);
    }

    CHECK(test.to_string() == std::string("00000000-0000-0000-0000-000000000000"));
    CHECK(test.is_nil());
}

TEST_CASE("Misc/UUID/copy_construction",
          "Test nil initialization of uuids")
{
    UUID test1 = UUID::with_generator<uuid_random>();
    UUID test2(test1);
    CHECK(test1 == test2);
}

TEST_CASE("Misc/UUID/copy_assignment",
          "Test nil initialization of uuids")
{
    UUID test1 = UUID::with_generator<uuid_random>();
    UUID test2;
    CHECK(test2 == UUID::with_generator<uuid_nil>());
    test2 = test1;
    CHECK(test1 == test2);
}

TEST_CASE("Misc/UUID/random",
          "Test random initialization of uuids")
{
    UUID test = UUID::with_generator<uuid_random>();
    CHECK(
        test.get_rfc4122_version() == UUID::RFC4122_VERSION_RANDOM);
    CHECK(
        test.get_rfc4122_variant() == UUID::RFC4122_ORIGINAL);
    CHECK(!test.is_nil());
}

TEST_CASE("Misc/UUID/constant",
          "Test constant initialization of uuids")
{
    UUID test = UUID(
        {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
         0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});

    CHECK(test.to_string() == std::string(
              "00112233-4455-6677-8899-aabbccddeeff"));
}

TEST_CASE("Misc/UUID/parse/valid",
          "Test parsing of valid UUIDs")
{

    UUID test;
    REQUIRE(
        parse_uuid("00112233-4455-6677-8899-aabbccddeeff", test));
    UUID ref = UUID(
        {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
         0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});

    CHECK(test == ref);
}

TEST_CASE("Misc/UUID/parse/valid_as_urn",
          "Test parsing of valid UUIDs in URN namespace")
{

    UUID test;
    REQUIRE(
        parse_uuid("urn:uuid:00112233-4455-6677-8899-aabbccddeeff", test));
    UUID ref = UUID(
        {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
         0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});

    CHECK(test == ref);
}

TEST_CASE("Misc/UUID/parse/invalid",
          "Test that parsing fails for invalid UUIDs")
{
    UUID test;
    CHECK(
        !parse_uuid("-00112233-4455-6677-8899-aabbccddeeff", test));
    CHECK(
        !parse_uuid("001122-334455-6677-8899-aabbccddeeff", test));
    CHECK(
        !parse_uuid("00112233-44-556677-8899-aabbccddeeff", test));
    CHECK(
        !parse_uuid("00112233-4455-6677-8899-zzbbccddeeff", test));
    CHECK(
        !parse_uuid("00112233-4455-6677-8899-abbccddeeff", test));
}
