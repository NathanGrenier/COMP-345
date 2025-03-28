add_library(towers STATIC
    Tower.cpp
    StandardTower.cpp
    RapidFireTower.cpp
    CannonTower.cpp
    Projectile.cpp
    TargetNearExit.cpp
    TargetNearTower.cpp
    TargetStrongest.cpp
    TargetWeakest.cpp
    TowerGroup.cpp
    TowerObserver.cpp
    powerups/Powerup.cpp)

target_include_directories(towers PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

# If the tower module depends on the core library:
target_link_libraries(towers PUBLIC vendor)