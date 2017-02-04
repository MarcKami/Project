#include "GUI.hh"
#include "System.hh"
#include "Logger.hh"
#include "MenuScene.hh"
#include "GameScene.hh"
#include "RankingScene.hh"
#include "IOManager.hh"

MenuScene::MenuScene (void) {
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BG_00 };
}

MenuScene::~MenuScene(void) {
}

void MenuScene::OnEntry(void) {
	lvlInfo = IOManager::LoadXML("lvl/info.XML"); //Load the levels stats from the XML
}

void MenuScene::OnExit(void) {
	
}

void MenuScene::Update(void) {
	static MouseCoords mouseCoords(0, 0);
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		Println("===============");
		Println("mxp: ", mouseCoords);
		mouseCoords = IM.GetMouseCoords();
		//Menu option
		if (mouseCoords.x > 479 && mouseCoords.x < 542 && mouseCoords.y > 376 && mouseCoords.y < 390) { // Easy
			SM.SetCurScene<GameScene>(lvlInfo[0], p_default);
		}
		if (mouseCoords.x > 462 && mouseCoords.x < 562 && mouseCoords.y > 414 && mouseCoords.y < 428) { // Medium
			SM.SetCurScene<GameScene>(lvlInfo[1], p_default);
		}
		if (mouseCoords.x > 479 && mouseCoords.x < 542 && mouseCoords.y > 452 && mouseCoords.y < 466) { // Hard
			SM.SetCurScene<GameScene>(lvlInfo[2], p_default);
		}
		if (mouseCoords.x > 454 && mouseCoords.x < 571 && mouseCoords.y > 490 && mouseCoords.y < 504) { // Ranking
			SM.SetCurScene<RankingScene>(rank, p_default);
		}
		if (mouseCoords.x > 479 && mouseCoords.x < 542 && mouseCoords.y > 528 && mouseCoords.y < 542) { // Exit
			//SDL_Quit();
			Logger::Println("Quit!");
		}
	}
}

void MenuScene::Draw(void) {
	m_background.Draw(); // Render background
	GUI::DrawTextSolid<FontID::TITTLE>("ASTEROIDS",
	{ W.GetWidth() >> 1, int(W.GetHeight()*.3f), 1, 1 },
	{ 115, 0, 180 });
	GUI::DrawTextBlended<FontID::CANDY>("By  Aliaga  and  Martos",
	{ W.GetWidth() >> 1, int(W.GetHeight()*.4f), 1, 1 },
	{ 0, 0, 0 });
	GUI::DrawTextBlended<FontID::CANDY>("EASY",
	{ W.GetWidth() >> 1, int(W.GetHeight()*.5f), 1, 1 },
	{ 0, 0, 0 });
	GUI::DrawTextBlended<FontID::CANDY>("MEDIUM",
	{ W.GetWidth() >> 1, int(W.GetHeight()*.55f), 1, 1 },
	{ 0, 0, 0 });
	GUI::DrawTextBlended<FontID::CANDY>("HARD",
	{ W.GetWidth() >> 1, int(W.GetHeight()*.6f), 1, 1 },
	{ 0, 0, 0 });
	GUI::DrawTextBlended<FontID::CANDY>("RANKING",
	{ W.GetWidth() >> 1, int(W.GetHeight()*.65f), 1, 1 },
	{ 0, 0, 0 });
	GUI::DrawTextBlended<FontID::CANDY>("EXIT",
	{ W.GetWidth() >> 1, int(W.GetHeight()*.7f), 1, 1 },
	{ 0, 0, 0 });
}