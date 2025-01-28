# v3.21 implemented semantic changes regarding $<TARGET_OBJECTS:...>
# See https://cmake.org/cmake/help/v3.21/command/target_link_libraries.html#linking-object-libraries-via-target-objects
cmake_minimum_required(VERSION 3.21)

add_library(LwIP INTERFACE)
add_library(LwIP_usage INTERFACE)

target_include_directories(LwIP_usage INTERFACE
  src
)


target_link_libraries(LwIP_usage INTERFACE
  base_config
)

target_link_libraries(LwIP INTERFACE LwIP_usage)



add_library(LwIP_bin OBJECT EXCLUDE_FROM_ALL
  src/api/api_lib.c
  src/api/api_msg.c
  src/api/err.c
  src/api/if_api.c
  src/api/netbuf.c
  src/api/netdb.c
  src/api/netifapi.c
  src/api/sockets.c
  src/api/tcpip.c
  src/core/altcp.c
  src/core/altcp_alloc.c
  src/core/altcp_tcp.c
  src/core/def.c
  src/core/dns.c
  src/core/inet_chksum.c
  src/core/init.c
  src/core/ip.c
  src/core/ipv4/autoip.c
  src/core/ipv4/dhcp.c
  src/core/ipv4/etharp.c
  src/core/ipv4/icmp.c
  src/core/ipv4/igmp.c
  src/core/ipv4/ip4.c
  src/core/ipv4/ip4_addr.c
  src/core/ipv4/ip4_frag.c
  src/core/ipv6/dhcp6.c
  src/core/ipv6/ethip6.c
  src/core/ipv6/icmp6.c
  src/core/ipv6/inet6.c
  src/core/ipv6/ip6.c
  src/core/ipv6/ip6_addr.c
  src/core/ipv6/ip6_frag.c
  src/core/ipv6/mld6.c
  src/core/ipv6/nd6.c
  src/core/mem.c
  src/core/memp.c
  src/core/netif.c
  src/core/pbuf.c
  src/core/raw.c
  src/core/stats.c
  src/core/sys.c
  src/core/tcp.c
  src/core/tcp_in.c
  src/core/tcp_out.c
  src/core/timeouts.c
  src/core/udp.c
  src/netif/bridgeif.c
  src/netif/bridgeif_fdb.c
  src/netif/ethernet.c
  src/netif/lowpan6.c
  src/netif/lowpan6_ble.c
  src/netif/lowpan6_common.c
  src/netif/ppp/auth.c
  src/netif/ppp/ccp.c
  src/netif/ppp/chap-md5.c
  src/netif/ppp/chap-new.c
  src/netif/ppp/chap_ms.c
  src/netif/ppp/demand.c
  src/netif/ppp/eap.c
  src/netif/ppp/ecp.c
  src/netif/ppp/eui64.c
  src/netif/ppp/fsm.c
  src/netif/ppp/ipcp.c
  src/netif/ppp/ipv6cp.c
  src/netif/ppp/lcp.c
  src/netif/ppp/magic.c
  src/netif/ppp/mppe.c
  src/netif/ppp/multilink.c
  src/netif/ppp/polarssl/arc4.c
  src/netif/ppp/polarssl/des.c
  src/netif/ppp/polarssl/md4.c
  src/netif/ppp/polarssl/md5.c
  src/netif/ppp/polarssl/sha1.c
  src/netif/ppp/ppp.c
  src/netif/ppp/pppapi.c
  src/netif/ppp/pppcrypt.c
  src/netif/ppp/pppoe.c
  src/netif/ppp/pppol2tp.c
  src/netif/ppp/pppos.c
  src/netif/ppp/upap.c
  src/netif/ppp/utils.c
  src/netif/ppp/vj.c
  src/netif/slipif.c
  src/netif/zepif.c
  src/system/OS/sys_arch.c
)
target_link_libraries(LwIP_bin PUBLIC LwIP_usage)

target_link_libraries(LwIP INTERFACE
  LwIP_bin
  $<TARGET_OBJECTS:LwIP_bin>
)

