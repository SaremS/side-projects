cmake_minimum_required(VERSION 3.4...3.18)

set(CMAKE_CXX_STANDARD 14 CACHE STRING "C++ version selection")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EIGENRAND_BUILD_TEST OFF CACHE BOOL "Disable EigenRand tests" FORCE)
set(EIGENRAND_BUILD_BENCHMARK OFF CACHE BOOL "Disable EigenRand benchmarks" FORCE)


project(stochastic_volatility_model
	VERSION 0.1
	DESCRIPTION "Stochastic volatility model with particle filter and finite difference filter"
	LANGUAGES CXX
)

include(FetchContent)
include_directories(include)
include_directories(external/EigenRand)
link_directories(external/EigenRand)

FetchContent_Declare(
  Eigen	
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen
  GIT_TAG        3.4.0
)

FetchContent_MakeAvailable(Eigen)


FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG        v2.13.6
)

FetchContent_MakeAvailable(pybind11)

pybind11_add_module(
  stochastic_volatility_model
  include/model/stochastic_volatility_model.h
  include/statistics/normal_distribution.h
  include/statistics/particles.h
  include/statistics/util_funs.h
  lib/model/stochastic_volatility_model.cpp
  lib/statistics/normal_distribution.cpp
  lib/statistics/particles.cpp
  lib/statistics/util_funs.cpp
)

target_link_libraries(stochastic_volatility_model PRIVATE Eigen3::Eigen)

target_compile_definitions(stochastic_volatility_model 
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

enable_testing()

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(
  unittest_normal_distribution
  include/statistics/normal_distribution.h
  lib/statistics/normal_distribution.cpp
  tests/unittest_normal_distribution.cpp
)

target_link_libraries(unittest_normal_distribution gtest_main Eigen3::Eigen)

add_executable(
  unittest_particles
  include/statistics/particles.h
  include/statistics/normal_distribution.h
  lib/statistics/normal_distribution.cpp
  lib/statistics/particles.cpp
  tests/unittest_particles.cpp
)

target_link_libraries(unittest_particles gtest_main pybind11::embed Eigen3::Eigen)

add_executable(
  unittest_utilfuns
  include/statistics/util_funs.h
  lib/statistics/util_funs.cpp
  tests/unittest_util_funs.cpp
)

target_link_libraries(unittest_utilfuns gtest_main)

add_executable(
  unittest_stochastic_volatility_model
  include/model/stochastic_volatility_model.h
  include/statistics/normal_distribution.h
  include/statistics/particles.h
  include/statistics/util_funs.h
  lib/model/stochastic_volatility_model.cpp
  lib/statistics/normal_distribution.cpp
  lib/statistics/particles.cpp
  lib/statistics/util_funs.cpp
  tests/unittest_stochastic_volatility_model.cpp
)

target_link_libraries(unittest_stochastic_volatility_model gtest_main pybind11::embed Eigen3::Eigen)

include(GoogleTest)
gtest_discover_tests(unittest_normal_distribution)
gtest_discover_tests(unittest_particles)
gtest_discover_tests(unittest_utilfuns)
gtest_discover_tests(unittest_stochastic_volatility_model)
