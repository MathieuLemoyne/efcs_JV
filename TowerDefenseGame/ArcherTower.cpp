#include "ArcherTower.h"  
#include "ContentPipeline.h"  

bool ArcherTower::init()  
{  
setTexture(ContentPipeline::getInstance().getArcherTowerTexture());  

return true;  
}  

void ArcherTower::draw(RenderWindow& renderWindow)  
{  
renderWindow.draw(*this);  
}
void ArcherTower::update(float dt)
{
}
void ArcherTower::shoot()
{

}