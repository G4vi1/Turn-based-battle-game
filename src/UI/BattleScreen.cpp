#include "UI/BattleScreen.hpp"
#include <iostream>
#include <string>

using namespace std;

namespace
{
constexpr float WINDOW_WIDTH = 1280.0f;
constexpr float WINDOW_HEIGHT = 720.0f;
constexpr float PANEL_X = 40.0f;
constexpr float PANEL_WIDTH = 1200.0f;
constexpr float ENEMY_PANEL_Y = 40.0f;
constexpr float HERO_PANEL_Y = 540.0f;
constexpr float PANEL_HEIGHT = 140.0f;
constexpr float ACTION_BUTTON_Y = 582.0f;
constexpr float SKILL_BUTTON_Y = 550.0f;
constexpr float SKILL_BUTTON_HEIGHT = 50.0f;
constexpr Uint64 EFFECT_DURATION_MS = 700;
constexpr float BULLET_HELL_X = 440.0f;
constexpr float BULLET_HELL_Y = 245.0f;
constexpr float DEFAULT_BULLET_HELL_WIDTH = 400.0f;
constexpr float DEFAULT_BULLET_HELL_HEIGHT = 250.0f;
}

// Constructor

BattleScreen::BattleScreen(
    Renderer& renderer,
    Battle& battle
)
    : renderer(renderer),
      battle(battle),
      hero(battle.getHero()),
      enemy(battle.getEnemy()),


      // Main actions

      skillButton(580, ACTION_BUTTON_Y, 180, 55, "SKILL"),
      itemButton(790, ACTION_BUTTON_Y, 180, 55, "ITEM"),
      defendButton(1000, ACTION_BUTTON_Y, 180, 55, "DEFEND"),

      backButton(380, 555, 120, 45, "BACK"),
      continueButton(880, ACTION_BUTTON_Y, 200, 55, "CONTINUE"),
      restartButton(780, ACTION_BUTTON_Y, 180, 55, "RESTART"),
      exitButton(1000, ACTION_BUTTON_Y, 180, 55, "EXIT"),

      heroHealthBar(70, 612, 250, 15, hero.getMaxHealth(), {40, 180, 70, 255}),
      heroManaBar(70, 662, 250, 15, hero.getMaxMana(), {150, 50, 200, 255}),

      enemyHealthBar(70, 112, 250, 15, enemy.getMaxHealth(), {40, 180, 70, 255}),
      enemyManaBar(70, 162, 250, 15, enemy.getMaxMana(), {150, 50, 200, 255})
{
    addBattleLog("Battle started. Archangel's turn.");
    updateSkillButtons();
    updateItemButtons();
}


void BattleScreen::draw(){
    renderer.clear();

    drawBackground();

    heroHealthBar.setHealth(hero.getHealth());
    heroManaBar.setHealth(hero.getMana());

    enemyHealthBar.setHealth(enemy.getHealth());
    enemyManaBar.setHealth(enemy.getMana());

    // Enemy status at the top center
    drawEnemyPanel();

    if (state == BattleScreenState::ACTIONS)
    {
        drawHeroPanel();
        drawActionSlots();
    }
    else if (state == BattleScreenState::SKILLS){
        updateSkillButtons();
        drawBottom();
    }
    else if (state == BattleScreenState::ITEMS){
        updateItemButtons();
        drawBottom();
    }
    else if (state == BattleScreenState::BULLET_HELL)
    {
        drawHeroPanel();
        drawBulletHell();
    }
    else
    {
        drawHeroPanel();

        if (state == BattleScreenState::ENEMY_TURN)
        {
            continueButton.draw(renderer);
        }
        else if (state == BattleScreenState::FINISHED)
        {
            restartButton.draw(renderer);
            exitButton.draw(renderer);
        }
    }

    drawTurnIndicator();
    drawBattleLog();
    drawBattleEffect();

    renderer.present();
}

