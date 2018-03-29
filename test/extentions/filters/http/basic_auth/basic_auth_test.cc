#include "extensions/filters/http/basic_auth/basic_auth.h"

#include "test/mocks/http/mocks.h"
#include "test/test_common/utility.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

class BasicAuthFilterTest : public testing::Test {
protected:
  BasicAuthFilterTest() {
    filter_ = std::make_unique<BasicAuthFilter>();
    filter_->setDecoderFilterCallbacks(callbacks_);
  }

  testing::NiceMock<Http::MockStreamDecoderFilterCallbacks> callbacks_;
  std::unique_ptr<BasicAuthFilter> filter_;
};

TEST_F(BasicAuthFilterTest, UnauthorizedRequest) {
  Http::TestHeaderMapImpl response_headers{
      {":status", "401"}, {"content-length", "12"}, {"content-type", "text/plain"}};
  EXPECT_CALL(callbacks_, encodeHeaders_(HeaderMapEqualRef(&response_headers), false)).Times(1);
  EXPECT_CALL(callbacks_, encodeData(testing::_, true)).Times(1);

  Http::TestHeaderMapImpl request_headers{{":method", "get"}, {"okok", "okok"}};
  EXPECT_EQ(Http::FilterHeadersStatus::StopIteration,
            filter_->decodeHeaders(request_headers, false));
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy