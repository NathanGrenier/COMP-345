add_library(towers STATIC
    Tower.cpp
    CannonTower.cpp
    Projectile.cpp
    RapidFireTower.cpp
    StandardTower.cpp
    
)

target_include_directories(towers PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

# If the tower module depends on the core library:
target_link_libraries(towers PUBLIC vendor)