#include <states/parts/MainGameState.h>
#include <critter/CritterGroup.h>
#include <Global.h>
#include <util/TextureLoader.h>
#include <towers/CannonTower.h>
#include <towers/RapidFireTower.h>
#include <towers/StandardTower.h>
#include <ui/DetailButton.h>

/** @class MainGameState
 *  @brief Implementation of the main game state.
 *
 *  This state is responsible for handling the main gameplay logic.
 *  It follows a singleton pattern to ensure only one instance exists.
 *
 */

 /// Static instance of MainGameState
MainGameState MainGameState::sMainGameState;

/**
 * @brief Retrieves the singleton instance of the MainGameState.
 *
 * @return Pointer to the MainGameState instance.
 */
MainGameState* MainGameState::get() {
	return &sMainGameState;
}

/**
 * @brief Handles the entry logic for the main game state.
 *
 * This function is called when transitioning into the main game state.
 *
 * @return Always returns true.
 */
bool MainGameState::enter() {
	if (Global::currentMap == nullptr || Global::currentMap->getName().empty()) {
		std::cerr << "Global::currentMap was null" << std::endl;
		return false;
	}

	std::cout << "Global::currentMap name: " + Global::currentMap->name << std::endl;

	map = new Map(*Global::currentMap);
	map->setFlowFieldVisibility(false);
	map->setCurrentRenderRect(Global::mapViewRect);

	critterGroup = new CritterGroup(waveLevel, playerGold, map->getSpawnerPos(Global::mapViewRect), map->getTargetPos(Global::mapViewRect), map);

	detailDisplay = DetailAttributeDisplay::DetailAttributeDisplay();
	bool success = detailDisplay.initializeComponents();

	// creating dummy Towers
	dummyStandardTower = new StandardTower(0, 0, 0, 12);
	dummyRapidFireTower = new RapidFireTower(0, 0, 0, 25);
	dummyCannonTower = new CannonTower(0, 0, 50);

	TowerObserver* towerObserver = detailDisplay.getTowerObserver();

	// adding dummy Towers to display values before buying Towers
	towerObserver->addToBuyTowers(dummyStandardTower);
	towerObserver->addToBuyTowers(dummyRapidFireTower);
	towerObserver->addToBuyTowers(dummyCannonTower);

	// attaching dummy Towers to Observer
	dummyStandardTower->attach(towerObserver);
	dummyRapidFireTower->attach(towerObserver);
	dummyCannonTower->attach(towerObserver);

	// Iterate through the map cells and populate the dictionary with wall cells
	for (auto& cell : map->cells) {
		if (cell.isWall) {
			wallCellDict[cell] = false; // Wall cell with no tower
		}
	}

	return true;
}

/**
 * @brief Handles the exit logic for the main game state.
 *
 * This function is called when transitioning out of the main game state.
 *
 * @return Always returns true.
 */
bool MainGameState::exit() {
	TextureLoader::deallocateTextures();

	if (critterGroup != nullptr) {
		delete critterGroup;
		critterGroup = nullptr;
	}

	if (map != nullptr) {
		delete map;
		map = nullptr;
	}

	// deallocates Tower objects
	for (int i = 0; i < towers.size(); i++)
	{
		delete towers[i];
	}
	towers.clear();

	return true;
}

/**
 * @brief Handles user input and events.
 *
 * @param e The SDL_Event object containing input data.
 */
