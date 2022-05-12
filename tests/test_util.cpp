#include "gtest/gtest.h"

// module
#include "util/log.hpp"

// std
#include "sstream"

using namespace wabby::util::log_severity;

class LoggerTest : public testing::Test
{
protected:
    
    // Sets up the test fixture.
    virtual void SetUp()
    {
        logger_.reset(new wabby::util::logger);
        std::unique_ptr<std::ostream> sink(new std::stringstream);
        sink_ = static_cast<std::ostringstream *>(&logger_->add_sink(std::move(sink)));
    }

    // Tears down the test fixture.
    virtual void TearDown()
    {}

    std::ostringstream *sink_;
    std::unique_ptr<wabby::util::logger> logger_;
};

TEST_F(LoggerTest, normal_output)
{
    auto &log = *logger_;
    WLOG(log, debug, "test {} {}", 1, 2);
    EXPECT_EQ(sink_->str(), "<debug> test 1 2\n");
}