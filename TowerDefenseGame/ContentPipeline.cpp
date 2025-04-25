#include "ContentPipeline.h"

ContentPipeline ContentPipeline::instance;

ContentPipeline& ContentPipeline::getInstance() { return instance; }

bool ContentPipeline::loadContent()
{
	if (!mapTextures[0].loadFromFile("Ressources\\Sprites\\Maps\\Map1.png")) return false;
	if (!mapTextures[1].loadFromFile("Ressources\\Sprites\\Maps\\Map2.png")) return false;
	if (!mapTextures[2].loadFromFile("Ressources\\Sprites\\Maps\\Map3.png")) return false;
	if (!hudMaskTexture.loadFromFile("Ressources\\Sprites\\Maps\\HudMask.png")) return false;

	//if (!darkKnightTexture.loadFromFile("Ressources\\Sprites\\Characters\\DarkKnight.png")) return false;
	if (!demonTexture.loadFromFile("Ressources\\Sprites\\Characters\\Demon.png")) return false;
	//if (!knightTexture.loadFromFile("Ressources\\Sprites\\Characters\\Knight.png")) return false;

	if (!greenBarTexture.loadFromFile("Ressources\\Sprites\\LifeBar\\GreenBar.png")) return false;
	if (!redBarTexture.loadFromFile("Ressources\\Sprites\\LifeBar\\RedBar.png")) return false;

	//if (!flagTexture.loadFromFile("Ressources\\Sprites\\Misc\\Flag.png")) return false;
	//if (!iconTexture.loadFromFile("Ressources\\Sprites\\Misc\\Icon.png")) return false;
	if (!towerEmplacementTexture.loadFromFile("Ressources\\Sprites\\Misc\\TowerEmplacement.png")) return false;
	if (!waypointTexture.loadFromFile("Ressources\\Sprites\\Misc\\Waypoint.png")) return false;

	if (!arrowTexture.loadFromFile("Ressources\\Sprites\\Projectiles\\Arrow.png")) return false;
	if (!blastTexture.loadFromFile("Ressources\\Sprites\\Projectiles\\Blast.png")) return false;
	if (!fireballTexture.loadFromFile("Ressources\\Sprites\\Projectiles\\Fireball.png")) return false;

	if (!titleScreenTexture.loadFromFile("Ressources\\Sprites\\Screens\\Title.jpg")) return false;
	if (!gameOverScreenTexture.loadFromFile("Ressources\\Sprites\\Screens\\GameOver.png")) return false;
	if (!victoryScreenTexture.loadFromFile("Ressources\\Sprites\\Screens\\Victory.png")) return false;

	if (!runesTexture.loadFromFile("Ressources\\Sprites\\Spells\\Runes.png")) return false;
	if (!sacredLightTexture.loadFromFile("Ressources\\Sprites\\Spells\\SacredLight.png")) return false;
	if (!plagueTexture.loadFromFile("Ressources\\Sprites\\Spells\\Plague.png")) return false;

	if (!archerTowerTexture.loadFromFile("Ressources\\Sprites\\Towers\\ArcherTower.png")) return false;
	if (!kingTowerTexture.loadFromFile("Ressources\\Sprites\\Towers\\KingTower.png")) return false;
	//if (!knightTowerTexture.loadFromFile("Ressources\\Sprites\\Towers\\KnightTower.png")) return false;
	if (!mageTowerTexture.loadFromFile("Ressources\\Sprites\\Towers\\MageTower.png")) return false;

	if (!archerAttackSoundBuffer.loadFromFile("Ressources\\Sounds\\SFX\\ArcherAttack.ogg")) return false;
	if (!demonAttackSoundBuffer.loadFromFile("Ressources\\Sounds\\SFX\\DemonAttack.ogg")) return false;
	if (!mageAttackSoundBuffer.loadFromFile("Ressources\\Sounds\\SFX\\MageAttack.ogg")) return false;
	//if (!knightAttackSoundBuffer.loadFromFile("Ressources\\Sounds\\SFX\\KnightAttack.ogg")) return false;
	if (!hitSoundBuffer.loadFromFile("Ressources\\Sounds\\SFX\\Hit.ogg")) return false;

	if (!comiciFont.loadFromFile("Ressources\\Font\\comici.ttf")) return false;

	return true;
}

const Texture& ContentPipeline::getMapTexture(const Maps mapNumber) const { return mapTextures[mapNumber]; }
const Texture& ContentPipeline::getHudmaskTexture() const { return hudMaskTexture; }

//const Texture& ContentPipeline::getDarkKnightTexture() const { return darkKnightTexture; }
const Texture& ContentPipeline::getDemonTexture() const { return demonTexture; }
//const Texture& ContentPipeline::getKnightTexture() const { return knightTexture; }

const Texture& ContentPipeline::getGreenBarTexture() const { return greenBarTexture; }
const Texture& ContentPipeline::getRedBarTexture() const { return redBarTexture; }

//const Texture& ContentPipeline::getFlagTexture() const { return flagTexture; }
//const Texture& ContentPipeline::getIconTexture() const { return iconTexture; }
const Texture& ContentPipeline::getTowerEmplacementTexture() const { return towerEmplacementTexture; }
const Texture& ContentPipeline::getWaypointTexture() const { return waypointTexture; }

const Texture& ContentPipeline::getArrowTexture() const { return arrowTexture; }
const Texture& ContentPipeline::getBlastTexture() const { return blastTexture; }
const Texture& ContentPipeline::getFireballTexture() const { return fireballTexture; }

const Texture& ContentPipeline::getTitleScreenTexture() const { return titleScreenTexture; }
const Texture& ContentPipeline::getGameOverScreenTexture() const { return gameOverScreenTexture; }
const Texture& ContentPipeline::getVictoryScreenTexture() const { return victoryScreenTexture; }


const Texture& ContentPipeline::getRunesTexture() const { return runesTexture; }
const Texture& ContentPipeline::getSacredLightTexture() const { return sacredLightTexture; }
const Texture& ContentPipeline::getPlagueTexture() const { return plagueTexture; }

const Texture& ContentPipeline::getArcherTowerTexture() const { return archerTowerTexture; }
const Texture& ContentPipeline::getKingTowerTexture() const { return kingTowerTexture; }
//const Texture& ContentPipeline::getKnightTowerTexture() const { return knightTowerTexture; }
const Texture& ContentPipeline::getMageTowerTexture() const { return mageTowerTexture; }

const SoundBuffer& ContentPipeline::getArcherAttackSoundBuffer() const { return archerAttackSoundBuffer; }
const SoundBuffer& ContentPipeline::getDemonAttackSoundBuffer() const { return demonAttackSoundBuffer; }
const SoundBuffer& ContentPipeline::getMageAttackSoundBuffer() const { return mageAttackSoundBuffer; }
//const SoundBuffer& ContentPipeline::getKnightAttackSoundBuffer() const { return knightAttackSoundBuffer; }
const SoundBuffer& ContentPipeline::getHitSoundBuffer() const { return hitSoundBuffer; }

const Font& ContentPipeline::getComiciFont() const { return comiciFont; }