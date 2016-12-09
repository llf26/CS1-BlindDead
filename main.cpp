/*
   COPYRIGHT (C) 2016 Larry Fritz (2837259) All rights reserved.
   CSI project 2 and 3 - Blind Dead
   Larry Fritz
   Version. 1.01 MM.DD.20YY.
   Purpose: Place a description here...
*/
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<cctype>
#include<iomanip>
#include<limits>
#include<string>

using std::cout;
using std::cin;
using std::endl;
using std::fstream;

//gameArray indicies
//gameArray[0] = players current room
const int CURRENT_ROOM_INDEX = 0;
//gameArray[1] = current room of zombie
const int ZOMBIE_ROOM_INDEX = 1;
//gameArray[2]
const int NUM_BULLETS_INDEX = 2;
//gameArray[3]
const int NUM_ROOMS_INDEX = 3;
//boolean whether player has grail or not
const int HAVE_GRAIL_INDEX = 4;

//roomArray indices
//roomArray[gameArray[CURRENT_ROOM_INDEX]][4] = 1 or 0 if the player is
//	or is not in the current room
const int PLAYER_INDEX = 4;
//roomArray[gameArray[ZOMBIE_ROOM_INDEX]][5] = 1, tracks where the zombie is
const int ZOMBIE_INDEX = 5;
//roomArray[room][6] = the room that the grail is in should be marked 1, all others 0
const int GRAIL_INDEX = 6;
// the number of rooms and number of bullets to fend off the zombies
const int MAX_ROOMS = 11;
const int MAX_BULLETS = 6;

//PART 1A prototypes
//reset simply resets the primary game variables
void reset(int &currentRoom, int &zombieRoom, int &numBullets, int &numRooms, bool &haveGrail);
//print game instructions
void instructions();
//print layout of game memory
void printMemory(int gameArray[],int roomArray[][7], int& numRooms);
// the menu (which calls functions printMemory and instructions)
bool menu(int &currentRoom, int &zombieRoom, int &numBullets, int &numRooms, bool &haveGrail, int gameArray[],int roomArray[][7]);

//PART 1B prototypes
//read in the input file for the map
void readMaze(int roomArray[][7], int gameArray[5], int& numRooms);
//return a random room
int getRandomRoom(int gameArray[5], int& numRooms);
//place the zombie
void placeZombie(int roomArray[][7], int gameArray[5], int& numRooms, int& zombieRoom);
//place the grail
void placeGrail(int roomArray[][7], int gameArray[5], int& numRooms);
// the setup function
void setup(int &currentRoom, int &zombieRoom, int &numBullets, int &numRooms, bool &haveGrail, int roomArray[][7], int gameArray[5]);

//PART 2 prototypes
bool checkZombie (int i, int roomArray[][7]);

bool checkGrail (int i, int roomArray[][7]);

bool checkNearZombie(int x, int roomArray[][7]);

bool checkNearGrail(int x, int roomArray[][7]);

void winOrLose(bool win, int gameArray[]);

void showConnectedRooms(int &currentRoom, int roomArray[][7]);

bool isConnected(int targetRoom, int gameArray[], int roomArray[][7]);
//PART 3 prototypes

void moveRoom(int x, int gameArray[], int roomArray[][7], bool& haveGrail, int& zombieRoom, int& currentRoom);

void shootRoom(int x, int gameArray[], int roomArray[][7], int& zombieRoom);

void moveZombie(int roomArray[][7], int& zombieRoom);

bool validateSelection(std::string toValidate);

void doSelection(std::string control, int gameArray[], int roomArray[][7], int& haveGrail, int& zombieRoom, int& numRooms, int& currentRoom);

void checkRoom(int gameArray[], int roomArray[][7], bool& haveGrail, int& currentRoom, int& zombieRoom, int& numRooms);

void running(int gameArray[], int roomArray[][7], bool& haveGrail, int& currentRoom, int& zombieRoom, int& numRooms);

int main()
{
	// these variables are passed throughout this program
	int roomArray[MAX_ROOMS][7],
	    gameArray[5],
	    currentRoom = 0,
       zombieRoom = 0,
       numBullets = 0,
       numRooms = 0;


//Start out without the grail
   bool haveGrail = false;


//Reset all game values to start the game
   reset(currentRoom, zombieRoom, numBullets, numRooms, haveGrail);


//Display the menu
   menu(currentRoom, zombieRoom, numBullets, numRooms, haveGrail, gameArray, roomArray);

//Loop to always display the menu once the game has begun
    while(1)
    {
    menu(currentRoom, zombieRoom, numBullets, numRooms, haveGrail, gameArray, roomArray);
    }


	return 0;
}
// build your functions here. Don't forget your pre and post conditions


//Pre: Take in all the game values that were previously non-default
void reset(int &currentRoom, int &zombieRoom, int &numBullets, int &numRooms, bool &haveGrail)
{
    currentRoom = 0;
    zombieRoom = 0;
    numBullets = 0;
    numRooms = 0;
    haveGrail = 0;
}
//Post: Set all game values to default

//Pre: nothing
void instructions()
{
    cout << "YOUR MISSION, SHOULD YOU CHOOSE TO ACCEPT IT, IS TO SEEK THE HOLY " <<
            "GRAIL WITHIN THE RUINS OF AN ANCIENT CHURCH. TO SUCCEED YOU MUST " <<
            "ENTER THE CHURCH, AVOID THE UNDEAD GUARDIANS, FIND THE GRAIL AND " <<
            "ESCAPE. YOU HAVE SIX SILVER BULLETS TO PROTECT YOU. IF THE ZOMBIES " <<
            "FIND YOU OR YOU RUN OUT OF BULLETS YOU WILL LOSE!" << endl << endl;

}
//Post: Display game instructions

//Pre: Take in the arrays associated with the game and the number of rooms read during setup
void printMemory(int gameArray[], int roomArray[][7], int& numRooms)
{
    //Labels for game array values
    cout << "Game Array: " << endl << "  [ current][ bullets][    rooms]" << endl << "  ";

    //Loop to display game array
    for(int i = 0; i  < 3; i++)
    {
        cout << "[       " << gameArray[i] << "]";
    }
    cout << endl << endl;

    //Label for room array values
    cout << "        [ north][ south][  west][  east][player][zombie][ grail]" << endl;
    for(int i = 0; i < numRooms; i++)
        {
            //Loop to display room array
            cout << "[R" << std::setw(2) << (i+1) << "] ->";
            for(int j = 0; j < 7; j++)
            {
            cout << "[" << std::setw(6) << roomArray[i][j] << "]";
            }
            cout << endl;
        }
        cout << endl;


}
//Post: Display the current game memory

//Pre: Player has just selected menu
bool menu(int &currentRoom, int &zombieRoom, int &numBullets, int &numRooms, bool &haveGrail, int gameArray[],int roomArray[][7])
{
    cout << "       " << "Enter, if you dare..." << endl
    << "*** THE TOMB OF THE LIVING DEAD ***" << endl << endl;

    cout << std::setw(6) << "Main Menu" << endl << "1. Instructions" << endl
    << "2. Begin" << endl << "3. Exit" << endl;

    cout << "Enter Menu Selection: ";

    //Set up validation loop flag
    int selection = 0;
    bool loopFlag = 1;


    //Show the menu until a valid option is chosen
    do
{

    cin >> selection;

    if ((cin.fail()) || selection < 1 || selection > 3)
    {
        cout << "You MUST choose an option between 1 and 3." << endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        loopFlag = false;
            cout << endl;

    //Construct to execute player choice
    switch(selection)
    {
        case 1: instructions();
                break;
        case 2: setup(currentRoom, zombieRoom, numBullets, numRooms, haveGrail, roomArray, gameArray);


                break;
        case 3: std::exit(0);
    }
    }
}while (loopFlag);





}
//Post: Execute player selection, validating input

//Pre: Nothing
void readMaze(int roomArray[][7], int gameArray[5], int& numRooms)
{
numRooms = 0;


std::ifstream inText;
inText.open("easyMaze.txt", std::ifstream::in);

inText >> numRooms;

 for(int i = 0; i < numRooms; i++)
        {
            for(int j = 0; j < 7; j++)
            {
            inText >> roomArray[i][j];
            }

        }
}
//Post: Number of rooms and room array have been configured based on a given text file

//Pre: Nothing
int getRandomRoom(int gameArray[5], int& numRooms)
{

int randRoom = rand() % numRooms + 1;
return randRoom;
}
//Post: A random room has been chosen based on the number of rooms

