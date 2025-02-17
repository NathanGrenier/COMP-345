# Design
## Game
- A player cannot win, they can only achieve a highscore. This score is represented by the highest wave they manage to reach.
- The player loses when they're gold reaches a negative value. 

### In the Future
- Add music and sound effects.
- Add the ability to view your highscores.
- Improved textures and fonts.

## Map
- Custom maps of any size can be generated an customized.
- A map has 1 spawner and target cell.
- Cells can be one of the following:
  - Path
  - Wall
  - Spawner
  - Target
- A map is only valid if there's a path from the spawner to the target cell.

### In the Future
- Ability to save custom maps.
- Ability to load pre-made maps.

## Tower
- Towers can be bought and placed.
- Types of towers include:
  - Standard
  - Rapid Fire
  - Cannon
- Towers fire Projectiles at critters depending on range.
- Projectiles are shot from towers at varying speeds and sizes.
- Towers can be sold and removed.
- Towers can be upgraded, increasing basic attributes. Increased attributes depends on Tower type: 
  - Standard gets a general increase
  - Rapid Fire has increased rate of fire
  - Cannon has increased range and power

### In the Future
- Adding different projectile effects, such as AoE damage 
## Critters

- Currently, critters are represented as simple red squares. While this is a placeholder for now, the final design will likely involve more detailed textures or sprites to better communicate their nature and characteristics. The use of a basic red square in early development serves two key purposes: 
  1. It simplifies initial testing and debugging, ensuring movement and pathfinding mechanics work correctly before investing in detailed visuals
  2. It maintains a clear contrast against the background, making it easier to track critters as they navigate the map.
- Critters move strictly in the four cardinal directions (up, down, left, right), avoiding diagonal movement. This design choice simplifies pathfinding, as it allows for more predictable movement patterns that can be handled efficiently using grid-based algorithms such as A* or BFS. More importantly, restricting movement to cardinal directions enhances strategic gameplay.
- Critters spawn in waves of ten, with each wave gradually increasing in difficulty by giving critters more health while maintaining their speed. This approach ensures that difficulty ramps up in a controlled and predictable manner, allowing players to adapt their strategies over time. By keeping speed constant, the game avoids sudden shifts in difficulty that could feel unfair. Instead of overwhelming the player with faster-moving enemies, the increasing health forces them to optimize tower placement and damage efficiency.

### In the Future

- Adding different critter types can force players to adjust their tactics. Some ideas include tank critters with high HP but slow movement, speedsters that are fast but fragile, swarm critters that overwhelm defenses, and armored critters that resist certain attacks. Some could also have special abilities like shields, self-healing, teleportation, or splitting into smaller critters when defeated.
- Critters could be affected by debuffs such as slows, burns, poison, freezing, or weakening, making them more vulnerable to attacks. Some critters might be immune to specific effects, like fire-resistant or poison-immune variants, requiring players to use a mix of tower types.
- Instead of following predictable paths, some critters could change routes based on tower placements, while others might attack nearby towers instead of just running to the exit. Pack leaders could boost nearby critters, and smart critters might avoid high-damage areas.

# Analysis

## Lab Computer Software and Tool Availability

- All lab computers have Microsoft Visual Studio installed on them. This IDE comes with the C++ build tools we need already installed.
- Microsoft's preferred package manager for C++ projects, vcpkg, requires Visual Studio to be >= Version 17.6. Most of the lab computers don't meet this requirement.

# References
- Vector TD: https://www.crazygames.com/game/vector-td
  - Used as inspiration for:
    - Critters Color Damage Multipliers
    - Some Tower Ideas
- Flow Field for Tower Defense Games: https://www.redblobgames.com/pathfinding/tower-defense/
- SDL3: https://wiki.libsdl.org/SDL3/FrontPage
- Lazy Foo' Productions SLD3 Tutorial: https://lazyfoo.net/tutorials/SDL3/index.php
  - Used as inspiration for:
    - Architectural pattern choice, specifically state machine
    - Sample code was read and used to write some parts of the assignment