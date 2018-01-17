#pragma once
#include <SDL_ttf.h>
#include <iostream>

class MDFont
{
public:
	MDFont();
	~MDFont();

	void init();

	void text(std::string);

	void setSize(TTF_Font *font);
};