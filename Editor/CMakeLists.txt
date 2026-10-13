# Collect all .cpp files in this folder

file(GLOB_RECURSE EDITOR_CODE 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# Add source to this project's executable.
# add_executable (Editor ${EDITOR_CODE})
# Add source to this project's executable.
add_executable (Editor GameEngineTemplate.cpp ${SOURCES}  )

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

# Copy Engine.lib within Editor build folder
add_custom_command(TARGET Editor POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:Engine>
	$<TARGET_FILE_DIR:Editor>
)