void BattleScreen::update(float deltaTime)
{
    if (state != BattleScreenState::BULLET_HELL)
    {
        return;
    }

    const bool* keyboardState = SDL_GetKeyboardState(nullptr);

    if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
    {
        battle.moveBulletHellSoulUp(deltaTime);
    }

    if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN])
    {
        battle.moveBulletHellSoulDown(deltaTime);
    }

    if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT])
    {
        battle.moveBulletHellSoulLeft(deltaTime);
    }

    if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT])
    {
        battle.moveBulletHellSoulRight(deltaTime);
    }

    battle.updateBulletHell(deltaTime);

    if (!battle.isBulletHellReadyToResolve())
    {
        return;
    }

    const double heroHealthBefore = hero.getHealth();
    battle.resolveBulletHell(battleMessage);
    addBattleLog(battleMessage);

    startEffect(hero.getHealth() < heroHealthBefore
                    ? static_cast<int>(VisualEffect::HERO_HIT)
                    : static_cast<int>(VisualEffect::NONE));

    if (battle.hasEnded())
    {
        state = BattleScreenState::FINISHED;
        battleMessage += enemy.isAlive() ? "  Victory!" : "  Defeat!";
        addBattleLog(enemy.isAlive() ? "Defeat!" : "Victory!");
    }
    else
    {
        state = BattleScreenState::ACTIONS;
    }
}

// Draws the background of the battle screen.

void BattleScreen::drawBackground()
{
    renderer.setDrawColor(
        20,
        20,
        20,
        255
    );

    renderer.drawFilledRect(
        0,
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );
}

// Draws the hero panel.

void BattleScreen::drawHeroPanel()
{
    renderer.setDrawColor(
        60,
        60,
        60,
        255
    );

    renderer.drawFilledRect(
        PANEL_X,
        HERO_PANEL_Y,
        PANEL_WIDTH,
        PANEL_HEIGHT
    );

    // Name

    renderer.drawText(
        hero.getName().c_str(),
        70,
        552,
        26
    );

    // Health text

    std::string healthText =
        std::to_string(static_cast<int>(hero.getHealth())) +
        " / " +
        std::to_string(static_cast<int>(hero.getMaxHealth()));

    renderer.drawText(
        ("HP " + healthText).c_str(),
        70,
        585,
        18
    );

    heroHealthBar.draw(renderer);

    // Mana text

    std::string manaText =
        std::to_string(static_cast<int>(hero.getMana())) +
        " / " +
        std::to_string(static_cast<int>(hero.getMaxMana()));

    renderer.drawText(
        ("MP " + manaText).c_str(),
        70,
        635,
        18
    );

    heroManaBar.draw(renderer);

    if (hero.isDefendingState())
    {
        renderer.drawText("DEFENDING", 390, 612, 18);
    }
}

// Draws the enemy panel.

void BattleScreen::drawEnemyPanel()
{
    renderer.setDrawColor(
        60,
        60,
        60,
        255
    );

    renderer.drawFilledRect(
        PANEL_X,
        ENEMY_PANEL_Y,
        PANEL_WIDTH,
        PANEL_HEIGHT
    );

    // Name

    renderer.drawText(
        enemy.getName().c_str(),
        70,
        52,
        26
    );

    // Health text

    std::string healthText =
        std::to_string(static_cast<int>(enemy.getHealth())) +
        " / " +
        std::to_string(static_cast<int>(enemy.getMaxHealth()));

    renderer.drawText(
        ("HP " + healthText).c_str(),
        70,
        85,
        18
    );

    enemyHealthBar.draw(renderer);

    // Mana text

    std::string manaText =
        std::to_string(static_cast<int>(enemy.getMana())) +
        " / " +
        std::to_string(static_cast<int>(enemy.getMaxMana()));

    renderer.drawText(
        ("MP " + manaText).c_str(),
        70,
        135,
        18
    );

    enemyManaBar.draw(renderer);

    if (enemy.isDefendingState())
    {
        renderer.drawText("DEFENDING", 390, 112, 18);
    }
}

void BattleScreen::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        float mouseX = event.motion.x;
        float mouseY = event.motion.y;

        if (state == BattleScreenState::ACTIONS)
        {
            skillButton.setHovered(
                skillButton.isHovered(mouseX, mouseY)
            );

            itemButton.setHovered(
                itemButton.isHovered(mouseX, mouseY)
            );

            defendButton.setHovered(
                defendButton.isHovered(mouseX, mouseY)
            );
        }
        else if (state == BattleScreenState::SKILLS || state == BattleScreenState::ITEMS)
        {
            vector<Button>& buttons = state == BattleScreenState::SKILLS
                                        ? skillButtons
                                        : itemButtons;
            for (Button& button : buttons)
            {
                button.setHovered(button.isHovered(mouseX, mouseY));
            }

            backButton.setHovered(
                backButton.isHovered(mouseX, mouseY)
            );
        }
        else if (state == BattleScreenState::ENEMY_TURN)
        {
            continueButton.setHovered(continueButton.isHovered(mouseX, mouseY));
        }
        else if (state == BattleScreenState::FINISHED)
        {
            restartButton.setHovered(restartButton.isHovered(mouseX, mouseY));
            exitButton.setHovered(exitButton.isHovered(mouseX, mouseY));
        }
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (state == BattleScreenState::ACTIONS)
        {
            if (skillButton.isClicked(event))
            {
                skillButton.setPressed(true);
                state = BattleScreenState::SKILLS;
            }

            if (itemButton.isClicked(event))
            {
                itemButton.setPressed(true);
                state = BattleScreenState::ITEMS;
            }

            if (defendButton.isClicked(event))
            {
                defendButton.setPressed(true);
                performHeroDefense();
            }
        }
        else if (state == BattleScreenState::SKILLS)
        {
            for (size_t index = 0; index < skillButtons.size(); ++index)
            {
                if (skillButtons[index].isClicked(event))
                {
                    skillButtons[index].setPressed(true);
                    performHeroSkill(index);
                    break;
                }
            }

            if (backButton.isClicked(event)){
                backButton.setPressed(true);
                state = BattleScreenState::ACTIONS;
            }
        }
        else if (state == BattleScreenState::ITEMS)
        {
            for (size_t index = 0; index < itemButtons.size(); ++index)
            {
                if (itemButtons[index].isClicked(event))
                {
                    itemButtons[index].setPressed(true);
                    performHeroItem(index);
                    break;
                }
            }

            if (backButton.isClicked(event))
            {
                backButton.setPressed(true);
                state = BattleScreenState::ACTIONS;
            }
        }
        else if (state == BattleScreenState::ENEMY_TURN)
        {
            if (continueButton.isClicked(event))
            {
                continueButton.setPressed(true);
                performEnemyTurn();
            }
        }
        else if (state == BattleScreenState::FINISHED)
        {
            if (restartButton.isClicked(event))
            {
                restartButton.setPressed(true);
                restartBattle();
            }
            else if (exitButton.isClicked(event))
            {
                exitButton.setPressed(true);
                closeRequested = true;
            }
        }
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        skillButton.setPressed(false);
        for (Button& skillButton : skillButtons)
        {
            skillButton.setPressed(false);
        }
        for (Button& itemButton : itemButtons)
        {
            itemButton.setPressed(false);
        }
        itemButton.setPressed(false);
        defendButton.setPressed(false);
        backButton.setPressed(false);
        continueButton.setPressed(false);
        restartButton.setPressed(false);
        exitButton.setPressed(false);
    }
}

void BattleScreen::performHeroSkill(size_t skillIndex)
{
    string heroMessage;

    if (!battle.heroUseSkill(skillIndex, heroMessage))
    {
        battleMessage = heroMessage;
        return;
    }

    battleMessage = heroMessage;
    addBattleLog(battleMessage);
    state = BattleScreenState::ACTIONS;

    const Skill& selectedSkill = hero.getSkills()[skillIndex];
    startEffect(selectedSkill.getType() == SkillType::Healing
                    ? static_cast<int>(VisualEffect::HERO_HEAL)
                    : static_cast<int>(VisualEffect::ENEMY_HIT));

    if (battle.hasEnded())
    {
        state = BattleScreenState::FINISHED;
        battleMessage += enemy.isAlive() ? "  Victory!" : "  Defeat!";
        addBattleLog(enemy.isAlive() ? "Defeat!" : "Victory!");
    }
    else
    {
        state = BattleScreenState::ENEMY_TURN;
    }
}

void BattleScreen::performHeroDefense()
{
    battle.heroDefend(battleMessage);
    addBattleLog(battleMessage);
    startEffect(static_cast<int>(VisualEffect::HERO_DEFEND));
    state = BattleScreenState::ENEMY_TURN;
}

void BattleScreen::performHeroItem(size_t itemIndex)
{
    string heroMessage;

    if (!battle.heroUseItem(itemIndex, heroMessage))
    {
        battleMessage = heroMessage;
        return;
    }

    battleMessage = heroMessage;
    addBattleLog(battleMessage);
    startEffect(static_cast<int>(VisualEffect::HERO_ITEM));
    state = BattleScreenState::ENEMY_TURN;
}

