#include <gtest/gtest.h>
#include <grf/grf.h>

TEST(GrfTest, GrfOpen) {
    GrfError err {};
    EXPECT_EQ(nullptr, ::grf_callback_open("non-existent", "r", &err, NULL));
#ifndef _WIN32
    // Supposed to return errno on unix platforms
    ASSERT_EQ(GE_ERRNO, err.type);
    EXPECT_EQ(ENOENT, errno);
#else
    // TODO(LinkZ)
#endif // _WIN32
}