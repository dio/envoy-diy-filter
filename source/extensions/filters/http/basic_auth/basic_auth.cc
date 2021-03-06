#include "extensions/filters/http/basic_auth/basic_auth.h"

#include "common/common/base64.h"
#include "common/common/utility.h"
#include "common/http/utility.h"

#include "absl/strings/match.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

static const std::string PREFIX{"Basic "};
static const Http::LowerCaseString WWW_AUTHENTICATE{"WWW-Authenticate"};

Http::FilterHeadersStatus BasicAuthFilter::decodeHeaders(Http::HeaderMap& headers, bool) {
  if (!authenticated(headers)) {
    Http::Utility::sendLocalReply(
        [&](Http::HeaderMapPtr&& headers, bool end_stream) -> void {
          headers->addReferenceKey(WWW_AUTHENTICATE, config_->realm_);
          decoder_callbacks_->encodeHeaders(std::move(headers), end_stream);
        },
        [&](Buffer::Instance& data, bool end_stream) -> void {
          decoder_callbacks_->encodeData(data, end_stream);
        },
        false, Http::Code::Unauthorized, UNAUTHORIZED);
    return Http::FilterHeadersStatus::StopIteration;
  }
  return Http::FilterHeadersStatus::Continue;
}

bool BasicAuthFilter::authenticated(const Http::HeaderMap& headers) {
  if (!headers.Authorization()) {
    return false;
  }

  absl::string_view value(headers.Authorization()->value().c_str());
  if (!absl::StartsWith(value, PREFIX)) {
    return false;
  }

  absl::string_view encoded(value.substr(PREFIX.size(), value.size() - PREFIX.size()));
  return config_->encoded_.size() == encoded.size() && absl::StartsWith(config_->encoded_, encoded);
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy