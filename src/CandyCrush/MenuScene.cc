#include "GUI.hh"
#include "System.hh"
#include "Logger.hh"
#include "MenuScene.hh"
#include "GameScene.hh"
#include "RankingScene.hh"


using namespace Logger;

#define CELL_WIDTH 80
#define CELL_HEIGHT 80

MenuScene::MenuScene (void) {
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BG_00 };
}

MenuScene::~MenuScene(void) {
}

void MenuScene::OnEntry(void) {
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
		if (mouseCoords.x > 479 && mouseCoords.x < 542 && mouseCoords.y > 376 && mouseCoords.y < 390) {//Easy
			//lvlInfo = IOManager::LoadXML("test.XML", dif);
			SM.SetCurScene<GameScene>(5,1,1,1);
			
		}
		if (mouseCoords.x > 462 && mouseCoords.x < 562 && mouseCoords.y > 414 && mouseCoords.y < 428) {//Medium
			SM.SetCurScene<GameScene>(3,2,2,2);
			
		}
		if (mouseCoords.x > 479 && mouseCoords.x < 542 && mouseCoords.y > 452 && mouseCoords.y < 466) {//Hard
			SM.SetCurScene<GameScene>(1,3,3,3);
			
		}
		if (mouseCoords.x > 454 && mouseCoords.x < 571 && mouseCoords.y > 490 && mouseCoords.y < 504) {//Ranking
			SM.SetCurScene<RankingScene>(0,0,0,0);
			Logger::Println("Ranking!");
		}
		if (mouseCoords.x > 479 && mouseCoords.x < 542 && mouseCoords.y > 528 && mouseCoords.y < 542) {//Exit
			Logger::Println("Quit!");
		}
	}
	else if (IM.IsMouseUp<MOUSE_BUTTON_LEFT>()) {
		Println("mxn: ", IM.GetMouseCoords());
	
	}
	if (IM.IsKeyHold<'a'>()) {
		
	}
	//if (IM.IsKeyDown<'0'>()) SM.SetCurScene<GameScene>();
	if (IM.IsKeyUp<KEY_BUTTON_DOWN>()) Println("down arrow up");
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