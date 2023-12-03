project(day_2)

include(${CMAKE_SOURCE_DIR}/helper.cmake)

file(GLOB SOURCE_FILES "*.h" "*.cpp")

add_include()
add_inputs()

message("${PROJECT_NAME} source files: ${SOURCE_FILES}")

add_executable(day_2 ${SOURCE_FILES})