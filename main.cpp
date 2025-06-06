#include <SFML/Graphics.hpp>
#include "src/GameObject.cpp"  // Include GameObject header
#include "Enemy.h"
#include <bits/stdc++.h>
#include <vector>
#include <cstdlib>  // For rand()
#include <ctime>    // For seeding random
#include "EnemyWaveDSA.h"
#include "Enemy.h" // Assuming you have an Enemy class
#include <SFML/Audio.hpp>


using namespace std;

struct LevelNode {
    std::string bgImagePath;
    int enemyCount = 3; // Default number of enemies for the level
    std::vector<LevelNode*> children;
};


EnemyWaveNode* enemyRoot = nullptr;
EnemyWaveNode* currentLevelNode = nullptr;
//std::vector<Enemy> enemies;

void spawnEnemiesForLevel(EnemyWaveNode* node, std::vector<Enemy>& enemies,
                          sf::Texture& textureR, sf::Texture& textureL,
                          const std::vector<sf::Vector2f>& spawnPositions) {
    enemies.clear();

    for (int i = 0; i < node->numEnemies; ++i) {
        sf::Vector2f pos = spawnPositions[i % spawnPositions.size()]; // Wrap if not enough positions
        enemies.emplace_back(textureR, textureL, pos, 1.0f); // Adjust speed if needed
    }
}

int i = 0;
int maxHealth = 40;
int currentHealth = 40;
int animationFrame = 0; // 0 = full, 4 = one heart left
bool isGameOver=false;
bool isGameCompleted=false;

void resolveEnemyCollisions(std::vector<Enemy>& enemies) {
    for (size_t i = 0; i < enemies.size(); ++i) {
        for (size_t j = i + 1; j < enemies.size(); ++j) {
            if (!enemies[i].isAlive() || !enemies[j].isAlive()) continue;

            auto& spriteA = enemies[i].getSprite();
            auto& spriteB = enemies[j].getSprite();

            if (spriteA.getGlobalBounds().intersects(spriteB.getGlobalBounds())) {
                // Push both enemies away from each other
                sf::Vector2f posA = spriteA.getPosition();
                sf::Vector2f posB = spriteB.getPosition();
                sf::Vector2f delta = posB - posA;

                float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
                if (distance == 0) distance = 1.0f;  // Prevent divide by zero

                sf::Vector2f pushDir = delta / distance;
                float pushAmount = 0.7f;  // tweak this value based on how aggressive the separation should be

                spriteA.move(-pushDir * pushAmount);
                spriteB.move(pushDir * pushAmount);
            }
        }
    }
}

void updateHealthBar(sf::Sprite& sprite, int currentHealth, sf::Texture& texture) {
    const int totalFrames = 5;
    const int maxHealth = 40;
    int frameHeight = texture.getSize().y / totalFrames;
    int frameWidth = texture.getSize().x;

    // Map diff HP to frame 0�4
    int healthSegment = maxHealth / totalFrames;
    int row = std::min((maxHealth - currentHealth) / healthSegment, totalFrames - 1);

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, row * frameHeight, frameWidth, frameHeight));
}

void loadNextScene(std::vector<GameObject>& obstacles, sf::RenderWindow& app) {
    srand(time(0)); // random number generator

    obstacles.clear(); // Removing previous obstacles

    std::vector<int> possibleX = {700, 1150, 1600}; // Fixed X positions
    std::vector<int> possibleY = {900, 700}; // Fixed Y positions
    std::vector<sf::Vector2f> obstaclePositions;

    int numObstacles = 2; // Number of obstacles in next scene

    // Shuffle the possible X positions so they are random but unique
    std::random_shuffle(possibleX.begin(), possibleX.end());

    // Get a random index from the list
    int randomIndex = rand() % possibleX.size();

    // Use the selected X value
    float randX = possibleX[randomIndex];

    // Shuffle the possible Y positions so they are random but unique
    std::random_shuffle(possibleY.begin(), possibleY.end());

    // Ensure at least one obstacle has Y = 1050
    obstaclePositions.push_back(sf::Vector2f(randX, 1050));

    //possibleX.erase(possibleX.begin() + randomIndex);

    for (int i = 0; i < numObstacles; i++) {
        //int x = possibleX[rand() % possibleX.size()];  // Pick random X from the list
        //float y = rand() % (app.getSize().y - 300) + 200;  // Random Y avoiding the top
        if (possibleX[i] == randX) continue; // Skip this one
        obstaclePositions.push_back(sf::Vector2f(possibleX[i], possibleY[i]));
    }

    // Initialize new obstacles at random positions
    for (const auto& pos : obstaclePositions) {
        obstacles.emplace_back("assets/obstacle1.png");
        obstacles.back().setScale(0.4f, 0.4f);
        obstacles.back().setPosition(pos.x, pos.y - (obstacles.back().getSize().y * 0.4f) - 110);
    }
}

int main()
{
    sf::RenderWindow app(sf::VideoMode::getDesktopMode(), "Side Scroller Game", sf::Style::Fullscreen);

    sf::Texture startTexture;
startTexture.loadFromFile("assets/startscreen.png");

sf::Sprite startSprite;
startSprite.setTexture(startTexture);

// Scale to fullscreen
sf::Vector2u windowSize = app.getSize();
sf::Vector2u textureSize = startTexture.getSize();

float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
startSprite.setScale(scaleX, scaleY);

// Show for 8 seconds
sf::Clock startClock;
while (startClock.getElapsedTime().asSeconds() < 8.0f) {
    app.clear();
    app.draw(startSprite);
    app.display();
}

    // Background (Make it fit the window)
    GameObject bg("assets/bg_1.png");
    float bgScaleX = static_cast<float>(app.getSize().x) / bg.getSize().x;
    float bgScaleY = static_cast<float>(app.getSize().y) / bg.getSize().y;
    bg.setScale(bgScaleX, bgScaleY);
    bg.setPosition(0, 0);

    // Floor (Full width & at the bottom)
    GameObject floor("assets/floor.png");
    float floorScaleX = static_cast<float>(app.getSize().x) / floor.getSize().x;
    float floorScaleY = 1.0f;
    floor.setScale(floorScaleX, floorScaleY);
    floor.setPosition(0, app.getSize().y - floor.getSize().y * floorScaleY + 290);

    // Load coin spritesheet (8 frames)
    sf::Texture coinTexture;
    coinTexture.loadFromFile("assets/coin_ss.png");

    sf::Sprite coinSprite;
    coinSprite.setTexture(coinTexture);
    coinSprite.setScale(1.2f, 1.2f);
    coinSprite.setPosition(app.getSize().x / 2 +150 , app.getSize().y / 2 -200);

    // Coin animation properties
    const int totalCoinFrames = 8;
    int coinFrameIndex = 0;
    sf::Clock coinAnimationClock;
    sf::Time coinFrameDuration = sf::milliseconds(100);

    // Load medkit spritesheet (8 frames)
    sf::Texture mediTexture;
    mediTexture.loadFromFile("assets/medkit.png");

    sf::Sprite mediSprite;
    mediSprite.setTexture(mediTexture);
    mediSprite.setScale(0.6f, 0.6f);
    mediSprite.setPosition(app.getSize().x / 2 +750, app.getSize().y/2 +150);

    // medkit animation properties
    const int totalmediFrames = 8;
    int mediFrameIndex = 0;
    sf::Clock mediAnimationClock;
    sf::Time mediFrameDuration = sf::milliseconds(100);

    // Load portal spritesheet (4 frames)
    sf::Texture portalTexture;
    portalTexture.loadFromFile("assets/portal.png");

    sf::Sprite portalSprite;
    portalSprite.setTexture(portalTexture);
    portalSprite.setScale(0.5f, 0.5f);
    portalSprite.setPosition(app.getSize().x / 2 +875, app.getSize().y/2 -100 );

    // portal animation properties
    const int totalportalFrames = 4;
    int portalFrameIndex = 0;
    sf::Clock portalAnimationClock;
    sf::Time portalFrameDuration = sf::milliseconds(100);

    // Load both left and right spritesheets for player
    sf::Texture playerTextureA, playerTextureD;
    playerTextureA.loadFromFile("assets/spritesheet_L.png");
    playerTextureD.loadFromFile("assets/spritesheet_R.png");

    // Player sprite
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTextureD);
    playerSprite.setScale(0.2f, 0.2f);
    playerSprite.setPosition((app.getSize().x - playerTextureD.getSize().x * 0.2f) / 4,
                             app.getSize().y - (playerTextureD.getSize().y * 0.2f) - 200);

    sf::Texture attackTexture;
    attackTexture.loadFromFile("assets/attack.png");

    sf::Texture dodgeTexture;
    dodgeTexture.loadFromFile("assets/dodge.png");

    sf::Texture healthBarTexture;