void BattleScreen::performEnemyTurn()
{
    if (battle.startEnemyBulletHell(battleMessage))
    {
        addBattleLog(battleMessage);
        state = BattleScreenState::BULLET_HELL;
        return;
    }

    const double heroHealthBefore = hero.getHealth();
    battle.enemyTurn(battleMessage);
    addBattleLog(battleMessage);

    startEffect(hero.getHealth() < heroHealthBefore
                    ? static_cast<int>(VisualEffect::HERO_HIT)
                    : static_cast<int>(VisualEffect::ENEMY_DEFEND));

    if (battle.hasEnded())
    {
        state = BattleScreenState::FINISHED;
        battleMessage += enemy.isAlive() ? "  Victory!" : "  Defeat!";
        addBattleLog(enemy.isAlive() ? "Defeat!" : "Victory!");
    }
    else
    {
        state = BattleScreenState::ACTIONS;
    }
}

void BattleScreen::drawBulletHell()
{
    const Arena& arena = battle.getBulletHellArena();
    const float arenaX = BULLET_HELL_X +
        (DEFAULT_BULLET_HELL_WIDTH - arena.getWidth()) / 2.0f;
    const float arenaY = BULLET_HELL_Y +
        (DEFAULT_BULLET_HELL_HEIGHT - arena.getHeight()) / 2.0f;

    renderer.setDrawColor(8, 8, 12, 255);
    renderer.drawFilledRect(
        arenaX,
        arenaY,
        arena.getWidth(),
        arena.getHeight());

    renderer.setDrawColor(240, 240, 240, 255);
    renderer.drawRect(
        arenaX,
        arenaY,
        arena.getWidth(),
        arena.getHeight());

    const Soul& soul = arena.getSoul();
    if (soul.canTakeDamage())
    {
        renderer.setDrawColor(235, 55, 70, 255);
    }
    else
    {
        renderer.setDrawColor(255, 220, 70, 255);
    }

    renderer.drawFilledRect(
        arenaX + soul.getX(),
        arenaY + soul.getY(),
        soul.getSize(),
        soul.getSize());

    renderer.setDrawColor(145, 75, 225, 255);
    for (const Bullet& bullet : arena.getBullets())
    {
        renderer.drawFilledRect(
            arenaX + bullet.getX(),
            arenaY + bullet.getY(),
            bullet.getSize(),
            bullet.getHeight());
    }

    renderer.drawText("DODGE!  WASD / ARROW KEYS", arenaX, 220, 18);
}

void BattleScreen::restartBattle()
{
    battle.reset();
    battleMessage.clear();
    battleLog.clear();
    addBattleLog("Battle restarted. Archangel's turn.");
    activeEffect = VisualEffect::NONE;
    state = BattleScreenState::ACTIONS;
}

bool BattleScreen::shouldClose() const
{
    return closeRequested;
}

// Draws the action slots, even for future ones.

void BattleScreen::drawActionSlots()
{

    skillButton.draw(renderer);

    itemButton.draw(renderer);

    defendButton.draw(renderer);
}

void BattleScreen::addBattleLog(const string& message)
{
    battleLog.push_back(message);

    constexpr size_t MAX_LOG_ENTRIES = 4;
    if (battleLog.size() > MAX_LOG_ENTRIES)
    {
        battleLog.erase(battleLog.begin());
    }
}

void BattleScreen::drawBattleLog()
{
    renderer.drawText("BATTLE LOG", 70, 210, 16);

    for (size_t index = 0; index < battleLog.size(); ++index)
    {
        renderer.drawText(battleLog[index].c_str(), 70, 240 + index * 24, 16);
    }
}

void BattleScreen::drawTurnIndicator()
{
    const char* turnText = "ARCHANGEL'S TURN";

    if (state == BattleScreenState::ENEMY_TURN)
    {
        turnText = "LEVIATHAN'S TURN";
    }
    else if (state == BattleScreenState::BULLET_HELL)
    {
        turnText = "LEVIATHAN'S ATTACK";
    }
    else if (state == BattleScreenState::FINISHED)
    {
        turnText = enemy.isAlive() ? "DEFEAT" : "VICTORY";
    }

    renderer.drawText(turnText, 900, 210, 18);
}

