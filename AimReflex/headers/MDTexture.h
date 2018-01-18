#include <SDL_ttf.h>
#include <iostream>

class MDTexture
{
public:
	MDTexture();

	~MDTexture();

	//Loading created renderer to have access to it from class
	bool init(SDL_Renderer *renderer);

	bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
	void setFont(TTF_Font *font);
#endif
	void free();

	void render(int x, int y, float scale);

	void setRenderer(SDL_Renderer *renderer);
	SDL_Texture* getTexture();

	int getWidth();
	int getHeight();

private:
	SDL_Texture *mTexture;
	SDL_Renderer *mRenderer;
	TTF_Font *mFont;

	int mWidth;
	int mHeight;
};
