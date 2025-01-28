# v3.21 implemented semantic changes regarding $<TARGET_OBJECTS:...>
# See https://cmake.org/cmake/help/v3.21/command/target_link_libraries.html#linking-object-libraries-via-target-objects
cmake_minimum_required(VERSION 3.21)

add_library(STM32Ethernet INTERFACE)
add_library(STM32Ethernet_usage INTERFACE)

target_include_directories(STM32Ethernet_usage INTERFACE
  src
)


target_link_libraries(STM32Ethernet_usage INTERFACE
  base_config
)

target_link_libraries(STM32Ethernet INTERFACE STM32Ethernet_usage)



add_library(STM32Ethernet_bin OBJECT EXCLUDE_FROM_ALL
  src/Dhcp.cpp
  src/Dns.cpp
  src/EthernetClient.cpp
  src/EthernetServer.cpp
  src/EthernetUdp.cpp
  src/STM32Ethernet.cpp
  src/utility/ethernetif.cpp
  src/utility/stm32_eth.cpp
)
target_link_libraries(STM32Ethernet_bin PUBLIC STM32Ethernet_usage)

target_link_libraries(STM32Ethernet INTERFACE
  STM32Ethernet_bin
  $<TARGET_OBJECTS:STM32Ethernet_bin>
)

