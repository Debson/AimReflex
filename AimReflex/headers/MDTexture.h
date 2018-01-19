#include <SDL_ttf.h>
#include <iostream>

class MDTexture
{
public:
	MDTexture();

	~MDTexture();
#ifdef _SDL_TTF_H
	bool loadFont(std::string path);
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
	void setFont(TTF_Font *font);
	void renderText(std::string text, Uint8 size, SDL_Color textColor);
#endif

	void free();
	void render(int x, int y, float scale);
	void setRenderer(SDL_Renderer *renderer);

	SDL_Texture* getTexture();

	int getWidth();
	int getHeight();

	bool init(SDL_Renderer *renderer);
	bool loadFromFile(std::string path);

private:
	SDL_Texture *mTexture;
	SDL_Renderer *mRenderer;
	TTF_Font *mFont;

	int mWidth;
	int mHeight;

	bool fontOpenedOnce;

};
