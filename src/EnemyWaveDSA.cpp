#include "EnemyWaveDSA.h"
#include <queue>

EnemyWaveNode* buildEnemyWaveTree(int maxLevels) {
    // Initialize the root level with 3 enemies and an initial background
    EnemyWaveNode* root = new EnemyWaveNode(1, 3, "assets/bg1.png"); // Level 1
    std::queue<EnemyWaveNode*> q;
    q.push(root);

    // List of background images for each level (loop or cycle through them)
    std::vector<std::string> bgPaths = {
        "assets/bg1.jpg", "assets/bg2.gif", "assets/bg3.png", "assets/bg4.png"
    };

    while (!q.empty()) {
        EnemyWaveNode* current = q.front();
        q.pop();

        if (current->level >= maxLevels) continue;

        // Select background for the next level (cycle if needed)
        std::string bg = bgPaths[(current->level) % bgPaths.size()];

        // Create one child per level (simple linear tree)
        EnemyWaveNode* child = new EnemyWaveNode(current->level + 1, current->numEnemies + 2, bg);
        current->children.push_back(child);
        q.push(child);
    }

    return root;
}
