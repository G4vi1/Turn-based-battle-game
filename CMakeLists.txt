cmake_minimum_required(VERSION 3.22)

project(TurnBasedBattleGame LANGUAGES C CXX)

include(FetchContent)

# Keep raylib fixed to a stable release so every environment uses the same API.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.2.tar.gz
    URL_HASH SHA256=df7a13129b1bec9e73a6962efff170beba310ee6eab8f94cd000b7316b4db5ee
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(raylib)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_executable(game
    src/main.cpp
    src/Battle.cpp
    src/Character.cpp
    src/Skill.cpp
    src/Item.cpp
    src/Inventory.cpp
    src/Game.cpp

    src/BulletHell/Arena.cpp
    src/BulletHell/Bullet.cpp
    src/BulletHell/BulletPattern.cpp
    src/BulletHell/Soul.cpp

    src/GameData/Characters.cpp

    src/Actions/AttackAction.cpp
    src/Actions/DefendAction.cpp
    src/Actions/ItemAction.cpp

    src/UI/Renderer.cpp
    src/UI/BattleScreen.cpp
    src/UI/Button.cpp
    src/UI/HealthBar.cpp
)

target_include_directories(game PRIVATE include)
target_link_libraries(game PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)