void MainGameState::handleEvent(SDL_Event& e) {
	// resets tower buy selection
	bool buttonClick = false;

	// handles hovering, clicking of buttons
	detailDisplay.handleButtonEvents(&e);

	// if click happens
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		// checking if buying tower
		std::vector<DetailDisplayComponent*> components = detailDisplay.getComponents();
		for (int i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<DetailButton*>(components[i]) != NULL)
			{
				if (dynamic_cast<DetailButton*>(components[i])->isClicked())
				{
					towerBuySelect = i - 1;
					buttonClick = true;

					// displays different values from dummy Towers
					switch (i)
					{
					case 1:
						detailDisplay.selectTower(dummyStandardTower);
						dummyStandardTower->notify();
						break;
					case 2:
						detailDisplay.selectTower(dummyRapidFireTower);
						dummyRapidFireTower->notify();
						break;
					case 3:
						detailDisplay.selectTower(dummyCannonTower);
						dummyCannonTower->notify();
						break;
					}
				}
			}
		}

		// checking if upgrading tower
		if ((dynamic_cast<DetailButton*>(detailDisplay.getTowerComponents()[7]))->isClicked())
		{
			buttonClick = true;
			int upgradeCost = detailDisplay.getTowerObserver()->getCurrentTower()->getUpgradeCost();

			// checks if enough coins for upgrade
			if (playerGold >= upgradeCost) {

				// checks if tower is already max level
				if (detailDisplay.getTowerObserver()->getCurrentTower()->upgrade())
				{
					playerGold -= upgradeCost;
				}
			}
		}

		// checking if selling tower
		if ((dynamic_cast<DetailButton*>(detailDisplay.getTowerComponents()[8]))->isClicked())
		{
			buttonClick = true;
			playerGold += detailDisplay.getTowerObserver()->getCurrentTower()->getRefundValue();

			// find tower to erase 
			for (int i = 0; i < towers.size(); i++)
			{
				if (towers[i] == detailDisplay.getTowerObserver()->getCurrentTower())
				{
					towers.erase(towers.begin() + i);
				}
			}

			detailDisplay.selectTower(nullptr);
		}
	}

	SDL_FRect currentRenderRect = map->getCurrentRenderRect();

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	// Calculate scaling factor
	float mapWidth = map->cellCountX * map->PIXELS_PER_CELL;
	float mapHeight = map->cellCountY * map->PIXELS_PER_CELL;
	float scaleX = currentRenderRect.w / mapWidth;
	float scaleY = currentRenderRect.h / mapHeight;
	float scale = std::min(scaleX, scaleY);

	// Convert mouse position
	Vector2D mousePosition(
		(mouseX - currentRenderRect.x) / scale / map->PIXELS_PER_CELL,
		(mouseY - currentRenderRect.y) / scale / map->PIXELS_PER_CELL
	);

	// Convert mouse position to cell coordinates (integers)
	int cellX = static_cast<int>(mousePosition.x);
	int cellY = static_cast<int>(mousePosition.y);

	// Ensure valid index range
	if (cellX < 0 || cellX >= map->cellCountX || cellY < 0 || cellY >= map->cellCountY) {
		return; // Out of bounds
	}

	// Compute the index for accessing the cell
	int index = cellX + cellY * map->cellCountX;
	Map::Cell targetCell = map->cells[index];

	float cellSize = map->PIXELS_PER_CELL;
	float targetX = (cellX * cellSize + currentRenderRect.x);
	float targetY = (cellY * cellSize + currentRenderRect.y);

	// Check if clicking
	if (!buttonClick && e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
		bool towerClick = false;

		// If clicking on the current tower
		for (int i = 0; i < towers.size(); i++) {
			if (towers[i]->isClicked()) {
				detailDisplay.selectTower(towers[i]);
				towers[i]->notify();
				towerClick = true;
			}
		}

		// If not clicking on a tower
		if (!towerClick) {
			// If placing down a tower on a valid, unoccupied wall cell
			if (towerBuySelect >= 0 && wallCellDict.find(targetCell) != wallCellDict.end() && !wallCellDict[targetCell]) {
				Tower* newTower = nullptr;

				// Checks for currently selected tower
				switch (towerBuySelect) {
				case 0: // Buy standard tower
					if (playerGold >= STANDARD_TOWER_COST) {
						playerGold -= STANDARD_TOWER_COST;
						newTower = new StandardTower(targetX, targetY, STANDARD_TOWER_COST);
					}
					towerBuySelect = -1;
					break;
				case 1: // Buy rapid fire tower
					if (playerGold >= RAPID_FIRE_TOWER_COST) {
						playerGold -= RAPID_FIRE_TOWER_COST;
						newTower = new RapidFireTower(targetX, targetY, RAPID_FIRE_TOWER_COST);
					}
					towerBuySelect = -1;
					break;
				case 2: // Buy cannon tower
					if (playerGold >= CANNON_TOWER_COST) {
						playerGold -= CANNON_TOWER_COST;
						newTower = new CannonTower(targetX, targetY, CANNON_TOWER_COST);
					}
					towerBuySelect = -1;
					break;
				}

				// If a new tower was successfully created, place it in the towers list
				if (newTower != nullptr) {
					towers.push_back(newTower);
					detailDisplay.selectTower(newTower);

					float scaleFactor = 1.5f;
					float newSize = map->PIXELS_PER_CELL * scaleFactor;
					float offset = (newSize - map->PIXELS_PER_CELL) / 2.0f;
					newTower->setCurrentRenderedRect({ targetX - offset, targetY - offset, newSize, newSize });

					newTower->attach(detailDisplay.getTowerObserver());
					newTower->notify();

					// Mark the wall cell as occupied
					wallCellDict[targetCell] = true;
				}
			}
			else {
				detailDisplay.selectTower(nullptr);
			}
		}
	}

}

