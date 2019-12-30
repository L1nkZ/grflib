#include <iostream>
#include <string>

#include <grf/grf.h>
#include <gtest/gtest.h>

class GrfTest : public testing::Test {
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

TEST_F(GrfTest, GrfOpenApi) {
    {
        GrfError err{};
        EXPECT_EQ(nullptr,
                  ::grf_callback_open("non-existent", "r", &err, NULL));
#ifndef _WIN32
        // Supposed to return errno on unix platforms
        ASSERT_EQ(GE_ERRNO, err.type);
        EXPECT_EQ(ENOENT, errno);
#else
        // TODO(LinkZ)
#endif // _WIN32
    }
}

void EmptyGrfOpenTest(const std::string &grf_path) {
    GrfError err{};
    Grf *p_grf = grf_open(grf_path.c_str(), "rb", &err);
    ASSERT_NE(nullptr, p_grf);
    EXPECT_EQ(0, p_grf->nfiles);
    grf_close(p_grf);
}

void SmallGrfOpenTest(const std::string &grf_path) {
    GrfError err{};
    Grf *p_grf = grf_open(grf_path.c_str(), "rb", &err);
    ASSERT_NE(nullptr, p_grf);
    EXPECT_EQ(8, p_grf->nfiles);
    grf_close(p_grf);
}

TEST_F(GrfTest, GrfOpen102) {
    {
        const std::string empty102 = test_data_path_ + "/empty/102.grf";
        EmptyGrfOpenTest(empty102);
    }

    {
        const std::string small102 = test_data_path_ + "/small/102.grf";
        SmallGrfOpenTest(small102);
    }
}

TEST_F(GrfTest, GrfOpen103) {
    {
        const std::string empty103 = test_data_path_ + "/empty/103.grf";
        EmptyGrfOpenTest(empty103);
    }

    {
        const std::string small103 = test_data_path_ + "/small/103.grf";
        SmallGrfOpenTest(small103);
    }
}

TEST_F(GrfTest, GrfOpen200) {
    {
        const std::string empty200 = test_data_path_ + "/empty/200.grf";
        EmptyGrfOpenTest(empty200);
    }

    {
        const std::string small200 = test_data_path_ + "/small/200.grf";
        SmallGrfOpenTest(small200);
    }
}