healthBarTexture.loadFromFile("assets/healthbar.png");

sf::Sprite healthBarSprite;
healthBarSprite.setTexture(healthBarTexture);

// Assuming all rows are of same height
int frameHeight = healthBarTexture.getSize().y / 5;
int frameWidth = healthBarTexture.getSize().x;

// Position at top-right
healthBarSprite.setScale(0.3f, 0.3f);
healthBarSprite.setPosition(app.getSize().x - frameWidth +610, 30);
updateHealthBar(healthBarSprite, currentHealth, healthBarTexture); // FIXES IT

// Game Over screen
sf::Texture gameOverTexture;
gameOverTexture.loadFromFile("assets/gameover.jpg");

sf::Sprite gameOverSprite;
gameOverSprite.setTexture(gameOverTexture);
gameOverSprite.setScale(
    (float)app.getSize().x / gameOverTexture.getSize().x,
    (float)app.getSize().y / gameOverTexture.getSize().y
);

// Buttons (Restart and Quit)
sf::RectangleShape restartButton(sf::Vector2f(250, 120));
restartButton.setFillColor(sf::Color::Yellow);
restartButton.setPosition(app.getSize().x / 2 - 346, app.getSize().y / 2 + 80);

sf::RectangleShape quitButton(sf::Vector2f(220, 120));
quitButton.setFillColor(sf::Color::Yellow);
quitButton.setPosition(app.getSize().x / 2 + 64, app.getSize().y / 2 + 80);

sf::Font gameOverFont;
gameOverFont.loadFromFile("fonts/RubikMonoOne-Regular.ttf");

sf::Text restartText("Restart", gameOverFont, 35);
restartText.setFillColor(sf::Color::Black);
restartText.setPosition(restartButton.getPosition().x + 20, restartButton.getPosition().y + 33);

sf::Text quitText("Quit", gameOverFont, 35);
quitText.setFillColor(sf::Color::Black);
quitText.setPosition(quitButton.getPosition().x + 55, quitButton.getPosition().y + 33);

sf::SoundBuffer attackBuffer, damageBuffer, coinBuffer, gameOverBuffer, jumpBuffer, completeBuffer;
sf::Sound attackSound, damageSound, coinSound, gameOverSound, jumpSound, completeSound;

attackBuffer.loadFromFile("sounds/sword-hit.ogg");
damageBuffer.loadFromFile("sounds/damage.mp3");
coinBuffer.loadFromFile("sounds/coin_collect.ogg");
//medkitBuffer.loadFromFile("sounds/medkit.mp3");
gameOverBuffer.loadFromFile("sounds/game-over.ogg");
jumpBuffer.loadFromFile("sounds/jump.ogg");
//bgBuffer.loadFromFile("sounds/game-bgm.mp3");
completeBuffer.loadFromFile("sounds/game-done.ogg");

attackSound.setBuffer(attackBuffer);
damageSound.setBuffer(damageBuffer);
coinSound.setBuffer(coinBuffer);
//medkitSound.setBuffer(medkitBuffer);
gameOverSound.setBuffer(gameOverBuffer);
jumpSound.setBuffer(jumpBuffer);
//bgSound.setBuffer(bgBuffer);
completeSound.setBuffer(completeBuffer);
jumpSound.setVolume(10);


