#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <iomanip>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../headers/Timer.h"
#include "../headers/Player.h"

SDL_Window *dWindow;

SDL_Renderer *dRenderer;

TTF_Font *gFont = NULL;

MDTexture dTargetTexture;
MDTexture dScoreText[2];
MDTexture dHitsText[2];
MDTexture dMissesText[2];
MDTexture dMissedTargetsText[2];
MDTexture dMaxComboText[2];
MDTexture dTotalTargetsText[2];
MDTexture dAimingText[2];
MDTexture dTotalHitsText;
MDTexture dTotalMissesText;
MDTexture dTotalMissedTargetsText;

MDTexture dXTexture;

std::vector<Target> target;

//Target target[GAME_TARGETS_COUNT];
Circle circle;
Player player;

bool init();

bool loadMedia();

void close();

void renderText(std::string text, Uint8 size, SDL_Color textColor, MDTexture *texture);

bool checkHit(std::vector<Target> target)
{
	int hit = 0;
	int miss = 0;

	for (std::vector<Target>::size_type i = 0; i != target.size(); ++i)
	{
		if (target[i].tHit)
		{
			hit++;
		}
		if (target[i].tMiss)
		{
			miss++;
		}
	}

	if (hit > 0)
	{
		player.hit();
	}
	else if (miss > 0)
	{
		player.miss();
	}
	return hit > 0;
}

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize");
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			// could not initialize linear texture 
			success = false;
		}
		else
		{
			dWindow = SDL_CreateWindow("DEBSON", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

			if (dWindow == NULL)
			{
				// could not create a window
				success = false;
			}
			else
			{
				dRenderer = SDL_CreateRenderer(dWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (dRenderer == NULL)
				{
					// could not create a renderer
					success = false;
				}
				else
				{

					SDL_SetRenderDrawColor(dRenderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.r);
					dTargetTexture.setRenderer(dRenderer);

					int imgFlags = IMG_INIT_PNG;
					if (!(IMG_Init(imgFlags) & imgFlags))
					{
						// SDL_image not initialize
						success = false;
					}

					if (TTF_Init() == -1)
					{
						printf("TTF not working fuck");
						success = false;
					}
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;
	if (!dTargetTexture.init(dRenderer))
	{
		printf("problem with loading renderer to MDTexture class");
		success = false;
	}
	else
	{
		if (!dTargetTexture.loadFromFile("resources/ball.png"))
		{
			printf("Could not load target's texture\n");
			success = false;
		}
	}

	dXTexture.init(dRenderer);
	if (!dXTexture.loadFromFile("resources/x.png"))
	{
		printf("Could not load X texture\n");
	}



	// Size of different texts
	Uint8 scoreSize = 48;
	Uint8 scoreNumberSize = 36;
	Uint8 textSize = 16;
	Uint8 numbersSize = 22;
	Uint8 otherNumbersSize = 15;

	SDL_Color textColor = { 102, 194, 255 };

	//Set text, size and color
	renderText("Score", scoreSize, textColor, &dScoreText[0]);

	renderText("Hits", textSize, textColor, &dHitsText[0]);

	renderText("Misses", textSize, textColor, &dMissesText[0]);

	renderText("Missed targets", textSize, textColor, &dMissedTargetsText[0]);

	renderText("MAX combo", textSize, textColor, &dMaxComboText[0]);

	renderText("Total targets", textSize, textColor, &dTotalTargetsText[0]);

	renderText("Aiming", textSize, textColor, &dAimingText[0]);



	renderText("0", scoreNumberSize, textColor, &dScoreText[1]);

	renderText("0", numbersSize, textColor, &dHitsText[1]);

	renderText("0", numbersSize, textColor, &dMissesText[1]);

	renderText("0", numbersSize, textColor, &dMissedTargetsText[1]);

	renderText("0", numbersSize, textColor, &dMaxComboText[1]);

	renderText("0", numbersSize, textColor, &dTotalTargetsText[1]);

	renderText("0", numbersSize, textColor, &dAimingText[1]);

	renderText("0", numbersSize, textColor, &dTotalHitsText);

	renderText("0", otherNumbersSize, textColor, &dTotalHitsText);

	renderText("0", otherNumbersSize, textColor, &dTotalMissesText);

	renderText("0", otherNumbersSize, textColor, &dTotalMissedTargetsText);


	return success;
}

void close()
{
	dTargetTexture.free();

	dTargetTexture.free();
	dScoreText[0].free();
	dScoreText[1].free();
	dHitsText[0].free();
	dHitsText[1].free();
	dMissesText[0].free();
	dMissesText[1].free();
	dMissedTargetsText[0].free();
	dMissedTargetsText[1].free();
	dMaxComboText[0].free();
	dMaxComboText[1].free();
	dTotalTargetsText[0].free();
	dTotalTargetsText[1].free();
	dAimingText[0].free();
	dAimingText[1].free();
	dTotalHitsText.free();
	dTotalMissesText.free();
	dTotalMissedTargetsText.free();

	TTF_CloseFont(gFont);
	gFont = NULL;

	SDL_DestroyRenderer(dRenderer);
	dRenderer = NULL;
	SDL_DestroyWindow(dWindow);
	dWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void renderText(std::string text, Uint8 size, SDL_Color textColor, MDTexture *texture)
{
	gFont = TTF_OpenFont("resources/ProggySmall.ttf", size);

	texture->setFont(gFont);
	texture->setRenderer(dRenderer);
	if (!texture->loadFromRenderedText(text, textColor))
	{
		printf("Unable to road texture\n");
	}
}

void renderLeftPanel()
{
	SDL_Color underlineColor = { 204, 204, 204 };
	SDL_Color textColor = { 102, 194, 255 };

	// Set back render draw blend mode to NONE to prevent from blending textures below (I think so)
	SDL_SetRenderDrawBlendMode(dRenderer, SDL_BLENDMODE_NONE);

	// Left Panel
	SDL_Rect leftPanel = { GAME_WIDTH, 0, SCREEN_WIDTH - GAME_WIDTH, GAME_HEIGHT };
	SDL_SetRenderDrawColor(dRenderer, 45, 89, 134, 255);
	SDL_RenderFillRect(dRenderer, &leftPanel);

	// Red outline of left Panel
	SDL_Rect leftPanelOutline = { GAME_WIDTH, 0, 3, GAME_HEIGHT };
	SDL_SetRenderDrawColor(dRenderer, 255, 0, 0, 1);
	SDL_RenderFillRect(dRenderer, &leftPanelOutline);

	// Total game score
	dScoreText[0].render(GAME_WIDTH + 10, 20, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_Rect scoreUnderline = { GAME_WIDTH + 10 , 20 + dScoreText[0].getHeight(), 242, 5 };
	SDL_RenderFillRect(dRenderer, &scoreUnderline);

	// Total hits through game time
	dHitsText[0].render(GAME_WIDTH + 10, 85, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 85 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 85 + dHitsText[0].getHeight());

	// Total misses through game time
	dMissesText[0].render(GAME_WIDTH + 10, 110, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 110 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 110 + dHitsText[0].getHeight());

	// Targets that have expired
	dMissedTargetsText[0].render(GAME_WIDTH + 10, 135, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 135 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 135 + dHitsText[0].getHeight());

	// Max combo (how many targets were hit in a row)
	dMaxComboText[0].render(GAME_WIDTH + 10, 165, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 165 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 165 + dHitsText[0].getHeight());

	// Total targets that appeared on game window
	dTotalTargetsText[0].render(GAME_WIDTH + 10, 190, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 190 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 190 + dHitsText[0].getHeight());

	// Aiming
	dAimingText[0].render(GAME_WIDTH + 10, 215, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 215 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 215 + dHitsText[0].getHeight());

	// Calculated points close to left bound

	// Points for hits
	dTotalHitsText.render(GAME_WIDTH + 250 - dTotalHitsText.getWidth(), 85, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 200, 85 + dHitsText[0].getHeight(), GAME_WIDTH + 250, 85 + dHitsText[0].getHeight());

	// Points for misses
	dTotalMissesText.render(GAME_WIDTH + 250 - dTotalMissesText.getWidth(), 110, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 200, 110 + dHitsText[0].getHeight(), GAME_WIDTH + 250, 110 + dHitsText[0].getHeight());

	// Points for missed targets
	dTotalMissedTargetsText.render(GAME_WIDTH + 250 - dTotalMissedTargetsText.getWidth(), 135, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 200, 135 + dHitsText[0].getHeight(), GAME_WIDTH + 250, 135 + dHitsText[0].getHeight());


	dScoreText[1].render(GAME_WIDTH + 250 - dScoreText[1].getWidth(), 27, 1);
	dHitsText[1].render(GAME_WIDTH + 180 - dHitsText[1].getWidth(), 82, 1);
	dMissesText[1].render(GAME_WIDTH + 180 - dMissesText[1].getWidth(), 107, 1);
	dMissedTargetsText[1].render(GAME_WIDTH + 180 - dMissedTargetsText[1].getWidth() , 132, 1);
	dMaxComboText[1].render(GAME_WIDTH + 180 - dMaxComboText[1].getWidth(), 162, 1);
	dTotalTargetsText[1].render(GAME_WIDTH + 180 - dTotalTargetsText[1].getWidth(), 187, 1);
	dAimingText[1].render(GAME_WIDTH + 180 - dAimingText[1].getWidth(), 212, 1);
	
	// *********************************************************************************
	//Render all numbers
	std::stringstream newText;
	newText << player.getScore();
	dScoreText[1].loadFromRenderedText(newText.str().c_str(), textColor);
	
	
	newText.str(std::string());
	newText << player.getHits();
	dHitsText[1].loadFromRenderedText(newText.str().c_str(), textColor);

	newText.str(std::string());
	newText << player.getMisses();
	dMissesText[1].loadFromRenderedText(newText.str().c_str(), textColor);

	newText.str(std::string());
	newText << player.getMissedTargets();
	dMissedTargetsText[1].loadFromRenderedText(newText.str().c_str(), textColor);

	newText.str(std::string());
	newText << player.getMaxCombo();
	dMaxComboText[1].loadFromRenderedText(newText.str().c_str(), textColor);

	newText.str(std::string());
	newText << player.getTotalTargets();
	dTotalTargetsText[1].loadFromRenderedText(newText.str().c_str(), textColor);

	// Aiming
	SDL_Color aimingColor;
	if (player.getAiming() < 0.55f && player.getAiming() != 0)
	{
		aimingColor = { 255, 153, 51 };

		if (player.getAiming() < 0.4f)
		{
			aimingColor = { 255, 51, 0 };
		}
	}
	else
	{
		aimingColor = textColor;
	}
	newText.str(std::string());
	newText << std::setprecision(3) << player.getAiming() << " sec";
	dAimingText[1].loadFromRenderedText(newText.str().c_str(), aimingColor);
	newText.str(std::string());

	if (player.getHits() > 0)
	{
		newText << "+";
	}
	newText << player.getHits() * HIT;
	dTotalHitsText.loadFromRenderedText(newText.str().c_str(), textColor);

	newText.str(std::string());
	newText << player.getMisses() * MISS * -1;
	dTotalMissesText.loadFromRenderedText(newText.str().c_str(), textColor);

	newText.str(std::string());
	newText << player.getMissedTargets() * TARGET_MISS * -1;
	dTotalMissedTargetsText.loadFromRenderedText(newText.str().c_str(), textColor);
	// *********************************************************************************
}

int main(int argc, char* args[])
{
	if (!init())
	{
		//could not initialize
	}
	else
	{
		if (!loadMedia())
		{
			//could not load media
		}
		else
		{
			Target newTargets[2];
			target.push_back(newTargets[0]);
			target.push_back(newTargets[1]);

			SDL_Event e;
			bool quit = false;
			bool callOnce = false;

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					bool targetWasHit = true;
					for (std::vector<Target>::size_type i = 0; i != target.size(); ++i)
					{
						target[i].handleInput(&e);
						if (target[i].tHit && targetWasHit)
						{
							//printf("%d\n", i);
							if (checkHit(target))
							{
								player.checkTargetLifeTime(target);
								target[i].reset(target, i);
								targetWasHit = false;
							}
						}
					}
					if (e.type == SDL_MOUSEBUTTONDOWN && targetWasHit)
					{
						if (checkHit(target))
						{

						}
						
						// Methods that can be called only once and when mouse button pressed
						player.checkScore(&target);
						// Check for target life time when button pressed(if hit)
					}
				}

				SDL_SetRenderDrawColor(dRenderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.r);
				SDL_RenderClear(dRenderer);

				for (std::vector<Target>::size_type i = 0; i != target.size(); ++i)
				{
					// Render targets to the screen
					target[i].render(&dTargetTexture, *dRenderer);
					target[i].update(&dXTexture);
					// Check if target expired
					if (target[i].targetMiss)
					{
						player.targetMiss();
						// check for target life time when target expires
						//player.checkTargetLifeTime(target);
						target[i].reset(target, i);
					}
				}
				// Function for rendering left panel with scores
				renderLeftPanel();

				SDL_RenderPresent(dRenderer);
			}
		}
	}

	close();

	return 0;
}