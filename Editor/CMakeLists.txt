file(GLOB_RECURSE EDITOR_CODE CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(Editor ${EDITOR_CODE})

target_compile_options(Editor PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/GR->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-rtti>
)

target_compile_options(Editor PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/EHs-c- /D_HAS_EXCEPTIONS=0>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-exceptions>
)

target_include_directories(Editor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(Editor PRIVATE Engine)