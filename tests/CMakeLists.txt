include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(math_test math_test.cpp)
target_link_libraries(math_test GTest::gtest_main)
target_compile_features(math_test PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(math_test)