//Pre: Nothing
void placeZombie(int roomArray[][7], int gameArray[5], int& numRooms, int& zombieRoom)
{

    while(zombieRoom < (numRooms / 2 + 1))
    {
        zombieRoom = getRandomRoom(gameArray, numRooms);
    }
   // cout << "Zombie Room: " << zombieRoom << endl;
    roomArray[zombieRoom - 1][5] = 1;

}
//Post: Zombie is placed into a random room from ((numRooms/2) + 1) to numRooms

//Pre: Nothing
void placeGrail(int roomArray[][7], int gameArray[5], int& numRooms)
{

    int grailRoom = 0;
    while(grailRoom < (numRooms / 2 + 1))
    {
        grailRoom = getRandomRoom(gameArray, numRooms);
    }

    roomArray[grailRoom - 1][6] = 1;

}
//Post: Grail is placed into a random room from ((numRooms/2) + 1) to numRooms

//Pre: Game values have been reset to default
void setup(int &currentRoom, int &zombieRoom, int &numBullets, int &numRooms, bool &haveGrail, int roomArray[][7], int gameArray[])
{
    //Seed random number generator
    srand(time(0));

    //Read numRooms and roomArray
    readMaze(roomArray, gameArray, numRooms);

    //Player set to room 1
    gameArray[0] = 1;

    //Player set to room 1 in roomArray
    roomArray[0][4] = 1;

    //Player set to room 1
    currentRoom = 1;

    //Give player a certain number of bullets
    gameArray[1] = MAX_BULLETS;

    numBullets = MAX_BULLETS;


    //Set display for number of rooms to numRooms
    gameArray[2] = numRooms;

    //Place zombie and grail randomly
    placeZombie(roomArray, gameArray, numRooms, zombieRoom);
    placeGrail(roomArray, gameArray, numRooms);

    //Start the gameplay process
    running(gameArray, roomArray, haveGrail, currentRoom, zombieRoom, numRooms);

}
//Pre: Nothing
bool checkZombie (int i, int roomArray[][7])
{
    if(roomArray[i-1][ZOMBIE_INDEX] == 1)
    {
    return 1;
    }

    else
    {
    return 0;
    }


}
//Post: Return a 0 or 1 based on whether the zombie is in a given room

//Pre: Nothing
bool checkGrail (int i, int roomArray[][7])
{
    if(roomArray[i-1][GRAIL_INDEX] == 1)
    {
       // cout << "||grail check: true||\n";
        return 1;
    }

    else
    {
       // cout << "||grail check: false||\n";
        return 0;
    }
}
//Post: Return a 0 or 1 based on whether the grail is in a given room

//Pre: Nothing
bool checkNearZombie(int x, int roomArray[][7])
{
    bool  nearZombie = 0;
    for(int y = 0; y < MAX_ROOMS; y++)
        for(int z = 0; z < 4; z++)
        if(roomArray[y][z] == x)
            {
            if(checkZombie((y + 1), roomArray))
            nearZombie = 1;
            }
            return nearZombie;
}
//Post: Returns a 0 or 1 based on whether the zombie is in an adjacent room to a given room

//Pre: Nothing
bool checkNearGrail(int x, int roomArray[][7])
{
    bool nearGrail = 0;
        for(int y = 0; y < MAX_ROOMS; y++)
            for(int z = 0; z < 4; z++)
            if(roomArray[y][z] == x)
            {
                if(checkGrail((y + 1), roomArray))
                {
                nearGrail = 1;
                }
            }

    return nearGrail;
}
//Post: Returns a 0 or 1 based on whether the grail is in an adjacent room to a given room

//Pre: A 0 or 1 is given to determine the game's outcome
void winOrLose(bool win, int gameArray[])
{
    if(win)
    {
        cout << "\n===========================================================\n";

        cout << "YOU HAVE EMERGED FROM THE TEMPLE WITH THE GRAIL!! YOU WIN!!""\n===========================================================\n";
    }
    else
    {
        cout << "\nAs you walk in the room the zombie sees you and devour you whole!\n"
                << "\nYOU LOST!" << endl;
    }
    gameArray[CURRENT_ROOM_INDEX] = -1;
}
//Post: Display a message based on the outcome of the game, then set the current room to -1 to end a loop in another function

//Pre: The current room is given
void showConnectedRooms(int &currentRoom, int roomArray[][7])
{
    //Loop to display connected rooms
    for(int i = 0; i < MAX_ROOMS; i++)
        for(int j = 0; j < 4; j++)
            if(roomArray[i][j] == currentRoom)
                cout << "Room " << (i + 1) << " is connected to the current room(Room " << currentRoom << ").\n" ;
                cout << endl;
}
//Post: Display all rooms connected to the player's current room

