// FILE: "main.cpp"

// GLOBAL HEADERS:
#include "global.h"

// STANDARD LIBRARY:
using namespace std;

// SCREEN DIMENSIONS:
const float S_WIDTH = 800;
const float S_HEIGHT = 600;

// The frames per second
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

// MOUSE:
bool LEFTMOUSE = false;
int MOUSE_X = 0;
int MOUSE_Y = 0;

// KEYBOARD:
unordered_map<SDL_Keycode, bool> KEYS;

// PROTOTYPES FUNCTIONS:
void gameMenu();
void gameStart();
void display();
void checkKeys();
void reshape(int width, int height);
bool init();
void checkCollisions();
void mouseMotion(int x, int y);
void mouse(int button, int state, int x, int y);
void update();

Menu *menu;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

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
  // Main loop flag
  bool quit = false;
  // Event handler
  SDL_Event event;
  // Frame time
  Uint32 frameStart, frameTime;
  while (!quit)
  {
    frameStart = SDL_GetTicks();

    // Event handling
    while (SDL_PollEvent(&event) != 0)
    {
      switch (event.type)
      {
      // User requests quit
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN:
        KEYS[event.key.keysym.sym] = true;
        break;
      case SDL_KEYUP:
        KEYS[event.key.keysym.sym] = false;
        break;
      }
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Render texture to screen
    display();

    // Update screen
    SDL_RenderPresent(renderer);

    // Calculate the time taken for this frame
    frameTime = SDL_GetTicks() - frameStart;

    Uint64 end = SDL_GetPerformanceCounter();

    // Delay the loop to achieve 60 FPS
    if (FRAME_DELAY > frameTime)
    {
      SDL_Delay(FRAME_DELAY - frameTime);
    }
  }
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  SDL_Quit();

  return 0;
}

// DECLARING EXTERNAL GAME VARIABLES:
HUD *hud;
Track *track;
Player *player;
Enemy *enemy;
vector<Car *> cars;
vector<Entity *> entities;

// DECLARING LOCAL GAME VARIABLES:
int lapLimit;
bool gameStarted;
bool raceStarted;
bool resetGame;
bool gameRun;
bool pause;
bool pauseKey;
Point startPosition(700, 1500);

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

vector<Point> calculatePolygonVertices(float radius, int corners)
{
  if (corners < 3)
  {
    cerr << "A polygon must have at least 3 corners." << endl;
    return {};
  }

  vector<Point> vertices;
  double angleIncrement = 2 * M_PI / corners; // Angle between consecutive vertices

  for (int i = 0; i < corners; ++i)
  {
    double x = radius * cos(i * angleIncrement);
    double y = radius * sin(i * angleIncrement);
    vertices.push_back(Point(x, y));
  }

  return vertices;
}

void gameStart()
{
  // LAP LIMIT:
  lapLimit = 3;

  // RACE STARTED FLAG:
  raceStarted = false;

  // RESET GAME FLAG:
  resetGame = false;

  vector<Point> verts;

  // vector<Point> triangleShape = calculatePolygonVertices(100, 4);
  // Entity *triangle = new Entity(verts, triangleShape, Point(S_WIDTH / 2 - 160, S_HEIGHT / 2), 0, NULL);
  // entities.push_back(triangle);

  // vector<Point> squareShape = calculatePolygonVertices(100, 3);
  // Entity *square = new Entity(verts, squareShape, Point(S_WIDTH / 2, S_HEIGHT / 2), 0, NULL);
  // entities.push_back(square);

  // vector<Point> pentagonShape = calculatePolygonVertices(100, 5);
  // Entity *pentagon = new Entity(verts, pentagonShape, Point(S_WIDTH / 2 + 160, S_HEIGHT / 2), 0, NULL);
  // entities.push_back(pentagon);

  if (!gameStarted)
  {
    // TRACK:
    track = new Track(Point(0, 0));

    // PLAYER:
    player = new Player(Point(startPosition.x, startPosition.y), -90);

    // ENEMY:
    enemy = new Enemy(startPosition - Point(200, 0), -90);

    // HUD:
    hud = new HUD();

    // STORING POINTERS IN A VECTOR:
    cars.push_back(player);
    cars.push_back(enemy);

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
    *player = Player(startPosition, -90);

    // ENEMY:
    *enemy = Enemy(startPosition - Point(200, 0), -90);

    // HUD:
    *hud = HUD();
  }
}

void checkKeys()
{
  if (gameRun)
  {
    // P:
    if (KEYS[SDLK_p])
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
    if (KEYS[SDLK_SPACE])
    {
      gameStart();
      gameRun = true;
    }
  }
}

void display()
{
  // for (size_t i = 0; i < entities.size(); ++i)
  // {
  //   for (size_t j = i + 1; j < entities.size(); ++j)
  //   {
  //     if (entities[i]->collides(*entities[j]))
  //     {
  //       entities[i]->highlighted = true;
  //       entities[j]->highlighted = true;
  //     }
  //   }
  // }

  for (Entity *entity : entities)
  {
    // entity->angle += (float)entity->shape.size() / 2;
    // entity->position.y = sin(entity->angle * (M_PI / 180)) * ((S_HEIGHT / 2) - 100) + (S_HEIGHT / 2) + 50;
    entity->draw();
    entity->highlighted = false;
  }
  // // RESET SCREEN TO BACKGROUND COLOR:
  // // glClear(GL_COLOR_BUFFER_BIT);

  if (gameRun)
  {
    // DRAW TRACK:
    track->draw();

    // DRAW CARS:
    for (int i = 0; i != cars.size(); i++)
    {
      // GET CURRENT CAR:
      Car *car = cars[i];

      // DRAW THE CURRENT CAR:
      // car->draw(Point(300, 300));
      car->draw(track->position);

      // UPDATE THE CURRENT CAR:
      if (!pause)
      {
        car->update();
      }
    }

    // UPDATE TRACK:
    if (!pause)
    {
      track->update();
    }

    // DRAW HUD:
    hud->draw();
  }
  else
  {
    // DRAWING MENU:
    menu->draw();
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
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    // Create window
    window = SDL_CreateWindow("Topdown Racing",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              S_WIDTH,
                              S_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
      SDL_Log("Window could not be created! SDL Error: %s\n",
              SDL_GetError());
      success = false;
    }
    else
    {
      // Create renderer for window
      renderer = SDL_CreateRenderer(
          window,
          -1,
          SDL_RENDERER_ACCELERATED);
      if (renderer == NULL)
      {
        SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
      }
      else
      {
        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
          SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
          success = false;
        }
        else
        {
          if (!TTF_Init())
          {
            SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
            success = false;
          }
        }
      }
    }
  }

  return success;
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
  // The final texture
  SDL_Texture *newTexture = NULL;

  // Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load(
      path.c_str());
  if (loadedSurface == NULL)
  {
    SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
  }
  else
  {
    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(
        renderer,
        loadedSurface);
    if (newTexture == NULL)
    {
      SDL_Log("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }

  return newTexture;
}

TTF_Font *loadFont(std::string path, int ptSize)
{
  TTF_Font *font = TTF_OpenFont(path.c_str(), ptSize);
  if (font == NULL)
  {
    SDL_Log("Unable to font file %s! SDL_ttf Error: %s\n", path.c_str(), TTF_GetError());
  }
  return font;
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
  box.push_back(Point(position.x, position.y + height));
  box.push_back(Point(position.x, position.y));
  box.push_back(Point(position.x + width, position.y));
  box.push_back(Point(position.x + width, position.y + height));
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
