#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

static constexpr int SCREEN_WIDTH = 1680;
static constexpr int SCREEN_HEIGHT = 1050;

static const sf::Vector2f KING_POS_MAP1 = { 1138.f, 600.f };
static const sf::Vector2f KING_POS_MAP2 = { 1138.f, 564.f };

static const sf::Vector2f DEMON_SPAWN_MAP1 = { 610.f, -100.f };
static const sf::Vector2f DEMON_SPAWN_MAP2 = { -100.f, 410.f };

enum TowerType { archer, mage, knight, king };
enum ProjectileType { arrow, blast, fireball };
enum SpellType { sacredLight, plague };
enum Maps { map1, map2 };

static const std::vector<sf::Vector2f> WAYPOINTS_MAP1 =
{
    {610.f,   8.f},
    {630.f, 222.f},
    {595.f, 444.f},
    {478.f, 514.f},
    {320.f, 558.f},
    {260.f, 620.f},
    {280.f, 720.f},
    {348.f, 812.f},
    {720.f, 830.f},
    {968.f, 850.f},
    {1110.f,682.f}
};
static const std::vector<sf::Vector2f> EMPLACEMENTS_MAP1 =
{
    {470.f, 170.f},
    {770.f, 250.f},
    {440.f, 370.f},
    {650.f, 520.f},
    {120.f, 650.f},
    {470.f, 700.f},
    {850.f, 710.f},
    {660.f, 950.f}
};


static const std::vector<sf::Vector2f> WAYPOINTS_MAP2_COMMON =
{
    {  88.f, 412.f},
    { 168.f, 465.f},
    { 222.f, 588.f},
    { 308.f, 670.f},
    { 424.f, 668.f},
    { 510.f, 590.f}
};
static const std::vector<sf::Vector2f> WAYPOINTS_MAP2_BRANCH_A =
{
    { 478.f, 468.f},
    { 516.f, 380.f},
    { 594.f, 360.f},
    { 806.f, 368.f},
    {1140.f, 450.f}
};
static const std::vector<sf::Vector2f> WAYPOINTS_MAP2_BRANCH_B =
{
    { 660.f, 598.f},
    { 804.f, 650.f},
    {1140.f, 680.f}
};

static const std::vector<sf::Vector2f> EMPLACEMENTS_MAP2 =
{
    {110.f, 620.f},
    {228.f, 320.f},
    {444.f, 780.f},
    {362.f, 530.f},
    {610.f, 222.f},
    {998.f, 270.f},
    {630.f, 460.f},
    {935.f, 520.f},
    {798.f, 760.f}
};

/* pour utiliser les waypoints dans le code
std::vector<Waypoint*> waypoints;
Waypoint* previous = nullptr;

for (const auto& pos : WAYPOINTS_LEVEL_1) {
    Waypoint* wp = new Waypoint(pos);
    if (previous) previous->setNext(wp);
    previous = wp;
    waypoints.push_back(wp);
}

*/