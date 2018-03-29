#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "extensions/filters/http/basic_auth/basic_auth.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {
class BasicAuthFilterConfigFactory : public Server::Configuration::NamedHttpFilterConfigFactory {
public:
  Server::Configuration::HttpFilterFactoryCb
  createFilterFactoryFromProto(const Protobuf::Message&, const std::string&,
                               Server::Configuration::FactoryContext&) override {
    return [](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      callbacks.addStreamDecoderFilter(Http::StreamDecoderFilterSharedPtr{new BasicAuthFilter()});
    };
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new Envoy::ProtobufWkt::Empty()};
  }

  std::string name() override { return "basic_auth"; }

  // Not implemented since this is only required by the deprecated v1 config.
  Server::Configuration::HttpFilterFactoryCb
  createFilterFactory(const Json::Object&, const std::string&,
                      Server::Configuration::FactoryContext&) override {
    NOT_IMPLEMENTED;
  }
};

static Registry::RegisterFactory<BasicAuthFilterConfigFactory,
                                 Server::Configuration::NamedHttpFilterConfigFactory>
    registered_;

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy