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


//       menuSelection = 0;
   bool haveGrail = false;

   //readMaze(roomArray, gameArray, numRooms);
   //getRandomRoom(gameArray, numRooms);
   //placeZombie(roomArray, gameArray, numRooms);

   reset(currentRoom, zombieRoom, numBullets, numRooms, haveGrail);

   menu(currentRoom, zombieRoom, numBullets, numRooms, haveGrail, gameArray, roomArray);

        return 0;



    while(1)
    {
    menu(currentRoom, zombieRoom, numBullets, numRooms, haveGrail, gameArray, roomArray);
    }


	return 0;
}
// build your functions here. Don't forget your pre and post conditions

void reset(int &currentRoom, int &zombieRoom, int &numBullets, int &numRooms, bool &haveGrail)
{
    currentRoom = 0;
    zombieRoom = 0;
    numBullets = 0;
    numRooms = 0;
    haveGrail = 0;
}

void instructions()
{
    cout << "YOUR MISSION, SHOULD YOU CHOOSE TO ACCEPT IT, IS TO SEEK THE HOLY " <<
            "GRAIL WITHIN THE RUINS OF AN ANCIENT CHURCH. TO SUCCEED YOU MUST " <<
            "ENTER THE CHURCH, AVOID THE UNDEAD GUARDIANS, FIND THE GRAIL AND " <<
            "ESCAPE. YOU HAVE SIX SILVER BULLETS TO PROTECT YOU. IF THE ZOMBIES " <<
            "FIND YOU OR YOU RUN OUT OF BULLETS YOU WILL LOSE!" << endl << endl;

}

void printMemory(int gameArray[], int roomArray[][7], int& numRooms)
{
    cout << "Game Array: " << endl << "  [ current][ bullets][    rooms]" << endl << "  ";
    for(int i = 0; i  < 3; i++)
    {
        cout << "[       " << gameArray[i] << "]";
    }
    cout << endl << endl;

    cout << "        [ north][ south][  west][  east][player][zombie][ grail]" << endl;
    for(int i = 0; i < numRooms; i++)
        {
            cout << "[R" << std::setw(2) << (i+1) << "] ->";
            for(int j = 0; j < 7; j++)
            {
            cout << "[" << std::setw(6) << roomArray[i][j] << "]";
            }
            cout << endl;
        }
        cout << endl;


}

bool menu(int &currentRoom, int &zombieRoom, int &numBullets, int &numRooms, bool &haveGrail, int gameArray[],int roomArray[][7])
{
    cout << "       " << "Enter, if you dare..." << endl
    << "*** THE TOMB OF THE LIVING DEAD ***" << endl << endl;

    cout << std::setw(6) << "Main Menu" << endl << "1. Instructions" << endl
    << "2. Begin" << endl << "3. Exit" << endl;

    cout << "Enter Menu Selection: ";

    int selection = 0;
    bool loopFlag = 1;



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

int getRandomRoom(int gameArray[5], int& numRooms)
{

int randRoom = rand() % numRooms + 1;
return randRoom;
}

void placeZombie(int roomArray[][7], int gameArray[5], int& numRooms, int& zombieRoom)
{

    while(zombieRoom < (numRooms / 2 + 1))
    {
        zombieRoom = getRandomRoom(gameArray, numRooms);
    }
    cout << "Zombie Room: " << zombieRoom << endl;
    roomArray[zombieRoom - 1][5] = 1;

}

void placeGrail(int roomArray[][7], int gameArray[5], int& numRooms)
{

    int grailRoom = 0;
    while(grailRoom < (numRooms / 2 + 1))
    {
        grailRoom = getRandomRoom(gameArray, numRooms);
    }
    cout << "Grail Room: " << grailRoom << endl;
    roomArray[grailRoom - 1][6] = 1;

}

void setup(int &currentRoom, int &zombieRoom, int &numBullets, int &numRooms, bool &haveGrail, int roomArray[][7], int gameArray[])
{
    srand(time(0));

    readMaze(roomArray, gameArray, numRooms);

    gameArray[0] = 1;

    roomArray[0][4] = 1;
    currentRoom = 1;

    gameArray[1] = MAX_BULLETS;

    numBullets = MAX_BULLETS;



    gameArray[2] = numRooms;

    placeZombie(roomArray, gameArray, numRooms, zombieRoom);
    placeGrail(roomArray, gameArray, numRooms);

    running(gameArray, roomArray, haveGrail, currentRoom, zombieRoom, numRooms);

}

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
                << "\nYOU LOST!\n";
    }
    gameArray[CURRENT_ROOM_INDEX] = -1;
}

void showConnectedRooms(int &currentRoom, int roomArray[][7])
{
    for(int i = 0; i < MAX_ROOMS; i++)
        for(int j = 0; j < 4; j++)
            if(roomArray[i][j] == currentRoom)
                cout << "Room " << (i + 1) << " is connected to the current room(Room " << currentRoom << ").\n" ;
                cout << endl;
}

