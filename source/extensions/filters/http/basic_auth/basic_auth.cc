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
static const std::string UNAUTHORIZED{"unauthorized"};

Http::FilterHeadersStatus BasicAuthFilter::decodeHeaders(Http::HeaderMap& headers, bool) {
  if (!authenticated(headers)) {
    Http::Utility::sendLocalReply(*decoder_callbacks_, false, Http::Code::Unauthorized,
                                  UNAUTHORIZED);
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

  // TODO(dio): Put username:password in configuration, once we have it.
  absl::string_view computed(Base64::encode("envoy:awesome", 13));
  return encoded.size() == computed.size() && absl::StartsWith(encoded, computed);
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy