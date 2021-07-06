#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class Config
{
	public:
		std::string entryPointName;
		int entryPoint;
};

class Player
{
	public:
		std::vector<std::string> inventory;
};

class Room
{
	public:
		// text outputs
		std::string name, echo, examine;

		// location names
		std::string northName, eastName, southName, westName;

		// location indexes
		int north = -1, east = -1, south = -1, west = -1;
};

class GameDeclarations
{
	public:
		Config config;
		Player player;
		std::vector<Room> rooms;

	// constructor for game declarations
	GameDeclarations(Config configObj, Player playerObj, std::vector<Room> roomsArr)
	{
		config = configObj;
		player = playerObj;
		rooms = roomsArr;
	}

	// empty constructor cus i'm lazy
	GameDeclarations()
	{

	}
};

std::vector<std::string> splitString(std::string str)
{
	// stream for reading the string
	std::istringstream stringstream(str);

	// vector for storing the output
	std::vector<std::string> result;

	// loop through string
	for(std::string tempStr; stringstream >> tempStr;)
	{
		// push string to back of vector
		result.push_back(tempStr);
	}

	return result;
}

std::vector<Room> setRoomIndexes(std::vector<Room> rooms)
{
	// loop through array of rooms
	for (int i = 0; i < rooms.size(); i++)
	{
		// update north indexes
		if (rooms[i].northName != "")
			for (int j = 0; j < rooms.size(); j++)
				if (rooms[j].name == rooms[i].northName)
					rooms[i].north = j;

		// update east indexes
		if (rooms[i].eastName != "")
			for (int j = 0; j < rooms.size(); j++)
				if (rooms[j].name == rooms[i].eastName)
					rooms[i].east = j;
		
		// update south indexes
		if (rooms[i].southName != "")
			for (int j = 0; j < rooms.size(); j++)
				if (rooms[j].name == rooms[i].southName)
					rooms[i].south = j;

		// update west indexes
		if (rooms[i].westName != "")
			for (int j = 0; j < rooms.size(); j++)
				if (rooms[j].name == rooms[i].westName)
					rooms[i].west = j;
	}

	return rooms;
}

GameDeclarations readTextFile(std::string filePath)
{
	// read text file
	std::ifstream read(filePath);

	// vector of rooms
	std::vector<Room> rooms;

	// config object
	Config config;

	// player object
	Player player;

	// if the file exists
	if (read)
	{
		// string to store the current line
		std::string line;

		// object to store current room
		Room currentRoom;

		// amount of room objects created
		int totalRooms = -1;

		// amount of conf objects created
		int totalConf = 0;

		// amount of inventory objects created
		int totalInv = 0;

		// loop through the file
		while (getline(read, line))
		{
			// if line is a room decleration
			if (line.front() == '$')
			{
				// remove the first char from the string
				line = line.erase(0, 1);

				// split the string
				std::vector<std::string> splitLine = splitString(line);

				// check room decleration type
				if (splitLine[0] == "ROOM")
				{
					// only do after first room created
					if (totalRooms > -1)
					{
						// store current room in array
						rooms.push_back(currentRoom);

						// create a new room object
						Room newRoom;

						// overwrite existing room
						currentRoom = newRoom;
					}

					// increase room counter
					totalRooms++;

					// string to store the room name
					std::string roomName;

					// combine the room name
					for (int i = 1; i < splitLine.size(); i++)
						roomName += splitLine[i] + " ";

					// remove last blank char from room name string
					roomName.pop_back();

					// update room name variable
					currentRoom.name = roomName;
				}

				if (splitLine[0] == "CONF")
				{
					// check if multiple configs exist
					if (totalConf > 0)
					{
						std::cout << "multiple configuration declarations detected, please ensure your text file only has one.\n";
						exit(EXIT_FAILURE);
					}

					// increase config counter
					totalConf++;
				}

				if (splitLine[0] == "INV")
				{
					// check if multiple configs exist
					if (totalInv > 0)
					{
						std::cout << "multiple inventory declarations detected, please ensure your text file only has one.\n";
						exit(EXIT_FAILURE);
					}

					// increase config counter
					totalInv++;
				}
			}

			// if line is room variable
			if (line.front() == '#')
			{
				// remove the first char from the string
				line = line.erase(0, 1);

				// split the string
				std::vector<std::string> splitLine = splitString(line);

				// line input string
				std::string lineInput;

				// combine the input string
				for (int i = 1; i < splitLine.size(); i++)
					lineInput += splitLine[i] + " ";

				// remove last blank char from input string
				lineInput.pop_back();

				if (splitLine[0] == "ECHO")
					currentRoom.echo = lineInput;

				if (splitLine[0] == "EXAM")
					currentRoom.examine = lineInput;

				if (splitLine[0] == "NORTH")
					currentRoom.northName = lineInput;

				if (splitLine[0] == "EAST")
					currentRoom.eastName = lineInput;

				if (splitLine[0] == "SOUTH")
					currentRoom.southName = lineInput;

				if (splitLine[0] == "WEST")
					currentRoom.westName = lineInput;

				if (splitLine[0] == "START")
					config.entryPointName = lineInput;

				if (splitLine[0] == "ITEM")
					player.inventory.push_back(lineInput);
			}
		}

		// if no rooms exist
		if (totalRooms == -1)
		{
			std::cout << "cannot read this text file, please double check your text file\n";
			exit(EXIT_FAILURE);
		}

		// add last room to the array
		rooms.push_back(currentRoom);

		// close the file
		read.close();
	}

	// if the file doesn't exist
	else
	{
		std::cout << "failed to read text file, please double check the path provided\n";
		exit(EXIT_FAILURE);
	}

	// set the room locations to the array indexes
	rooms = setRoomIndexes(rooms);

	// create declaration object
	GameDeclarations gameDeclarations(config, player, rooms);

	return gameDeclarations;
}

