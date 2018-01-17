#include <iostream>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <vector>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../Circle.h"
#include "../headers/Target.h"
#include "../headers/Player.h"

SDL_Window *dWindow;

SDL_Renderer *dRenderer;

TTF_Font *gFont = NULL;

MDTexture dDotTexture;
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


Target target[GAME_TARGETS_COUNT];
Circle circle;
Player player;

bool init();

bool loadMedia();

void close();

void renderText(std::string text, Uint8 size, SDL_Color textColor, std::string style, MDTexture *texture);

void checkHit(Target *target)
{
	int hit = 0;
	int miss = 0;

	for (int i = 0; i < player.getTargetCount(); ++i)
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

	player.checkScore();
	printf("%d\n", player.getScore());
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
				dRenderer = SDL_CreateRenderer(dWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_ACCELERATED);
				if (dRenderer == NULL)
				{
					// could not create a renderer
					success = false;
				}
				else
				{

					SDL_SetRenderDrawColor(dRenderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.r);
					dDotTexture.setRenderer(dRenderer);

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
	if (!dDotTexture.init(dRenderer))
	{
		printf("problem with loading renderer to class property");
		success = false;
	}
	else
	{
		if (!dDotTexture.loadFromFile("resources/ball.png"))
		{
			// could not loade dot.png
			success = false;
		}
	}
	Uint8 scoreSize = 48;
	Uint8 scoreNumberSize = 36;
	Uint8 textSize = 16;
	Uint8 numbersSize = 22;
	Uint8 otherNumbersSize = 15;


	SDL_Color textColor = { 102, 194, 255 };
	renderText("Score", scoreSize, textColor, "regular", &dScoreText[0]);

	renderText("Hits", textSize, textColor,"regular", &dHitsText[0]);

	renderText("Misses", textSize, textColor, "regular", &dMissesText[0]);

	renderText("Missed targets", textSize, textColor, "regular", &dMissedTargetsText[0]);

	renderText("MAX combo", textSize, textColor, "regular", &dMaxComboText[0]);

	renderText("Total targets", textSize, textColor, "regular", &dTotalTargetsText[0]);

	renderText("Aiming", textSize, textColor, "regular", &dAimingText[0]);



	renderText("0", scoreNumberSize, textColor, "bold", &dScoreText[1]);

	renderText("0", numbersSize, textColor, "bold", &dHitsText[1]);

	renderText("0", numbersSize, textColor, "bold", &dMissesText[1]);

	renderText("0", numbersSize, textColor, "bold", &dMissedTargetsText[1]);

	renderText("0", numbersSize, textColor, "bold", &dMaxComboText[1]);

	renderText("0", numbersSize, textColor, "bold", &dTotalTargetsText[1]);

	renderText("0", numbersSize, textColor, "bold", &dAimingText[1]);

	renderText("0", numbersSize, textColor, "bold", &dTotalHitsText);

	renderText("0", otherNumbersSize, textColor, "regular", &dTotalHitsText);

	renderText("0", otherNumbersSize, textColor, "regular", &dTotalMissesText);

	renderText("0", otherNumbersSize, textColor, "regular", &dTotalMissedTargetsText);



	return success;
}

void close()
{
	dDotTexture.free();
	//dScoreText.free();

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

void renderText(std::string text, Uint8 size, SDL_Color textColor, std::string style, MDTexture *texture)
{
	if (style == "regular")
	{
		gFont = TTF_OpenFont("resources/ProggySmall.ttf", size);
	}
	else if (style == "bold")
	{
		gFont = TTF_OpenFont("resources/ProggySmall.ttf", size);
	}
	//SDL_Color textColor = { 255, 255, 255 };
	texture->setFont(gFont);
	texture->setRenderer(dRenderer);
	if (!texture->loadFromRenderedText(text, textColor))
	{
		printf("cant load fucking texture xd");
	}
}

void renderLeftPanel()
{
	SDL_Color lineColor = { 204, 204, 204 };
	SDL_Color textColor = { 102, 194, 255 };

	// Left Panel
	SDL_Rect leftPanel = { SCREEN_WIDTH, 0, GAME_WIDTH - SCREEN_WIDTH, GAME_HEIGHT };
	SDL_SetRenderDrawColor(dRenderer, 45, 89, 134, 255);
	SDL_RenderFillRect(dRenderer, &leftPanel);

	// Red outline of left Panel
	SDL_Rect leftPanelOutline = { GAME_WIDTH, 0, 3, GAME_HEIGHT };
	SDL_SetRenderDrawColor(dRenderer, 255, 0, 0, 1);
	SDL_RenderFillRect(dRenderer, &leftPanelOutline);

	// Total game score
	dScoreText[0].render(GAME_WIDTH + 10, 20, 1);
	SDL_SetRenderDrawColor(dRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
	SDL_Rect scoreUnderline = { GAME_WIDTH + 10 , 20 + dScoreText[0].getHeight(), 242, 5 };
	SDL_RenderFillRect(dRenderer, &scoreUnderline);

	// Total hits through game time
	dHitsText[0].render(GAME_WIDTH + 10, 85, 1);
	SDL_SetRenderDrawColor(dRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 85 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 85 + dHitsText[0].getHeight());

	// Total misses through game time
	dMissesText[0].render(GAME_WIDTH + 10, 110, 1);
	SDL_SetRenderDrawColor(dRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 110 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 110 + dHitsText[0].getHeight());

	// Targets that have expired
	dMissedTargetsText[0].render(GAME_WIDTH + 10, 135, 1);
	SDL_SetRenderDrawColor(dRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 135 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 135 + dHitsText[0].getHeight());

	// Max combo (how many targets were hit in a row)
	dMaxComboText[0].render(GAME_WIDTH + 10, 165, 1);
	SDL_SetRenderDrawColor(dRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 165 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 165 + dHitsText[0].getHeight());

	// Total targets that appeared on game window
	dTotalTargetsText[0].render(GAME_WIDTH + 10, 190, 1);
	SDL_SetRenderDrawColor(dRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 190 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 190 + dHitsText[0].getHeight());

	// Aiming
	dAimingText[0].render(GAME_WIDTH + 10, 215, 1);
	SDL_SetRenderDrawColor(dRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 215 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 215 + dHitsText[0].getHeight());

	// Calculated points
	dTotalHitsText.render(GAME_WIDTH + 250 - dTotalHitsText.getWidth(), 85, 1);
	SDL_SetRenderDrawColor(dRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 200, 85 + dHitsText[0].getHeight(), GAME_WIDTH + 250, 85 + dHitsText[0].getHeight());

	dTotalMissesText.render(GAME_WIDTH + 250 - dTotalMissesText.getWidth(), 110, 1);
	SDL_SetRenderDrawColor(dRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 200, 110 + dHitsText[0].getHeight(), GAME_WIDTH + 250, 110 + dHitsText[0].getHeight());


	dTotalMissedTargetsText.render(GAME_WIDTH + 250 - dTotalMissedTargetsText.getWidth(), 135, 1);
	SDL_SetRenderDrawColor(dRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 200, 135 + dHitsText[0].getHeight(), GAME_WIDTH + 250, 135 + dHitsText[0].getHeight());


	dScoreText[1].render(GAME_WIDTH + 250 - dScoreText[1].getWidth(), 27, 1);
	dHitsText[1].render(GAME_WIDTH + 180 - dHitsText[1].getWidth(), 82, 1);
	dMissesText[1].render(GAME_WIDTH + 180 - dMissesText[1].getWidth(), 107, 1);
	dMissedTargetsText[1].render(GAME_WIDTH + 180 - dMissedTargetsText[1].getWidth() , 132, 1);
	dMaxComboText[1].render(GAME_WIDTH + 180 - dMaxComboText[1].getWidth(), 162, 1);
	dTotalTargetsText[1].render(GAME_WIDTH + 180 - dTotalTargetsText[1].getWidth(), 187, 1);
	dAimingText[1].render(GAME_WIDTH + 180 - dAimingText[1].getWidth(), 212, 1);
	
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
			SDL_Event e;
			int alpha = 255;
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

					for (int i = 0; i < player.getTargetCount(); ++i)
					{
						target[i].handleInput(&e);
					}
					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						checkHit(target);
					}
				}

				SDL_SetRenderDrawColor(dRenderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.r);
				SDL_RenderClear(dRenderer);

				for (int i = 0; i < player.getTargetCount(); ++i)
				{
					for (int j = 0; j < player.getTargetCount(); ++j)
					{
						if (i == j)
						{

						}
						else
						{
							if ((abs(target[i].getPosY() - target[j].getPosY()) < 25.f) &&
								(abs(target[i].getPosX() - target[j].getPosX()) < 25.f))
							{
								target[j].reset();
								printf("%d close to each other\n", i);
							}
						}

					}
					// Render targets to the screen
					target[i].render(&dDotTexture, *dRenderer);

					// Check if target expired
					if (target[i].targetMiss)
					{
						player.targetMiss();
						target[i].reset();
					}
				}
				renderLeftPanel();
				SDL_RenderPresent(dRenderer);
			}
		}
	}

	close();

	return 0;
}