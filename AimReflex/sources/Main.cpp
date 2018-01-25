#include <cmath>
#include <sstream>
#include <iomanip>

#include <SDL_image.h>

#include "../headers/Timer.h"
#include "../headers/Player.h"
#include "../headers/UI.h"

SDL_Window *dWindow;
SDL_Renderer *dRenderer;
TTF_Font *gFont = NULL;

MDUI UIScreen;

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

// Background color adjuster
MDTexture backgroundChangeColorText;
MDTexture bRText;
MDTexture bGText;
MDTexture bBText;
MDTexture bRNumText;
MDTexture bGNumText;
MDTexture bBNumText;
MDUI bButtonPlus[3];
MDUI bButtonMinus[3];

// Target color adjuster
MDTexture targetChangeColorText;
MDTexture tRNumText;
MDTexture tGNumText;
MDTexture tBNumText;
MDUI tButtonPlus[3];
MDUI tButtonMinus[3];

// Colors
SDL_Color backgroundColor = BACKGROUND_COLOR;
SDL_Color targetColor = TARGET_COLOR;

// Vector of targets
std::vector<Target> target;

// Player class to control game logic
Player player;

// Declare functions
bool checkHit(std::vector<Target> target);
void initRenderers();
bool init();
bool loadMedia();
void close();
void renderLeftPanel();
SDL_Color renderColorAdjuster(SDL_Color colorToChange,
	MDTexture *adjusterTitle, MDTexture *rNumText, MDTexture *gNumText, MDTexture *bNumText,
	MDUI *buttonPlus, MDUI *buttonMinus,
	int offsetX, int offsetY);

// Main function
int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Could not initialize!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Could not load media!\n");
		}
		else
		{
			SDL_Event e;
			// Start with 2 targets
			Target newTargets[START_TARGET_COUNT];
			target.push_back(newTargets[0]);
			target.push_back(newTargets[1]);

            MDTimer fps;
            int frame = 0;

			bool quit = false;
			bool quitStartScreen = false;
			bool startCounter = false;
			bool gameStarted = false;
			bool pause = false;
			bool keyAlreadyPressed = false;

			while (!quit)
			{
			    // Start timer that regulates frames per seconds
                fps.start();

				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{// If exit clicked, exit game
						quit = true;
					}
					else if (e.type == SDL_MOUSEBUTTONDOWN && UIScreen.handleStartScreenInput(&e) && !startCounter)
					{// If mouse button pressed on starting screen, start counter, !startCounter used to enter do not enter if after counter started
						startCounter = true;
					}

					if (e.key.keysym.sym == SDLK_ESCAPE && e.type == SDL_KEYDOWN && gameStarted && !keyAlreadyPressed)
					{//Pause game, key can be pressed only once
						if (pause)
						{
							pause = false;
						}
						else
						{
							pause = true;
						}
						keyAlreadyPressed = true;
					}
					else if (e.key.keysym.sym == SDLK_ESCAPE && e.type == SDL_KEYUP && gameStarted)
					{// If ESC is up, set keyAlreadyPressed to get ESC input event only once
						keyAlreadyPressed = false;
					}

					for (int i = 0; i < 3; ++i)
					{// Check if any of button was pressed, if pressed go out of for loop
						bButtonPlus[i].handleInput(&e);
						bButtonMinus[i].handleInput(&e);
						tButtonPlus[i].handleInput(&e);
						tButtonMinus[i].handleInput(&e);
						if (bButtonPlus[i].isPressed() || bButtonMinus[i].isPressed() ||
							tButtonPlus[i].isPressed() || tButtonMinus[i].isPressed())
						{
							break;
						}
					}

					if (quitStartScreen && !pause)
					{// If start counting is finished, start checking events and other properties of target
						/// If hit, dont call checkHit function in next for loop iteration
						bool targetWasHit = true;
						for (std::vector<Target>::size_type i = 0; i != target.size(); ++i)
						{
							target[i].handleInput(&e);
							if (target[i].tHit && targetWasHit)
							{
								if (checkHit(target))
								{
									player.checkTargetLifeTime(target);
									target[i].reset(target, i);
									targetWasHit = false;
								}
							}
						}
						if (e.type == SDL_MOUSEBUTTONDOWN && targetWasHit)
						{// Methods that can be called only once and when mouse button pressed
							checkHit(target);
							player.checkScore(&target);
						}
					}
				} //end Poll event while

				if (quitStartScreen && !pause)
				{// If start counting is finished, start rendering actuall game, if paused, stop rendering
					gameStarted = true;
					SDL_SetRenderDrawColor(dRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.r);
					SDL_RenderClear(dRenderer);

					for (std::vector<Target>::size_type i = 0; i != target.size(); ++i)
					{
						/// Render targets to the screen
						target[i].setTargetBordersColor(targetColor);
						target[i].render(&dTargetTexture, *dRenderer);
						target[i].renderDeathX(&dXTexture);
						/// Unpause targets timers
						target[i].unpauseTargetTimer();
						/// Check if target expired
						if (target[i].targetMiss)
						{
							player.targetMiss();
							target[i].reset(target, i);
						}
					}
					++frame;
					if(fps.getTicks() < 1000 / FRAMES_PER_SECOND)
                    {
                        SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.getTicks());
                    }
				}
				else if (pause)
				{// If ESC clicked, render pause screen
					UIScreen.renderPauseScreen(backgroundColor);
					for (std::vector<Target>::size_type i = 0; i != target.size(); ++i)
					{// Pause all timers to prevent from counting target's life time while paused
						target[i].pauseTargetTimer();
					}
				}
				else
				{
					if (startCounter)
					{// Start start screen counter
						if (UIScreen.startCounter(3000, backgroundColor))
						{// startCounter() will return true after 3000ms
							quitStartScreen = true;
						}
					}
					else
					{// Redner start screen
						UIScreen.renderStartScreen(backgroundColor);
					}
				}
				/// Render left panel with scores
				renderLeftPanel();
				for (int i = 0; i < 3; ++i)
				{// Reset buttons state on end of a frame(TODO do it more efficent?)
					bButtonPlus[i].resetButtonState();
					bButtonMinus[i].resetButtonState();
					tButtonPlus[i].resetButtonState();
					tButtonMinus[i].resetButtonState();
				}

				SDL_RenderPresent(dRenderer);
			}
		}
	}
	// Free memory
	close();

	return 0;
}

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

