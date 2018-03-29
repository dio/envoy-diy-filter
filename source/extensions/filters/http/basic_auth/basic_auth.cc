#include "extensions/filters/http/basic_auth/basic_auth.h"

#include "common/http/utility.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

Http::FilterHeadersStatus BasicAuthFilter::decodeHeaders(Http::HeaderMap&, bool) {
  return Http::FilterHeadersStatus::StopIteration;
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy