[![ChrisRuff](https://circleci.com/gh/ChrisRuff/Ruff.svg?style=svg)](https://app.circleci.com/pipelines/github/ChrisRuff/Ruff)

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

### Sorting ###
Algorithms implemented are:
* ##### Selection Sort
```
std::vector<int> list{0,8,5,10,1};
ruff::selectionSort(list);
```
* ##### Bubble Sort
```
std::vector<int> list{0,8,5,10,1};
ruff::bubbleSort(list);
```
* ##### Insertion Sort
```
std::vector<int> list{0,8,5,10,1};
ruff::insertionSort(list);
```
* ##### Quick Sort
```
std::vector<int> list{0,8,5,10,1};
ruff::quickSort(list);
```
* ##### Merge Sort
```
std::vector<int> list{0,8,5,10,1};
ruff::mergeSort(list);
```
* ##### Radix Sort
```
std::vector<int> list{0,8,5,10,1};
ruff::radixSort(list, [](int a){return a;});
```

