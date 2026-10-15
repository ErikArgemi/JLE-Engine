# Collect all .cpp files in this folder

file(GLOB_RECURSE EDITOR_CODE CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

list(FILTER EDITOR_CODE EXCLUDE REGEX
    ".*/cpu_features/cmake/ci/sample/main\\.cpp$"
)

# Add source to this project's executable.
add_executable (Editor ${EDITOR_CODE})

# Disable RTTI
target_compile_options(Editor PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/GR->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-rtti>
)

# Disable Exceptions
target_compile_options(Editor PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/EHs-c- /D_HAS_EXCEPTIONS=0>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-exceptions>
)

target_include_directories(Editor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(Editor PRIVATE Engine)

target_link_libraries(Editor PRIVATE dxgi)
add_subdirectory(cpu_features)
target_link_libraries(Editor PRIVATE CpuFeatures::cpu_features)

# Copy Engine.lib within Editor build folder
add_custom_command(TARGET Editor POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:Engine>
	$<TARGET_FILE_DIR:Editor>
)