add_library(tower STATIC
    CritterTower.cpp
    CritterProjectile.cpp
    Tower.cpp
    CannonTower.cpp
    Projectile.cpp
    RapidFireTower.cpp
    StandardTower.cpp
)

target_include_directories(tower PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

# If the tower module depends on the core library:
target_link_libraries(tower PUBLIC vendor)