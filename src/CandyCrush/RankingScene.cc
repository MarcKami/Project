#include "GUI.hh"
#include "System.hh"
#include "Logger.hh"
#include "MenuScene.hh"
#include "RankingScene.hh"



using namespace Logger;

#define CELL_WIDTH 80
#define CELL_HEIGHT 80

RankingScene::RankingScene(void) {
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BG_01 };
}

RankingScene::~RankingScene(void) {
}

void RankingScene::OnEntry(void) {
}

void RankingScene::OnExit(void) {
}

void RankingScene::Update(void) {
	static MouseCoords mouseCoords(0, 0);
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		Println("===============");
		Println("mxp: ", mouseCoords);
		mouseCoords = IM.GetMouseCoords();
		//Menu option
		if (mouseCoords.x > 168 && mouseCoords.x < 240 && mouseCoords.y > 680 && mouseCoords.y < 710) {//Ranking
			SM.SetCurScene<MenuScene>(0,0,0,0);
			Logger::Println("Ranking!");
		}
		if (mouseCoords.x > 785 && mouseCoords.x < 850 && mouseCoords.y > 680 && mouseCoords.y < 710) {//Exit
			
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

void RankingScene::Draw(void) {
	m_background.Draw(); // Render background
	GUI::DrawTextSolid<FontID::TITTLE>("RANKING",
	{ W.GetWidth() >> 1, int(W.GetHeight()*.1f), 1, 1 },
	{ 115, 0, 180 });
	//Draw Ranking
	GUI::DrawTextBlended<FontID::CANDY>("MENU",
	{ int(W.GetWidth()*.2f), int(W.GetHeight()*.9f), 1, 1 },
	{ 255, 255, 255 });
	GUI::DrawTextBlended<FontID::CANDY>("EXIT",
	{ int(W.GetWidth()*.8f), int(W.GetHeight()*.9f), 1, 1 },
	{ 255, 255, 255 });
}