add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor states ui states/parts)

# target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Submodules
add_subdirectory(states)
add_subdirectory(ui)