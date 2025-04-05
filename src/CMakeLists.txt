if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (WIN32_RESOURCES icon.rc)
endif()

add_executable(${PROJECT_NAME} WIN32 ${WIN32_RESOURCES})

set_target_properties(COMP-345 PROPERTIES
    WIN32_EXECUTABLE $<IF:$<CONFIG:Release>,TRUE,FALSE>
)

target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor states ui map util towers)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Submodules
add_subdirectory(states)
add_subdirectory(ui)
add_subdirectory(map)
add_subdirectory(util)
add_subdirectory(critter)
add_subdirectory(towers)

# Copy Assets to the build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor states ui map util critter towers)
