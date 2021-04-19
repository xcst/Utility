add_library(util STATIC static_vector.h node_graph.cpp node_graph.h)

target_link_libraries(util PRIVATE
        project_options
        project_warnings
)