//Pre: Given a room to check
bool isConnected(int targetRoom, int gameArray[], int roomArray[][7])
{
    bool isConnected = false;

    for(int j = 0; j < 4; j++)
        if(roomArray[targetRoom-1][j] == gameArray[CURRENT_ROOM_INDEX])
            isConnected = true;
            return isConnected;
}
//Post: Display whether the given room is connected to the current room

//Pre: Given a room to move to and the current room
void moveRoom(int x, int gameArray[], int roomArray[][7], bool& haveGrail, int& zombieRoom, int& currentRoom)
{
    //If thhe current room is connected to the desired room,

    if(isConnected(x, gameArray, roomArray) == 1)
    {
    //Set the old room to 0 in the room array
    roomArray[currentRoom - 1][PLAYER_INDEX] = 0;

    //Set the desired room to 1 in the room array
    roomArray[x - 1][PLAYER_INDEX] = 1;

    //Set the current room to the desired room
    gameArray[0] = x;
    currentRoom = x;

    //If the player already had the grail, move it to the desired room
    if(haveGrail == 1)
    {
        for(int i = 0; i < gameArray[2]; i++)
        {
        roomArray[i][GRAIL_INDEX] = 0;
        }
        roomArray[x - 1][GRAIL_INDEX] = 1;


    }

        //If the player didn't previously have the grail and
        //the grail is in the desired room, set haveGrail to 1
        if(checkGrail(gameArray[0], roomArray))
        {
        haveGrail = 1;
        }

        //If the player reaches room 1 with the grail in hand, they win
        if((gameArray[0] == 1) && (haveGrail == 1))
        {
            winOrLose(1, gameArray);
            exit(0);
        }

        //If the player walks into a room when the zombie is in it, they lose
        if(gameArray[0] == zombieRoom)
        {
            winOrLose(0, gameArray);
        }



    }

    //If the desired room isn't connected to the current room,
    //notify the player.
    else
    {
    cout << "The room you selected is not connected to you." << endl;
    }

}

//Pre: Given a desired room to shoot into and the current room
void shootRoom(int x, int gameArray[], int roomArray[][7], int& zombieRoom)
{

    //If the desired room is connected to the current room:
    if(isConnected(x, gameArray, roomArray) == 1)
    {
     //The player uses up a bullet
     gameArray[1]--;

     //If the zombie is in the desired room:
        if(checkZombie(x, roomArray) == 1)
        {
            //Kill the zombie and notify the player
            roomArray[x - 1][ZOMBIE_INDEX] = 0;
            zombieRoom = 0;
            cout << "You hit the zombie and it fell to the floor! You feel a sudden sense of peace." << endl;
        }

     //If the zombie was not in the room, notify the player
        else
        {
         cout << "The zombie was not in the room, you missed!\n";
        }
    }

    //If the desired room was not connected to the current room, notify the player
    else
     {
         cout << "The room you selected is not connected to your current room.\n";
     }

}
//Post: The zombie is dead if the player shot into its room and the player
//loses a bullet, or the player misses and loses a bullet.

//Pre: The player has attempted to change rooms
void moveZombie(int roomArray[][7], int& zombieRoom)
{
    //If the zombie is alive:
    if(zombieRoom)
    {
    int newRoom;

    //Generate a new room that is connected to its current room,
    //set it to 1 and set the old room to 0
    newRoom = rand() % 4;

        if(roomArray[zombieRoom - 1][newRoom] != 0)
        {
        roomArray[zombieRoom - 1][ZOMBIE_INDEX] = 0;

        roomArray[roomArray[zombieRoom - 1][newRoom] - 1][5] = 1;

        zombieRoom = roomArray[zombieRoom - 1][newRoom];
        }
    }


}

//Pre: Player makes a menu selection
bool validateSelection(std::string toValidate)
{
    std::string first = toValidate.substr(0,1);
    if((first.compare("Q") == 0) || (first.compare("q") == 0))
        return 1;

    if((first.compare("D") == 0) || (first.compare("d") == 0))
        return 1;

    if((first.compare("S") == 0) || (first.compare("s") == 0))
        return 1;

    if((first.compare("M") == 0) || (first.compare("m") == 0))
        return 1;

    else
    return 0;

}
//Post: Execute the desired selection after validating input

