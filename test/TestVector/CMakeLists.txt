set(SOURCES TestVector.h TestVector.cpp)

add_executable(TestVector ${SOURCES})

target_link_libraries(TestVector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/liblogger.dll.a)
target_link_libraries(TestVector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/libvector.dll.a)

set_target_properties(TestVector PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\bin
)

