#include "GUI.hh"
#include "System.hh"
#include "MenuScene.hh"
#include "RankingScene.hh"
#include "IOManager.hh"

RankingScene::RankingScene(void) {
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BG_01 };
}

RankingScene::~RankingScene(void) {
}

void RankingScene::OnEntry(void) {
	rankInfo = IOManager::ReadBinary("lvl/score.bin");
	it = rankInfo.end();
	rankInfo.insert(std::pair<int, char*>(int(-p_data.m_score), p_data.name));
	rankInfo.erase(--it);
	IOManager::WriteBinary("lvl/score.bin", rankInfo);
}

void RankingScene::OnExit(void) {
}

void RankingScene::Update(void) {
	static MouseCoords mouseCoords(0, 0);
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		mouseCoords = IM.GetMouseCoords();
		//Menu option
		if (mouseCoords.x > 168 && mouseCoords.x < 240 && mouseCoords.y > 680 && mouseCoords.y < 710) { // Menu
			SM.SetCurScene<MenuScene>(menu, p_default);
		}
		if (mouseCoords.x > 785 && mouseCoords.x < 850 && mouseCoords.y > 680 && mouseCoords.y < 710) { // Exit
			//Logger::Println("Quit!");
		}
	}
}

void RankingScene::Draw(void) {
	m_background.Draw(); // Render background
	GUI::DrawTextSolid<FontID::TITTLE>("RANKING",
	{ W.GetWidth() >> 1, int(W.GetHeight()*.1f), 1, 1 },
	{ 115, 0, 180 });
	pos = 1;
	yPos = 0.25f;
	for (auto it = rankInfo.begin(); it != rankInfo.end(); ++it) {
		GUI::DrawTextBlended<FontID::CANDY>(std::to_string(pos++),
		{ int(W.GetWidth()*.4f), int(W.GetHeight()*yPos), 1, 1 },
		{ 255, 255, 255 });
		GUI::DrawTextBlended<FontID::CANDY>(std::string(it->second),
		{ int(W.GetWidth()*.45f), int(W.GetHeight()*yPos), 1, 1 },
		{ 255, 255, 255 });
		GUI::DrawTextBlended<FontID::CANDY>(std::to_string(-it->first),
		{ int(W.GetWidth()*.6f), int(W.GetHeight()*yPos), 1, 1 },
		{ 255, 255, 255 });
		yPos += 0.06f;
	}
	GUI::DrawTextBlended<FontID::CANDY>("MENU",
	{ int(W.GetWidth()*.2f), int(W.GetHeight()*.9f), 1, 1 },
	{ 255, 255, 255 });
	GUI::DrawTextBlended<FontID::CANDY>("EXIT",
	{ int(W.GetWidth()*.8f), int(W.GetHeight()*.9f), 1, 1 },
	{ 255, 255, 255 });
}