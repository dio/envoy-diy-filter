#include "extensions/filters/http/basic_auth/basic_auth.h"
#include "test/test_common/utility.h"

#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

class BasicAuthFilterTest : public testing::Test {
protected:
  void SetUp() override { filter_.reset(new BasicAuthFilter()); }

  void doRequest(Http::TestHeaderMapImpl&& headers, bool end_stream) {
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, end_stream));
  }

  std::unique_ptr<BasicAuthFilter> filter_;
};

TEST_F(BasicAuthFilterTest, Request) { doRequest({{":method", "get"}}, true); }

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy