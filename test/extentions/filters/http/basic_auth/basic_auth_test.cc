#include "extensions/filters/http/basic_auth/basic_auth.h"

#include "test/mocks/http/mocks.h"
#include "test/test_common/utility.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

class BasicAuthFilterTest : public testing::Test {
protected:
  void SetUp() override {
    filter_.reset(new BasicAuthFilter());
    filter_->setDecoderFilterCallbacks(callbacks_);
  }

  testing::NiceMock<Http::MockStreamDecoderFilterCallbacks> callbacks_;
  std::unique_ptr<BasicAuthFilter> filter_;
};

TEST_F(BasicAuthFilterTest, UnauthorizedRequestWithoutAuthorizationHeader) {
  Http::TestHeaderMapImpl response_headers{
      {":status", "401"}, {"content-length", "12"}, {"content-type", "text/plain"}};
  EXPECT_CALL(callbacks_, encodeHeaders_(HeaderMapEqualRef(&response_headers), false)).Times(1);
  EXPECT_CALL(callbacks_, encodeData(testing::_, true)).Times(1);

  Http::TestHeaderMapImpl request_headers{{":method", "get"}};
  EXPECT_EQ(Http::FilterHeadersStatus::StopIteration,
            filter_->decodeHeaders(request_headers, false));
}

TEST_F(BasicAuthFilterTest, UnauthorizedRequestWithInvalidAuthorizationPrefix) {
  Http::TestHeaderMapImpl response_headers{
      {":status", "401"}, {"content-length", "12"}, {"content-type", "text/plain"}};
  EXPECT_CALL(callbacks_, encodeHeaders_(HeaderMapEqualRef(&response_headers), false)).Times(1);
  EXPECT_CALL(callbacks_, encodeData(testing::_, true)).Times(1);

  Http::TestHeaderMapImpl request_headers{{":method", "get"}, {"authorization", "Advance ZW52b3k6YXdlc29tZQ=="}};
  EXPECT_EQ(Http::FilterHeadersStatus::StopIteration,
            filter_->decodeHeaders(request_headers, false));
}

TEST_F(BasicAuthFilterTest, UnauthorizedRequestWithInvalidAuthorizationValue) {
  Http::TestHeaderMapImpl response_headers{
      {":status", "401"}, {"content-length", "12"}, {"content-type", "text/plain"}};
  EXPECT_CALL(callbacks_, encodeHeaders_(HeaderMapEqualRef(&response_headers), false)).Times(1);
  EXPECT_CALL(callbacks_, encodeData(testing::_, true)).Times(1);

  Http::TestHeaderMapImpl request_headers{{":method", "get"}, {"authorization", "Basic INVALID"}};
  EXPECT_EQ(Http::FilterHeadersStatus::StopIteration,
            filter_->decodeHeaders(request_headers, false));
}

TEST_F(BasicAuthFilterTest, AuthorizedRequest) {
  Http::TestHeaderMapImpl request_headers{{":method", "get"},
                                          // base64 of envoy:awesome is ZW52b3k6YXdlc29tZQ=
                                          {"authorization", "Basic ZW52b3k6YXdlc29tZQ=="}};
  EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(request_headers, false));
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy