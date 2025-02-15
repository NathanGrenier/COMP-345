add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp  "entities/UI/LTexture.h" "entities/LTimer.h")
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)