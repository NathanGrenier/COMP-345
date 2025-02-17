add_library(util STATIC
    TextureLoader.cpp 
    Vector2D.cpp)

target_include_directories(util PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

# If the states module depends on the core library:
target_link_libraries(util PUBLIC vendor)