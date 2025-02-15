add_library(states STATIC
    ExitState.cpp
    IntroState.cpp
    TitleState.cpp
)
# Expose the public headers for states (located in src/include/states)
target_include_directories(states PUBLIC ${CMAKE_SOURCE_DIR}/src/include/states)

# If the states module depends on the core library:
target_link_libraries(states PUBLIC vendor)