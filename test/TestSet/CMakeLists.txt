set(SOURCES TestSet.h TestSet.cpp)

add_executable(TestSet ${SOURCES})

target_link_libraries(TestSet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/liblogger.dll.a)
target_link_libraries(TestSet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/libvector.dll.a)
target_link_libraries(TestSet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/libset.dll.a)

set_target_properties(TestSet PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\bin
        )