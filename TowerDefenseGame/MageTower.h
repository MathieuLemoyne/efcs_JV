#pragma once
#include "Tower.h"

class MageTower : public Tower
{
public:
    bool init();
    bool shoot() override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    int  getDamage() const override;

private:
    enum State { Idle, Animating };
    State  state = Idle;
    int    currentFrame = 0;
    float  animationTime = 0.f;
    bool   fireReady = false;

    const float frameDuration = 0.15f;
    const int   frameCount = 3;
    const int   frameWidth = 150;
    const int   frameHeight = 150;
};
