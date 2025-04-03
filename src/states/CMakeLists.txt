add_library(states STATIC
    ExitState.cpp
    IntroState.cpp
    TitleState.cpp
    "MainGameState.cpp"
    "MapSelectState.cpp" 
    "MapEditorState.cpp"
    "EndScreenState.cpp" 
    "GameOverState.cpp" 
    "WinGameState.cpp"  "OptionsState.cpp")

target_include_directories(states PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

# If the states module depends on the core library:
target_link_libraries(states PUBLIC vendor)