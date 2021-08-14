// FILE: "main.cpp"

// GLOBAL HEADERS:
#include "global.h"

// STANDARD LIBRARY:
using namespace std;

// SCREEN DIMENSIONS:
const float S_WIDTH = 800;
const float S_HEIGHT = 600;

//The frames per second
const int FRAMES_PER_SECOND = 20;

// MOUSE:
bool LEFTMOUSE = false;
int MOUSE_X = 0;
int MOUSE_Y = 0;

// KEYBOARD:
vector<bool> KEYS(256, false);

// PROTOTYPES FUNCTIONS:
void gameMenu();
void gameStart();
void display();
void checkKeys();
void reshape(int width, int height);
bool init();
void checkCollisions();
void keyDown(SDL_Keysym keysym);
void keyUp(SDL_Keysym keysym);
void mouseMotion(int x, int y);
void mouse(int button, int state, int x, int y);
void update();

Menu *menu;
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

// MAIN FUNCTION:
int main(int argc, char *args[])
{
  STUBBED("Create window");
  // PARSING COMMAND LINE ARGUMENTS TO GLUT:
  // glutInit(&argc, argv);

  // SETUP GLUT DISPLAY MODE AND WINDOW:
  // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  // glutInitWindowSize(S_WIDTH, S_HEIGHT);

  // WINDOW TITLE:
  STUBBED("Set window title");
  // glutCreateWindow("Topdown Racing");
  init();

  // GLUT FUNCTIONS:
  STUBBED("Set update function");
  // glutDisplayFunc(display);
  STUBBED("Set window resize function");
  // glutReshapeFunc(reshape);
  STUBBED("Set mouse button function");
  // glutMouseFunc(mouse);
  STUBBED("Set mouse move function");
  // glutPassiveMotionFunc(mouseMotion);
  STUBBED("Set render function");
  // glutIdleFunc(update);

  // GLUT KEYBOARD:
  // glutIgnoreKeyRepeat(GLUT_DEVICE_IGNORE_KEY_REPEAT);
  STUBBED("Set key down function");
  // glutKeyboardFunc(keyDown);
  STUBBED("Set key up function");
  // glutKeyboardUpFunc(keyUp);

  // INITIALIZING GAME MENU:
  // gameMenu();

  gameStart();
  gameRun = true;

  // START MAIN LOOP:
  STUBBED("Start main loop");
  //Main loop flag
  bool quit = false;
  //Event handler
  SDL_Event e;
  while (!quit)
  {
    Uint64 start = SDL_GetPerformanceCounter();
    //Clear screen
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gRenderer);

    //Render texture to screen
    display();

    //Update screen
    SDL_RenderPresent(gRenderer);

    Uint64 end = SDL_GetPerformanceCounter();

    float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
    float elapsedMS = elapsed * 1000.0f;
    // SDL_Log("Current FPS: %f\n", 1.0f / elapsed);
    SDL_Delay(floor(16.666f - elapsedMS));
    while (SDL_PollEvent(&e) != 0)
    {
      //User requests quit
      // if (e.type == SDL_QUIT)
      // {
      //   quit = true;
      // }
      if (e.type == SDL_KEYDOWN)
      {
        keyDown(e.key.keysym);
        if (e.key.keysym.sym == SDLK_SPACE) {
          quit = true;
        }
      }
      if (e.type == SDL_KEYUP)
      {
        keyUp(e.key.keysym);
      }
    }
  }
  // glutMainLoop();
  return 0;
}

// DECLARING EXTERNAL GAME VARIABLES:
HUD *hud;
Track *track;
Player *player;
Enemy *enemy;
vector<Car *> cars;

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
  gameRun = false;

  // INITIALIZING PAUSE FLAGS:
  pause = false;
  pauseKey = false;

  if (!gameStarted)
  {
    // INITIALIZE MENU:
    menu = new Menu();
  }
  else
  {
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
  resetGame = false;

  if (!gameStarted)
  {
    // TRACK:
    track = new Track(Point(0, 0));

    // PLAYER:
    player = new Player(Point(50, 150));

    // ENEMY:
    enemy = new Enemy(startPosition - Point(200, 0), 90);

    // HUD:
    hud = new HUD();

    // STORING POINTERS IN A VECTOR:
    cars.push_back(player);
    // cars.push_back(enemy);

    // GAME STARTED FLAG:
    gameStarted = true;

    // START
    player->immobilized = false;
    enemy->immobilized = false;
    // raceStarted = true;
  }
  else
  {
    // TRACK:
    *track = Track(Point(0, 0));

    // PLAYER:
    *player = Player(startPosition, 90);

    // ENEMY:
    *enemy = Enemy(startPosition - Point(200, 0), 90);

    // HUD:
    *hud = HUD();
  }
}

void checkKeys()
{
  if (gameRun)
  {
    // P:
    if (KEYS['p'] || KEYS['P'])
    {
      if (!pauseKey)
      {
        if (raceStarted && !resetGame)
        {
          pause = !pause;
        }
        pauseKey = true;
      }
    }
    else
    {
      pauseKey = false;
    }
  }
  else
  {
    // SPACE BAR:
    if (KEYS[32])
    {
      gameStart();
      gameRun = true;
    }
  }
}

