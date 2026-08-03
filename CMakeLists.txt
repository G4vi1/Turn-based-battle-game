cmake_minimum_required(VERSION 3.10)

project(TurnBasedBattleGame)

find_package(SDL3 REQUIRED)

add_executable(game
    src/main.cpp
    src/Battle.cpp
    src/Character.cpp
    src/Skill.cpp
    src/Game.cpp

    src/Actions/AttackAction.cpp
    src/Actions/DefendAction.cpp

    src/UI/Renderer.cpp
    src/UI/BattleScreen.cpp
    src/UI/Button.cpp
)

target_include_directories(game PRIVATE include)
target_link_libraries(game PRIVATE SDL3::SDL3)