file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*/GameEngineTemplate\\.cpp$")

add_library(Engine STATIC ${ENGINE_SOURCES} "Modules/Camera.h" "Modules/Camera.cpp")

target_include_directories(Engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    ${OPENGL_INCLUDE_DIR}
    ${IL_INCLUDE_DIR}
)

# PUBLIC so Editor inherits all of these automatically
target_link_libraries(Engine PUBLIC
    glm::glm
    SDL3::SDL3
    ${OPENGL_LIBRARIES}
    glad::glad
    imgui::imgui
    imguizmo::imguizmo
    nlohmann_json::nlohmann_json
    assimp::assimp
    ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES}
    CpuFeatures::cpu_features
)

if (WIN32)
    target_link_libraries(Engine PUBLIC dxgi)
endif()

# Disable RTTI
target_compile_options(Engine PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/GR->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-rtti>
)

# Disable exceptions
target_compile_options(Engine PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/EHs-c- /D_HAS_EXCEPTIONS=0>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-exceptions>
)