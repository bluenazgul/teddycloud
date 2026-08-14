#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "tb2_ruid.h"

static void test_canonicalize(void)
{
    char canonical[TB2_RUID_SIZE];

    assert(tb2_ruid_canonicalize("a1b2c3d4e5f60789", canonical));
    assert(strcmp(canonical, "A1B2C3D4E5F60789") == 0);
    assert(tb2_ruid_canonicalize("00000AF012345678", canonical));
    assert(strcmp(canonical, "00000AF012345678") == 0);

    assert(!tb2_ruid_canonicalize(NULL, canonical));
    assert(!tb2_ruid_canonicalize("0123456789ABCDE", canonical));
    assert(!tb2_ruid_canonicalize("0123456789ABCDEFG", canonical));
    assert(!tb2_ruid_canonicalize("0123456789ABCDEG", canonical));
    assert(!tb2_ruid_canonicalize("0123456789ABCDEF", NULL));
}

static void test_uid_conversion(void)
{
    char canonical[TB2_RUID_SIZE];
    uint64_t uid;

    assert(tb2_ruid_to_uid("0123456789abcdef", &uid));
    assert(uid == UINT64_C(0xEFCDAB8967452301));

    tb2_ruid_from_uid(uid, canonical);
    assert(strcmp(canonical, "0123456789ABCDEF") == 0);

    assert(!tb2_ruid_to_uid("not-a-valid-ruid", &uid));
    assert(!tb2_ruid_to_uid("0123456789ABCDEF", NULL));
    tb2_ruid_from_uid(uid, NULL);
}

static void test_classification(void)
{
    assert(tb2_ruid_classify("00000af012345678") == TB2_RUID_SYSTEM);
    assert(tb2_ruid_classify("00000AF0FFFFFFFF") == TB2_RUID_SYSTEM);
    assert(tb2_ruid_classify("00000AF112345678") == TB2_RUID_CONTENT);
    assert(tb2_ruid_classify("A0000AF012345678") == TB2_RUID_CONTENT);
    assert(tb2_ruid_classify("invalid") == TB2_RUID_INVALID);
    assert(tb2_ruid_classify(NULL) == TB2_RUID_INVALID);
}

int main(void)
{
    test_canonicalize();
    test_uid_conversion();
    test_classification();
    return 0;
}
