cmake_minimum_required(VERSION 3.10)

project(TurnBasedBattleGame)

add_executable(game
    src/main.cpp
    src/Battle.cpp
    src/Character.cpp
    src/Skill.cpp
    src/Game.cpp
)

target_include_directories(game PRIVATE include)