/**
 * @brief Updates the game logic for the main game state.
 *
 * This function is called every frame to update the game's logic.
 */
void MainGameState::update() {
	critterGroup->generateCritters(0.16f);
	critterGroup->update(0.016f);

	// checks tower ranges
	for (int i = 0; i < towers.size(); i++)
	{
		// checks if critters exist
		if (critterGroup->getCritters().size())
		{
			// finds a target critter
			Critter* targettedCritter = towers[i]->findCritter(critterGroup->getCritters());

			// shoot if there is a critter in tower range
			if (targettedCritter)
			{
				float deltaX = targettedCritter->getPosition().x - towers[i]->x;
				float deltaY = targettedCritter->getPosition().y - towers[i]->y;
				float angleRad = atan2(deltaY, deltaX) + 90;
				float angleDeg = angleRad * (180.0f / 3.1416);

				towers[i]->setRotation(angleDeg);
				towers[i]->shootProjectile(targettedCritter);
			}
			// deletes already fired projectiles if there is no critter in range
			else
			{
				towers[i]->clearProjectiles();
			}
		}

		// deletes already fired projectiles if no critters
		else
		{
			towers[i]->clearProjectiles();
		}
	}

	if (playerGold < 0) {
		warningMessage = "You are out of gold!";
	} else {
		warningMessage = "";
	}
}

/**
 * @brief Renders the main game state.
 *
 * This function is called every frame to render the game's visuals.
 */
void MainGameState::render() {
	map->drawOnTargetRect(gRenderer, map->getCurrentRenderRect());

	detailDisplay.render();

	critterGroup->render(gRenderer);

	float towerWidth = static_cast<float>(map->getPixelPerCell().w);

	for (auto& tower : towers) {
		tower->render();
	}

	// Render player gold
	renderText("Gold: " + std::to_string(playerGold), 10.0f, 10.0f);

	// Render wave level
	std::string waveText = "Wave: " + std::to_string(waveLevel);
	float waveX = (Global::kScreenWidth - (waveText.length() * 10.0f)) / 2.0f;
	renderText(waveText, waveX, 10.0f);

	// Render warning message if any
	if (!warningMessage.empty()) {
		float warningX = (Global::kScreenWidth - (warningMessage.length() * 10.0f)) / 2.0f;
		float warningY = Global::kScreenHeight - 40.0f;
		renderText(warningMessage, warningX, warningY);
	}
}

/**
 * @brief Helper function to render text at a specific position on the screen.
 *
 * @param text The text to be rendered.
 * @param x The x-coordinate of the text.
 * @param y The y-coordinate of the text.
 */
void MainGameState::renderText(const std::string& text, float x, float y) {
	SDL_Color textColor = { 0, 0, 0, 255 };
	LTexture textTexture;
	textTexture.loadFromRenderedText(text, textColor);
	textTexture.render(x, y);
}