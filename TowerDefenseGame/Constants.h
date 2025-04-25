#pragma once

static const int SCREEN_WIDTH = 1680;
static const int SCREEN_HEIGHT = 1050;
static const sf::Vector2f KING_POS = { 6969, 6969 };

enum TowerType {archer, mage, knight, king};
enum ProjectileType { arrow, blast, fireball };
enum SpellType { sacredLight, plague };
enum Maps { map1, map2, map3 };
static const std::vector<sf::Vector2f> WAYPOINTS_LEVEL_1 = {
    {100, 200},
    {250, 220},
    {400, 300},
    {600, 320},
    {800, 400}
};
static const std::vector<sf::Vector2f> WAYPOINTS_LEVEL_2 = {
    {80, 150},
    {200, 200},
    {350, 240},
    {500, 270},
    {700, 350},
    {900, 420}
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