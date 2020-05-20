cmake_minimum_required(VERSION 3.10)
include(FetchContent)
include(CMakePackageConfigHelpers)
set(CMAKE_CXX_STANDARD 17)

project(sico VERSION 1.0.0)

option(SICO_USE_EIGEN "Use the Eigen library for linear algebra" OFF)
option(SICO_USE_HOLTHAUS_UNITS "Use Holthaus Units library" OFF)
option(SICO_USE_BOOST_UNITS "Use Boost Units library" OFF)
option(SICO_BUILD_TESTS "Build tests" ON)
option(SICO_TEST_COVERAGE "Enable test coverage" ON)

if (SICO_USE_HOLTHAUS_UNITS AND SICO_USE_BOOST_UNITS)
  message(FATAL_ERROR "Incompatible options SICO_USE_HOLTHAUS_UNITS and SICO_USE_BOOST_UNITS")
endif()

if(MSVC)
  set(SICO_COMP_OPTS /Wall /WX /wd4514)#4514: deleted inline functions
else()
  set(SICO_COMP_OPTS -Wall -Wextra -pedantic -Werror -frounding-math)
endif()

add_library(sico STATIC 
  include/sico/sico.hpp
  src/sico.cpp

  include/sico/types/linear_algebra.hpp

  include/sico/types/ecef.hpp
  src/types/ecef.cpp
  include/sico/types/enu.hpp
  src/types/enu.cpp
  include/sico/types/lla.hpp
  src/types/lla.cpp
  include/sico/types/local.hpp
  src/types/local.cpp
  include/sico/types/ned.hpp
  include/sico/types/orientations.hpp
  src/types/orientations.cpp

  include/sico/conversions/ecef_lla.hpp
  src/conversions/ecef_lla.cpp
  include/sico/conversions/enu_local.hpp
  src/conversions/enu_local.cpp
  include/sico/conversions/geo_tangent.hpp
  src/conversions/geo_tangent.cpp
  include/sico/conversions/lla_mercator.hpp
  src/conversions/lla_mercator.cpp
  include/sico/conversions/lla_utm.hpp
  src/conversions/lla_utm.cpp
  include/sico/conversions/local_local.hpp
  src/conversions/local_local.cpp

  include/sico/frames/local_tangent.hpp
  src/frames/local_tangent.cpp
  include/sico/frames/mercator.hpp
  src/frames/mercator.cpp
  include/sico/frames/object_local.hpp
  src/frames/object_local.cpp
)

if (SICO_USE_HOLTHAUS_UNITS)
  FetchContent_Declare(
    Units
    GIT_REPOSITORY https://github.com/nholthaus/units.git
    GIT_TAG        v3.0.0.alpha-2
  )
  FetchContent_MakeAvailable(Units)
  set(Units_DIR ${Units_SOURCE_DIR})
  target_compile_definitions(sico PUBLIC SICO_USE_HOLTHAUS_UNITS)
endif()

if (SICO_USE_EIGEN)
  set(Eigen_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/eigen-3.3.7)
  target_compile_definitions(sico PUBLIC SICO_USE_EIGEN)
endif()

if (SICO_USE_BOOST_UNITS)
  find_package(Boost REQUIRED)
  target_compile_definitions(sico PUBLIC SICO_USE_BOOST_UNITS)
endif()

target_include_directories(sico
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${Units_DIR}
    ${Eigen_DIR}
    ${Boost_INCLUDE_DIRS}
)
target_compile_options(sico PRIVATE ${SICO_COM_OPTS})

install(TARGETS sico DESTINATION lib EXPORT sico-config)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION ".")
install(DIRECTORY ${Units_DIR}/units DESTINATION "include/units")
export(TARGETS sico FILE sico-config.cmake)
install(EXPORT sico-config DESTINATION cmake)
write_basic_package_version_file(sico-config-version.cmake COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sico-config-version.cmake DESTINATION cmake)

add_executable(sico-examples examples/examples.cpp)
target_link_libraries(sico-examples sico)
target_include_directories(sico-examples PUBLIC ${Units_DIR} ${Eigen_DIR} ${Boost_INCLUDE_DIRS})

if (SICO_BUILD_TESTS)
  
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.12.1
  )
  FetchContent_MakeAvailable(Catch2)
  set(Catch2_DIR ${Catch2_SOURCE_DIR})

  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Catch2_DIR}/contrib)

  set(SICO_TEST_FILES
      tests/main.cpp
      tests/conversions.ecef_lla.t.cpp
      tests/conversions.enu_local.t.cpp
      tests/conversions.geo_tangent.t.cpp
      tests/conversions.lla_mercator.t.cpp
      tests/conversions.lla_utm.t.cpp
      tests/conversions.local_local.t.cpp
      tests/frames.local_tangent.t.cpp
      tests/frames.mercator.t.cpp
      tests/frames.object_local.t.cpp
      tests/types.angles.t.cpp
      tests/types.linear_algebra.t.cpp
      tests/types.operators.t.cpp
      tests/sico.c.t.cpp
  )
  add_executable(sicotests ${SICO_TEST_FILES})
  add_dependencies(sicotests Catch2)
  target_link_libraries(sicotests sico)
  target_include_directories(sicotests PUBLIC
   ${Units_DIR}
   ${Eigen_DIR}
   ${Boost_INCLUDE_DIRS}
   ${Catch2_DIR}/single_include
  )
  target_compile_options(sicotests PUBLIC ${SICO_COM_OPTS})
  if(SICO_TEST_COVERAGE)
    target_compile_options(sicotests PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(sicotests PUBLIC -fprofile-arcs -ftest-coverage)
  endif()

  include(CTest)
  include(Catch)
  include(ParseAndAddCatchTests)
  ParseAndAddCatchTests(sicotests)

endif()