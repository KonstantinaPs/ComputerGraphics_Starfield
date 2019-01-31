#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModel.h"
#include <stdint.h>

using namespace std;
using glm::vec3;
using glm::mat3;

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define FULLSCREEN_MODE false


/* ----------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                            */
int t;
/*Create a new variable to store the locations of all stars*/
vector<vec3> stars(1000);

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update();
void Draw(screen* screen);
void Interpolate(float a, float b, vector<float>& result);
void Interpolate3d(vec3 a, vec3 b, vector<vec3>& result);

int main(int argc, char* argv[]) {

	/*Initialize the stars 3d positions*/
	for (size_t s = 0; s<stars.size(); s++) {

		float x = float(rand()) / float(RAND_MAX);
		float y = float(rand()) / float(RAND_MAX);
		//Scaling x and y between -1 and 1
		x = 2 * x - 1;
		y = 2 * y - 1;
		float z = float(rand()) / float(RAND_MAX);

		stars[s].x = x;
		stars[s].y = y;
		stars[s].z = z;
	}

	screen *screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE);
	t = SDL_GetTicks();	/*Set start value for timer.*/

	while (NoQuitMessageSDL())
	{
		Update();
		Draw(screen);
		SDL_Renderframe(screen);
	}

	SDL_SaveImage(screen, "screenshot.bmp");

	KillSDL(screen);
	return 0;
}

/*Place your drawing here*/
void Draw(screen* screen)
{
	/* Clear buffer */
	memset(screen->buffer, 0, screen->height*screen->width * sizeof(uint32_t));

	/*The following code draws the starfield effect*/
	//vec3 white(1,1,1);

	/*Loop through all stars and set random positions*/
	for (size_t s = 0; s<stars.size(); s++) {
		/*Make the projection of the random 3d points to a 2d image plane*/
		int u = (SCREEN_HEIGHT*stars[s].x) / (2 * stars[s].z) + SCREEN_WIDTH / 2;
		int v = (SCREEN_HEIGHT*stars[s].y) / (2 * stars[s].z) + SCREEN_HEIGHT / 2;
		/*Draw the stars as white pixels*/
		vec3 color = 0.2f * vec3(1, 1, 1) / (stars[s].z*stars[s].z);
		PutPixelSDL(screen, u, v, color);
	}

}

/*Place updates of parameters here*/
void Update()
{
	/* Compute frame time */
	static int t = SDL_GetTicks();
	int t2 = SDL_GetTicks();
	float dt = float(t2 - t);
	t = t2;

	/* Update variables*/
	for (int s = 0; s<stars.size(); ++s) {
		/* Update location of stars*/
		stars[s].z = stars[s].z - 0.001*dt;

		if (stars[s].z <= 0) {
			stars[s].z += 1;
		}
		if (stars[s].z > 1) {
			stars[s].z -= 1;
		}

	}
}

/*3D Interpolation*/
void Interpolate3d(vec3 a, vec3 b, vector<vec3>& result) {
	vec3 step(0.0, 0.0, 0.0);

	step.x = (b.x - a.x) / (float)(result.size() - 1);
	step.y = (b.y - a.y) / (float)(result.size() - 1);
	step.z = (b.z - a.z) / (float)(result.size() - 1);
	for (unsigned int i = 0; i < result.size(); ++i) {
		result[i].x = a.x + i * step.x;
		result[i].y = a.y + i * step.y;
		result[i].z = a.z + i * step.z;
	}
}