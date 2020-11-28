add_library(compact SHARED
        include/ICompact.h
        ICompact.cpp
        CompactImpl.cpp)

target_include_directories(compact PUBLIC include)

set_target_properties(compact PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
        )

target_link_libraries(compact PUBLIC logger)
target_link_libraries(compact PUBLIC vector)