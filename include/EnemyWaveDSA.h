#pragma once
#include <vector>
#include <string>

struct EnemyWaveNode {
    int level;
    int numEnemies;
    std::string bgImagePath;  // New field for background image
    std::vector<EnemyWaveNode*> children;

    EnemyWaveNode(int lvl, int count, const std::string& bgPath)
        : level(lvl), numEnemies(count), bgImagePath(bgPath) {}
};

EnemyWaveNode* buildEnemyWaveTree(int maxLevels);
