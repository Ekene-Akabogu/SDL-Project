#include "sdl.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 500;
const int IMAGE_WIDTH = SCREEN_WIDTH + 300;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
int speed = 0.1;
int CameraX = 0;
int textureWidthDiff = IMAGE_WIDTH - SCREEN_WIDTH;

SDL_Texture* loadTexture(const std::string& path) {
	// The final texture
	SDL_Texture* newTexture = NULL;

	// Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL) {
			printf("Unable to create texture from %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

bool initSDL()
{
	//Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create Window
	gWindow = SDL_CreateWindow("Image Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		printf("Windows could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create Renderer
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Set renderer color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	//Allows for (Initializes) Image Loading abilities using SDL_image Library
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	return true;
}

void closeSDL()
{
	// Destroy Window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	if (!initSDL()) {
		printf("Failed to initialize SDL!\n");
		return -1;
	}

	SDL_Texture* background = loadTexture("./Assets/background1.png");
	SDL_Texture* frame1 = loadTexture("./Assets/Frame1.png");
	SDL_Texture* frame2 = loadTexture("./Assets/Frame2.png");
	SDL_Texture* image = loadTexture("./Assets/3D_Image.png");
	SDL_Texture* Blender_Image = loadTexture("./Assets/Blender_Image.jpg");

	// Main loop flag
	bool quit = false;

	// Event handler
	SDL_Event e;

	while (!quit) {
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			// User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}


		// Clear screen
		SDL_RenderClear(gRenderer);

		SDL_RenderCopy(gRenderer, background, NULL, NULL);

		SDL_Rect frame1Dest = { 50, 150, 300, 300 };
		SDL_RenderCopy(gRenderer, frame1, NULL, &frame1Dest);

		SDL_Rect frame2Dest = { 400, 150, 300, 300 };
		SDL_RenderCopy(gRenderer, frame2, NULL, &frame2Dest);

		int imageWidth, imageHeight;
		SDL_QueryTexture(image, NULL, NULL, &imageWidth, &imageHeight);
		SDL_Rect imageDest = { frame1Dest.x + 10, frame1Dest.y + 10, frame1Dest.w - 20, frame1Dest.h - 20 };
		SDL_RenderCopy(gRenderer, image, NULL, &imageDest);

		int Blender_ImageWidth, Blender_ImageHeight;
		SDL_QueryTexture(Blender_Image, NULL, NULL, &Blender_ImageWidth, &Blender_ImageHeight);
		SDL_Rect Blender_ImageDest = { frame2Dest.x + 10, frame2Dest.y + 10, frame2Dest.w - 20, frame2Dest.h - 20 };
		SDL_RenderCopy(gRenderer, Blender_Image, NULL, &Blender_ImageDest);

		// Update screen
		SDL_RenderPresent(gRenderer);
	}

	// Destroy textures
	//SDL_DestroyTexture(enemyTexture);
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(frame1);
	SDL_DestroyTexture(frame2);
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(Blender_Image);


	// Close SDL
	closeSDL();

	return 0;
}