sf::Music bgMusic;
if (bgMusic.openFromFile("sounds/game-bgm.ogg")) {
    bgMusic.setLoop(true);
    bgMusic.setVolume(50); // optional
    bgMusic.play();
}


    // Create a view (camera)
    sf::View cameraView(sf::FloatRect(0, 0, 1920, 1080));

    sf::Font font;
    if (!font.loadFromFile("fonts/RubikMonoOne-Regular.ttf")) {
        // handle error
        cout<<"font not loaded";
    }

    sf::Text warningText;
    warningText.setFont(font);
    warningText.setString("NO GOING BACK,CHAMP!!");
    warningText.setCharacterSize(60);
    warningText.setFillColor(sf::Color::Red);
    //warningText.setStyle(sf::Text::Bold);
    warningText.setPosition(playerSprite.getPosition().x - 850, 50); // Adjust as needed

    sf::Text completionText;
    completionText.setFont(font);
    completionText.setString("CONGRATS!! THATS IT FOR NOW");
    completionText.setCharacterSize(60);
    completionText.setFillColor(sf::Color::Red);
    //warningText.setStyle(sf::Text::Bold);
    completionText.setPosition(playerSprite.getPosition().x - 850, 50); // Adjust as needed

    sf::Text levelClearedText;
levelClearedText.setFont(font);
levelClearedText.setString("Level Cleared!!");
levelClearedText.setCharacterSize(60);
levelClearedText.setFillColor(sf::Color::Red);
levelClearedText.setPosition(0.f, 200.f); // center-top position
bool showLevelCleared = false;
sf::Clock levelClearedTimer;

int score = 0;
sf::Text scoreText;
scoreText.setFont(font);
scoreText.setCharacterSize(50);
scoreText.setFillColor(sf::Color::Black);
scoreText.setPosition(40, 20);
scoreText.setString("Score:0");


    // Player animation properties
    int frameIndex = 0;
    const int totalFrames = 3;
    sf::Clock animationClock;
    sf::Time frameDuration = sf::milliseconds(100);

    std::vector<sf::Vector2f> spawnPositions = {
        {500, 500}, {1000, 500}, {1500, 500}, {2000, 500}
    };


    // Define multiple obstacles
    std::vector<GameObject> obstacles;
    std::vector<sf::Vector2f> obstaclePositions = {
        //{300, 1000},
        {700, 1050},  // First obstacle position
        {1150, 850},  // Second obstacle position
        {1600, 950}  // Third obstacle position
    };

    // Load and position obstacles
    for (const auto& pos : obstaclePositions) {
        obstacles.emplace_back("assets/obstacle1.png"); //  Directly construct in-place
        obstacles.back().setScale(0.4f, 0.4f);
        obstacles.back().setPosition(pos.x, pos.y - (obstacles.back().getSize().y * 0.4f) - 110);
    }

    std::vector<Enemy> enemies;
    sf::Texture enemyTextureL, enemyTextureR;
    enemyTextureL.loadFromFile("assets/enemyL.png");
    enemyTextureR.loadFromFile("assets/enemyR.png");


    // Spawn 3 enemies at random spawn positions
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, spawnPositions.size() - 1);

    for (int i = 0; i < 3; ++i) {
        int index = dist(gen);
        enemies.emplace_back(enemyTextureR, enemyTextureL, spawnPositions[index]);
    }

    enemyRoot = buildEnemyWaveTree(5); // 5 levels
    EnemyWaveNode* rootLevelNode = enemyRoot;
    currentLevelNode = enemyRoot;
    spawnEnemiesForLevel(currentLevelNode, enemies, enemyTextureR, enemyTextureL, spawnPositions);

LevelNode* level1 = new LevelNode();
level1->bgImagePath = "assets/bg_1.png";
level1->enemyCount = 3;

