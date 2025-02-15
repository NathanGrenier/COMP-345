add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp LTimer.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor states ui)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Submodules
add_subdirectory(states)
add_subdirectory(ui)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor states ui)