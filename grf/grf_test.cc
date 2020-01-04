#include <iostream>
#include <string>
#include <vector>

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

// This test is supposed to crash/throw in case of failure
TEST_F(GrfTest, GrfOpenRegressionCheck) {
    const std::vector<std::string> regs = {"reg001.grf", "reg002.grf",
                                           "reg003.grf", "reg004.grf"};
    for (const auto &reg : regs) {
        GrfError err{};
        auto p_grf = ::grf_callback_open(
            (test_data_path_ + "/grf/" + reg).c_str(), "r", &err, nullptr);
        if (p_grf != nullptr) {
            ::grf_close(p_grf);
        }
    }
}

void EmptyGrfOpenTest(const std::string &grf_path, uint32_t expected_version) {
    GrfError err{};
    Grf *p_grf = ::grf_open(grf_path.c_str(), "rb", &err);
    ASSERT_NE(nullptr, p_grf);
    EXPECT_EQ(GRF_TYPE_GRF, p_grf->type);
    EXPECT_EQ(expected_version, p_grf->version);
    EXPECT_EQ(0, p_grf->nfiles);
    ::grf_close(p_grf);
}

void SmallGrfOpenTest(const std::string &grf_path, uint32_t expected_version) {
    const std::map<std::string, size_t> expected_content{
        {"data\\06guild_r.gat", 800014},
        {"data\\06guild_r.gnd", 454622},
        {"data\\06guild_r.rsw", 69798},
        {"data\\sprite\\\xB8\xF3\xBD\xBA\xC5\xCD\\high_orc.act", 491076},
        {"data\\sprite\\\xB8\xF3\xBD\xBA\xC5\xCD\\high_orc.spr", 250592},
        {"data\\texture\\chdesk-side1.bmp", 33844},
        {"data\\texture\\chdesk-side2.bmp", 33844},
        {"data\\texture\\chdesk-side3.bmp", 17460}};

    GrfError err{};
    Grf *p_grf = ::grf_open(grf_path.c_str(), "rb", &err);
    ASSERT_NE(nullptr, p_grf);
    EXPECT_EQ(GRF_TYPE_GRF, p_grf->type);
    EXPECT_EQ(expected_version, p_grf->version);
    ASSERT_EQ(expected_content.size(), p_grf->nfiles);
    // Check that files are present and of the correct size
    for (uint32_t i = 0; i < p_grf->nfiles; i++) {
        const auto elem = expected_content.find(p_grf->files[i].name);
        ASSERT_NE(std::cend(expected_content), elem);
        EXPECT_EQ(elem->second, p_grf->files[i].real_len);
    }

    ::grf_close(p_grf);
}

TEST_F(GrfTest, GrfOpen102) {
    {
        const std::string empty102 = test_data_path_ + "/grf/102-empty.grf";
        EmptyGrfOpenTest(empty102, 0x102);
    }

    {
        const std::string small102 = test_data_path_ + "/grf/102-small.grf";
        SmallGrfOpenTest(small102, 0x102);
    }
}

TEST_F(GrfTest, GrfOpen103) {
    {
        const std::string empty103 = test_data_path_ + "/grf/103-empty.grf";
        EmptyGrfOpenTest(empty103, 0x103);
    }

    {
        const std::string small103 = test_data_path_ + "/grf/103-small.grf";
        SmallGrfOpenTest(small103, 0x103);
    }
}

TEST_F(GrfTest, GrfOpen200) {
    {
        const std::string empty200 = test_data_path_ + "/grf/200-empty.grf";
        EmptyGrfOpenTest(empty200, 0x200);
    }

    {
        const std::string small200 = test_data_path_ + "/grf/200-small.grf";
        SmallGrfOpenTest(small200, 0x200);
    }
}

TEST_F(GrfTest, GpfOpen) {
    {
        const std::map<std::string, size_t> expected_content{
            {"data\\lua files", GRFFILE_DIR_SZORIG},           // Directory
            {"data\\lua files\\datainfo", GRFFILE_DIR_SZORIG}, // Directory
            {"data\\lua files\\datainfo\\accessoryid.lub", 43611},
            {"data\\lua files\\datainfo\\accname.lub", 61867}};

        const std::string gpf_path = test_data_path_ + "/gpf/102-small.gpf";
        GrfError err{};
        Grf *p_grf = ::grf_open(gpf_path.c_str(), "rb", &err);
        ASSERT_NE(nullptr, p_grf);
        // GPF is actually the same file format as GRF for this version
        EXPECT_EQ(GRF_TYPE_GRF, p_grf->type);
        EXPECT_EQ(expected_content.size(), p_grf->nfiles);
        // Check that files are present and of the correct size
        for (uint32_t i = 0; i < p_grf->nfiles; i++) {
            const auto elem = expected_content.find(p_grf->files[i].name);
            ASSERT_NE(std::cend(expected_content), elem);
            EXPECT_EQ(elem->second, p_grf->files[i].real_len);
        }

        ::grf_close(p_grf);
    }

    {
        const std::string gpf_path = test_data_path_ + "/gpf/103-small.gpf";
        GrfError err{};
        Grf *p_grf = ::grf_open(gpf_path.c_str(), "rb", &err);
        ASSERT_NE(nullptr, p_grf);
        // GPF is actually the same file format as GRF for this version
        EXPECT_EQ(GRF_TYPE_GRF, p_grf->type);
        EXPECT_EQ(89, p_grf->nfiles);
        ::grf_close(p_grf);
    }
}

TEST_F(GrfTest, GrfGetFile) {
    const std::string grf_path = test_data_path_ + "/grf/200-small.grf";
    GrfError err{};
    Grf *p_grf = ::grf_open(grf_path.c_str(), "rb", &err);
    ASSERT_NE(nullptr, p_grf);

    {
        // Get file by index
        constexpr uint32_t id = 3; // <...>\high_orc.act
        size_t data_size = 0;
        void *p_data = ::grf_index_get(p_grf, id, &data_size, &err);
        EXPECT_NE(nullptr, p_data);
        EXPECT_EQ(491076, data_size);
    }

    {
        // Get file by name
        const char *p_file_name = "data\\06guild_r.gnd";
        size_t data_size = 0;
        void *p_data = ::grf_get(p_grf, p_file_name, &data_size, &err);
        EXPECT_NE(nullptr, p_data);
        EXPECT_EQ(454622, data_size);
    }

    ::grf_close(p_grf);
}
