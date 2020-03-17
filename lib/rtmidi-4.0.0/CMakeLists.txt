add_library(
    rtMidi
    rtMidi.h
    rtMidi.cpp
)

target_include_directories(
    rtMidi PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(WIN32)
    target_compile_definitions(rtMidi PUBLIC __WINDOWS_MM__)
    target_link_libraries(rtMidi odbc32.lib odbccp32.lib winmm.lib)
endif()

set_property(TARGET rtMidi PROPERTY
             MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")