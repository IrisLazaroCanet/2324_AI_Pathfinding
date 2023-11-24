#include "SceneTSP.h"

SceneTSP::SceneTSP()
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

	for (int i = 0; i < NUM_LOCATIONS; i++)
	{
		coinPositionIDs.push_back(graph->GetRandomNodeID());
		coinPositions.push_back(graph->GetNodeFromId(coinPositionIDs[i]).second->GetCell());
	}

	PC->SetAlgorithmToAStar();
	HC->SetFunctionToManhattan();
}

SceneTSP::~SceneTSP()
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

void SceneTSP::update(float dtime, SDL_Event* event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
	{
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;

		break;
	}
	default:
		break;
	}


	while (!coinPositionIDs.empty())
	{
		//Calculate closest node
		closestNodeID = FindClosestNodeID(agentPositionID);
		//Calculate path to the closest node
		path = PC->FindPath(graph, graph->GetNodeFromId(agentPositionID).second, graph->GetNodeFromId(closestNodeID).second);
		for (int i = 0; i < path->points.size(); i++)
		{
			agents[0]->addPathPoint(graph->CellToPix(path->points[i]));
		}

		agents[0]->update(dtime, event);

		//When the agent has arrived to the goal, update its ID to match the goal's
		agentPositionID = closestNodeID;
	}
}

void SceneTSP::draw()
{
	drawMaze();
	drawCoins();

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

const char* SceneTSP::getTitle()
{
	return "SDL Path Finding :: Traveling Salesman Problem Demo";
}

void SceneTSP::drawMaze()
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

void SceneTSP::drawCoins()
{
	for(int i = 0; i < coinPositions.size(); i++)
	{
		Vector2D coin_coords = maze->cell2pix(coinPositions[i]);
		int offset = CELL_SIZE / 2;
		SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
		SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
	}
}

bool SceneTSP::loadTextures(char* filename_bg, char* filename_coin)
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

std::string SceneTSP::FindClosestNodeID(std::string originID)
{
	float shortestDistance = -1.f;
	float calculatedDistance = -1.f;
	std::string closestNodeID;
	int closestNodeIndex = 0;

	for (int i = 0; i < coinPositionIDs.size(); i++)
	{
		calculatedDistance = HC->CalculateHeuristic(
			graph->GetNodeFromId(originID).second->GetCell(),
			graph->GetNodeFromId(coinPositionIDs[i]).second->GetCell()
		);

		if (i == 0)
		{
			shortestDistance = calculatedDistance;
			closestNodeID = coinPositionIDs[i];
		}
		else if (calculatedDistance < shortestDistance)
		{
			shortestDistance = calculatedDistance;
			closestNodeID = coinPositionIDs[i];
			closestNodeIndex = i;
		}
	}

	coinPositionIDs.erase(coinPositionIDs.begin() + closestNodeIndex);
	return closestNodeID;
}
