#pragma once
#include <string>

class HighScoreManager {
public:
    static void load();
    static void save();
    static int getHighScore();
    static int getHighWave();
    static void updateIfBetter(int score, int wave);

private:
    static int highScore;
    static int highWave;
    static const std::string filename;
};
