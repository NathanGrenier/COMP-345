add_library(ui STATIC
    LButton.cpp
    LTexture.cpp
    TowerButton.cpp
    MainMenuButton.cpp)

target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

# If the states module depends on the core library:
target_link_libraries(ui PUBLIC vendor)