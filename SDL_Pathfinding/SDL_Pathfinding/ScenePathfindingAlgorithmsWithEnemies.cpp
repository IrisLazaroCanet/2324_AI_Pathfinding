#include "ScenePathfindingAlgorithmsWithEnemies.h"

ScenePathfindingAlgorithmsWithEnemies::ScenePathfindingAlgorithmsWithEnemies()
{
	draw_grid = false;
	maze = new Grid("../res/maze.csv");
	graph = new Graph(maze);

	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));

	Agent* agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agent->setBehavior(new PathFollowing);
	agent->setTarget(Vector2D(-20, -20));
	agents.push_back(agent);

	agentPositionID = graph->GetRandomNodeID();
	agents[0]->setPosition(graph->CellToPix(
		graph->GetNodeFromId(agentPositionID).second->GetCell()
	));

	coinPositionID = graph->GetRandomNodeID();
	coinPosition = graph->GetNodeFromId(coinPositionID).second->GetCell();

	//Agente 1
	Agent* otherAgent1 = new Agent;
	otherAgent1->loadSpriteTexture("../res/soldier.png", 4);
	otherAgent1->setBehavior(new PathFollowing);
	otherAgent1->setTarget(Vector2D(0, 0));
	agents.push_back(otherAgent1);

	//Agente 2
	Agent* otherAgent2 = new Agent;
	otherAgent2->loadSpriteTexture("../res/soldier.png", 4);
	otherAgent2->setBehavior(new PathFollowing);
	otherAgent2->setTarget(Vector2D(0, 0));
	agents.push_back(otherAgent2);


	agents[1]->setPosition(graph->CellToPix(Vector2D(8, 2)));
	std::vector<Vector2D> points;
	points.push_back(Vector2D(8, 3));
	points.push_back(Vector2D(8, 4));
	points.push_back(Vector2D(8, 5));
	for (int i = 0; i < points.size(); i++)
	{
		agents[1]->addPathPoint(graph->CellToPix(points[i]));
	}

	agents[2]->setPosition(Vector2D(1200, 700));
}

ScenePathfindingAlgorithmsWithEnemies::~ScenePathfindingAlgorithmsWithEnemies()
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

void ScenePathfindingAlgorithmsWithEnemies::update(float dtime, SDL_Event* event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
	{
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;

		if (event->key.keysym.scancode == SDL_SCANCODE_A)
		{
			//A*
			PC->SetAlgorithmToAStar();
			Path* newPath = PC->FindPath(graph, graph->GetNodeFromId(agentPositionID).second, graph->GetNodeFromId(coinPositionID).second);
			for (int i = 0; i < newPath->points.size(); i++)
			{
				agents[0]->addPathPoint(graph->CellToPix(newPath->points[i]));
			}
		}

		if (event->key.keysym.scancode == SDL_SCANCODE_B)
		{
			//BFS
			PC->SetAlgorithmToBFS();
			Path* newPath = PC->FindPath(graph, graph->GetNodeFromId(agentPositionID).second, graph->GetNodeFromId(coinPositionID).second);
			for (int i = 0; i < newPath->points.size(); i++)
			{
				agents[0]->addPathPoint(graph->CellToPix(newPath->points[i]));
			}
		}

		if (event->key.keysym.scancode == SDL_SCANCODE_D)
		{
			//Dijkstra
			PC->SetAlgorithmToDijkstra();
			Path* newPath = PC->FindPath(graph, graph->GetNodeFromId(agentPositionID).second, graph->GetNodeFromId(coinPositionID).second);
			for (int i = 0; i < newPath->points.size(); i++)
			{
				agents[0]->addPathPoint(graph->CellToPix(newPath->points[i]));
			}
		}

		if (event->key.keysym.scancode == SDL_SCANCODE_G)
		{
			//Greedy
			PC->SetAlgorithmToGreedy();
			Path* newPath = PC->FindPath(graph, graph->GetNodeFromId(agentPositionID).second, graph->GetNodeFromId(coinPositionID).second);
			for (int i = 0; i < newPath->points.size(); i++)
			{
				agents[0]->addPathPoint(graph->CellToPix(newPath->points[i]));
			}
		}
		break;
	}
	default:
		break;
	}

	agents[0]->update(dtime, event);

	//If we have arrived to the coin, reset the agent's position	
	if ((agents[0]->getCurrentTargetIndex() == -1) && (maze->pix2cell(agents[0]->getPosition()) == coinPosition))
	{
		agents[0]->setPosition(graph->CellToPix(
			graph->GetNodeFromId(agentPositionID).second->GetCell()
		));
	}

	//movementEnemies();
	agents[1]->update(dtime, event);
	agents[2]->update(dtime, event);
}

void ScenePathfindingAlgorithmsWithEnemies::draw()
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

const char* ScenePathfindingAlgorithmsWithEnemies::getTitle()
{
	return "SDL Path Finding :: PathFinding Algorithms Demo";
}

void ScenePathfindingAlgorithmsWithEnemies::drawMaze()
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

void ScenePathfindingAlgorithmsWithEnemies::drawCoin()
{
	Vector2D coin_coords = maze->cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

bool ScenePathfindingAlgorithmsWithEnemies::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface* image = IMG_Load(filename_bg);
	if (!image) {
		std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

void ScenePathfindingAlgorithmsWithEnemies::movementEnemies()
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
