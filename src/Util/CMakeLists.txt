add_library(util SHARED
        Export.h
        ReturnCode.h)

target_include_directories(util PUBLIC include)

set_target_properties(util PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
        )
