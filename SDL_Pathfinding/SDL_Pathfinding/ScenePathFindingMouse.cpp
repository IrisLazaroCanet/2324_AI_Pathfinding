#include "ScenePathFindingMouse.h"

using namespace std;

ScenePathFindingMouse::ScenePathFindingMouse()
{
	draw_grid = false;
	maze = new Grid("../res/maze.csv");

	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));

	Agent* agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agent->setBehavior(new PathFollowing);
	agent->setTarget(Vector2D(-20, -20));
	agents.push_back(agent);

	//Agente 1
	Agent* otherAgent1 = new Agent;
	otherAgent1->loadSpriteTexture("../res/zombie1.png", 8);
	otherAgent1->setBehavior(new PathFollowing);
	otherAgent1->setTarget(Vector2D(0, 0));
	agents.push_back(otherAgent1);

	//Agente 2
	Agent* otherAgent2 = new Agent;
	otherAgent2->loadSpriteTexture("../res/zombie2.png", 8);
	otherAgent2->setBehavior(new PathFollowing);
	otherAgent2->setTarget(Vector2D(0, 0));
	agents.push_back(otherAgent2);

	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1, -1);
	while (!maze->isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
	agents[0]->setPosition(maze->cell2pix(rand_cell));

	// set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1, -1);
	while ((!maze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell) < 3))
		coinPosition = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

	agents[1]->setPosition(Vector2D(80, 210));
	agents[2]->setPosition(Vector2D(1200, 700));
}

ScenePathFindingMouse::~ScenePathFindingMouse()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void ScenePathFindingMouse::update(float dtime, SDL_Event* event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			Vector2D cell = maze->pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));
			if (maze->isValidCell(cell)) {
				agents[0]->addPathPoint(maze->cell2pix(cell));
			}
		}
		break;
	default:
		break;
	}
	std::cout << "Posición del agente[1]: x = " << agents[1]->getPosition().x << ", y = " << agents[1]->getPosition().y << std::endl;
	std::cout << "Posición del agente[2]: x = " << agents[2]->getPosition().x << ", y = " << agents[2]->getPosition().y << std::endl;
	movementEnemies();
	agents[0]->update(dtime, event);
	agents[1]->update(dtime, event);
	agents[2]->update(dtime, event);

	// if we have arrived to the coin, replace it in a random cell!
	if ((agents[0]->getCurrentTargetIndex() == -1) && (maze->pix2cell(agents[0]->getPosition()) == coinPosition))
	{
		coinPosition = Vector2D(-1, -1);
		while ((!maze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, maze->pix2cell(agents[0]->getPosition())) < 3))
			coinPosition = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
	}

}

void ScenePathFindingMouse::draw()
{
	drawMaze();
	drawCoin();

	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	agents[0]->draw();
	agents[1]->draw();
	agents[2]->draw();
}

const char* ScenePathFindingMouse::getTitle()
{
	return "SDL Path Finding :: PathFinding Mouse Demo";
}

void ScenePathFindingMouse::drawMaze()
{
	SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
	SDL_Rect rect;
	Vector2D coords;
	for (int j = 0; j < maze->getNumCellY(); j++)
	{
		for (int i = 0; i < maze->getNumCellX(); i++)
		{
			if (!maze->isValidCell(Vector2D((float)i, (float)j)))
			{
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
				coords = maze->cell2pix(Vector2D((float)i, (float)j)) - Vector2D((float)CELL_SIZE / 2, (float)CELL_SIZE / 2);
				rect = { (int)coords.x, (int)coords.y, CELL_SIZE, CELL_SIZE };
				SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &rect);
			}
			else {
				// Do not draw if it is not necessary (bg is already black)
			}
		}
	}
	//Alternative: render a backgroud texture:
	//SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
}

void ScenePathFindingMouse::drawCoin()
{
	Vector2D coin_coords = maze->cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}


bool ScenePathFindingMouse::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface* image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

