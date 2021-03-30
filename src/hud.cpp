// FILE: "hud.cpp"

#include "hud.h"

HUD::HUD()
{
  Point center(S_WIDTH / 2, S_HEIGHT / 2);

  // BOXES:
  vector<Point> goBox      = getBox(340, 120, Point(-170, -60));
  vector<Point> oneBox     = getBox(68,  120, Point(-34,  -60));
  vector<Point> twoBox     = getBox(150, 120, Point(-75,  -60));
  vector<Point> threeBox   = getBox(150, 120, Point(-75,  -60));
  vector<Point> youWinBox  = getBox(668, 90,  Point(-334, -45));
  vector<Point> youLoseBox = getBox(684, 76,  Point(-342, -38));
  vector<Point> pausedBox  = getBox(580, 78,  Point(-290, -39));

  // CREATING OBJECTS TO DRAW:
  entities.push_back(Entity(goBox,      goBox,      center, 0, loadSurface("textures/text/GO.png")));
  entities.push_back(Entity(oneBox,     oneBox,     center, 0, loadSurface("textures/text/1.png")));
  entities.push_back(Entity(twoBox,     twoBox,     center, 0, loadSurface("textures/text/2.png")));
  entities.push_back(Entity(threeBox,   threeBox,   center, 0, loadSurface("textures/text/3.png")));
  entities.push_back(Entity(youWinBox,  youWinBox,  center, 0, loadSurface("textures/text/YOU_WIN.png")));
  entities.push_back(Entity(youLoseBox, youLoseBox, center, 0, loadSurface("textures/text/YOU_LOSE.png")));
  entities.push_back(Entity(pausedBox,  pausedBox,  center, 0, loadSurface("textures/text/PAUSED.png")));

  // ADDITIONAL VARIABLES:
  raceWon = 0;

  // FINALLY START RECORDING TIME:
  start = clock();
  timer = 0;
  seconds = 0;
}

void HUD::draw()
{
  // UPDATE TIMER:
  timer = clock() - start;

  // GETTING CURRENT TIME IN SECONDS:
  seconds = (roundUp(timer, 1000) / 1000);

  // IF GAME ISN'T PAUSED:
  if (!pause)
  {
    // IF RACE HASN'T STARTED:
    if (!raceStarted)
    {
      // SHOW DIFFERENT ENTITIES ON DIFFERENT SECONDS:
      switch (seconds)
      {
      case 1:
        // 3...
        entities[3].draw();
        break;
      case 2:
        // 2...
        entities[2].draw();
        break;
      case 3:
        // 1...
        entities[1].draw();
        break;
      case 4:
        // GO!
        entities[0].draw();

        // CARS CAN NOW MOVE:
        player->immobilized = false;
        enemy->immobilized = false;
        break;
      case 5:
        // RACE HAS BEEN STARTED:
        raceStarted = true;
        break;
      }
    }
    // IF RACE HASN'T BEEN WON YET CHECK CURRENT LAPS AGAINST LAP LIMIT:
    if (raceWon == 0)
    {
      if (player->currentLap == lapLimit)
        raceWon = 1;
      if (enemy->currentLap == lapLimit)
        raceWon = 2;
    }

    // YOU WIN!:
    if (raceWon == 1)
      entities[4].draw();

    // YOU LOSE!:
    if (raceWon == 2)
      entities[5].draw();

    // SET UP TIMER FOR RESET:
    if (raceWon != 0 && !resetGame)
    {
      start = clock();
      resetGame = true;
    }

    // WAITING 5 SECONDS AFTER THE GAME HAS ENDED:
    if (seconds == 5 && resetGame)
    {
      // RESETTING GAME:
      gameRun = false;
    }
  }
  else
  {
    // PAUSED:
    entities[6].draw();
  }

  // DRAWING CURRENT LAP:
  glLoadIdentity();
  stringstream hud;
  hud << "LAP: " << player->currentLap << "/" << lapLimit << endl;
  glColor3f(1.0f, 0.0f, 0.0f);
  STUBBED("RENDER LAP COUNTER");
  // renderBitmapString(6, S_HEIGHT - 20, GLUT_BITMAP_HELVETICA_18, hud.str().c_str());
}
