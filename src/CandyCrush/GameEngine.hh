/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "System.hh"

#pragma region GAME_SCENES
#include "GameScene.hh"
#include "MenuScene.hh"
#include "RankingScene.hh"
#pragma endregion TODO

//! Initializes game needs and controls the game loop
namespace GameEngine {
	//! Loads main resources such as game images and fonts
	void LoadMedia(void) {
		R.LoadFont<FontID::ARIAL>("fnt/arial.ttf", 40);
		R.LoadFont<FontID::CANDY>("fnt/ARCADECLASSIC.ttf", 30);
		R.LoadFont<FontID::FACTORY>("fnt/candsb.ttf", 80);
		R.LoadFont<FontID::TITTLE>("fnt/ARCADECLASSIC.ttf", 180);
		R.LoadTexture<ObjectID::CELL_EMPTY>("gfx/empty.png");
		R.LoadTexture<ObjectID::BULLET>("gfx/bullet.png");
		R.LoadTexture<ObjectID::PLAYER>("gfx/player.png");
		R.LoadTexture<ObjectID::ROCK_L>("gfx/rock_L.png");
		R.LoadTexture<ObjectID::ROCK_M1>("gfx/rock_M1.png");
		R.LoadTexture<ObjectID::ROCK_M2>("gfx/rock_M2.png");
		R.LoadTexture<ObjectID::ROCK_S1>("gfx/rock_S1.png");
		R.LoadTexture<ObjectID::ROCK_S2>("gfx/rock_S2.png");
		R.LoadTexture<ObjectID::ROCK_XS1>("gfx/rock_XS1.png");
		R.LoadTexture<ObjectID::ROCK_XS2>("gfx/rock_XS2.png");
		R.LoadTexture<ObjectID::BG_00>("gfx/bg0.jpg");
		R.LoadTexture<ObjectID::BG_01>("gfx/bg1.jpg");
	}
	//! Adds the game scenes into the Scene Manager and decides which is the first screen
	void AddScenes(void) {
		LevelData menu = { 0,0,0,0 };
		PlayerData p_default = { "   ", 0 };
		SM.AddScene<MenuScene>();//Cambiar  GameScene por MenuScene
		SM.AddScene<GameScene>();
		SM.AddScene<RankingScene>();
		SM.SetCurScene<MenuScene>(menu, p_default);
	}
	/** 
	 * Runs the game specifying the window's name and dimensions
	 * @tparam screenWidth Determines the window's width
	 * @tparam screenHeight Determines the window's height
	 * @param name Determines the window's title
	 */
	template<int screenWidth, int screenHeight>
	void Run(std::string &&name) {
		Window::Instance(std::move(name), screenWidth, screenHeight); // Initialize window Singleton instance for the first time
		LoadMedia(); // Loads the resource assets
		AddScenes(); // Loads the scenes
		bool m_isRunning{ true }; // Decides if the game loop is running
		Scene *&m_curScene(SM.GetCurScene()); // Defines a reference to a pointer that points to the current scene pointer (mindblown)
		while (!IM.HasQuit() && m_isRunning) { // Checks while game's still playable
		#pragma region GAME_UPDATE
			TM.Update([&] {
				switch (m_curScene->GetState()) { // Check for the state of the screen
				case SceneState::RUNNING:	IM.Update(); m_curScene->Update(); break; // Updates the InputManager and the current scene
				case SceneState::EXIT:	m_isRunning = false; break; // Triggers an end of the game looping
				case SceneState::SLEEP: default:;
				}});
		#pragma endregion
		#pragma region GAME_DRAW
			if (m_curScene->CheckState<SceneState::RUNNING>()) { // If screen object exists and its state is running
				R.Clear();	// Clear the screen buffer
				m_curScene->Draw(); // Call the draw method of the scene
				R.Render();	// Update the screen buffer with all sprites that were pushed
			}
		#pragma endregion
		}
	}
}