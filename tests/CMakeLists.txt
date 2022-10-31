enable_testing()

add_executable(
        tests
        test_geometry_objects.cpp
        test_graph.cpp)
target_link_libraries(
        tests
        GTest::gtest_main
        barrier_resilience
)

include(GoogleTest)
gtest_discover_tests(tests)
