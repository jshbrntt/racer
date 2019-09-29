// FILE: "main.cpp"

// GLOBAL HEADERS:
#include "global.h"

// STANDARD LIBRARY:
using namespace std;

// SCREEN DIMENSIONS:
const float S_WIDTH		= 800;
const float S_HEIGHT	= 600;

// MOUSE:
bool LEFTMOUSE	= false;
int	MOUSE_X		= 0;
int MOUSE_Y		= 0;

// KEYBOARD:
vector<bool> KEYS(256, false);

// PROTOTYPES FUNCTIONS:
void gameMenu();
void gameStart();
void display();
void checkKeys();
void reshape(int width, int height);
void init();
void checkCollisions();
void keyDown (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);
void mouseMotion(int x, int y);
void mouse(int button, int state, int x, int y);
void update();

// MAIN FUNCTION:
int main(int argc, char **argv)
{
	// PARSING COMMAND LINE ARGUMENTS TO GLUT:
	glutInit(&argc, argv);

	// SETUP GLUT DISPLAY MODE AND WINDOW:
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(S_WIDTH, S_HEIGHT);

	// WINDOW TITLE:
	glutCreateWindow("Topdown Racing");
	init();

	// GLUT FUNCTIONS:
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMotion);
	glutIdleFunc(update);

	// GLUT KEYBOARD:
	glutIgnoreKeyRepeat(GLUT_DEVICE_IGNORE_KEY_REPEAT);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	// INITIALIZING GAME MENU:
	gameMenu();

	// START MAIN LOOP:
	glutMainLoop();
}

// DECLARING EXTERNAL GAME VARIABLES:
Menu* menu;
HUD* hud;
Track* track;
Player* player;
Enemy* enemy;
vector<Car*> cars;

// DECLARING LOCAL GAME VARIABLES:
int lapLimit;
bool gameStarted;
bool raceStarted;
bool resetGame;
bool gameRun;
bool pause;
bool pauseKey;
Point startPosition(700, 2100);

void gameMenu()
{
	// STOP GAME FROM RUNNING:
	gameRun		= false;

	// INITIALIZING PAUSE FLAGS:
	pause		= false;
	pauseKey	= false;
	
	if(!gameStarted)
	{
		// INITIALIZE MENU:
		menu = new Menu();
	} else {
		// RESET MENU:
		*menu = Menu();
	}
}

void gameStart()
{
	// LAP LIMIT:
	lapLimit = 3;

	// RACE STARTED FLAG:
	raceStarted = false;

	// RESET GAME FLAG:
	resetGame	= false;

	if(!gameStarted)
	{
		// TRACK:
		track	= new Track(Point(0, 0));

		// PLAYER:
		player	= new Player(startPosition, 90);

		// ENEMY:
		enemy	= new Enemy(startPosition - Point(200, 0), 90);

		// HUD:
		hud		= new HUD();

		// STORING POINTERS IN A VECTOR:
		cars.push_back(player);
		cars.push_back(enemy);

		// GAME STARTED FLAG:
		gameStarted = true;
	} else {
		// TRACK:
		*track		= Track(Point(0, 0));

		// PLAYER:
		*player		= Player(startPosition, 90);

		// ENEMY:
		*enemy		= Enemy(startPosition - Point(200, 0), 90);

		// HUD:
		*hud		= HUD();
	}
}

void checkKeys()
{
	if(gameRun)
	{
		// P:
		if(KEYS['p'] || KEYS['P'])
		{
			if(!pauseKey)
			{
				if(raceStarted && !resetGame)
				{
					pause = !pause;
				}
				pauseKey = true;
			}
		} else {
			pauseKey = false;
		}
	} else {
		// SPACE BAR:
		if(KEYS[32])
		{
			gameStart();
			gameRun = true;
		}
	}
}

