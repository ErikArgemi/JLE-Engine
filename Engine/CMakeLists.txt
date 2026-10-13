# Find required packages
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
#added vcpkg
find_package(nlohmann_json CONFIG REQUIRED)

# Collect all .cpp files in this folder
file(GLOB_RECURSE ENGINE_CODE 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# Create Engine library
add_library (Engine ${ENGINE_CODE})
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(Engine PUBLIC glad::glad)

# more target_link_libraries calls ...
target_link_libraries(Engine PRIVATE glm::glm)
target_link_libraries(Engine PRIVATE SDL3::SDL3)

target_link_libraries(Engine PRIVATE ${OPENGL_LIBRARIES})
target_include_directories(Engine PRIVATE ${OPENGL_INCLUDE_DIR})

target_link_libraries(Engine PRIVATE glad::glad)
target_link_libraries(Engine PRIVATE imgui::imgui)
target_link_libraries(Engine PRIVATE imguizmo::imguizmo)
#added vcpkg
target_link_libraries(Engine PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(Engine PRIVATE dxgi)
add_subdirectory(cpu_features)
target_link_libraries(Engine PRIVATE CpuFeatures::cpu_features)