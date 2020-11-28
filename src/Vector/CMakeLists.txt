add_library(vector SHARED
        include/IVector.h
        IVector.cpp
        VectorImpl.cpp)

target_include_directories(vector PUBLIC include)

set_target_properties(vector PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
        )

target_link_libraries(vector PUBLIC logger)