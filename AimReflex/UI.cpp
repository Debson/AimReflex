#include "UI.h"


MDUI::MDUI()
{
	timerStarted = true;
	startScreenRect = { 0, 0, GAME_WIDTH, GAME_HEIGHT };
	textColor = { 102, 194, 255 };
	buttonX = 0;
	buttonY = 0;
	buttonSize = 16;
	//text = "0";
}

bool MDUI::init(SDL_Renderer *renderer)
{
	uiRenderer = renderer;
	uiFont = NULL;
	if (uiFont != NULL)
	{
		textTexture.setFont(uiFont);
	}
	if (uiRenderer != NULL)
	{
		textTexture.setRenderer(uiRenderer);
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
		startScreenColor = color;

		SDL_SetRenderDrawColor(uiRenderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(uiRenderer, &startScreenRect);

		textTexture.renderText("CLICK TO START", 25, textColor);

		textTexture.render((GAME_WIDTH - textTexture.getWidth()) / 2, (GAME_HEIGHT - textTexture.getHeight()) / 2, 1);
	}
}

void MDUI::renderPauseScreen()
{
	SDL_SetRenderDrawBlendMode(uiRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(uiRenderer, startScreenColor.r, startScreenColor.g, startScreenColor.b, 2);
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
				printf("button pressed\n");
			}
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

bool MDUI::startCounter(int time)
{
	// EDITABLE
	SDL_SetRenderDrawColor(uiRenderer, startScreenColor.r, startScreenColor.g, startScreenColor.b, startScreenColor.a);
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
/*
Uint32 MDUI::callback(Uint32 interval, void *param)
{
	return ((MDUI*) param)->startCounter(param);
}*/