void initRenderers()
{
	// Set renderer to every texture
	// TODO Is it possible to do it more efficent?
	dTargetTexture.setRenderer(dRenderer);
	dTargetTexture.setRenderer(dRenderer);
	dScoreText[0].setRenderer(dRenderer);
	dScoreText[1].setRenderer(dRenderer);
	dHitsText[0].setRenderer(dRenderer);
	dHitsText[1].setRenderer(dRenderer);
	dMissesText[0].setRenderer(dRenderer);
	dMissesText[1].setRenderer(dRenderer);
	dMissedTargetsText[0].setRenderer(dRenderer);
	dMissedTargetsText[1].setRenderer(dRenderer);
	dMaxComboText[0].setRenderer(dRenderer);
	dMaxComboText[1].setRenderer(dRenderer);
	dTotalTargetsText[0].setRenderer(dRenderer);
	dTotalTargetsText[1].setRenderer(dRenderer);
	dAimingText[0].setRenderer(dRenderer);
	dAimingText[1].setRenderer(dRenderer);
	dTotalHitsText.setRenderer(dRenderer);
	dTotalMissesText.setRenderer(dRenderer);
	dTotalMissedTargetsText.setRenderer(dRenderer);
	bRText.setRenderer(dRenderer);
	bGText.setRenderer(dRenderer);
	bBText.setRenderer(dRenderer);

	UIScreen.init(dRenderer);
	// Background color adjuster
	backgroundChangeColorText.setRenderer(dRenderer);
	bRNumText.setRenderer(dRenderer);
	bGNumText.setRenderer(dRenderer);
	bBNumText.setRenderer(dRenderer);
	bButtonPlus[0].init(dRenderer);
	bButtonPlus[1].init(dRenderer);
	bButtonPlus[2].init(dRenderer);
	bButtonMinus[0].init(dRenderer);
	bButtonMinus[1].init(dRenderer);
	bButtonMinus[2].init(dRenderer);

	// Target color adjuster
	targetChangeColorText.setRenderer(dRenderer);
	tRNumText.setRenderer(dRenderer);
	tGNumText.setRenderer(dRenderer);
	tBNumText.setRenderer(dRenderer);
	tButtonPlus[0].init(dRenderer);
	tButtonPlus[1].init(dRenderer);
	tButtonPlus[2].init(dRenderer);
	tButtonMinus[0].init(dRenderer);
	tButtonMinus[1].init(dRenderer);
	tButtonMinus[2].init(dRenderer);
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
			dWindow = SDL_CreateWindow("Aim Reflex", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

			if (dWindow == NULL)
			{
				// could not create a window
				success = false;
			}
			else
			{
				dRenderer = SDL_CreateRenderer(dWindow, -1, SDL_RENDERER_ACCELERATED);
				if (dRenderer == NULL)
				{
					// could not create a renderer
					success = false;
				}
				else
				{

					SDL_SetRenderDrawColor(dRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.r);
					initRenderers();

					int imgFlags = IMG_INIT_PNG;
					if (!(IMG_Init(imgFlags) & imgFlags))
					{
						// SDL_image not initialize
						success = false;
					}

					if (TTF_Init() == -1)
					{
						printf("TTF not working");
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
		if (!dTargetTexture.loadFromFile("resources/ball1.png"))
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
	Uint8 rgbTextSize = 22;

	SDL_Color textColor = { 102, 194, 255 };
	SDL_Color red = { 255, 0, 0 };
	SDL_Color green = { 0, 255, 0 };
	SDL_Color blue = { 0, 0, 255 };

	//Set text, size and color
	dScoreText[0].renderText("Score", scoreSize, textColor);

	dHitsText[0].renderText("Hits", textSize, textColor);

	dMissesText[0].renderText("Misses", textSize, textColor);

	dMissedTargetsText[0].renderText("Missed targets", textSize, textColor);

	dMaxComboText[0].renderText("MAX combo", textSize, textColor);

	dTotalTargetsText[0].renderText("Total targets", textSize, textColor);

	dAimingText[0].renderText("Aiming", textSize, textColor);



	dScoreText[1].renderText("0", scoreNumberSize, textColor);

	dHitsText[1].renderText("0", numbersSize, textColor);

	dMissesText[1].renderText("0", numbersSize, textColor);

	dMissedTargetsText[1].renderText("0", numbersSize, textColor);

	dMaxComboText[1].renderText("0", numbersSize, textColor);

	dTotalTargetsText[1].renderText("0", numbersSize, textColor);

	dAimingText[1].renderText("0", numbersSize, textColor);

	dTotalHitsText.renderText("0", otherNumbersSize, textColor);

	dTotalMissesText.renderText("0", otherNumbersSize, textColor);

	dTotalMissedTargetsText.renderText("0", otherNumbersSize, textColor);

	bRText.renderText("R:", rgbTextSize, red);

	bGText.renderText("G:", rgbTextSize, green);

	bBText.renderText("B:", rgbTextSize, textColor);

	backgroundChangeColorText.renderText("BACKGROUND COLOR", rgbTextSize - 2, textColor);

	bRNumText.renderText("0", rgbTextSize + 3, textColor);

	bGNumText.renderText("0", rgbTextSize + 3, textColor);

	bBNumText.renderText("0", rgbTextSize + 3, textColor);

	targetChangeColorText.renderText("TARGET COLOR", rgbTextSize - 2, textColor);

	tRNumText.renderText("0", rgbTextSize + 3, textColor);

	tGNumText.renderText("0", rgbTextSize + 3, textColor);

	tBNumText.renderText("0", rgbTextSize + 3, textColor);

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
	backgroundChangeColorText.free();
	bRText.free();
	bGText.free();
	bBText.free();
	bRNumText.free();
	bGNumText.free();
	bBNumText.free();
	targetChangeColorText.free();
	tRNumText.free();
	tGNumText.free();
	tBNumText.free();

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

SDL_Color renderColorAdjuster(SDL_Color colorToChange,
							MDTexture *adjusterTitle, MDTexture *rNumText, MDTexture *gNumText,MDTexture *bNumText,
							MDUI *buttonPlus, MDUI *buttonMinus,
							int offsetX, int offsetY)
{
	SDL_Color rgbTextColor = { 0, 0, 0 };
	SDL_Color color = colorToChange;
	std::stringstream newText;

	adjusterTitle->render(GAME_WIDTH + 9 + offsetX, 275 + offsetY, 1);

	SDL_Rect blank = { GAME_WIDTH + 35 + offsetX, 293 + offsetY, 60, 14 };
	SDL_RenderFillRect(dRenderer, &blank);
	blank = { GAME_WIDTH + 35 + offsetX, 313 + offsetY, 60, 14 };
	SDL_RenderFillRect(dRenderer, &blank);
	blank = { GAME_WIDTH + 35 + offsetX, 333 + offsetY, 60, 14 };
	SDL_RenderFillRect(dRenderer, &blank);

	bRText.render(GAME_WIDTH + 29 - bRText.getWidth() + offsetX, 308 - bRText.getHeight() + offsetY, 1);
	bGText.render(GAME_WIDTH + 29 - bGText.getWidth() + offsetX, 328 - bGText.getHeight() + offsetY, 1);
	bBText.render(GAME_WIDTH + 29 - bBText.getWidth() + offsetX, 348 - bBText.getHeight() + offsetY, 1);

	rNumText->render(GAME_WIDTH + 83 - rNumText->getWidth() + offsetX, 309 - rNumText->getHeight() + offsetY, 1);
	gNumText->render(GAME_WIDTH + 83 - gNumText->getWidth() + offsetX, 329 - gNumText->getHeight() + offsetY, 1);
	bNumText->render(GAME_WIDTH + 83 - bNumText->getWidth() + offsetX, 349 - bNumText->getHeight() + offsetY, 1);

	buttonPlus[0].createButtonMinus(GAME_WIDTH + 40 + offsetX, 300 + offsetY, 0, 0);
	buttonPlus[1].createButtonMinus(GAME_WIDTH + 40 + offsetX, 320 + offsetY, 0, 0);
	buttonPlus[2].createButtonMinus(GAME_WIDTH + 40 + offsetX, 340 + offsetY, 0, 0);

	buttonMinus[0].createButtonPlus(GAME_WIDTH + 90 + offsetX, 300 + offsetY, 0, 0);
	buttonMinus[1].createButtonPlus(GAME_WIDTH + 90 + offsetX, 320 + offsetY, 0, 0);
	buttonMinus[2].createButtonPlus(GAME_WIDTH + 90 + offsetX, 340 + offsetY, 0, 0);

	// Additional variables since SDL_color is Uint8 type
	short int colorR = colorToChange.r;
	short int colorG = colorToChange.g;
	short int colorB = colorToChange.b;

	// If button pressed, change backgroundColors
	if (buttonPlus[0].isPressed())
	{
		colorR -= 20;
	}

	if (buttonPlus[1].isPressed())
	{
		colorG -= 20;
	}
	if (buttonPlus[2].isPressed())
	{
		colorB -= 20;
	}

	if (buttonMinus[0].isPressed())
	{
		colorR += 20;
	}
	if (buttonMinus[1].isPressed())
	{
		colorG += 20;
	}
	if (buttonMinus[2].isPressed())
	{
		colorB += 20;
	}

	// Check if out of color bounds
	if (colorR > 255)
	{
		colorR = 255;
	}
	else if (colorR < 0)
	{
		colorR = 0;
	}

	if (colorG > 255)
	{
		colorG = 255;
	}
	else if (colorG < 0)
	{
		colorG = 0;
	}

	if (colorB > 255)
	{
		colorB = 255;
	}
	else if (colorB < 0)
	{
		colorB = 0;
	}

	colorToChange.r = colorR;
	colorToChange.g = colorG;
	colorToChange.b = colorB;

	newText.str(std::string());
	newText << (short int)colorToChange.r;
	rNumText->loadFromRenderedText(newText.str().c_str(), rgbTextColor);

	newText.str(std::string());
	newText << (short int)colorToChange.g;
	gNumText->loadFromRenderedText(newText.str().c_str(), rgbTextColor);

	newText.str(std::string());
	newText << (short int)colorToChange.b;
	bNumText->loadFromRenderedText(newText.str().c_str(), rgbTextColor);

	return colorToChange;
}

void renderLeftPanel()
{
	SDL_Color underlineColor = { 204, 204, 204 };
	SDL_Color textColor = { 102, 194, 255 };
	//Set back render draw blend mode to NONE to prevent from blending textures below (I think so)
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
	dMaxComboText[0].render(GAME_WIDTH + 10, 160, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 160 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 160 + dHitsText[0].getHeight());

	// Total targets that appeared on game window
	dTotalTargetsText[0].render(GAME_WIDTH + 10, 185, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 185 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 185 + dHitsText[0].getHeight());

	// Aiming
	dAimingText[0].render(GAME_WIDTH + 10, 210, 1);
	SDL_SetRenderDrawColor(dRenderer, underlineColor.r, underlineColor.g, underlineColor.b, 255);
	SDL_RenderDrawLine(dRenderer, GAME_WIDTH + 10, 210 + dHitsText[0].getHeight(), GAME_WIDTH + 180, 210 + dHitsText[0].getHeight());

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
	dMaxComboText[1].render(GAME_WIDTH + 180 - dMaxComboText[1].getWidth(), 157, 1);
	dTotalTargetsText[1].render(GAME_WIDTH + 180 - dTotalTargetsText[1].getWidth(), 182, 1);
	dAimingText[1].render(GAME_WIDTH + 180 - dAimingText[1].getWidth(), 207, 1);

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

	// Create and render background color adjuster
	backgroundColor = renderColorAdjuster(backgroundColor, &backgroundChangeColorText, &bRNumText, &bGNumText,
		&bBNumText, bButtonPlus, bButtonMinus, 0, -20);

	// Create and render background color adjuster
	targetColor = renderColorAdjuster(targetColor, &targetChangeColorText, &tRNumText, &tGNumText,
		&tBNumText, tButtonPlus, tButtonMinus, 0, 80);

	// Set color of target's texture
	SDL_SetTextureBlendMode(dTargetTexture.getTexture(), SDL_BLENDMODE_BLEND);
	SDL_SetTextureColorMod(dTargetTexture.getTexture(), targetColor.r, targetColor.g, targetColor.b);

}
