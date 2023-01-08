add_library(
        barrier_resilience STATIC
        with_graph_construction/graph.cpp
        with_graph_construction/ford_fulkerson.cpp
        with_graph_construction/even_tarjan.cpp)

target_include_directories(barrier_resilience PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})