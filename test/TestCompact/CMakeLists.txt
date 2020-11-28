set(SOURCES TestCompact.h TestCompact.cpp)

add_executable(TestCompact ${SOURCES})

target_link_libraries(TestCompact PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/liblogger.dll.a)
target_link_libraries(TestCompact PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/libvector.dll.a)
target_link_libraries(TestCompact PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/libcompact.dll.a)

set_target_properties(TestCompact PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\bin
        )