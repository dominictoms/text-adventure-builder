#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class Room
{
	public:
		// text outputs
		std::string name;
		std::string echo;
		std::string examine;

		// location names
		std::string northName;
		std::string eastName;
		std::string southName;
		std::string westName;

		// location indexes
		int north = -1;
		int east = -1;
		int south = -1;
		int west = -1;
};

class Player
{
	public:
		std::vector<std::string> inventory;
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

std::vector<Room> readTextFile(std::string filePath)
{
	// read text file
	std::ifstream read(filePath);

	// array of rooms
	std::vector<Room> rooms;

	// if the file exists
	if (read)
	{
		// string to store the current line
		std::string line;

		// start off with a room
		Room currentRoom;

		// amount of room objects created
		int totalRooms = -1;

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

					// update room variables
					currentRoom.name = splitLine[1];
				}
			}

			// if line is room variable
			if (line.front() == '#')
			{
				// remove the first char from the string
				line = line.erase(0, 1);

				// split the string
				std::vector<std::string> splitLine = splitString(line);

				if (splitLine[0] == "ECHO")
					for (int i = 1; i < splitLine.size(); i++)
						currentRoom.echo += splitLine[i] + " ";

				if (splitLine[0] == "EXAM")
					for (int i = 1; i < splitLine.size(); i++)
						currentRoom.examine += splitLine[i] + " ";

				if (splitLine[0] == "NORTH")
					currentRoom.northName = splitLine[1];

				if (splitLine[0] == "EAST")
					currentRoom.eastName = splitLine[1];

				if (splitLine[0] == "SOUTH")
					currentRoom.southName = splitLine[1];

				if (splitLine[0] == "WEST")
					currentRoom.westName = splitLine[1];
			}
		}

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

	return rooms;
}

void playGame(std::vector<Room> rooms)
{
	// boolean to determine if game is running
	bool running = true;

	// integer to store current room index
	int roomIndex = 0;

	// integer to store previous room index
	int prevRoomIndex = -1;

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
			std::cout << rooms[roomIndex].echo << "\n";

			// update previous room index
			prevRoomIndex = roomIndex;
		}

		// display input prompt
		std::cout << "> ";

		// get user input
		std::cin >> input;

		// newline
		std::cout << "\n";

		// interpret commands
		if (input == "examine")
			std::cout << rooms[roomIndex].examine << "\n";

		else if (input == "look")
			std::cout << rooms[roomIndex].echo << "\n";

		else if (input == "north")
			roomIndex = rooms[roomIndex].north;

		else if (input == "east")
			roomIndex = rooms[roomIndex].east;

		else if (input == "south")
			roomIndex = rooms[roomIndex].south;

		else if (input == "west")
			roomIndex = rooms[roomIndex].west;

		else
			std::cout << "you cannot perform this action\n";
	}
}

int main(int argc, char** argv)
{
	// pointer to array of rooms
	std::vector<Room> rooms;

	// check if first argument is provided
	if (argv[1])
		rooms = readTextFile(argv[1]);

	// if first argument is not provided
	else
	{
		std::cout << "no text file provided, please provide a text file to run the program\n";
        exit(EXIT_FAILURE);
	}

	// play the game
	playGame(rooms);

	// exit the main method (should never reach this)
	return 0;
}