void ScenePathFindingMouse::movementEnemies()
{
	//Agent 1 Movement
	if (agent1PathIndex == 0)
	{
		agents[1]->addPathPoint(Vector2D(240, 210));
		if (agents[1]->getPosition().x >= 236 &&
			agents[1]->getPosition().x <= 239 &&
			agents[1]->getPosition().y >= 209 &&
			agents[1]->getPosition().y <= 211)
		{
			agent1PathIndex = 1;
		}
	}

	if (agent1PathIndex == 1)
	{
		agents[1]->addPathPoint(Vector2D(242, 464));

		if (agents[1]->getPosition().x >= 241 &&
			agents[1]->getPosition().x <= 243 &&
			agents[1]->getPosition().y >= 460 &&
			agents[1]->getPosition().y <= 463)
		{
			agent1PathIndex = 2;
		}
	}

	if (agent1PathIndex == 2)
	{
		agents[1]->addPathPoint(Vector2D(1040, 464));
		if (agents[1]->getPosition().x >= 1036 &&
			agents[1]->getPosition().x <= 1039 &&
			agents[1]->getPosition().y >= 463 &&
			agents[1]->getPosition().y <= 465)
		{
			agent1PathIndex = 3;
		}
	}

	if (agent1PathIndex == 3)
	{
		agents[1]->addPathPoint(Vector2D(1040, 339));
		if (agents[1]->getPosition().x >= 1040 &&
			agents[1]->getPosition().x <= 1042 &&
			agents[1]->getPosition().y >= 340 &&
			agents[1]->getPosition().y <= 343)
		{
			agent1PathIndex = 4;
		}
	}


	if (agent1PathIndex == 4)
	{
		agents[1]->addPathPoint(Vector2D(1042, 80));
		if (agents[1]->getPosition().x >= 1041 &&
			agents[1]->getPosition().x <= 1043 &&
			agents[1]->getPosition().y >= 81 &&
			agents[1]->getPosition().y <= 83)
		{
			agent1PathIndex = 5;
		}
	}

	if (agent1PathIndex == 5)
	{
		agents[1]->addPathPoint(Vector2D(306, 100));
		if (agents[1]->getPosition().x >= 306.5 &&
			agents[1]->getPosition().x <= 309 &&
			agents[1]->getPosition().y >= 99 &&
			agents[1]->getPosition().y <= 101)
		{
			agent1PathIndex = 6;
		}
	}

	if (agent1PathIndex == 6)
	{
		agents[1]->addPathPoint(Vector2D(83, 80));
		if (agents[1]->getPosition().x >= 84 &&
			agents[1]->getPosition().x <= 86 &&
			agents[1]->getPosition().y >= 79 &&
			agents[1]->getPosition().y <= 81)
		{
			agent1PathIndex = 7;
		}
	}

	if (agent1PathIndex == 7)
	{
		agents[1]->addPathPoint(Vector2D(80, 210));
		if (agents[1]->getPosition().x >= 78 &&
			agents[1]->getPosition().x <= 80 &&
			agents[1]->getPosition().y >= 206 &&
			agents[1]->getPosition().y <= 208)
		{
			agents[1]->setPosition(Vector2D(80, 210));
			agent1PathIndex = 0;
		}
	}

	//Agent 2 Movement
	if (agent2PathIndex == 0)
	{
		agents[2]->addPathPoint(Vector2D(1072, 688));
		if (agents[2]->getPosition().x >= 1072.5 &&
			agents[2]->getPosition().x <= 1075 &&
			agents[2]->getPosition().y >= 687 &&
			agents[2]->getPosition().y <= 689)
		{
			agent2PathIndex = 1;
		}
	}

	if (agent2PathIndex == 1)
	{
		agents[2]->addPathPoint(Vector2D(1069, 590));
		if (agents[2]->getPosition().x >= 1067 &&
			agents[2]->getPosition().x <= 1069 &&
			agents[2]->getPosition().y >= 591 &&
			agents[2]->getPosition().y <= 593)
		{
			agent2PathIndex = 2;
		}
	}

	if (agent2PathIndex == 2)
	{
		agents[2]->addPathPoint(Vector2D(945, 592));
		if (agents[2]->getPosition().x >= 947 &&
			agents[2]->getPosition().x <= 949 &&
			agents[2]->getPosition().y >= 590 &&
			agents[2]->getPosition().y <= 592)
		{
			agent2PathIndex = 3;
		}
	}

	if (agent2PathIndex == 3)
	{
		agents[2]->addPathPoint(Vector2D(944, 685));
		if (agents[2]->getPosition().x >= 942 &&
			agents[2]->getPosition().x <= 944 &&
			agents[2]->getPosition().y >= 681 &&
			agents[2]->getPosition().y <= 684)
		{
			agent2PathIndex = 4;
		}
	}


	if (agent2PathIndex == 4)
	{
		agents[2]->addPathPoint(Vector2D(722, 656));
		if (agents[2]->getPosition().x >= 723.06 &&
			agents[2]->getPosition().x <= 726 &&
			agents[2]->getPosition().y >= 655 &&
			agents[2]->getPosition().y <= 658)
		{
			agent2PathIndex = 5;
		}
	}

	if (agent2PathIndex == 5)
	{
		agents[2]->addPathPoint(Vector2D(719, 464));
		if (agents[2]->getPosition().x >= 718 &&
			agents[2]->getPosition().x <= 720 &&
			agents[2]->getPosition().y >= 465 &&
			agents[2]->getPosition().y <= 467)
		{
			agent2PathIndex = 6;
		}
	}

	if (agent2PathIndex == 6)
	{
		agents[2]->addPathPoint(Vector2D(626, 464));
		if (agents[2]->getPosition().x >= 627 &&
			agents[2]->getPosition().x <= 630 &&
			agents[2]->getPosition().y >= 462 &&
			agents[2]->getPosition().y <= 464)
		{
			agent2PathIndex = 7;
		}
	}

	if (agent2PathIndex == 7)
	{
		agents[2]->addPathPoint(Vector2D(623, 340));
		if (agents[2]->getPosition().x >= 621 &&
			agents[2]->getPosition().x <= 624 &&
			agents[2]->getPosition().y >= 341 &&
			agents[2]->getPosition().y <= 344)
		{
			agent2PathIndex = 8;
		}
	}

	if (agent2PathIndex == 8)
	{
		agents[2]->addPathPoint(Vector2D(432, 336));
		if (agents[2]->getPosition().x >= 432 &&
			agents[2]->getPosition().x <= 435 &&
			agents[2]->getPosition().y >= 335 &&
			agents[2]->getPosition().y <= 357)
		{
			agent2PathIndex = 9;
		}
	}

	if (agent2PathIndex == 9)
	{
		agents[2]->addPathPoint(Vector2D(432, 200));
		if (agents[2]->getPosition().x >= 430 &&
			agents[2]->getPosition().x <= 433 &&
			agents[2]->getPosition().y >= 200.7 &&
			agents[2]->getPosition().y <= 203)
		{
			agent2PathIndex = 10;
		}
	}

	if (agent2PathIndex == 10)
	{
		agents[2]->addPathPoint(Vector2D(1210, 208));
		if (agents[2]->getPosition().x >= 1206 &&
			agents[2]->getPosition().x <= 1209 &&
			agents[2]->getPosition().y >= 206 &&
			agents[2]->getPosition().y <= 209)
		{
			agent2PathIndex = 11;
		}
	}

	if (agent2PathIndex == 11)
	{
		agents[2]->addPathPoint(Vector2D(1200, 700));
		if (agents[2]->getPosition().x >= 1199 &&
			agents[2]->getPosition().x <= 1201 &&
			agents[2]->getPosition().y >= 696 &&
			agents[2]->getPosition().y <= 699)
		{
			agents[2]->setPosition(Vector2D(1200, 700));
			agent2PathIndex = 0;
		}
	}
}