void display()
{
	// RESET SCREEN TO BACKGROUND COLOR:
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(gameRun)
	{
		// DRAW TRACK:
		track->draw();
	
		// DRAW CARS:
		for(int i = 0; i != cars.size(); i++)
		{
			// GET CURRENT CAR:
			Car* car = cars[i];

			// DRAW THE CURRENT CAR:
			car->draw(track->position);
			
			// UPDATE THE CURRENT CAR:
			if(!pause)	car->update();
		}

		// UPDATE TRACK:
		if(!pause)	track->update();

		// DRAW HUD:
		hud->draw();
	} else {
		// DRAWING MENU:
		menu->draw();
	}

	// CHECK KEYS:
	checkKeys();
	
	// FLUSH AND SWAP BUFFERS:
	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	// SETUP VIEWPORT:
	glViewport(0, 0, width, height);

	// SELECT PROJECTION MATRIX:
	glMatrixMode(GL_PROJECTION);

	// RESET SELECTED MATRIX:
	glLoadIdentity();

	// SETUP COORDINATE SYSTEM;
	gluOrtho2D(0, S_WIDTH, 0 , S_HEIGHT); 

	// SELECT MODELVIEW MATRIX:
	glMatrixMode(GL_MODELVIEW);

	// RESET SELECTED MATRIX:
	glLoadIdentity();
}
void init()
{
	// SET BACKGROUND COLOR TO BLACK:
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void keyDown (unsigned char key, int x, int y)
{
	// PREVENTING KEYS GETTING LOCKED IN STATE AFTER CASE CHANGE:
	if(key >= 65 && key <= 90)	KEYS[key+32] = true;
	if(key >= 97 && key <= 122)	KEYS[key-32] = true;
	KEYS[key] = true;
}

void keyUp (unsigned char key, int x, int y)
{
	// PREVENTING KEYS GETTING LOCKED IN STATE AFTER CASE CHANGE:
	if(key >= 65 && key <= 90)	KEYS[key+32] = false;
	if(key >= 97 && key <= 122)	KEYS[key-32] = false;
	KEYS[key] = false;
}

void mouseMotion(int x, int y)
{
	// CURRENT MOUSE POSITION:
	MOUSE_Y = S_HEIGHT - y;
	MOUSE_X = x;
}

void mouse(int button, int state, int x, int y)
{
	// DETECTING MOUSE BUTTON PRESSES:
	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN)
			{
				LEFTMOUSE = true;
			} else {
				LEFTMOUSE = false;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			break;
		default:
			break;
	}
}

void update()
{
	glutPostRedisplay();
}

GLuint PNGtoGLtexture(const char *filename)
{
	// IMPORTING PNG INTO OPENGL:
	GLuint textureID = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	if(textureID == 0)
	{
		printf("SOIL loading error: '%s'\n", filename);
		exit(1);
	}
	return textureID;
}

int roundUp(int num, int multiple)
{
	// ROUNDING A NUMBER TO A SPECIFIC MULTIPLE:
	if(multiple == 0) return 0;
	int add = multiple / abs(multiple);
	return ((num + multiple - add) / multiple) * multiple;
}

vector<Point> getBox(float width, float height, Point position)
{
	// GENERATING THE VERTICES OF A BOX:
	vector<Point> box;
	box.push_back(Point(position.x,			position.y));
	box.push_back(Point(position.x,			position.y + height));
	box.push_back(Point(position.x + width,	position.y + height));
	box.push_back(Point(position.x + width,	position.y));
	return box;
}

vector<Point> getPoly(int sides, float radius, Point position)
{
	// GENERATING THE VERTICES OF A CONVEX POLYGON:
	float angle = (M_PI * 2) / sides;
	bool even = (sides % 2 == 0) ? true : false;
	vector<Point> verts;
	for (int i = 0; i < sides; i++)
	{
		float x	= even ? (radius * cos((i * angle) + (angle / 2)) + position.x) : (radius * cos(i * angle) + position.x);
		float y	= even ? (radius * sin((i * angle) + (angle / 2)) + position.y) : (radius * sin(i * angle) + position.y);
		verts.push_back(Point(x, y));
	}
	return verts;
}

void renderBitmapString(float x, float y, void* font, const char* string)
{
	// RENDERING STRING TO OPENGL:
	const char *c;
	glLoadIdentity();
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}