LevelNode* level2 = new LevelNode();
level2->bgImagePath = "assets/bg_2.png";


    // Movement and gravity variables
    bool isJumping = false;
    bool bottomCollision=false;
    bool isCoinCollected=false;
    bool ismediCollected=false;
    int sceneCounter=0;
    bool nextScene=false;
    const float gravity = 0.07f;
    const float JumpStrength = 5.0f;
    float VelocityY = 0.0f;
    bool portalTriggered = false;
sf::Clock portalCooldownClock;
sf::Clock playerAttackClock;
sf::Time attackCooldown = sf::milliseconds(300); // 0.3s cooldown
bool Attack = false;
sf::Clock attackSpriteClock;
sf::Time attackSpriteDuration = sf::milliseconds(200);  // 0.2 sec
bool isAttacking = false;




    // main game loop
    while (app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                app.close();
        }

        // Movement logic
        bool isMoving = false;
        bool Attack=false;
        bool Dodge=false;
        bool showWarning = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !isGameOver) {
            playerSprite.move(-3.0f, 0.0f);
            playerSprite.setTexture(playerTextureA);
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !isGameOver) {
            playerSprite.move(+3.0f, 0.0f);
            playerSprite.setTexture(playerTextureD);
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isJumping && !bottomCollision && !isGameOver) {
            VelocityY = -JumpStrength;
            isJumping = true;
            jumpSound.play();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && playerAttackClock.getElapsedTime() > attackCooldown && !isGameOver) {
            Attack = true;
            isAttacking=true;
            playerAttackClock.restart();
            attackSpriteClock.restart();       // for visibility duration
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !isGameOver){
            //dodge the enemy
            Dodge=true;
        }
        //attack sprite
        sf::Sprite attackSprite;
        attackSprite.setTexture(attackTexture);
        attackSprite.setScale(0.2f,0.2f);
        attackSprite.setPosition(playerSprite.getPosition().x,playerSprite.getPosition().y);

        //dodge sprite
        sf::Sprite dodgeSprite;
        dodgeSprite.setTexture(dodgeTexture);
        dodgeSprite.setScale(0.2f,0.2f);
        dodgeSprite.setPosition(playerSprite.getPosition().x,playerSprite.getPosition().y+5);

        // Gravity
        VelocityY += gravity;
        playerSprite.move(0, VelocityY);

        // Player animation logic
        if (isMoving) {
            if (animationClock.getElapsedTime() >= frameDuration) {
                frameIndex = (frameIndex + 1) % totalFrames;
                animationClock.restart();
            }
        } else {
            frameIndex = 0;
        }

        // Set texture rect for player animation
        int frameWidth = playerTextureD.getSize().x / totalFrames;
        playerSprite.setTextureRect(sf::IntRect(frameWidth * frameIndex, 0, frameWidth, playerTextureD.getSize().y));

        // Coin animation logic
        if (coinAnimationClock.getElapsedTime() >= coinFrameDuration) {
            coinFrameIndex = (coinFrameIndex + 1) % totalCoinFrames;
            coinAnimationClock.restart();
        }

        // Set texture rect for coin animation
        int coinFrameWidth = coinTexture.getSize().x / totalCoinFrames;
        coinSprite.setTextureRect(sf::IntRect(coinFrameWidth * coinFrameIndex, 0, coinFrameWidth, coinTexture.getSize().y));

        // medkit animation logic

        if(mediAnimationClock.getElapsedTime() >=mediFrameDuration){
            mediFrameIndex = (mediFrameIndex + 1) % totalmediFrames;
            mediAnimationClock.restart();
        }

        // Set texture rect for medkit animation
        int mediFrameWidth = mediTexture.getSize().x / totalmediFrames;
        mediSprite.setTextureRect(sf::IntRect(mediFrameWidth * mediFrameIndex, 0, mediFrameWidth, mediTexture.getSize().y));

        // portal animation logic
        if (portalAnimationClock.getElapsedTime() >= portalFrameDuration) {
            portalFrameIndex = (portalFrameIndex + 1) % totalportalFrames;
            portalAnimationClock.restart();
        }

        // Set texture rect for portal animation
        int portalFrameWidth = portalTexture.getSize().x / totalportalFrames;
        portalSprite.setTextureRect(sf::IntRect(portalFrameWidth * portalFrameIndex, 0, portalFrameWidth, portalTexture.getSize().y));

        // Ground collision
        if (playerSprite.getPosition().y >= 670) {
            playerSprite.setPosition(playerSprite.getPosition().x, 670);
            VelocityY = 0;
            isJumping = false;
        }

        // Window boundary check
        if (playerSprite.getPosition().x < 0){
            playerSprite.setPosition(0, playerSprite.getPosition().y);
            showWarning = true;
        }
        if (playerSprite.getPosition().x > app.getSize().x) {
    // Prevent right boundary movement if it's the last level
    if (currentLevelNode->children.empty()) {
        // Last level � clamp to screen boundary
        isGameCompleted=true;
        playerSprite.setPosition(app.getSize().x - playerSprite.getGlobalBounds().width + 167, playerSprite.getPosition().y);
    } else {
        // Not last level � move to next scene
        playerSprite.setPosition(0, playerSprite.getPosition().y);
        nextScene = true;
        sceneCounter++;
    }
}


        // Collision with obstacle
        sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
        bool onTOP = false;

        for (auto &obstacle : obstacles) {
        sf::FloatRect obstacleBounds = obstacle.getSprite().getGlobalBounds();
        obstacleBounds.top += 77;  //tolerance for top collisison
        obstacleBounds.height-=272;   //tolerance for bottom collision
        //playerBounds.height/=2;


        if (playerBounds.intersects(obstacleBounds)) {
            //top collision check
            if (playerBounds.top + playerBounds.height - VelocityY - 100 <= obstacleBounds.top + 100 &&
                playerBounds.left < obstacleBounds.left + obstacleBounds.width - 155 &&
                playerBounds.left + playerBounds.width > obstacleBounds.left + 90) {
                if (!isJumping) {
                    playerSprite.setPosition(playerSprite.getPosition().x, obstacleBounds.top - playerBounds.height/2 );
                    onTOP = true;
                }
                VelocityY = 0;
                isJumping = false;
            }
            //side collision check
            if (playerBounds.left < obstacleBounds.left + obstacleBounds.width - 155 &&
                playerBounds.left + playerBounds.width > obstacleBounds.left + 90 && !onTOP) {
                if (playerBounds.left < obstacleBounds.left) {
                    playerSprite.setPosition(obstacleBounds.left - playerBounds.width + 90, playerSprite.getPosition().y);
                } else if (playerBounds.left + playerBounds.width > obstacleBounds.left + obstacleBounds.width) {
                    playerSprite.setPosition(obstacleBounds.left + obstacleBounds.width - 155, playerSprite.getPosition().y);
                }
            }
            // **Bottom Collision - Prevent Jumping Through Obstacles**
            float playerTop = playerBounds.top;
            float obstacleBottom = obstacleBounds.top + obstacleBounds.height;

            if (playerTop <= obstacleBottom && VelocityY < 0 &&
                playerBounds.left < obstacleBounds.left + obstacleBounds.width - 155 &&
                playerBounds.left + playerBounds.width > obstacleBounds.left + 90) {
                // Player hits the bottom of the obstacle while jumping
                VelocityY = 0; // Stop the jump
                playerSprite.setPosition(playerSprite.getPosition().x, obstacleBottom); // Push player down slightly
            }
        }
        }

        for (auto& enemy : enemies) {
            if(enemy.isAlive() && !isGameOver){
            enemy.update(sf::Vector2f(playerSprite.getPosition().x, playerSprite.getPosition().y));
            }
        }

