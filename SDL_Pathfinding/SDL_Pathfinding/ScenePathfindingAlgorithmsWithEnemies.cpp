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


	agents[1]->setPosition(graph->CellToPix(Vector2D(2, 3)));
	agents[2]->setPosition(graph->CellToPix(Vector2D(20, 12)));


	movementEnemies1();
	movementEnemies2();
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

	agentPathTimer += dtime;

	// 77 i 143
	if (agents[1]->getPosition().x > 76 &&
		agents[1]->getPosition().x < 78 &&
		agents[1]->getPosition().y >142 &&
		agents[1]->getPosition().y < 144
		&& agentPathTimer >= 0.2
		)
	{
		movementEnemies1();
	}


	// 658 i 387
	if (agents[2]->getPosition().x > 657 &&
		agents[2]->getPosition().x < 659 &&
		agents[2]->getPosition().y >397 &&
		agents[2]->getPosition().y < 399
		&& agentPathTimer >= 0.2
		)
	{
		movementEnemies2();
	}


	if (agentPathTimer >= 0.2)
	{
		agentPathTimer = 0;
	}

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

void ScenePathfindingAlgorithmsWithEnemies::movementEnemies1()
{
	std::vector<Vector2D> points;
	points.push_back(Vector2D(2, 5));
	points.push_back(Vector2D(2, 6));
	points.push_back(Vector2D(2, 7));
	points.push_back(Vector2D(2, 8));
	points.push_back(Vector2D(2, 9));
	points.push_back(Vector2D(2, 10));
	points.push_back(Vector2D(3, 10));
	points.push_back(Vector2D(4, 10));
	points.push_back(Vector2D(5, 10));
	points.push_back(Vector2D(6, 10));
	points.push_back(Vector2D(7, 10));
	points.push_back(Vector2D(7, 11));
	points.push_back(Vector2D(7, 12));
	points.push_back(Vector2D(7, 13));
	points.push_back(Vector2D(7, 14));
	points.push_back(Vector2D(8, 14));
	points.push_back(Vector2D(9, 14));
	points.push_back(Vector2D(10, 14));
	points.push_back(Vector2D(11, 14));
	points.push_back(Vector2D(12, 14));
	points.push_back(Vector2D(13, 14));
	points.push_back(Vector2D(14, 14));
	points.push_back(Vector2D(15, 14));
	points.push_back(Vector2D(16, 14));
	points.push_back(Vector2D(17, 14));
	points.push_back(Vector2D(18, 14));
	points.push_back(Vector2D(19, 14));
	points.push_back(Vector2D(20, 14));
	points.push_back(Vector2D(21, 14));
	points.push_back(Vector2D(22, 14));
	points.push_back(Vector2D(22, 15));
	points.push_back(Vector2D(22, 16));
	points.push_back(Vector2D(22, 17));
	points.push_back(Vector2D(22, 18));
	points.push_back(Vector2D(22, 19));
	points.push_back(Vector2D(22, 20));
	points.push_back(Vector2D(23, 20));
	points.push_back(Vector2D(24, 20));
	points.push_back(Vector2D(25, 20));
	points.push_back(Vector2D(25, 21));
	points.push_back(Vector2D(26, 21));
	points.push_back(Vector2D(27, 21));
	points.push_back(Vector2D(28, 21));
	points.push_back(Vector2D(29, 21));
	points.push_back(Vector2D(29, 20));
	points.push_back(Vector2D(29, 19));
	points.push_back(Vector2D(29, 18));
	points.push_back(Vector2D(30, 18));
	points.push_back(Vector2D(31, 18));
	points.push_back(Vector2D(32, 18));
	points.push_back(Vector2D(33, 18));
	points.push_back(Vector2D(33, 19));
	points.push_back(Vector2D(33, 20));
	points.push_back(Vector2D(33, 21));
	points.push_back(Vector2D(34, 21));
	points.push_back(Vector2D(35, 21));
	points.push_back(Vector2D(36, 21));
	points.push_back(Vector2D(37, 21));
	points.push_back(Vector2D(37, 20));
	points.push_back(Vector2D(37, 19));
	points.push_back(Vector2D(37, 18));
	points.push_back(Vector2D(37, 17));
	points.push_back(Vector2D(37, 16));
	points.push_back(Vector2D(37, 15));
	points.push_back(Vector2D(37, 14));
	points.push_back(Vector2D(37, 13));
	points.push_back(Vector2D(37, 12));
	points.push_back(Vector2D(37, 11));
	points.push_back(Vector2D(37, 10));
	points.push_back(Vector2D(37, 9));
	points.push_back(Vector2D(37, 8));
	points.push_back(Vector2D(37, 7));
	points.push_back(Vector2D(37, 6));
	points.push_back(Vector2D(36, 6));
	points.push_back(Vector2D(35, 6));
	points.push_back(Vector2D(34, 6));
	points.push_back(Vector2D(33, 6));
	points.push_back(Vector2D(33, 6));
	points.push_back(Vector2D(32, 6));
	points.push_back(Vector2D(31, 6));
	points.push_back(Vector2D(30, 6));
	points.push_back(Vector2D(29, 6));
	points.push_back(Vector2D(28, 6));
	points.push_back(Vector2D(27, 6));
	points.push_back(Vector2D(26, 6));
	points.push_back(Vector2D(25, 6));
	points.push_back(Vector2D(24, 6));
	points.push_back(Vector2D(23, 6));
	points.push_back(Vector2D(22, 6));
	points.push_back(Vector2D(22, 5));
	points.push_back(Vector2D(21, 5));
	points.push_back(Vector2D(20, 5));
	points.push_back(Vector2D(19, 5));
	points.push_back(Vector2D(18, 5));
	points.push_back(Vector2D(18, 4));
	points.push_back(Vector2D(18, 3));
	points.push_back(Vector2D(18, 2));
	points.push_back(Vector2D(17, 2));
	points.push_back(Vector2D(16, 2));
	points.push_back(Vector2D(15, 2));
	points.push_back(Vector2D(14, 2));
	points.push_back(Vector2D(13, 2));
	points.push_back(Vector2D(12, 2));
	points.push_back(Vector2D(11, 2));
	points.push_back(Vector2D(10, 2));
	points.push_back(Vector2D(9, 2));
	points.push_back(Vector2D(8, 2));
	points.push_back(Vector2D(7, 2));
	points.push_back(Vector2D(6, 2));
	points.push_back(Vector2D(5, 2));
	points.push_back(Vector2D(4, 2));
	points.push_back(Vector2D(3, 2));
	points.push_back(Vector2D(2, 2));
	points.push_back(Vector2D(2, 3));
	points.push_back(Vector2D(2, 4));



	for (int i = 0; i < points.size(); i++)
	{
		agents[1]->addPathPoint(graph->CellToPix(points[i]));
	}

}