void playGame(std::vector<Room> rooms, Config config, Player player)
{
	// get the entry point number
	for (int i = 0; i < rooms.size(); i++)
		if (rooms[i].name == config.entryPointName)
			config.entryPoint = i;

	// integer to store current room index
	int roomIndex = config.entryPoint;

	// integer to store previous room index
	int prevRoomIndex = -1;

	// boolean to determine if game is running
	bool running = true;

	// string to store current user input
	std::string input;

	while (running)
	{
		// if player is out of bounds
		if (roomIndex < 0)
		{
			// inform the player they are out of bounds
			std::cout << "you cannot go in this direction\n";

			// set the player back to their previous room
			roomIndex = prevRoomIndex;
		}

		// display room info
		if (roomIndex != prevRoomIndex)
		{
			// display the rooms description
			std::cout << rooms[roomIndex].echo;

			// update previous room index
			prevRoomIndex = roomIndex;
		}

		// newline
		std::cout << "\n";

		// display input prompt
		std::cout << "> ";

		// get user input
		std::cin >> input;

		// newline
		std::cout << "\n";

		// interpret commands
		if (input == "examine")
			std::cout << rooms[roomIndex].examine;

		else if (input == "look")
			std::cout << rooms[roomIndex].echo;

		else if (input == "north")
			roomIndex = rooms[roomIndex].north;

		else if (input == "east")
			roomIndex = rooms[roomIndex].east;

		else if (input == "south")
			roomIndex = rooms[roomIndex].south;

		else if (input == "west")
			roomIndex = rooms[roomIndex].west;

		else if (input == "inventory")
			for (int i = 0; i < player.inventory.size(); i++)
				std::cout << player.inventory[i] << "\t";

		else if (input == "exit")
			exit(EXIT_SUCCESS);

		else
			std::cout << "you cannot perform this action\n";
	}
}

int main(int argc, char** argv)
{
	// object storing all game declartations from text file
	GameDeclarations gameDeclarations;

	// check if first argument is provided
	if (argv[1])
		gameDeclarations = readTextFile(argv[1]);

	// if first argument is not provided
	else
	{
		std::cout << "no text file provided, please provide a text file to run the program\n";
        exit(EXIT_FAILURE);
	}

	// play the game
	playGame(gameDeclarations.rooms, gameDeclarations.config, gameDeclarations.player);

	// exit the main method (should never reach this)
	return 0;
}
