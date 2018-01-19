#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "../headers/MDTexture.h"

MDTexture::MDTexture()
{
	mRenderer = NULL;
	mFont = NULL;
	mTexture = NULL;
	
	fontOpenedOnce = false;

	mWidth = 0;
	mHeight = 0;
}

MDTexture::~MDTexture()
{
	free();
}

bool MDTexture::init(SDL_Renderer *renderer)
{
	mRenderer = renderer;

	return mRenderer != NULL;
}

bool MDTexture::loadFromFile(std::string path)
{
	free();

	SDL_Texture *newTexture = NULL;

	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("could not load PNG");
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0x00, 0x00));

		newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			// could not create texture from surface
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;

	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool MDTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	free();

	SDL_Surface *textSurface = TTF_RenderText_Solid(mFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("text error text error text error text error\n");
	}
	else
	{
		if (mRenderer == NULL)
		{
			printf("lol");
			getchar();
		}
		mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("text surface error text surface error text surface error \n");
			getchar();
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}

	return mTexture != NULL;
}

void MDTexture::setFont(TTF_Font *font)
{
	mFont = font;
}

void MDTexture::renderText(std::string text, Uint8 size, SDL_Color textColor)
{
	if (!fontOpenedOnce)
	{//Open font only once in case this method is called multiple times in other function(performance issues)
		mFont = TTF_OpenFont("resources/ProggySmall.ttf", size);
		setFont(mFont);
		fontOpenedOnce = true;
	}

	if (!loadFromRenderedText(text, textColor))
	{
		printf("Unable to load texture\n");
	}
}
#endif

void MDTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void MDTexture::render(int x, int y, float scale)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	renderQuad.w *= scale;
	renderQuad.h *= scale;

	SDL_RenderCopy(mRenderer, mTexture, NULL, &renderQuad);
}

void MDTexture::setRenderer(SDL_Renderer *renderer)
{
	mRenderer = renderer;
}

SDL_Texture* MDTexture::getTexture()
{
	return mTexture;
}

int MDTexture::getWidth()
{
	return mWidth;
}

int MDTexture::getHeight()
{
	return mHeight;
}