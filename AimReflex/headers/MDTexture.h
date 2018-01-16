
class MDTexture
{
public:
	MDTexture();

	~MDTexture();

	//Loading created renderer to have access to it from class
	bool init(SDL_Renderer *renderer);

	bool loadFromFile(std::string path);

	void free();

	void render(int x, int y, float scale, SDL_Renderer *renderer);

	int getWidth();
	int getHeight();

private:
	SDL_Texture *mTexture;
	SDL_Renderer *mRenderer;

	int mWidth;
	int mHeight;
};
