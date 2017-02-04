#pragma once
#include "Grid.hh"
#include "Scene.hh"
#include <map>



// GameScene class with the main gameplay mode
class MenuScene : public Scene {
public:
	explicit MenuScene();
	~MenuScene() override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	Sprite m_background;
	std::multimap<int, char*> rankInfo;
	int m_score{ 0 };
	std::vector<LevelData> lvlInfo;
};