#pragma once

static const int SCREEN_WIDTH = 1680;
static const int SCREEN_HEIGHT = 1050;
static const sf::Vector2f KING_POS = { 6969, 6969 };

enum TowerType {archer, mage, knight, king};
enum ProjectileType { arrow, blast, fireball };
enum SpellType { sacredLight, plague };
enum Maps { map1, map2, map3 };
static const std::vector<sf::Vector2f> WAYPOINTS_MAP1 = {
    {610.f, 8.f},
    {630.f, 222.f},
    {595.f, 444.f},
    {478.f, 514.f},
    {320.f, 558.f},
    {260.f, 620.f},
    {280.f, 720.f},
    {348.f, 812.f},
    {720.f, 830.f},
    {968.f, 850.f},
    {1110.f, 682.f}
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