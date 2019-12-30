#include <iostream>
#include <string>

#include <grf/rgz.h>
#include <gtest/gtest.h>

class RgzTest : public testing::Test {
  protected:
    void SetUp() override {
        const char *p_env = std::getenv("TESTDIR");
        if (p_env == nullptr) {
            std::cerr << "TESTDIR has not been set." << std::endl;
            return;
        }

        test_data_path_ = p_env;
    }

    void TearDown() override {}

    std::string test_data_path_{};
};

TEST_F(RgzTest, RgzOpenApi) {
    {
        RgzError err{};
        EXPECT_EQ(nullptr, ::rgz_callback_open("non-existent", &err, NULL));
#ifndef _WIN32
        // Supposed to return errno on unix platforms
        ASSERT_EQ(GE_ERRNO, err.type);
        EXPECT_EQ(ENOENT, errno);
#else
        // TODO(LinkZ)
#endif // _WIN32
    }
}

TEST_F(RgzTest, RgzOpen) {
    {
        const std::string rgz_path =
            test_data_path_ + "/rgz/2011-9-9data_gm.rgz";
        RgzError err{};
        Rgz *p_rgz = rgz_open(rgz_path.c_str(), &err);
        ASSERT_NE(nullptr, p_rgz);
        EXPECT_EQ(GRF_TYPE_RGZ, p_rgz->type);
        // This format is not versioned
        EXPECT_EQ(0, p_rgz->version);
        EXPECT_EQ(2, p_rgz->nfiles);
        rgz_close(p_rgz);
    }

    {
        const std::string rgz_path =
            test_data_path_ + "/rgz/2011-10-24data_gm2.rgz";
        RgzError err{};
        Rgz *p_rgz = rgz_open(rgz_path.c_str(), &err);
        ASSERT_NE(nullptr, p_rgz);
        EXPECT_EQ(GRF_TYPE_RGZ, p_rgz->type);
        // This format is not versioned
        EXPECT_EQ(0, p_rgz->version);
        EXPECT_EQ(4, p_rgz->nfiles);
        rgz_close(p_rgz);
    }
}
