# Assignment 2

## What Was Done

- **Part #1 (Tower Observer):**
  - Added base classes for Observer and Observable.
  - Created TowerObserver.
  - Tower now extends Observable.
  - Added DetailAttributeDisplay as a wrapper to show the attributes for a Tower or a Critter, as well as Buying/Selling/Upgrading towers.
- **Part #2 (Map Observer):** 
  - Critters implement a subscriber class that allows them to get updates from the Map class. 
  - The map class publishes the next direction the crtter should move in.
- **Part #3 (Critter Observer):** 
    - CritterObserver is used by DetailDisplay to view Critter's information and display it in a set of UI elements, specifically LTextures. 
    - The user can select a moving critter and view it's health, strength, reward amount and speed.
    - The user can view the critter's help deplete as their health goes down, as it notifies the observer (the UI element) about its depleting health.
- Critters and towers were integrated on the map. 
- Maps have their size dynamically changed based on their size.
- A map editor was made.
	- You can specify the size of a map.
	- You can save maps.
		- You can rename map.
		- You can edit a loaded map.
	- You can load maps.
- You can select a map to play on.