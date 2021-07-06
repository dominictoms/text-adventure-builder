# Text Adventure Builder

## Introduction
### About This Program
Just a quick little program I wrote for creating classic command line text adventures from very basic text files.

## Documentation
### Compilation
To compile the program simply run GCC.

```
$ g++ main.cpp -o main
```

Or alternatively, use Clang.

```
$ clang++ main.cpp -o main  
```

### Writing a Text Adventure
This program interprets text files and converts them into playable text adventures. An example text file follows.

```
$ROOM   dungeon
#ECHO   you awaken in a dark dungeon
#EXAM   the dungeon is very dark
#NORTH  forest
#EAST   cave

$ROOM   forest
#ECHO   you approach the enterance to a forest
#EXAM   the forest looks dangerous
#SOUTH  dungeon

$ROOM   cave
#ECHO   you approach the enterance to a cave
#EXAM   the cave is very cold and damp
#WEST   dungeon
```

The text file can now be interpreted by the program by providing the path to the text file as an argument.

```
$ ./main adventure.txt  
```

### Text File Syntax
The program will interpret two types of instructions, declarations and variables. A declaration is used for declaring a new object such as a room. All declarations start with a `$` symbol.

#### Declarations
|Syntax		|Description															|
|-----------|-----------------------------------------------------------------------|
|`$ROOM`	|Create a new room.														|

Variables follow the declaration and will always effect the previous declaration. Variables are used for providing all of the details relevant to the previous declaration. All variables start with a `#` symbol.

#### Variables
|Syntax		|Description															|
|-----------|-----------------------------------------------------------------------|
|`#ECHO`	|Text to be displayed when player visits a new room.					|
|`#EXAM`	|Additional details a player can read about a room by typing `examine`.	|
|`#NORTH`	|The room the player will visit when traveling north.					|
|`#EAST`	|The room the player will visit when traveling east.					|
|`#SOUTH`	|The room the player will visit when traveling south.					|
|`#WEST`	|The room the player will visit when traveling west.					|
