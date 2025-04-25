#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"

using namespace sf;

class ContentPipeline
{
public:
	static ContentPipeline& getInstance();
	bool loadContent();

	const Texture& getMapTexture(const Maps mapNumber) const;
	const Texture& getHudmaskTexture() const;

	//const Texture& getDarkKnightTexture() const;
	const Texture& getDemonTexture() const;
	//const Texture& getKnightTexture() const;

	const Texture& getGreenBarTexture() const;
	const Texture& getRedBarTexture() const;

	//const Texture& getFlagTexture() const;
	//const Texture& getIconTexture() const;
	const Texture& getTowerEmplacementTexture() const;
	const Texture& getWaypointTexture() const;

	const Texture& getArrowTexture() const;
	const Texture& getBlastTexture() const;
	const Texture& getFireballTexture() const;

	const Texture& getTitleScreenTexture() const;
	const Texture& getGameOverScreenTexture() const;
	const Texture& getVictoryScreenTexture() const;

	const Texture& getRunesTexture() const;
	const Texture& getSacredLightTexture() const;
	const Texture& getPlagueTexture() const;

	const Texture& getArcherTowerTexture() const;
	const Texture& getKingTowerTexture() const;
	//const Texture& getKnightTowerTexture() const;
	const Texture& getMageTowerTexture() const;

	const SoundBuffer& getArcherAttackSoundBuffer() const;
	const SoundBuffer& getDemonAttackSoundBuffer() const;
	const SoundBuffer& getHitSoundBuffer() const;
	//const SoundBuffer& getKnightAttackSoundBuffer() const;
	const SoundBuffer& getMageAttackSoundBuffer() const;

	const Font& getComiciFont() const;

private:
	ContentPipeline() {}
	ContentPipeline(ContentPipeline& other) {}
	void operator=(const ContentPipeline&) {}
	static ContentPipeline instance;

	Texture mapTextures[3];
	Texture hudMaskTexture;

	//Texture darkKnightTexture;
	Texture demonTexture;
	//Texture knightTexture;

	Texture redBarTexture;
	Texture greenBarTexture;

	//Texture flagTexture;
	//Texture iconTexture;
	Texture towerEmplacementTexture;
	Texture waypointTexture;

	Texture arrowTexture;
	Texture blastTexture;
	Texture fireballTexture;

	Texture titleScreenTexture;
	Texture gameOverScreenTexture;
	Texture victoryScreenTexture;

	Texture runesTexture;
	Texture plagueTexture;
	Texture sacredLightTexture;

	Texture archerTowerTexture;
	Texture kingTowerTexture;
	//Texture knightTowerTexture;
	Texture mageTowerTexture;

	SoundBuffer archerAttackSoundBuffer;
	SoundBuffer demonAttackSoundBuffer;
	SoundBuffer hitSoundBuffer;
	//SoundBuffer knightAttackSoundBuffer;
	SoundBuffer mageAttackSoundBuffer;

	Font comiciFont;
};

