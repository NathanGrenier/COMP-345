add_library(util STATIC
    Vector2D.cpp
    Observable.cpp
    Observer.cpp
    TextureManager.cpp 
    AudioManger.cpp)

target_include_directories(util PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

# If the states module depends on the core library:
target_link_libraries(util PUBLIC vendor)