#include "HighScoreManager.h"
#include <fstream>

// Voici deux ressources qui nous ont permis de faire ce bonus:
// https://cplusplus.com/forum/beginner/78887/
// https://stackoverflow.com/questions/11405662/persistent-data-settings-in-c-application

int HighScoreManager::highScore = 0;
int HighScoreManager::highWave = 0;
const std::string HighScoreManager::filename = "highscore.txt";

void HighScoreManager::load() {
    std::ifstream in(filename);
    if (in >> highScore >> highWave) {
    }
    else {
        highScore = 0;
        highWave = 0;
    }
}

void HighScoreManager::save() {
    std::ofstream out(filename);
    out << highScore << " " << highWave;
}

int HighScoreManager::getHighScore() {
    return highScore;
}

int HighScoreManager::getHighWave() {
    return highWave;
}

void HighScoreManager::updateIfBetter(int score, int wave) {
    if (score > highScore || (score == highScore && wave > highWave)) {
        highScore = score;
        highWave = wave;
        save();
    }
}
