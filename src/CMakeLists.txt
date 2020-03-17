project(Capstone VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Non entry point files
set(SOURCE_FILES
    midi/midi.cpp midi/midi.h
    midi/MidiDispatch.cpp midi/MidiDispatch.h
    config.h
)

function(build_executable name)
    add_executable(${name} ${ARGV1} ${SOURCE_FILES} "${name}.cpp")

    # Set properties
    set_property(TARGET ${name} PROPERTY
                 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Include Directories
    target_include_directories(${name} PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}")
    target_include_directories(${name} PUBLIC
                           "${CMAKE_CURRENT_BINARY_DIR}")
endfunction()

configure_file(config.h.in config.h)

build_executable(midi-test)
target_link_libraries(midi-test PRIVATE rtMidi)
target_link_libraries(midi-test PRIVATE timing)

build_executable(ui-test "WIN32")
target_link_libraries(ui-test PRIVATE rtMidi)
target_link_libraries(ui-test PRIVATE timing)

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(ui-test PRIVATE ${wxWidgets_LIBRARIES})