void ScenePathfindingAlgorithmsWithEnemies::movementEnemies2()
{
	std::vector<Vector2D> points2;
	points2.push_back(Vector2D(20, 11));
	points2.push_back(Vector2D(20, 10));
	points2.push_back(Vector2D(19, 10));
	points2.push_back(Vector2D(18, 10));
	points2.push_back(Vector2D(17, 10));
	points2.push_back(Vector2D(16, 10));
	points2.push_back(Vector2D(15, 10));
	points2.push_back(Vector2D(14, 10));
	points2.push_back(Vector2D(13, 10));
	points2.push_back(Vector2D(13, 9));
	points2.push_back(Vector2D(13, 8));
	points2.push_back(Vector2D(13, 7));
	points2.push_back(Vector2D(13, 6));
	points2.push_back(Vector2D(13, 6));
	points2.push_back(Vector2D(12, 6));
	points2.push_back(Vector2D(11, 6));
	points2.push_back(Vector2D(10, 6));
	points2.push_back(Vector2D(9, 6));
	points2.push_back(Vector2D(8, 6));
	points2.push_back(Vector2D(7, 6));
	points2.push_back(Vector2D(6, 6));
	points2.push_back(Vector2D(5, 6));
	points2.push_back(Vector2D(4, 6));
	points2.push_back(Vector2D(3, 6));
	points2.push_back(Vector2D(2, 6));
	points2.push_back(Vector2D(2, 7));
	points2.push_back(Vector2D(2, 8));
	points2.push_back(Vector2D(2, 9));
	points2.push_back(Vector2D(2, 10));
	points2.push_back(Vector2D(2, 11));
	points2.push_back(Vector2D(2, 12));
	points2.push_back(Vector2D(2, 13));
	points2.push_back(Vector2D(2, 14));
	points2.push_back(Vector2D(2, 15));
	points2.push_back(Vector2D(2, 16));
	points2.push_back(Vector2D(2, 17));
	points2.push_back(Vector2D(2, 18));
	points2.push_back(Vector2D(2, 19));
	points2.push_back(Vector2D(2, 20));
	points2.push_back(Vector2D(2, 21));
	points2.push_back(Vector2D(3, 21));
	points2.push_back(Vector2D(4, 21));
	points2.push_back(Vector2D(5, 21));
	points2.push_back(Vector2D(6, 21));
	points2.push_back(Vector2D(6, 20));
	points2.push_back(Vector2D(6, 19));
	points2.push_back(Vector2D(6, 18));
	points2.push_back(Vector2D(7, 18));
	points2.push_back(Vector2D(8, 18));
	points2.push_back(Vector2D(9, 18));
	points2.push_back(Vector2D(10, 18));
	points2.push_back(Vector2D(10, 19));
	points2.push_back(Vector2D(10, 20));
	points2.push_back(Vector2D(10, 21));
	points2.push_back(Vector2D(11, 21));
	points2.push_back(Vector2D(12, 21));
	points2.push_back(Vector2D(13, 21));
	points2.push_back(Vector2D(13, 20));
	points2.push_back(Vector2D(14, 20));
	points2.push_back(Vector2D(15, 20));
	points2.push_back(Vector2D(16, 20));
	points2.push_back(Vector2D(17, 20));
	points2.push_back(Vector2D(17, 19));
	points2.push_back(Vector2D(17, 18));
	points2.push_back(Vector2D(17, 17));
	points2.push_back(Vector2D(17, 16));
	points2.push_back(Vector2D(17, 15));
	points2.push_back(Vector2D(17, 14));
	points2.push_back(Vector2D(17, 13));
	points2.push_back(Vector2D(18, 13));
	points2.push_back(Vector2D(19, 13));
	points2.push_back(Vector2D(20, 13));
	points2.push_back(Vector2D(20, 12));


	for (int i = 0; i < points2.size(); i++)
	{
		agents[2]->addPathPoint(graph->CellToPix(points2[i]));
	}
}