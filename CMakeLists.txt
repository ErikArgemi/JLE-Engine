# CMakeList.txt : CMake project for GameEngineTemplate, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)          # request C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # require it
set(CMAKE_CXX_EXTENSIONS OFF)       # optional: disable compiler-specific extensions

project (GameEngineTemplate)

add_subdirectory(Engine)
add_subdirectory(Editor)

# Disable RTTI
#target_compile_options(${PROJECT_NAME} PRIVATE
#    $<$<CXX_COMPILER_ID:MSVC>:/GR->
#    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-rtti>
#)

# Disable Exceptions
#target_compile_options(${PROJECT_NAME} PRIVATE
#    $<$<CXX_COMPILER_ID:MSVC>:/EHs-c- /D_HAS_EXCEPTIONS=0>
#    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-exceptions>
#)


# Setting custom working directory within root's 'launch.vs.json and copying within .vs folder
configure_file(
    "${CMAKE_SOURCE_DIR}/launch.vs.json"
    "${CMAKE_SOURCE_DIR}/.vs/launch.vs.json"
    COPYONLY
)

# For generating the release, we need to copy assets folder to build folder
file(COPY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${CMAKE_BINARY_DIR})

#file(GLOB_RECURSE SOURCES
#    src/*.h
#    src/*.cpp
#    src/*.json
#)