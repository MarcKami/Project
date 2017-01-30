#pragma once
#include "Grid.hh"
#include "Scene.hh"
#include <map>

// GameScene class with the main gameplay mode
class RankingScene : public Scene {
public:
	explicit RankingScene();
	~RankingScene() override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	Sprite m_background;
	int dif;
	int m_score{ 0 };
	std::map<std::string, int> lvlInfo;

};