#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "extensions/filters/http/basic_auth/basic_auth.h"
#include "source/extensions/filters/http/basic_auth/config.pb.validate.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

static const std::string BASIC_AUTH_FILTER{"diy.basic_auth"};

class BasicAuthFilterConfigFactory : public Server::Configuration::NamedHttpFilterConfigFactory {
public:
  Server::Configuration::HttpFilterFactoryCb
  createFilterFactoryFromProto(const Protobuf::Message& proto, const std::string&,
                               Server::Configuration::FactoryContext&) override {

    const diy::BasicAuth& proto_config =
        Envoy::MessageUtil::downcastAndValidate<const diy::BasicAuth&>(proto);

    BasicAuthFilterConfigPtr config =
        std::make_shared<BasicAuthFilterConfig>(BasicAuthFilterConfig(proto_config));

    return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      callbacks.addStreamDecoderFilter(
          Http::StreamDecoderFilterSharedPtr{new BasicAuthFilter(config)});
    };
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new diy::BasicAuth()};
  }

  std::string name() override { return BASIC_AUTH_FILTER; }

  // Not implemented since this is only required deprecated v1 config.
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