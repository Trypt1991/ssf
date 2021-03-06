#ifndef SSF_CORE_NETWORK_PROTOCOL_H_
#define SSF_CORE_NETWORK_PROTOCOL_H_

#include <list>
#include <string>

#include <ssf/layer/data_link/circuit_helpers.h>
#include <ssf/layer/data_link/basic_circuit_protocol.h>
#include <ssf/layer/data_link/simple_circuit_policy.h>
#include <ssf/layer/parameters.h>
#include <ssf/layer/physical/tcp.h>
#include <ssf/layer/physical/tlsotcp.h>
#include <ssf/layer/proxy/basic_proxy_protocol.h>

#include "core/circuit/config.h"

namespace ssf {

namespace config {
class Config;
}  // config

namespace network {

class NetworkProtocol {
 public:
  using Query = ssf::layer::ParameterStack;

  using ProxyTCPProtocol =
      ssf::layer::proxy::basic_ProxyProtocol<ssf::layer::physical::tcp>;

  template <class Layer>
  using TLSboLayer = ssf::layer::cryptography::basic_CryptoStreamProtocol<
      Layer, ssf::layer::cryptography::buffered_tls>;

  using TLSPhysicalProtocol = TLSboLayer<ProxyTCPProtocol>;

  using CircuitTLSProtocol = ssf::layer::data_link::basic_CircuitProtocol<
      TLSPhysicalProtocol, ssf::layer::data_link::CircuitPolicy>;
  using TLSoCircuitTLSProtocol = TLSboLayer<CircuitTLSProtocol>;

  using CircuitProtocol = ssf::layer::data_link::basic_CircuitProtocol<
      ProxyTCPProtocol, ssf::layer::data_link::CircuitPolicy>;

  using PlainProtocol = CircuitProtocol;
  using TLSProtocol = CircuitTLSProtocol;
  using FullTLSProtocol = TLSoCircuitTLSProtocol;

#ifdef TLS_OVER_TCP_LINK
  using Protocol = FullTLSProtocol;
#elif TCP_ONLY_LINK
  using Protocol = PlainProtocol;
#endif

 public:
  static Query GenerateClientQuery(const std::string& remote_addr,
                                   const std::string& remote_port,
                                   const ssf::config::Config& ssf_config,
                                   const ssf::circuit::NodeList& circuit_nodes);

  static Query GenerateServerQuery(const std::string& remote_addr,
                                   const std::string& remote_port,
                                   const ssf::config::Config& ssf_config);

  static Query GenerateClientTCPQuery(
      const std::string& remote_addr, const std::string& remote_port,
      const ssf::config::Config& ssf_config,
      const ssf::circuit::NodeList& circuit_nodes);

  static Query GenerateClientTLSQuery(
      const std::string& remote_addr, const std::string& remote_port,
      const ssf::config::Config& ssf_config,
      const ssf::circuit::NodeList& circuit_nodes);

  static Query GenerateServerTCPQuery(const std::string& remote_addr,
                                      const std::string& remote_port,
                                      const ssf::config::Config& ssf_config);

  static Query GenerateServerTLSQuery(const std::string& remote_addr,
                                      const std::string& remote_port,
                                      const ssf::config::Config& ssf_config);

  static ssf::layer::LayerParameters TlsConfigToLayerParameters(
      const ssf::config::Config& ssf_config);

  static ssf::layer::LayerParameters ProxyConfigToLayerParameters(
      const ssf::config::Config& ssf_config, bool acceptor_endpoint);
};

}  // network
}  // ssf

#endif  // SSF_CORE_NETWORK_PROTOCOL_H_