void BattleScreen::startEffect(int effect)
{
    activeEffect = static_cast<VisualEffect>(effect);
    effectEndsAt = SDL_GetTicks() + EFFECT_DURATION_MS;
}

void BattleScreen::drawBattleEffect()
{
    if (activeEffect == VisualEffect::NONE)
    {
        return;
    }

    if (SDL_GetTicks() >= effectEndsAt)
    {
        activeEffect = VisualEffect::NONE;
        return;
    }

    const char* label = "IMPACT!";
    Uint8 red = 190;
    Uint8 green = 60;
    Uint8 blue = 60;

    if (activeEffect == VisualEffect::HERO_HEAL)
    {
        label = "HEALING!";
        red = 50;
        green = 160;
        blue = 90;
    }
    else if (activeEffect == VisualEffect::HERO_ITEM)
    {
        label = "ITEM USED!";
        red = 210;
        green = 160;
        blue = 50;
    }
    else if (activeEffect == VisualEffect::HERO_DEFEND ||
             activeEffect == VisualEffect::ENEMY_DEFEND)
    {
        label = "DEFENSE!";
        red = 55;
        green = 110;
        blue = 190;
    }

    renderer.setDrawColor(red, green, blue, 255);
    renderer.drawFilledRect(520, 340, 240, 70);
    renderer.drawText(label, 570, 360, 24);
}

void BattleScreen::drawBottom()
{
    // Hero status
    drawHeroPanel();

    // Back button
    backButton.draw(renderer);

    if (state == BattleScreenState::ITEMS)
    {
        bool hasUsableItem = false;
        const vector<InventorySlot>& slots = hero.getInventory().getSlots();

        for (size_t index = 0; index < slots.size(); ++index)
        {
            hasUsableItem = hasUsableItem || hero.canUseItem(index);
        }

        if (slots.empty())
        {
            renderer.drawText("NO ITEMS", 530, 510, 20);
        }
        else if (!hasUsableItem)
        {
            renderer.drawText("HP AND MP ARE FULL", 530, 510, 20);
        }
    }

    vector<Button>& buttons = state == BattleScreenState::SKILLS
                                ? skillButtons
                                : itemButtons;
    for (Button& button : buttons)
    {
        button.draw(renderer);
    }
}

// Rebuilds the skill controls from the hero's current skill list.

void BattleScreen::updateSkillButtons()
{
    const vector<Skill>& skills = hero.getSkills();
    skillButtons.clear();
    skillButtons.reserve(skills.size());

    for (size_t index = 0; index < skills.size(); ++index)
    {
        const float x = index % 2 == 0 ? 530.0f : 880.0f;
        const float y = SKILL_BUTTON_Y + (index / 2) * (SKILL_BUTTON_HEIGHT + 10.0f);
        const Skill& skill = skills[index];

        skillButtons.emplace_back(
            x,
            y,
            300,
            SKILL_BUTTON_HEIGHT,
            skill.getName() + " - " + to_string(skill.getManaCost()) + " MP"
        );

        skillButtons.back().setEnabled(hero.hasEnoughMana(skill.getManaCost()));
    }
}

void BattleScreen::updateItemButtons()
{
    const vector<InventorySlot>& slots = hero.getInventory().getSlots();
    itemButtons.clear();
    itemButtons.reserve(slots.size());

    for (size_t index = 0; index < slots.size(); ++index)
    {
        const float x = index % 2 == 0 ? 530.0f : 880.0f;
        const float y = SKILL_BUTTON_Y + (index / 2) * (SKILL_BUTTON_HEIGHT + 10.0f);
        const InventorySlot& slot = slots[index];
        const Item& item = slot.item;

        itemButtons.emplace_back(
            x,
            y,
            300,
            SKILL_BUTTON_HEIGHT,
            item.getName() + " x" + to_string(slot.quantity) +
                " HP" + to_string(static_cast<int>(item.getHealthRestore())) +
                " MP" + to_string(item.getManaRestore())
        );

        const bool usable = hero.canUseItem(index);
        itemButtons.back().setEnabled(usable);
    }

    // The inventory remains accessible even when every item is temporarily
    // unusable, so the player can inspect quantities and effects.
    itemButton.setEnabled(!slots.empty());
}
