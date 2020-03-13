cmake_minimum_required(VERSION 3.10)
project(Capstone VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Capstone main.cpp)
add_library(rtmidi-4.0.0 STATIC
            "${PROJECT_SOURCE_DIR}/rtmidi-4.0.0/RtMidi.cpp"
            "${PROJECT_SOURCE_DIR}/rtmidi-4.0.0/RtMidi.h")
if(WIN32)
target_compile_definitions(rtmidi-4.0.0 PUBLIC __WINDOWS_MM__)
target_link_libraries(rtmidi-4.0.0 odbc32.lib odbccp32.lib winmm.lib)
endif()

configure_file(config.h.in config.h)

target_include_directories(Capstone PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/rtmidi-4.0.0")
target_link_libraries(Capstone rtmidi-4.0.0)