void display()
{
  // RESET SCREEN TO BACKGROUND COLOR:
  // glClear(GL_COLOR_BUFFER_BIT);

  if (gameRun)
  {
    // DRAW TRACK:
    // track->draw();

    // DRAW CARS:
    for (int i = 0; i != cars.size(); i++)
    {
      // GET CURRENT CAR:
      Car *car = cars[i];

      // DRAW THE CURRENT CAR:
      car->draw(Point(300, 300));

      // UPDATE THE CURRENT CAR:
      if (!pause)
        car->update();
    }

    // UPDATE TRACK:
    // if (!pause)
    //   track->update();

    // DRAW HUD:
    // hud->draw();
  }
  else
  {
    // DRAWING MENU:
    // menu->draw();
  }

  // CHECK KEYS:
  checkKeys();

  // FLUSH AND SWAP BUFFERS:
  // glFlush();
  // STUBBED("Swap buffers");
  // glutSwapBuffers();
}

void reshape(int width, int height)
{
  // SETUP VIEWPORT:
  // glViewport(0, 0, width, height);

  // // SELECT PROJECTION MATRIX:
  // glMatrixMode(GL_PROJECTION);

  // // RESET SELECTED MATRIX:
  // glLoadIdentity();

  // // SETUP COORDINATE SYSTEM;
  // gluOrtho2D(0, S_WIDTH, 0, S_HEIGHT);

  // // SELECT MODELVIEW MATRIX:
  // glMatrixMode(GL_MODELVIEW);

  // // RESET SELECTED MATRIX:
  // glLoadIdentity();
}

bool init()
{
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    //Create window
    gWindow = SDL_CreateWindow("Topdown Racing",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               S_WIDTH,
                               S_HEIGHT,
                               SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
      SDL_Log("Window could not be created! SDL Error: %s\n",
             SDL_GetError());
      success = false;
    }
    else
    {
      //Create renderer for window
      gRenderer = SDL_CreateRenderer(
          gWindow,
          -1,
          SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL)
      {
        SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
      }
      else
      {
        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
          SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
          success = false;
        }
      }
    }
  }

  return success;
}

void keyDown(SDL_Keysym keysym)
{
  // PREVENTING KEYS GETTING LOCKED IN STATE AFTER CASE CHANGE:
  if (keysym.sym >= 65 && keysym.sym <= 90)
    KEYS[keysym.sym + 32] = true;
  if (keysym.sym >= 97 && keysym.sym <= 122)
    KEYS[keysym.sym - 32] = true;
  KEYS[keysym.sym] = true;
}

void keyUp(SDL_Keysym keysym)
{
  // PREVENTING KEYS GETTING LOCKED IN STATE AFTER CASE CHANGE:
  if (keysym.sym >= 65 && keysym.sym <= 90)
    KEYS[keysym.sym + 32] = false;
  if (keysym.sym >= 97 && keysym.sym <= 122)
    KEYS[keysym.sym - 32] = false;
  KEYS[keysym.sym] = false;
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
  STUBBED("Check mouse button state");
  // switch (button)
  // {
  // case GLUT_LEFT_BUTTON:
  //   if (state == GLUT_DOWN)
  //   {
  //     LEFTMOUSE = true;
  //   }
  //   else
  //   {
  //     LEFTMOUSE = false;
  //   }
  //   break;
  // case GLUT_RIGHT_BUTTON:
  //   break;
  // default:
  //   break;
  // }
}

void update()
{
  STUBBED("Redisplay required");
  // glutPostRedisplay();
}

SDL_Texture *loadTexture(std::string path)
{
  //The final texture
  SDL_Texture *newTexture = NULL;

  //Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load(
      path.c_str());
  if (loadedSurface == NULL)
  {
    SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
  }
  else
  {
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(
        gRenderer,
        loadedSurface);
    if (newTexture == NULL)
    {
      SDL_Log("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }

  return newTexture;
}

int roundUp(int num, int multiple)
{
  // ROUNDING A NUMBER TO A SPECIFIC MULTIPLE:
  if (multiple == 0)
    return 0;
  int add = multiple / abs(multiple);
  return ((num + multiple - add) / multiple) * multiple;
}

vector<Point> getBox(float width, float height, Point position)
{
  // GENERATING THE VERTICES OF A BOX:
  vector<Point> box;
  box.push_back(Point(position.x, position.y));
  box.push_back(Point(position.x, position.y + height));
  box.push_back(Point(position.x + width, position.y + height));
  box.push_back(Point(position.x + width, position.y));
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
    float x = even ? (radius * cos((i * angle) + (angle / 2)) + position.x) : (radius * cos(i * angle) + position.x);
    float y = even ? (radius * sin((i * angle) + (angle / 2)) + position.y) : (radius * sin(i * angle) + position.y);
    verts.push_back(Point(x, y));
  }
  return verts;
}

void renderBitmapString(float x, float y, void *font, const char *string)
{
  // RENDERING STRING TO OPENGL:
  // const char *c;
  // glLoadIdentity();
  // glRasterPos2f(x, y);
  // for (c = string; *c != '\0'; c++)
  // {
  //   STUBBED("Render character");
  //   glutBitmapCharacter(font, *c);
  // }
}
