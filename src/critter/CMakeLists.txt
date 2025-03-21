add_library(critter STATIC
Critter.cpp
CritterGroup.cpp
NormalCritter.cpp
FastCritter.cpp
TankCritter.cpp
CritterObserver.cpp) 

target_include_directories(critter PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

# If the states module depends on the core library:
target_link_libraries(critter PUBLIC vendor)