//Pre: Selection has been validated
void doSelection(std::string control, int gameArray[], int roomArray[][7], bool& haveGrail, int& zombieRoom, int& numRooms, int& currentRoom)
{
    //String to save player's selection
    std::string first = control.substr(0,1);

    //Int to save room if player chooses to move rooms
    int room;

    //Bool to assert whether the zombie needs to move
    bool doMoveZombie = 0;

    //If player chooses to quit, exit the program
    if((first.compare("Q") == 0) || (first.compare("q") == 0))
    {
        exit (0);
    }

    //If player chooses to print memory, print memory
    if((first.compare("D") == 0) || (first.compare("d") == 0))
    {
        printMemory(gameArray, roomArray, numRooms);
        doMoveZombie = 0;
    }

    //If player chooses to shoot a room, take in their desired room
    //then attempt to shoot at the room, Move the zombie afterwards
    if((first.compare("S") == 0) || (first.compare("s") == 0))
    {
        cin >> room;
        shootRoom(room, gameArray, roomArray, zombieRoom);
        doMoveZombie = 1;
    }

    //If player chooses to move rooms, take in their desired room
    //then attempt to move to it. Move the zombie afterwards
    if((first.compare("M") == 0) || (first.compare("m") == 0))
    {
       cin >> room;
       moveRoom(room, gameArray, roomArray, haveGrail, zombieRoom, currentRoom);
       doMoveZombie = 1;
    }

    //If the zombie is alive and needs to be moved, move it.
    if(doMoveZombie && (zombieRoom != 0))
    {
        moveZombie(roomArray, zombieRoom);

        //If the player walks into the zombie room, they lose the game
        if(gameArray[0] == zombieRoom)
        {
            winOrLose(0, gameArray);

        }
    }


}
//Post: Selection has been executed

//Pre: The menu function has been called
void waitForMove(int gameArray[], int roomArray[][7], bool& haveGrail, int& zombieRoom, int& numRooms, int& currentRoom)
{
    //Loop flag for menu loop
    bool loopFlag = 0;

   //String for choice to be validated
    std::string toValidate;

    //Display the menu until a valid selection has been made.
    do
    {
        cout << "Player menu:" << endl
        << "Q: End the game" << endl
        << "D: Print the game memory" << endl
        << "S #: Shoot into the selected room" << endl
        << "M #: Move to the selected room" << endl;

        cin >> toValidate;
        if(validateSelection(toValidate) == 1)
        {
            doSelection(toValidate, gameArray, roomArray, haveGrail, zombieRoom, numRooms, currentRoom);
            loopFlag = 1;
        }

        else
            cout << "That selection was invalid.";
    }while(loopFlag == 0);

}
//Post:: A menu selection has been made and executed

//Pre: The player has successfully moved into a new room
void checkRoom(int gameArray[], int roomArray[][7], bool& haveGrail, int& currentRoom, int& zombieRoom, int& numRooms)
{
    //Display how many bullets are left
    cout << endl << "Current room: " << gameArray[0] << endl
    << "Bullets left: " << gameArray[1] << endl;

    //If the player has the grail, notify them
    if(haveGrail)
        cout << "You have the grail!" << endl;

    //If the zombie is in a room adjacent to the player, notify them
    if(checkNearZombie(gameArray[0], roomArray))
        cout << "You hear a zombie snarl nearby..." << endl;

    //If the grail is in a room adjacent to the player, notify them
    if(checkNearGrail(gameArray[0], roomArray))
        cout << "You feel the grail's aura somewhere nearby..." << endl;

    //Show all connected rooms to the player's current room
    showConnectedRooms(currentRoom, roomArray);

    //Wait for the player to select a move
    waitForMove(gameArray, roomArray, haveGrail, zombieRoom, numRooms, currentRoom);

}
//Post: The player is notified about the traits of their new room

//Pre: Nothing
void running(int gameArray[], int roomArray[][7], bool& haveGrail, int& currentRoom, int& zombieRoom, int& numRooms)
{
    //If the game has been called to end, end it
    //While the game should still be running, check each new room
    while(gameArray[0] != -1)
        checkRoom(gameArray, roomArray, haveGrail, currentRoom, zombieRoom, numRooms);
}
//Post: The game has either kept running or ended








