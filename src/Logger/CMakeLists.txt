add_library(logger SHARED
        include/ILogger.h
        ILogger.cpp
        LoggerImpl.cpp)

target_include_directories(logger PUBLIC include)

set_target_properties(logger PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
        )