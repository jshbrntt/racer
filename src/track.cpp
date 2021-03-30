// FILE: "track.cpp"

#include "track.h"

Track::Track() {}
Track::Track(Point position)
{
  // POSITION PARSING:
  this->position = position;

  // TILE PARAMETERS:
  tileSize = 200;

  // IMPORTING TILE TEXTURES:
  textures.push_back(loadSurface("textures/tiles/universal/track.png"));   // 0:  Track
  textures.push_back(loadSurface("textures/tiles/grass/bound.png"));       // 1:  Bound
  textures.push_back(loadSurface("textures/tiles/universal/lineh.png"));   // 2:  LineH
  textures.push_back(loadSurface("textures/tiles/universal/linev.png"));   // 3:  LineV
  textures.push_back(loadSurface("textures/tiles/grass/topleft.png"));     // 4:  Top-Left
  textures.push_back(loadSurface("textures/tiles/grass/topright.png"));    // 5:  Top-Right
  textures.push_back(loadSurface("textures/tiles/grass/bottomleft.png"));  // 6:  Bottom-Left
  textures.push_back(loadSurface("textures/tiles/grass/bottomright.png")); // 7:  Bottom-Right
  textures.push_back(loadSurface("textures/tiles/grass/left.png"));        // 8:  Left
  textures.push_back(loadSurface("textures/tiles/grass/right.png"));       // 9:  Right
  textures.push_back(loadSurface("textures/tiles/grass/bottom.png"));      // 10: Bottom
  textures.push_back(loadSurface("textures/tiles/grass/top.png"));         // 11: Top
  textures.push_back(loadSurface("textures/tiles/universal/turbo.png"));   // 12: Turbo

  // AI MAP:
  aiMap.push_back(Point(3,  5));
  aiMap.push_back(Point(5,  3)); // NO TURBO
  aiMap.push_back(Point(12, 7));
  aiMap.push_back(Point(18, 6));
  aiMap.push_back(Point(21, 6));
  aiMap.push_back(Point(25, 9));
  aiMap.push_back(Point(24, 14));
  aiMap.push_back(Point(21, 15));
  aiMap.push_back(Point(19, 15));
  aiMap.push_back(Point(15, 13)); // NO TURBO
  aiMap.push_back(Point(13, 13));
  aiMap.push_back(Point(9,  15));
  aiMap.push_back(Point(4,  14)); // NO TURBO
  aiMap.push_back(Point(3,  10));

  // TURBO MAP:
  turboMap.push_back(Point(0,  0));
  turboMap.push_back(Point(6,  2)); // TURBO
  turboMap.push_back(Point(0,  0));
  turboMap.push_back(Point(0,  0));
  turboMap.push_back(Point(0,  0));
  turboMap.push_back(Point(0,  0));
  turboMap.push_back(Point(0,  0));
  turboMap.push_back(Point(0,  0));
  turboMap.push_back(Point(0,  0));
  turboMap.push_back(Point(16, 12)); // TURBO
  turboMap.push_back(Point(0,  0));
  turboMap.push_back(Point(0,  0));
  turboMap.push_back(Point(5,  13)); // TURBO
  turboMap.push_back(Point(0,  0));

  // TRACK LAYOUT:
  const int tW = 28;
  const int tH = 18;

  // TRACK TILE MAP:
  int tileMap[tH][tW] = {
      {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  //  0
      {1,  1,  1,  1,  1, 11, 11, 11,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  //  1
      {1,  1,  1,  1,  4,  0, 12,  0,  5,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  //  2
      {1,  1,  1,  4,  0,  0,  0,  0,  0,  5,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  //  3
      {1,  1,  4,  0,  0,  7, 10,  6,  0,  0,  5,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  //  4
      {1,  8,  0,  0,  7,  1,  1,  1,  6,  0,  0,  5,  1,  1,  1,  1, 11, 11, 11, 11, 11, 11, 11,  1,  1,  1,  1,  1},  //  5
      {1,  8,  0,  0,  9,  1,  1,  1,  1,  6,  0,  0,  5, 11, 11,  4,  0,  0,  0,  0,  0,  0,  0,  5,  1,  1,  1,  1},  //  6
      {1,  8,  2,  2,  9,  1,  1,  1,  1,  1,  6,  0,  0,  0,  0,  0,  7, 10, 10, 10, 10, 10,  6,  0,  5,  1,  1,  1},  //  7
      {1,  8,  0,  0,  9,  1,  1,  1,  1,  1,  1, 10, 10, 10, 10, 10,  1,  1,  1,  1,  1,  1,  1,  6,  0,  5,  1,  1},  //  8
      {1,  8,  0,  0,  5,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  6,  0,  9,  1},  //  9
      {1,  1,  6,  0,  0,  9,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  8,  0,  9,  1},  // 10
      {1,  1,  8,  0,  0,  9,  1,  1,  1,  1,  1,  1,  1, 11, 11, 11, 11,  1,  1,  1,  1,  1,  1,  1,  8,  0,  9,  1},  // 11
      {1,  1,  8,  0,  0,  5,  1,  1,  1,  1,  1,  1,  4,  0,  0,  0, 12,  5,  1,  1,  1,  1,  1,  1,  4,  0,  9,  1},  // 12
      {1,  1,  8,  0,  0, 12,  5, 11,  1,  1,  1,  4,  0,  0,  0,  0,  0,  0,  5,  1,  1,  1,  1,  4,  0,  0,  9,  1},  // 13
      {1,  1,  1,  6,  0,  0,  0,  0,  5, 11,  4,  0,  0,  7, 10,  6,  0,  0,  0,  5, 11, 11,  4,  0,  0,  7,  1,  1},  // 14
      {1,  1,  1,  1,  6,  0,  0,  0,  0,  0,  0,  0,  7,  1,  1,  1, 10,  6,  0,  0,  0,  0,  0,  0,  7,  1,  1,  1},  // 15
      {1,  1,  1,  1,  1, 10, 10, 10, 10, 10, 10, 10,  1,  1,  1,  1,  1,  1, 10, 10, 10, 10, 10, 10,  1,  1,  1,  1},  // 16
      {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1}}; // 17
    /* 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 */

  // TILE SHAPES:
  vector<Point> square = getBox(tileSize, tileSize);
  vector<Point> smallOctagon = getPoly(8, tileSize / 4, Point(tileSize / 2, tileSize / 2));

  vector<Point> bottomLeft;
  bottomLeft.push_back(Point(0, 0));
  bottomLeft.push_back(Point(0, tileSize));
  bottomLeft.push_back(Point(tileSize, 0));

  vector<Point> topLeft;
  topLeft.push_back(Point(0, 0));
  topLeft.push_back(Point(0, tileSize));
  topLeft.push_back(Point(tileSize, tileSize));

  vector<Point> topRight;
  topRight.push_back(Point(0, tileSize));
  topRight.push_back(Point(tileSize, tileSize));
  topRight.push_back(Point(tileSize, 0));

  vector<Point> bottomRight;
  bottomRight.push_back(Point(0, 0));
  bottomRight.push_back(Point(tileSize, tileSize));
  bottomRight.push_back(Point(tileSize, 0));

  // PARSING TILE MAP ARRAY TO A VECTOR:
  for (int i = 0; i < tH; i++)
  {
    vector<int> tileRow;
    vector<Entity> mapRow;
    for (int j = 0; j < tW; j++)
    {
      if ((tileMap[i][j] < int(textures.size())) && (tileMap[i][j] >= 0))
      {
        tileRow.push_back(tileMap[i][j]);
        switch (tileMap[i][j])
        {
        // 4:  Top-Left
        // 5:  Top-Right
        // 6:  Bottom-Left
        // 7:  Bottom-Right
        // 12: Turbo
        default:
          mapRow.push_back(Entity(square, square, Point(position.x + (j * tileSize), position.y + (((tH - 1) - i) * tileSize)), 0, textures[tileMap[i][j]]));
          break;
        case 4:
          mapRow.push_back(Entity(square, topLeft, Point(position.x + (j * tileSize), position.y + (((tH - 1) - i) * tileSize)), 0, textures[tileMap[i][j]]));
          break;
        case 5:
          mapRow.push_back(Entity(square, topRight, Point(position.x + (j * tileSize), position.y + (((tH - 1) - i) * tileSize)), 0, textures[tileMap[i][j]]));
          break;
        case 6:
          mapRow.push_back(Entity(square, bottomLeft, Point(position.x + (j * tileSize), position.y + (((tH - 1) - i) * tileSize)), 0, textures[tileMap[i][j]]));
          break;
        case 7:
          mapRow.push_back(Entity(square, bottomRight, Point(position.x + (j * tileSize), position.y + (((tH - 1) - i) * tileSize)), 0, textures[tileMap[i][j]]));
          break;
        case 12:
          mapRow.push_back(Entity(square, smallOctagon, Point(position.x + (j * tileSize), position.y + (((tH - 1) - i) * tileSize)), 0, textures[tileMap[i][j]]));
          break;
        }
      }
      else
      {
        cout << "Tile-Map Parsing ERROR: " << i << " " << j << endl;
        system("PAUSE");
        exit(1);
      }
    }
    tiles.push_back(tileRow);
    map.push_back(mapRow);
  }
#if DEBUG == 1
  printAIMap();
#endif
}

void Track::draw()
{
  // ITERATING THROUGH ROWS:
  for (int i = 0; i != map.size(); i++)
  {
    // CURRENT ROW OF THE TILE MAP:
    vector<Entity> row = map[i];

    // ITERATING THROUGH TILES IN CURRENT ROW:
    for (int j = 0; j != row.size(); j++)
    {
      // GETTING TILE'S POSITION RELATIVE TO THE VIEWPORT:
      Point relative = row[j].position + position;

      //USE TO DEMONSTRATE CLIPPING WORKS:
      int offset = 0;

      // CLIPPING TILES OUT OF VIEWPORT:
      if ((relative.x + tileSize >= offset && relative.x <= S_WIDTH - offset) &&
          (relative.y + tileSize >= offset && relative.y <= S_HEIGHT - offset))
      {
        row[j].draw(position);
      }
    }
  }
}

void Track::update()
{
  // UPDATE TRACK POSITION RELATIVE TO THE PLAYER'S POSITION:
  track->position = Point(player->position.x - (S_WIDTH / 2), player->position.y - (S_HEIGHT / 2)) * -1;
  //track->position = Point(enemy->position.x - (S_WIDTH / 2), enemy->position.y - (S_HEIGHT / 2)) * -1;
}

Point previousIndex = Point(0, 0);
Point Track::getTileIndex(Point position)
{
  // INITIALIZING VARIABLES:
  int mapWidth = tiles[0].size();
  int mapHeight = tiles.size();
  Point index((roundUp(position.x, tileSize) / tileSize) - 1,
              mapHeight - (roundUp(position.y, tileSize) / tileSize));

  // CHECKING MAP BOUNDARIES:
  if ((index.x < 0 || index.x > (mapWidth - 1)) ||
      (index.y < 0 || index.y > (mapHeight - 1)))
  {
    // IF POSITION IS OUT OF BOUNDS RETURN LAST POSITION:
    index = previousIndex;
  }

  // SAVE CURRENT INDEX FOR OUT OF BOUNDS OCCURENCES:
  previousIndex = index;

  // RETURN TILE INDEX:
  return index;
}

void Track::printAIMap()
{
  // CLEAR COMMAND PROMPT WINDOW:
  system("CLS");

  // -1: Target
  //  0: Track
  //  1: Bound
  //  2: LineH
  //  3: LineV
  //  4: Top-Left
  //  5: Top-Right
  //  6: Bottom-Left
  //  7: Bottom-Right
  //  8: Left
  //  9: Right
  // 10: Bottom
  // 11: Top
  // 12: Turbo

  // DRAW TILEMAP AND MARK POSITION:
  stringstream asciiMap;
  vector<vector<int>> copyTiles = tiles;

  // MARKING AI MAP:
  for (int i = 0; i != aiMap.size(); i++)
  {
    copyTiles[aiMap[i].y][aiMap[i].x] = -1;
    ;
  }

  // DRAWING ASCII MAP:
  for (int i = 0; i != copyTiles.size(); i++)
  {
    vector<int> row = copyTiles[i];
    for (int j = 0; j != row.size(); j++)
    {
      switch (row[j])
      {
      default:
        asciiMap << " ";
        break;
      case -1:
        asciiMap << "X";
        break;
      case 0:
        asciiMap << " ";
        break;
      case 1:
        asciiMap << "#";
        break;
      case 2:
        asciiMap << "-";
        break;
      case 3:
        asciiMap << "|";
        break;
      case 4:
        asciiMap << "/";
        break;
      case 5:
        asciiMap << "\\";
        break;
      case 6:
        asciiMap << "\\";
        break;
      case 7:
        asciiMap << "/";
        break;
      case 8:
        asciiMap << "|";
        break;
      case 9:
        asciiMap << "|";
        break;
      case 10:
        asciiMap << "-";
        break;
      case 11:
        asciiMap << "-";
        break;
      case 12:
        asciiMap << "O";
        break;
      }
    }
    asciiMap << "\n";
  }
  SDL_Log("%s\n", asciiMap.str().c_str());
}
