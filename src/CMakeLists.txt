find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

add_library(
        barrier_resilience STATIC
        with_graph_construction/graph.cpp
        with_graph_construction/ford_fulkerson.cpp
        with_graph_construction/even_tarjan.cpp
        barrier_resilience/find_levels.cpp
        barrier_resilience/barrier_resilience.cpp
        barrier_resilience/blocking_family.cpp)

target_include_directories(barrier_resilience PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})