bool attackedEnemy = false;
std::vector<Enemy*> collidingEnemies;

for (auto& enemy : enemies) {
    if (!enemy.isAlive()) continue;

    if (playerSprite.getGlobalBounds().intersects(enemy.getBounds())) {
        if (Dodge) continue;

        // Store all colliding enemies first
        collidingEnemies.push_back(&enemy);
    }
}

// Attack only ONE enemy from the colliding list
if (Attack && !attackedEnemy && !collidingEnemies.empty() && !isGameOver) {
    Enemy* target = collidingEnemies[0];  // Pick the first one arbitrarily (or use random index if you want)

    target->takeHit();
    attackedEnemy = true;
    attackSound.play();

    if (!target->isAlive()) {
        score += 10;
        scoreText.setString("Score: " + std::to_string(score));
        std::cout << "Enemy defeated! Score: " << score << std::endl;
    }
}

// Handle attacks from remaining enemies (if not dodged)
for (auto* enemyPtr : collidingEnemies) {
    if (!Attack && enemyPtr->canAttack() && !isGameOver) {
        currentHealth = std::max(0, currentHealth - 1);
        updateHealthBar(healthBarSprite, currentHealth, healthBarTexture);
        enemyPtr->resetAttackCooldown();

        std::cout << "Player hit! Health: " << currentHealth << std::endl;
        damageSound.play();

        if (currentHealth == 0) {
            std::cout << "Game Over!\n";
            // Game over logic
            isGameOver=true;
            gameOverSound.play();
        }
    }
}

        if (!portalTriggered && portalSprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds()) && sceneCounter>=5) {
    if (!currentLevelNode->children.empty()) {
        showLevelCleared = true;
        ismediCollected=false;
        isCoinCollected=false;
        levelClearedTimer.restart();

        currentLevelNode = currentLevelNode->children[0];

        enemies.clear();
        spawnEnemiesForLevel(currentLevelNode, enemies, enemyTextureR, enemyTextureL, spawnPositions);

        bg = GameObject(currentLevelNode->bgImagePath);
        float bgScaleX = static_cast<float>(app.getSize().x) / bg.getSize().x;
        float bgScaleY = static_cast<float>(app.getSize().y) / bg.getSize().y;
        bg.setScale(bgScaleX, bgScaleY);
        bg.setPosition(0, 0);

        portalSprite.setPosition(portalSprite.getPosition().x+10, portalSprite.getPosition().y);

        portalTriggered = true;
        portalCooldownClock.restart();

        sceneCounter=0;
        currentHealth=40;
        updateHealthBar(healthBarSprite, currentHealth, healthBarTexture);
    }
}

