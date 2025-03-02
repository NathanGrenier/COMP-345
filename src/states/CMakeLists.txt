add_library(states STATIC
    ExitState.cpp
    IntroState.cpp
    TitleState.cpp
    parts/Part1State.cpp
    parts/Part2State.cpp
    parts/MainGameState.cpp
    parts/MapSelectState.cpp 
    parts/MapEditorState.cpp
)

target_include_directories(states PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

# If the states module depends on the core library:
target_link_libraries(states PUBLIC vendor)