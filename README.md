# 2-D Side-Scroller Adventure Game (SFML - C++)

## Overview  
This project is a **classic side-scrolling adventure game** built using **SFML (Simple and Fast Multimedia Library) in C++**. The game features a **2D platformer experience** where the player explores handcrafted levels, overcomes challenging obstacles, fights enemies with varying difficulty, and collects helpful items and power-ups. The core gameplay loop includes action, exploration, and progression through increasingly difficult stages, inspired by classic arcade-style games but powered by modern design principles and smooth mechanics.

## Features  
- **Sprite creation** – All visual elements of the game, such as the player character, enemies, environment tiles, collectibles, and UI, are built using a wide range of 2D sprites. These assets help create a visually engaging and stylistic platformer experience. The sprite system supports animations such as walking, jumping, attacking, idle stance, and death.  

- **Smooth Player Movement** – The movement system is responsive and optimized for fast-paced gameplay. Players can run, jump, fall, and perform combat actions seamlessly. The player’s physics include running and jump heights, giving the player full control over movement precision.  

- **Multiple Levels** – The game contains multiple levels or "worlds", each uniquely designed with increasing complexity. Levels include different terrain styles, enemy placements, secret areas. Completing a level unlocks the next, encouraging player progression.  

- **Enemies** – The game includes  enemies patrol areas and chase the player and attack him . On higher levels the no of enemies increases which increses the difficulty level , player need strategies to defeat, such as dodging timed attacks or should dodge enemies and make continous attcks to defeat them .  

- **Collectibles** – Spread throughout levels are collectible items such as coins, health boosts and players will go through portal to higher level . Collecting these may increase score, unlock rewards, or provide access to secret levels. 

- **Physics Engine** – A custom 2D physics system handles realistic mechanics like gravity, momentum, and collisions. The physics ensure that the player and other objects behave naturally, preventing issues like floating, clipping, or sliding through walls. It includes support for slopes and  wall-jumps.  

- **Dynamic Scrolling** – The camera follows the player smoothly as they move through the world. It dynamically pans both horizontally and vertically depending on the level design, ensuring that the player stays centered and aware of their surroundings.   

- **Sound and Music** – The game includes background music for each level to match the mood or theme (e.g., forest, dungeon, snow world). Sound effects are used for actions like jumping, collecting items, enemy attacks, taking damage, activating switches, and completing levels. These audio elements significantly enhance the atmosphere and feedback of gameplay.
  
## Game Mechanics  

- **Player Controls:**  
  - **Movement:**  
    - **Move Left:** `A` / `Left Arrow`  
    - **Move Right:** `D` / `Right Arrow`  
    - **Jump:** `W`  
    - These controls are highly responsive and designed for platformer precision. Players can chain movement with combat , enabling advanced maneuvers such as jump attacks or timed dodges.  

  - **Combat:**  
    - **Attack:** `Spacebar`
    - **Dodge:** `E`
    - Players can perform melee attacks using a basic weapon (e.g., sword ) and can dodge enemies attack through shield . Later versions may introduce ranged attacks or magic abilities. The combat system includes hit detection, attack cooldowns, and enemy reactions such as knockback or damage animation and if the player hit by enemies it cost him to lost his health bar .  

  - **Game Controls:**  
    - **Pause:** `ESC`  
    - Freezes the game and opens a pause menu. Options may include Restart,play again and  Quit to Main Menu.

- **Enemies:**  
  - Enemies play a critical role in increasing difficulty and maintaining excitement.  
  - **No of Enemies increses at higher level :** Chase the player once spotted and use basic attacks.
    
- **Power-ups:**  
  - Power-ups are placed throughout the game and provide temporary or permanent boosts. These can be hidden, dropped by enemies, or found in treasure chests.  
  - **Health Boost:** Restores part or all of the player’s health bar. Essential after difficult battles or traps.  
  