// Reset portal trigger after cooldown (e.g., 1 second)
if (portalTriggered && portalCooldownClock.getElapsedTime().asSeconds() > 1.f) {
    portalTriggered = false;
}



         // coin collection check
        if (!isCoinCollected && playerSprite.getGlobalBounds().intersects(coinSprite.getGlobalBounds())) {
            isCoinCollected = true; // Hide coin once collected
            coinSound.play();
            cout << "SCORE IS "<< ++score<<"\n";
            scoreText.setString("Score: " + std::to_string(score));
        }

        // medkit collection check
        if (!ismediCollected && playerSprite.getGlobalBounds().intersects(mediSprite.getGlobalBounds())) {
            ismediCollected = true; // Hide coin once collected
            coinSound.play();
            currentHealth+=8;
            cout << "HEALTH IS "<< currentHealth<<"\n";
            updateHealthBar(healthBarSprite, currentHealth, healthBarTexture);
        }

        // Keep the camera centered on the player
        cameraView.setCenter(playerSprite.getPosition().x, 540); // Keep Y constant

        // Apply the camera view
        app.setView(cameraView);

        if (nextScene) {
            loadNextScene(obstacles, app);
            //ismediCollected=false;
            isCoinCollected=false;
            nextScene = false;  // Prevent continuous reloading
        }

        resolveEnemyCollisions(enemies);

        // Render everything
        app.clear();
        float cameraLeft = cameraView.getCenter().x - cameraView.getSize().x / 2.0f;
        float tileWidth = bg.getSize().x * bg.getScale().x;

        // Calculate how far to draw background/floor based on player�s x + offset
        float maxDrawX = playerSprite.getPosition().x + 1920; // Adjust as needed
        int startTile = static_cast<int>(cameraLeft / tileWidth) - 1;
        int endTile = static_cast<int>(maxDrawX / tileWidth) + 2;

        for (int i = startTile; i <= endTile; i++) {
            float tileX = i * tileWidth;
            // Draw background
            bg.setPosition(tileX, 0);
            bg.draw(app);

            // Draw floor
            floor.setPosition(tileX, app.getSize().y - floor.getSize().y * floor.getScale().y + 290);
            floor.draw(app);
        }

        if (isAttacking && attackSpriteClock.getElapsedTime() < attackSpriteDuration) {
    app.draw(attackSprite);
} else if (Dodge) {
    app.draw(dodgeSprite);
} else {
    app.draw(playerSprite);
}

        if(Attack && Dodge){
            app.draw(playerSprite);
        }
        for (auto &obstacle : obstacles) {
            obstacle.draw(app);
        }
        for (auto& enemy : enemies) {
            if(enemy.isAlive()){
            enemy.getSprite().setScale(0.2f, 0.2f); // Adjust these values as needed
            app.draw(enemy.getSprite());
            }
        }
        if(!isCoinCollected){
            app.draw(coinSprite);
        }
        if(!ismediCollected){
            app.draw(mediSprite);
        }
        if(sceneCounter>=5 && !currentLevelNode->children.empty()){
            app.draw(portalSprite);
        }

        if (showWarning) {
            app.draw(warningText);
        }
        if(isGameCompleted){
            app.draw(completionText);
        }
        // Show "Level Cleared!!" for 2 seconds
