#include "ScenePathfindingAlgorithms.h"

ScenePathfindingAlgorithms::ScenePathfindingAlgorithms()
{
	draw_grid = false;
	maze = new Grid("../res/maze.csv");

	//TEST
	graph = new Graph(maze);
	//

	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));

	Agent* agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agent->setBehavior(new PathFollowing);
	agent->setTarget(Vector2D(-20, -20));
	agents.push_back(agent);

	// set agent position coords to the center of a random cell
	/*
	Vector2D rand_cell(-1, -1);
	while (!maze->isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
	agents[0]->setPosition(maze->cell2pix(rand_cell));
	*/

	//Set agent position as random node
	/*
	agentPositionNodeWithID = graph->GetRandomNodeWithID();
	agents[0]->setPosition(graph->CellToPix(
		agentPositionNodeWithID.second->GetCell())
	);
	*/

	agentPositionID = graph->GetRandomNodeID();
	agents[0]->setPosition(graph->CellToPix(
		graph->GetNodeFromId(agentPositionID).second->GetCell()
	));

	//Set the coin position as random node
	/*
	coinPositionNodeWithID = graph->GetRandomNodeWithID();
	coinPosition = coinPositionNodeWithID.second->GetCell();
	*/

	coinPositionID = graph->GetRandomNodeID();
	coinPosition = graph->GetNodeFromId(coinPositionID).second->GetCell();

	// set the coin in a random cell (but at least 3 cells far from the agent)
	/*
	coinPosition = Vector2D(-1, -1);
	while ((!maze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell) < 3))
		coinPosition = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
	*/

}

ScenePathfindingAlgorithms::~ScenePathfindingAlgorithms()
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

void ScenePathfindingAlgorithms::update(float dtime, SDL_Event* event)
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

	//Remove this code, in order to let the user check different algorithms with the same instance of the problem
	// if we have arrived to the coin, replace it in a random cell!
	/*
	if ((agents[0]->getCurrentTargetIndex() == -1) && (maze->pix2cell(agents[0]->getPosition()) == coinPosition))
	{
		coinPosition = Vector2D(-1, -1);
		while ((!maze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, maze->pix2cell(agents[0]->getPosition())) < 3))
			//coinPosition = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
			coinPosition = graph->GetNodeFromId(coinPositionID).second->GetCell();
	}
	*/

}

void ScenePathfindingAlgorithms::draw()
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
}

const char* ScenePathfindingAlgorithms::getTitle()
{
	return "SDL Path Finding :: PathFinding Algorithms Demo";
}

void ScenePathfindingAlgorithms::drawMaze()
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

void ScenePathfindingAlgorithms::drawCoin()
{
	Vector2D coin_coords = maze->cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

bool ScenePathfindingAlgorithms::loadTextures(char* filename_bg, char* filename_coin)
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
