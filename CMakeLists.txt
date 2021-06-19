cmake_minimum_required(VERSION 3.6)

project(worldMap)

# Add preprocess macros
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Include directory
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/3rdParty/freeglut/include
    ${CMAKE_SOURCE_DIR}/mapengine/include)

# Header and source files
file(GLOB MAP_HEADERS
    "${CMAKE_SOURCE_DIR}/mapengine/include/*.h")

file(GLOB MAP_SRC
    "${CMAKE_SOURCE_DIR}/mapengine/src/*.c")

# Generate target
add_executable(${PROJECT_NAME} WIN32
    ${MAP_HEADERS}
    ${MAP_SRC})

# Link
target_link_libraries(${PROJECT_NAME} debug     ${PROJECT_SOURCE_DIR}/3rdParty/freeglut/lib/freeglut.lib)
target_link_libraries(${PROJECT_NAME} optimized ${PROJECT_SOURCE_DIR}/3rdParty/freeglut/lib/freeglut.lib)
find_package(OpenGL)
target_link_libraries(${PROJECT_NAME} debug     ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} optimized ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} debug     ${OPENGL_glu_LIBRARY})
target_link_libraries(${PROJECT_NAME} optimized ${OPENGL_glu_LIBRARY})
# target_link_libraries(${PROJECT_NAME} debug     XInput)
# target_link_libraries(${PROJECT_NAME} optimized XInput)

# Others
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

target_compile_options(${PROJECT_NAME} PRIVATE /Wall)

if (MSVC)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()