bool isConnected(int targetRoom, int gameArray[], int roomArray[][7])
{
    bool isConnected = false;
    //cout << "Please enter a room number to test if it is connected to current room: ";
    //int x = inputValidation(1, MAX_ROOMS);
    for(int j = 0; j < 4; j++)
        if(roomArray[targetRoom-1][j] == gameArray[CURRENT_ROOM_INDEX])
            isConnected = true;
            return isConnected;
}

void moveRoom(int x, int gameArray[], int roomArray[][7], bool& haveGrail, int& zombieRoom, int& currentRoom)
{

    if(isConnected(x, gameArray, roomArray) == 1)
    {
    roomArray[gameArray[CURRENT_ROOM_INDEX -1]][PLAYER_INDEX] = 1;

    roomArray[x - 1][PLAYER_INDEX] = 1;

    gameArray[0] = x;
    currentRoom = x;


    if(haveGrail == 1)
    {
        for(int i = 0; i < gameArray[2]; i++)
        {
        roomArray[i][GRAIL_INDEX] = 0;
        }
        roomArray[x - 1][GRAIL_INDEX] = 1;


    }

        if(checkGrail(gameArray[0], roomArray))
        {
        haveGrail = 1;
        }

        if((gameArray[0] == 1) && (haveGrail == 1))
        {
            winOrLose(1, gameArray);
            exit(0);
        }

        if(gameArray[0] == zombieRoom)
        {
            winOrLose(0, gameArray);
        }



    }

    else
    {
    cout << "The room you selected is not connected to you." << endl;
    }

}

void shootRoom(int x, int gameArray[], int roomArray[][7], int& zombieRoom)
{


    if(isConnected(x, gameArray, roomArray) == 1)
    {
     gameArray[1]--;
     if(checkZombie(x, roomArray) == 1)
     {
            roomArray[x - 1][ZOMBIE_INDEX] = 0;
            zombieRoom = 0;
            cout << "You hit the zombie and fell to the floor! You feel a great sense of peace." << endl;
     }
     else
     {
         cout << "The zombie was not in the room, you missed!\n";
     }
    }
    else
     {
         cout << "The room you selected is not connected to your current room.\n";
     }

}


void moveZombie(int roomArray[][7], int& zombieRoom)
{
    if(zombieRoom)
    {
    int newRoom;

    newRoom = rand() % 4;

        if(roomArray[zombieRoom - 1][newRoom] != 0)
        {
        roomArray[zombieRoom - 1][ZOMBIE_INDEX] = 0;

        roomArray[roomArray[zombieRoom - 1][newRoom] - 1][5] = 1;

        zombieRoom = roomArray[zombieRoom - 1][newRoom];
        }
    }


}

bool validateSelection(std::string toValidate)
{
    std::string first = toValidate.substr(0,1);
    if(first.compare("Q") == 0)
        return 1;

    if(first.compare("D") == 0)
        return 1;

    if(first.compare("S") == 0)
        return 1;

    if(first.compare("M") == 0)
        return 1;

    else
    return 0;

}

void doSelection(std::string control, int gameArray[], int roomArray[][7], bool& haveGrail, int& zombieRoom, int& numRooms, int& currentRoom)
{
    std::string first = control.substr(0,1);

    int room;
    bool doMoveZombie = 0;

    if(first.compare("Q") == 0)
    {
        exit (0);
    }

    if(first.compare("D") == 0)
    {
        printMemory(gameArray, roomArray, numRooms);
        doMoveZombie = 0;
    }

    if(first.compare("S") == 0)
    {
        cin >> room;
        shootRoom(room, gameArray, roomArray, zombieRoom);
        doMoveZombie = 1;
    }

    if(first.compare("M") == 0)
    {
       cin >> room;
       moveRoom(room, gameArray, roomArray, haveGrail, zombieRoom, currentRoom);
       doMoveZombie = 1;
    }


    if(doMoveZombie && (zombieRoom != 0))
    {
        moveZombie(roomArray, zombieRoom);
        if(gameArray[0] == zombieRoom)
        {
            winOrLose(0, gameArray);

        }
    }


}

void waitForMove(int gameArray[], int roomArray[][7], bool& haveGrail, int& zombieRoom, int& numRooms, int& currentRoom)
{
    bool loopFlag = 0;

    std::string toValidate;

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

void checkRoom(int gameArray[], int roomArray[][7], bool& haveGrail, int& currentRoom, int& zombieRoom, int& numRooms)
{
    cout << endl << "Current room: " << gameArray[0] << endl
    << "Bullets left: " << gameArray[1] << endl;

    if(haveGrail)
        cout << "You have the grail!" << endl;

    if(checkNearZombie(gameArray[0], roomArray))
        cout << "You hear a zombie snarl nearby..." << endl;

    if(checkNearGrail(gameArray[0], roomArray))
        cout << "You feel the grail's aura somewhere nearby..." << endl;

    showConnectedRooms(currentRoom, roomArray);

    waitForMove(gameArray, roomArray, haveGrail, zombieRoom, numRooms, currentRoom);

}

void running(int gameArray[], int roomArray[][7], bool& haveGrail, int& currentRoom, int& zombieRoom, int& numRooms)
{
    while(gameArray[0] != -1)
        checkRoom(gameArray, roomArray, haveGrail, currentRoom, zombieRoom, numRooms);
}