if (showLevelCleared) {
    if (levelClearedTimer.getElapsedTime().asSeconds() < 3.0f) {
        app.draw(levelClearedText);
    } else {
        showLevelCleared = false;
    }
}
// Reset attack visibility after duration
if (attackSpriteClock.getElapsedTime() >= attackSpriteDuration) {
    isAttacking = false;
}

    app.setView(app.getDefaultView());
    app.draw(healthBarSprite);
    app.draw(scoreText);

    if (isGameOver) {
    app.clear();

    // Draw Game Over screen and buttons
    app.draw(gameOverSprite);
    app.draw(restartButton);
    app.draw(quitButton);
    app.draw(restartText);
    app.draw(quitText);
    app.display();

    // Poll for button click
    while (app.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            app.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = app.mapPixelToCoords(sf::Mouse::getPosition(app));

            if (restartButton.getGlobalBounds().contains(mousePos)) {
    // Reset everything
    ismediCollected=false;
    isCoinCollected=false;
    gameOverSound.pause();
    //completeSound.pause();
    isGameOver = false;
    isGameCompleted=false;
    currentLevelNode = rootLevelNode; // Reset to level 1
    currentHealth = 40;
    updateHealthBar(healthBarSprite, currentHealth, healthBarTexture);
    score = 0;
    scoreText.setString("Score: 0");

    // Reset player position
    playerSprite.setPosition(300, 670);

    // Update background
    bg = GameObject(currentLevelNode->bgImagePath);
    float bgScaleX = static_cast<float>(app.getSize().x) / bg.getSize().x;
    float bgScaleY = static_cast<float>(app.getSize().y) / bg.getSize().y;
    bg.setScale(bgScaleX, bgScaleY);
    bg.setPosition(0, 0);

    // Clear and respawn enemies
    enemies.clear();
    for (int i = 0; i < 3; ++i) {
        int index = dist(gen);
        enemies.emplace_back(enemyTextureR, enemyTextureL, spawnPositions[index]);
    }

    break;
}

            if (quitButton.getGlobalBounds().contains(mousePos)) {
                app.close();
            }
        }
    }

    continue; // Skip game logic when game over
}

        app.display();
    }

    return EXIT_SUCCESS;
}
