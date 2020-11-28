add_library(set SHARED
        include/ISet.h
        ISet.cpp
        SetImpl.cpp)

target_include_directories(set PUBLIC include)

set_target_properties(set PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
        )

target_link_libraries(set PUBLIC logger)
target_link_libraries(set PUBLIC vector)