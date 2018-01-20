#include "../headers/UI.h"


MDUI::MDUI()
{
	uiRenderer = NULL;
	uiFont = NULL;
	timerStarted = true;
	startScreenRect = { 0, 0, GAME_WIDTH, GAME_HEIGHT };
	textColor = { 102, 194, 255 };
	buttonX = 0;
	buttonY = 0;
	buttonSize = 16;

	buttonPressed = false;
	screenColor = { 0, 0, 0 };
}

MDUI::~MDUI()
{
	textTexture.free();
	startScreenTitle.free();
	startScreenInfo[0].free();
	startScreenInfo[1].free();
	startScreenInfo[2].free();
	startScreenInfo[3].free();
	startScreenInfoNum[0].free();
	startScreenInfoNum[1].free();
	startScreenInfoNum[2].free();
	startScreenInfoNum[3].free();
	buttonPlus.free();
	buttonMinus.free();
}

bool MDUI::init(SDL_Renderer *renderer)
{
	uiRenderer = renderer;
	if (uiFont != NULL)
	{
		textTexture.setFont(uiFont);
	}
	if (uiRenderer != NULL)
	{
		textTexture.setRenderer(uiRenderer);
		startScreenTitle.setRenderer(uiRenderer);
		startScreenInfo[0].setRenderer(uiRenderer);
		startScreenInfo[1].setRenderer(uiRenderer);
		startScreenInfo[2].setRenderer(uiRenderer);
		startScreenInfo[3].setRenderer(uiRenderer);
		startScreenInfoNum[0].setRenderer(uiRenderer);
		startScreenInfoNum[1].setRenderer(uiRenderer);
		startScreenInfoNum[2].setRenderer(uiRenderer);
		startScreenInfoNum[3].setRenderer(uiRenderer);
		buttonPlus.setRenderer(uiRenderer);
		buttonMinus.setRenderer(uiRenderer);

		if (!buttonPlus.loadFromFile("resources/plus.png"))
		{
			printf("Cant load buttonPlus texture!\n");
		}

		if (!buttonMinus.loadFromFile("resources/minus.png"))
		{
			printf("Cant load buttonPlus texture!\n");
		}

	}

	return (uiFont && uiRenderer) != NULL;
}

void MDUI::createButtonPlus(int x, int y, int w, int h)
{
	buttonX = x;
	buttonY = y;
	SDL_Rect button = { x - buttonSize / 2 , y - buttonSize / 2, buttonSize, buttonSize };
	SDL_RenderCopy(uiRenderer, buttonPlus.getTexture(), NULL, &button);
}

void MDUI::createButtonMinus(int x, int y, int w, int h)
{
	buttonX = x;
	buttonY = y;
	SDL_Rect button = { x - buttonSize / 2 , y - buttonSize / 2, buttonSize, buttonSize };
	SDL_RenderCopy(uiRenderer, buttonMinus.getTexture(), NULL, &button);
	
	//SDL_SetRenderDrawColor(uiRenderer, 0, 255, 0, 255);
	//SDL_Rect collider = { buttonX , buttonY , buttonSize, buttonSize };
	//SDL_RenderDrawRect(uiRenderer, &collider);

}

void MDUI::renderStartScreen(SDL_Color color)
{
	if (uiRenderer == NULL)
	{
		printf("Initialize renderer!\n");
	}
	else
	{
		screenColor = color;

		SDL_SetRenderDrawColor(uiRenderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(uiRenderer, &startScreenRect);

		textTexture.renderText("CLICK TO START", 25, textColor);
		startScreenTitle.renderText("Aim Reflex", 68, textColor);
		SDL_Color color = { 102, 102, 102 };
		startScreenInfo[0].renderText("Hit:     pts", 16, color);
		startScreenInfo[1].renderText("Miss:     pts", 16, color);
		startScreenInfo[2].renderText("Left target:     pts", 16, color);
		startScreenInfo[3].renderText("Press     to pause", 16, color);
		color = { 255, 255, 255 };
		startScreenInfoNum[0].renderText("+300", 16, color);
		startScreenInfoNum[1].renderText("-100", 16, color);
		startScreenInfoNum[2].renderText("-500", 16, color);
		startScreenInfoNum[3].renderText("ESC", 16, color);


		textTexture.render((GAME_WIDTH - textTexture.getWidth()) / 2, (GAME_HEIGHT - textTexture.getHeight()) / 2, 1);
		startScreenTitle.render(15, 12, 1);
		startScreenInfo[0].render(15, 70, 1);
		startScreenInfo[1].render(15, 85, 1);
		startScreenInfo[2].render(15, 100, 1);
		startScreenInfo[3].render(15, 115, 1);
		startScreenInfoNum[0].render(45, 70, 1);
		startScreenInfoNum[1].render(51, 85, 1);
		startScreenInfoNum[2].render(100, 100, 1);
		startScreenInfoNum[3].render(57, 115, 1);
	}
}

void MDUI::renderPauseScreen(SDL_Color color)
{
	screenColor = color;
	SDL_SetRenderDrawColor(uiRenderer, screenColor.r, screenColor.g, screenColor.b, 3);
	SDL_RenderFillRect(uiRenderer, &startScreenRect);

	textTexture.renderText("CLICK ESC TO UNPAUSE", 25, textColor);

	textTexture.render((GAME_WIDTH - textTexture.getWidth()) / 2, (GAME_HEIGHT - textTexture.getHeight()) / 2, 1);
}

void MDUI::handleInput(SDL_Event *e)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;

		if (x < buttonX - buttonSize / 2)
		{
			inside = false;
		}
		else if (x > buttonX + buttonSize / 2)
		{
			inside = false;
		}
		else if (y < buttonY - buttonSize / 2)
		{
			inside = false;
		}
		else if (y > buttonY + buttonSize / 2)
		{
			inside = false;
		}

		if (inside)
		{
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				buttonPressed = true;
			}
		}
		else
		{
			buttonPressed = false;
		}
	}
}

bool MDUI::handleStartScreenInput(SDL_Event *e)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	bool inside = true;

	if (x > GAME_WIDTH)
	{
		inside = false;
	}
	else if (x < 0)
	{
		inside = false;
	}
	else if (y > GAME_HEIGHT)
	{
		inside = false;
	}
	else if (y < 0)
	{
		inside = false;
	}

	if (inside && e->type == SDL_MOUSEBUTTONDOWN)
	{

	}

	return inside;
}

bool MDUI::startCounter(int time, SDL_Color color)
{
	// EDITABLE
	screenColor = color;
	SDL_SetRenderDrawColor(uiRenderer, screenColor.r, screenColor.g, screenColor.b, screenColor.a);
	SDL_RenderFillRect(uiRenderer, &startScreenRect);

	if (timerStarted)
	{
		timer.start();
		timerStarted = false;
	}
	if (timer.getTicks() > time)
	{
		timer.stop();
		return true;
	}
	if (timer.getTicks() > 2000)
	{
		textTexture.renderText("STARTING IN 1 SEC", 25, textColor);
	}
	else if (timer.getTicks() > 1000)
	{
		textTexture.renderText("STARTING IN 2 SEC", 25, textColor);
	}
	else if (timer.getTicks() > 0)
	{
		textTexture.renderText("STARTING IN 3 SEC", 25, textColor);
	}
	textTexture.render((GAME_WIDTH - textTexture.getWidth()) / 2, (GAME_HEIGHT - textTexture.getHeight()) / 2, 1);

	return false;
}

bool MDUI::isPressed()
{
	return buttonPressed;
}

void MDUI::resetButtonState()
{
	buttonPressed = false;
}
