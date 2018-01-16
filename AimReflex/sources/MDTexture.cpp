#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "../headers/MDTexture.h"

MDTexture::MDTexture()
{
	mTexture = NULL;
	mRenderer = NULL;

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

void MDTexture::render(int x, int y, float scale, SDL_Renderer *renderer)
{
	mRenderer = renderer;
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	renderQuad.w *= scale;
	renderQuad.h *= scale;

	SDL_RenderCopy(renderer, mTexture, NULL, &renderQuad);
}

int MDTexture::getWidth()
{
	return mWidth;
}

int MDTexture::getHeight()
{
	return mHeight;
}