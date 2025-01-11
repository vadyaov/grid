add_executable(network
    network.cc
    test_network.cc
)

target_include_directories(network PRIVATE ${CMAKE_SOURCE_DIR}/eigen-3.4.0)