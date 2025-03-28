#include <SFML/Graphics.hpp>
#include "src/GameObject.cpp"  // Include GameObject header
#include <bits/stdc++.h>
#include <vector>

using namespace std;

int i = 0, score=0;
int health=0;

int main()
{
    // Create SFML window
    sf::RenderWindow app(sf::VideoMode(1920, 1080), "SFML - GAND-MARIO");

    // Background (Make it fit the window)
    GameObject bg("assets/bg2.gif");
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

    // Player animation properties
    int frameIndex = 0;
    const int totalFrames = 3;
    sf::Clock animationClock;
    sf::Time frameDuration = sf::milliseconds(100);

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

    // Movement and gravity variables
    bool isJumping = false;
    bool bottomCollision=false;
    bool isCoinCollected=false;
    bool ismediCollected=false;
    const float gravity = 0.07f;
    const float JumpStrength = 5.0f;
    float VelocityY = 0.0f;


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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            playerSprite.move(-3.0f, 0.0f);
            playerSprite.setTexture(playerTextureA);
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            playerSprite.move(+3.0f, 0.0f);
            playerSprite.setTexture(playerTextureD);
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isJumping && !bottomCollision) {
            VelocityY = -JumpStrength;
            isJumping = true;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            //attack the enemy
            Attack=true;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
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
        if (mediAnimationClock.getElapsedTime() >= mediFrameDuration) {
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
        if (playerSprite.getPosition().x < 0)
            playerSprite.setPosition(0, playerSprite.getPosition().y);
        if (playerSprite.getPosition().x > app.getSize().x - 150)
            playerSprite.setPosition(app.getSize().x - 150, playerSprite.getPosition().y);

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
         // coin collection check
        if (!isCoinCollected && playerSprite.getGlobalBounds().intersects(coinSprite.getGlobalBounds())) {
            isCoinCollected = true; // Hide coin once collected
            cout << "SCORE IS "<< ++score<<"\n";
        }

        // medkit collection check
        if (!ismediCollected && playerSprite.getGlobalBounds().intersects(mediSprite.getGlobalBounds())) {
            ismediCollected = true; // Hide coin once collected
            cout << "HEALTH IS "<< ++health<<"\n";
        }

        // Render everything
        app.clear();
        bg.draw(app);
        floor.draw(app);
        if(!Attack && !Dodge){
            app.draw(playerSprite);
        }
        if(Attack && !Dodge){
            app.draw(attackSprite);
        }
        if(Dodge && !Attack){
            app.draw(dodgeSprite);
        }
        if(Attack && Dodge){
            app.draw(playerSprite);
        }
        for (auto &obstacle : obstacles) {
            obstacle.draw(app);
        }
        if(!isCoinCollected){
            app.draw(coinSprite);
        }
        if(!ismediCollected){
            app.draw(mediSprite);
        }
        app.draw(portalSprite);
        app.display();
    }

    return EXIT_SUCCESS;
}
