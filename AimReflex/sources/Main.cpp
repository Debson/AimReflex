#include <iostream>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "../headers/Target.h"
#include "../headers/Player.h"

SDL_Window *dWindow;

SDL_Renderer *dRenderer;

MDTexture dDotTexture;

std::vector<Target> targets;

Target target[GAME_TARGETS_COUNT];
Player player;

bool init();

bool loadMedia();

void close();

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
				SDL_SetRenderDrawColor(dRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					// SDL_image not initialize
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

	return success;
}

void close()
{
	dDotTexture.free();

	SDL_DestroyRenderer(dRenderer);
	dRenderer = NULL;
	SDL_DestroyWindow(dWindow);
	dWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

void renderLeftPanel()
{
	// Left Panel
	SDL_Rect leftPanel = { SCREEN_WIDTH, 0, GAME_WIDTH - SCREEN_WIDTH, GAME_HEIGHT };
	SDL_SetRenderDrawColor(dRenderer, 0, 0, 0, 1);
	SDL_RenderFillRect(dRenderer, &leftPanel);

	// Red outline of left Panel
	SDL_Rect leftPanelOutline = { GAME_WIDTH, 0, 3, GAME_HEIGHT };
	SDL_SetRenderDrawColor(dRenderer, 255, 0, 0, 1);
	SDL_RenderFillRect(dRenderer, &leftPanelOutline);
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

				SDL_SetRenderDrawColor(dRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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