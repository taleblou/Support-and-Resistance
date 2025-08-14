cmake_minimum_required(VERSION 3.16)
project(srlevels VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(srlevels
    src/ohlcv.cpp
    src/horizontal.cpp
    src/fibonacci.cpp
    src/psychological.cpp
    src/trendlines.cpp
)

target_include_directories(srlevels PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(srlevels::srlevels ALIAS srlevels)

add_executable(srlines src/cli.cpp)
target_link_libraries(srlines PRIVATE srlevels)

# Tests
enable_testing()
add_executable(sr_tests tests/tests.cpp)
target_link_libraries(sr_tests PRIVATE srlevels)
add_test(NAME sr_tests COMMAND sr_tests)

# Install
include(GNUInstallDirs)
install(TARGETS srlevels srlines
    EXPORT srlevelsTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT srlevelsTargets
    FILE srlevelsTargets.cmake
    NAMESPACE srlevels::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/srlevels
)

# Config files
include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/srlevelsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/srlevelsConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/srlevels
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/srlevelsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/srlevelsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/srlevelsConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/srlevels
)

# pkg-config
configure_file(pkgconfig/srlevels.pc.in srlevels.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/srlevels.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
