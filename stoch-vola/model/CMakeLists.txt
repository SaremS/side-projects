cmake_minimum_required(VERSION 3.4...3.18)

project(stochastic_volatility_model
	VERSION 0.1
	DESCRIPTION "Stochastic volatility model with particle filter and finite difference filter"
	LANGUAGES CXX
)

include_directories(include)
include(FetchContent)


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

target_link_libraries(unittest_normal_distribution gtest_main)

add_executable(
  unittest_particles
  include/statistics/particles.h
  include/statistics/normal_distribution.h
  lib/statistics/normal_distribution.cpp
  lib/statistics/particles.cpp
  tests/unittest_particles.cpp
)

target_link_libraries(unittest_particles gtest_main)

include(GoogleTest)
gtest_discover_tests(unittest_normal_distribution)
gtest_discover_tests(unittest_particles)
