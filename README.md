# Current Features:
### Searching 
* ASTAR

### UI Engine
Simple engine for creating 2D environments. 

#### UI Examples
[Circles](examples/ui/circles): Creates an environment to interact with circle collisions

[Gravity](examples/ui/gravity): Simulates gravity around points

[AStar](examples/ui/astar): Visualizes the AStar path finding algorithm

[Matching](examples/ui/matching): Shows live matching results using opencv matchers and visualizes them with the ui engine

[Sprites](examples/ui/sprites): Shows some tree sprites being animated and spreading

### Matching using opencv 
| Detectors | Matchers  |
| --------  | --------  |
| ORB       | BFMatcher |
| AKAZE     | Flann     |
| BRISK     |           |
| DAISY     |           | 

Note: Daisy does not work with BFMatcher currently

