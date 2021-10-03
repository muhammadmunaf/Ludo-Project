//============================================================================
// Name        : Muhammad Munaf
// Final Project of Programming Fundamentals
//============================================================================

#ifndef CENTIPEDE_CPP_
#define CENTIPEDE_CPP_
#include "util.h"
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <fstream>

using namespace std;

//Prototypes
void DiceRollGenerator();
void GitiOutsideHouse();
void Movement();
void TurnCheck();
void PlayerDeadChecker();
bool DoubleGitiCheck();
void WinPathCheck();
bool SafeBlocks();
void ScoreCalculator();
void GitiInHouse();
void BlockChecker();
void Leaderboard();
void WritingLeaderboardFile();
bool FirstTurnCheck();


/* Variables and their uses
- start is to show menu before going into the actual game
- no_of_players will input the number of players from the user
- dice_roll stores the dice roll of the current turn
- move_giti stores the x-cordinate of whichever giti the user wants to move in that turn
	(wherever move_giti+1 is used it refers to the y-cordinate of that giti)
- takenout is to make sure if user takes out a giti on 6 then no other giti moves and to stop the score from increasing for no reason
*/
int start=0, no_of_players=0, dice_roll=0, turn=0, move_giti=20, takenout=0;

string score_show[4] = {"0","0","0","0"}; // this array stores the scores of each player in a string

string HighScore_Array[10]; // for leaderboard view

/* Explanation for following array, column wise:
	player #, score, 1x, 1y, 2x, 2y, 3x, 3y, 4x, 4y, giti(s) killed, giti(s) inside house
- where 1x is position of first giti on x axis, and 1y is of 1st giti on y axis
	(initially, all these have the cordinates of them inside their base)
- gitis killed is used to determine whether that player's giti can go into the home path
- gitis inside hosue is to check if player has won or not
*/
int player_data[4][12]={
	{1,0,119,119,199,119,119,199,199,199,0,0}, 
	{2,0,199,631,119,631,119,551,199,551,0,0},
	{3,0,551,551,631,631,631,551,551,631,0,0},
	{4,0,631,199,631,119,551,119,551,199,0,0}
};

/* This function will generate a random number between 1-6 for a dice roll
Made this a sepearte function to keep everyting simple, if i put this inside
the PrintableKeys function it would get really messy
*/
void DiceRollGenerator()
{
	srand(time(0));
	do{
		dice_roll=rand()%10;
	}while(dice_roll<1 || dice_roll>6);

	cout << dice_roll << endl;
	
}

/* This function checks if a block is being formed or not and if it is, then 
it will add +2 to the score of that player it is different than the function
DoubleGitiCheck as that function checks if there is a block of another player
in it's current position but this function checks if a block is formed of the
player whose turn it is
*/
void BlockChecker()
{
	for(int i=2;i<9;i+=2)
	{
		if((player_data[turn][move_giti]==player_data[turn][i] && player_data[turn][move_giti+1]==player_data[turn][i+1]) && move_giti!=i)
		{
			player_data[turn][1]+=2;
			cout << "Block Made\n";
		}
	}
}

/* This function will check if there is a giti on the current position or not, if there is, it 
returns true, otherwise false which is used to decide if other giti can move over it or kill it
*/
bool DoubleGitiCheck()
{
	for (int i=0; i<4; i++)
	{
		for(int j=2;j<9;j+=2)
		{
			for(int k=2;k<9;k+=2)
			{
				if(player_data[i][j]==player_data[i][k] && player_data[i][j+1]==player_data[i][k+1] && k!=j)
				{
					if(player_data[turn][move_giti]==player_data[i][j] && turn!=i)
					{
						//cout << "double\n";
						return true;
					}
				}
			}	
		}
	}
	//cout << "not double\n";
	return false;
}

/* This function will bring the giti out of the base to the starting point
It will check if dice roll was 6 or not, if yes it'll compare the values of each giti with it's inital one
if it matches it will give option of either taking it out or move some other giti by 6
*/
void GitiOutsideHouse()
{
	//this array stores the initial values of all the pieces of each player, it is used to check whether a 
	//piece can be taken out or not when player gets a 6
	static int check_house_array[4][4]={
		{119,199,119,199},
		{199,119,119,199},
		{551,631,631,551},
		{631,631,551,551}
	};

	
	int var=2; //this will check only x values of each piece to check if it's inside base or not

	while(dice_roll==6 && takenout==0)
	{
		for(int i=0;i<4;i++)
		{
			if(player_data[turn][var]==check_house_array[turn][i])
			{
				if(turn==0)
				{
					player_data[turn][var]=327;
					player_data[turn][var+1]=87;
					takenout=1;
				}
				else if(turn==1)
				{
					player_data[turn][var]=87;
					player_data[turn][var+1]=423;
					takenout=1;
				}
				else if(turn==2)
				{
					player_data[turn][var]=423;
					player_data[turn][var+1]=663;
					takenout=1;
				}
				else if(turn==3)
				{
					player_data[turn][var]=663;
					player_data[turn][var+1]=327;
					takenout=1;
				}				
			}
		}
		var+=2;
		if(var==10)
			break;
		
	}
}	

/* This function manages the movement of each giti. 
it moves every giti from it's respective position forward and a loop is used to move it multiple times according 
to the dice roll.
It is really confusing so a sepearte function is nessesary
*/
void Movement()
{
	
	for(int i=0; i<dice_roll;i++)
	{
		if(takenout==1)
		{
			break;
		}

		if(DoubleGitiCheck()) // if giti is on another giti which is double, it won't move forward
		{
			break;
		}
	
		if((player_data[turn][move_giti]==375 && player_data[turn][move_giti+1]<400) && turn==0)
			WinPathCheck();
		else if((player_data[turn][move_giti+1]==375 && player_data[turn][move_giti]<400) && turn==1)
			WinPathCheck();
		else if((player_data[turn][move_giti]==375 && player_data[turn][move_giti+1]>400) && turn==2)
			WinPathCheck();
		else if((player_data[turn][move_giti+1]==375 && player_data[turn][move_giti]>400) && turn==3)
			WinPathCheck();
		else if (player_data[turn][move_giti]==327 && (player_data[turn][move_giti+1]>=39 && player_data[turn][move_giti+1]<232))
		{
			player_data[turn][move_giti+1] += 48;
			player_data[turn][1] += 1;
		}
		else if(player_data[turn][move_giti]==327 && player_data[turn][move_giti+1]<=280) 
		{
			player_data[turn][move_giti]=279;
			player_data[turn][move_giti+1]=327;
			player_data[turn][1] += 1;
		}
		else if (player_data[turn][move_giti+1]==327 && (player_data[turn][move_giti]>39 && player_data[turn][move_giti]<=279))
		{
			player_data[turn][move_giti] -= 48;	
			player_data[turn][1] += 1;
		}
		else if (player_data[turn][move_giti]==39 && (player_data[turn][move_giti+1]>=327 && player_data[turn][move_giti+1]<423))
		{
			player_data[turn][move_giti+1] += 48;
			player_data[turn][1] += 1;	
		}
		else if (player_data[turn][move_giti+1]==423 && (player_data[turn][move_giti]>=39 && player_data[turn][move_giti]<279))
		{
			player_data[turn][move_giti] += 48;	
			player_data[turn][1] += 1;
		}
		else if(player_data[turn][move_giti]==279 && player_data[turn][move_giti+1]==423)
		{
			player_data[turn][move_giti]=327;
			player_data[turn][move_giti+1]=471;
			player_data[turn][1] += 1;
		}
		else if (player_data[turn][move_giti]==327 && (player_data[turn][move_giti+1]>=471 && player_data[turn][move_giti+1]<711))
		{
			player_data[turn][move_giti+1] += 48;	
			player_data[turn][1] += 1;
		}
		else if (player_data[turn][move_giti+1]==711 && (player_data[turn][move_giti]>=327 && player_data[turn][move_giti]<423))
		{
			player_data[turn][move_giti] += 48;	
			player_data[turn][1] += 1;
		}
		else if (player_data[turn][move_giti]==423 && (player_data[turn][move_giti+1]>471 && player_data[turn][move_giti+1]<=711))
		{
			player_data[turn][move_giti+1] -= 48;	
			player_data[turn][1] += 1;
		}
		else if(player_data[turn][move_giti]==423 && player_data[turn][move_giti+1]==471)
		{
			player_data[turn][move_giti]=471;
			player_data[turn][move_giti+1]=423;
			player_data[turn][1] += 1;
		}
		else if (player_data[turn][move_giti+1]==423 && (player_data[turn][move_giti]>=471 && player_data[turn][move_giti]<711))
		{
			player_data[turn][move_giti] += 48;
			player_data[turn][1] += 1;
		}
		else if (player_data[turn][move_giti]==711 && (player_data[turn][move_giti+1]<=431 && player_data[turn][move_giti+1]>327))
		{
			player_data[turn][move_giti+1] -= 48;
			player_data[turn][1] += 1;
		}
		else if (player_data[turn][move_giti+1]==327 && (player_data[turn][move_giti]<=711 && player_data[turn][move_giti]>471))
		{
			player_data[turn][move_giti] -= 48;	
			player_data[turn][1] += 1;		
		}
		else if(player_data[turn][move_giti]==471 && player_data[turn][move_giti+1]==327)
		{
			player_data[turn][move_giti]=423;
			player_data[turn][move_giti+1]=279;
			player_data[turn][1] += 1;
		}
		else if (player_data[turn][move_giti]==423 && (player_data[turn][move_giti+1]<=279 && player_data[turn][move_giti+1]>39))
		{
			player_data[turn][move_giti+1] -= 48;
			player_data[turn][1] += 1;
		}
		else if (player_data[turn][move_giti+1]==39 && (player_data[turn][move_giti]<=423 && player_data[turn][move_giti]>327))
		{
			player_data[turn][move_giti] -= 48;	
			player_data[turn][1] += 1;	
		}
	}

}

/* This function checks whose turn will be next, not nessery to make a seperate
 function for this but made one as it is used more than once, so to keep it simple function is made
*/
void TurnCheck()
{
	if(turn==no_of_players)
	{
		turn=0;
	}
}

/* This function will see if the giti of whose turn is, comes on the top of another giti
which is not a block, it will send the other one back to it's base
*/
void PlayerDeadChecker()
{
	for(int i=0;i<4;i++)
		for(int j=2;j<9;j+=2)
		{
			if((player_data[turn][move_giti]==player_data[i][j] && player_data[turn][move_giti+1]==player_data[i][j+1]) && turn!=i)
			{
				if(SafeBlocks()) // if gitis are on a safe block, it is not killed
				{
					return;
				}

				if(DoubleGitiCheck()) // if gitis are double, none will be removed to put back into the base
				{
					return;
				}

				if(i==0)
				{
					player_data[i][j]=119;
					player_data[i][j+1]=119;
					player_data[turn][10]++;
				}
				else if(i==1)
				{
					player_data[i][j]=199;
					player_data[i][j+1]=631;
					player_data[turn][10]++;
				}
				else if(i==2)
				{
					player_data[i][j]=551;
					player_data[i][j+1]=551;
					player_data[turn][10]++;
				}
				else if(i==3)
				{
					player_data[i][j]=631;
					player_data[i][j+1]=199;
					player_data[turn][10]++;
				}
				else
				{
					return;
				}
				
				player_data[turn][1]+=10; // adding +10 to the score for removing a giti
			}
		}

	
}

/* This function will check if any giti is in the position where it can enter the path to its house
and if it can then it'll move it accordingly
also checks whether that giti has killed another one before or not
*/
void WinPathCheck()
{
	if(turn==0) // for red
	{
		if((player_data[turn][move_giti]==375 && player_data[turn][move_giti+1]<400) && player_data[turn][10]!=0)
		{
			if(player_data[turn][move_giti+1]<327)
			{
				player_data[turn][move_giti+1]+=48;
				player_data[turn][1]++;
				GitiInHouse();
			}
		}
	}
	else if(turn==1) // for blue
	{
		if((player_data[turn][move_giti+1]==375 && player_data[turn][move_giti]<400) && player_data[turn][10]!=0)
		{
			if(player_data[turn][move_giti]<327)
			{
				player_data[turn][move_giti]+=48;
				player_data[turn][1]++;
				GitiInHouse();
			}	
		}
	}
	else if(turn==2) // for yellow
	{
		if((player_data[turn][move_giti]==375 && player_data[turn][move_giti+1]>400) && player_data[turn][10]!=0)
		{
			if(player_data[turn][move_giti+1]>423)
			{
				player_data[turn][move_giti+1]-=48;
				player_data[turn][1]++;
				GitiInHouse();
			}	
		}
	}
	else if(turn==3) // for green
	{
		if((player_data[turn][move_giti+1]==375 && player_data[turn][move_giti]>400) && player_data[turn][10]!=0)
		{
			if(player_data[turn][move_giti]>423)
			{
				player_data[turn][move_giti]-=48;
				player_data[turn][1]++;
				GitiInHouse();
			}	
		}
	}
}

/* the following function will check if any giti is completely inside the house or not
if it is, it will add 15 score to the respective player
a seperate function is used as this code is used multiple times and to keep things simple adn clean
*/
void GitiInHouse()
{
	if(turn==0) // red
	{
		if(player_data[turn][move_giti]==375 && player_data[turn][move_giti+1]==327)
		{
			player_data[turn][1]+= 14; // adding score
			player_data[turn][11]++; // adding no. of giti inside house
		}
	}
	else if(turn==1) // blue
	{
		if(player_data[turn][move_giti]==327 && player_data[turn][move_giti+1]==375)
		{
			player_data[turn][1]+= 14;
			player_data[turn][11]++;
		}
	}
	else if(turn==2) // yellow
	{
		if(player_data[turn][move_giti]==375 && player_data[turn][move_giti+1]==423)
		{
			player_data[turn][1]+= 14;
			player_data[turn][11]++;
		}
	}
	else if(turn==3) // green
	{
		if(player_data[turn][move_giti]==423 && player_data[turn][move_giti+1]==375)
		{
			player_data[turn][1]+= 14;
			player_data[turn][11]++;
		}
	}
}

/* this function converts the integer score from the player_data array to string into the score_show array
It is required as an int variable/array can't be used in the DrawString() function provided
*/
void ScoreCalculator()
{
    stringstream score_temp;
    score_temp << player_data[turn][1];
	score_show[turn]= score_temp.str();
}

/* This function checks if giti is on a safe block or not and then returns true or false respectively
It does that by checking if the corrdinates of the giti matches those of any of the safe blocks 
*/
bool SafeBlocks()
{
	if(player_data[turn][move_giti]==327 && player_data[turn][move_giti+1]==87) //red
	{
		cout << "Safe Block\n";
		return true;
	}	
	else if(player_data[turn][move_giti]==87 && player_data[turn][move_giti+1]==423) //blue
	{
		cout << "Safe Block\n";
		return true;
	}
	else if(player_data[turn][move_giti]==423 && player_data[turn][move_giti+1]==663) //yellow
	{
		cout << "Safe Block\n";
		return true;
	}
	else if(player_data[turn][move_giti]==663 && player_data[turn][move_giti+1]==327) //green
	{
		cout << "Safe Block\n";
		return true;
	}
	else if(player_data[turn][move_giti]==135 && player_data[turn][move_giti+1]==327) //gray.red
	{
		cout << "Safe Block\n";
		return true;
	}
	else if(player_data[turn][move_giti]==327 && player_data[turn][move_giti+1]==615) //gray.blue
	{
		cout << "Safe Block\n";
		return true;
	}
	else if(player_data[turn][move_giti]==615 && player_data[turn][move_giti+1]==423) //gray.yellow
	{
		cout << "Safe Block\n";
		return true;
	}
	else if(player_data[turn][move_giti]==423 && player_data[turn][move_giti+1]==135) //gray.green
	{
		cout << "Safe Block\n";
		return true;
	}
	else 
		return false;
}

// function for canva size (not edited)
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

/* This function extracts the information from the file 'highscores.txt' and puts it in a string array of size 10
It is needed as we can't directly display the contents of the file on the screen
*/
void Leaderboard()
{
	ifstream file_hs("highscore.txt");
	if(file_hs.is_open())
	{
		for(int i=0;i<10;i++)
			file_hs >> HighScore_Array[i];
	}
	else
		cout << "File not accessed";

	int check=0; // to stop the program from replacing every entry below it, it will make sure only 1 is removed

	for(int i=0; i<10 ; i++) // to check if any current score is more than the ones in the highscore list
	{
		int score;
		score = stoi(HighScore_Array[i]); // converting string to integer so it can be compared

		for(int j=0 ; j<4 ; j++)
		{
			if(score<player_data[j][1]) 
			{
				string *arr_scores = new string[11];

				for(int k=0;k<10;k++) // copying the array
				{
					if(k>=i) // to leave space for the new entry
					{
						arr_scores[k+1]=HighScore_Array[k];
					}
					else
						arr_scores[k]=HighScore_Array[k];
				}

				if(j==0)
					arr_scores[i]= score_show[j] + "-Player1";
				else if(j==1)
					arr_scores[i]= score_show[j] + "-Player2";
				else if(j==2)
					arr_scores[i]= score_show[j] + "-Player3";
				else if(j==3)
					arr_scores[i]= score_show[j] + "-Player4";
				

				for(int k=0; k<10;k++) // to not copy the last value and keep the desending order
				{
					HighScore_Array[k]=arr_scores[k];
				}

				delete []arr_scores;
				check=1;
			}

			if(check==1)
				break;
		}

		if(check==1)
			break;
	}
	WritingLeaderboardFile();
}

/* This function is to rewrite the highscore.txt file to update it
*/
void WritingLeaderboardFile()
{
	ofstream myfile ("highscore.txt");
	if(myfile.is_open())
	{
		for(int i=0;i<10;i++)
			myfile << HighScore_Array[i] << endl;

		myfile.close();
	}
	else 
		cout << "File not written\n";

}

/* This function is to decide whose turn will be first according to the dice roll of each player
It takes the first dice roll of each player and stores it into an array and then finds the max value
and then gives the turn to the player having that max value
*/
bool FirstTurnCheck()
{
    static int firstroll[4] = {0,0,0,0}; // static to keep the values stored as the function is called again and again to fill the array
    int max=1;
    
    DiceRollGenerator();
	firstroll[turn]=dice_roll;
    
    if(turn==(no_of_players-1))
    {
        for(int i=0 ; i< 4 ; i++)
        {
            if(firstroll[i]>max)
            {
                turn=i;
                max=firstroll[i];
            }
        }
        //cout << "First turn: " << turn << endl;
        return true;
    }
    else
	{	turn ++;
        return false;
	}
}

/* Main canvas drawing function
*/
void GameDisplay(){
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(0/*Red Component*/, 0,	//148.0/255/*Green Component*/,
			0.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors

	//Displaying the Ludo Board
	{
		DrawSquare( 0, 0, 750, colors[BLACK]); //setting black background
		
		//squares grid
		int rows=15,cols=15;
		for(int i=0 ; i<15 ; i++)
		{
			for(int j=0 ; j<15 ; j++)
			{
				DrawSquare( rows, cols, 48, colors[WHITE]);
				cols+=48;
			}
			rows+=48;
			cols=15;
		}

		//bases
		DrawSquare(15, 15, 288, colors[RED]); //down-left
		DrawSquare(15, 447, 288, colors[BLUE]); //top-left
		DrawSquare(447, 15, 288, colors[GREEN]); //down-right
		DrawSquare(447, 447, 288, colors[YELLOW]); //top-right

		//circle inside base
		DrawCircle(159, 159, 100, colors[WHITE]); // red
		DrawCircle(591, 591, 100, colors[WHITE]); // yellow
		DrawCircle(159, 591, 100, colors[WHITE]); // blue
		DrawCircle(591, 159, 100, colors[WHITE]); // green

		//blue home path
		rows=63;
		for(int i=0 ; i<5 ; i++)
		{
			DrawSquare(rows, 351, 48, colors[BLUE]);
			rows+=48;
		}

		//green home path
		rows=447;
		for(int i=0 ; i<5 ; i++)
		{
			DrawSquare(rows, 351, 48, colors[GREEN]);
			rows+=48;
		}

		//red home path
		cols=63;
		for(int i=0 ; i<5 ; i++)
		{
			DrawSquare( 351, cols, 48, colors[RED]);
			cols+=48;
		}

		//Yellow home path
		cols=447;
		for(int i=0 ; i<5 ; i++)
		{
			DrawSquare( 351, cols, 48, colors[YELLOW]);
			cols+=48;
		}
		//homes
		DrawTriangle(303, 303, 303, 447, 375, 375, colors[BLUE]);
		DrawTriangle(303, 303, 447, 303, 375, 375, colors[RED]);
		DrawTriangle(303, 447, 447, 447, 375, 375, colors[YELLOW]);
		DrawTriangle(447, 303, 447, 447, 375, 375, colors[GREEN]);

		//base start points
		DrawSquare(63, 399, 48, colors[BLUE]);
		DrawSquare(303, 63, 48, colors[RED]);
		DrawSquare(639, 303, 48, colors[GREEN]);
		DrawSquare(399, 639, 48, colors[YELLOW]);

		//stops (safe blocks)
		DrawSquare(399, 111, 48, colors[GRAY]);
		DrawSquare(111, 303, 48, colors[GRAY]);
		DrawSquare(303, 591, 48, colors[GRAY]);
		DrawSquare(591, 399, 48, colors[GRAY]);

		//extra lines to make the pattern perfect
		DrawLine(303, 447, 447, 447, 1, colors[BLACK]);
		DrawLine(447, 303, 447, 447, 1, colors[BLACK]);
	}
	//scoreboard
	{
		DrawRectangle(750,500, 320, 235, colors[WHITE]); // white area for board
		DrawString( 770, 700, "Score Board", colors[BLACK]);
		DrawString( 770, 450, "Press Spacebar to roll dice", colors[WHITE]);

		//showing players on scoreboard
		switch(no_of_players)
		{
			case 2:
					DrawString( 770, 650, "Player 1 ", colors[RED]);
					DrawString( 770, 620, "Player 2 ", colors[BLUE]);
					break;
			case 3:
					DrawString( 770, 650, "Player 1 ", colors[RED]);
					DrawString( 770, 620, "Player 2 ", colors[BLUE]);
					DrawString( 770, 590, "Player 3", colors[YELLOW]);
					break;
			case 4:
					DrawString( 770, 650, "Player 1", colors[RED]);
					DrawString( 770, 620, "Player 2", colors[BLUE]);
					DrawString( 770, 590, "Player 3", colors[YELLOW]);
					DrawString( 770, 560, "Player 4", colors[GREEN]);
					break;
		}

		// displaying scores
		switch(no_of_players)
		{
			case 2:
					DrawString( 900, 650, score_show[0], colors[RED]);
					DrawString( 900, 620, score_show[1], colors[BLUE]);
					break;
			case 3:
					DrawString( 900, 650, score_show[0], colors[RED]);
					DrawString( 900, 620, score_show[1], colors[BLUE]);
					DrawString( 900, 590, score_show[2], colors[YELLOW]);
					break;
			case 4:
					DrawString( 900, 650, score_show[0], colors[RED]);
					DrawString( 900, 620, score_show[1], colors[BLUE]);
					DrawString( 900, 590, score_show[2], colors[YELLOW]);
					DrawString( 900, 560, score_show[3], colors[GREEN]);
					break;
		}

		// shows which players have won, all 4 giti's inside house
		if(player_data[0][11]==4)
			DrawString( 960, 650, "Victory", colors[RED]);
		if(player_data[1][11]==4)
			DrawString( 960, 620, "Victory", colors[BLUE]);
		if(player_data[2][11]==4)
			DrawString( 960, 590, "Victory", colors[YELLOW]);
		if(player_data[3][11]==4)
			DrawString( 960, 560, "Victory", colors[GREEN]);
	}

	//dice rolls
	{
		switch (dice_roll)
		{
			case 1:
				DrawSquare(850,150,120,colors[WHITE]);
				DrawCircle(910,210,10, colors[BLACK]);
				break;
			case 2:
				DrawSquare(850,150,120,colors[WHITE]);
				DrawCircle(880,240,10, colors[BLACK]);
				DrawCircle(940,180,10, colors[BLACK]);
				break;
			case 3:
				DrawSquare(850,150,120,colors[WHITE]);
				DrawCircle(880,240,10, colors[BLACK]);
				DrawCircle(940,180,10, colors[BLACK]);
				DrawCircle(910,210,10, colors[BLACK]);
				break;
			case 4:
				DrawSquare(850,150,120,colors[WHITE]);
				DrawCircle(880,240,10, colors[BLACK]);
				DrawCircle(940,180,10, colors[BLACK]);
				DrawCircle(940,240,10, colors[BLACK]);
				DrawCircle(880,180,10, colors[BLACK]);
				break;
			case 5:
				DrawSquare(850,150,120,colors[WHITE]);
				DrawCircle(880,240,10, colors[BLACK]);
				DrawCircle(940,180,10, colors[BLACK]);
				DrawCircle(940,240,10, colors[BLACK]);
				DrawCircle(880,180,10, colors[BLACK]);
				DrawCircle(910,210,10, colors[BLACK]);
				break;
			case 6:
				DrawSquare(850,150,120,colors[WHITE]);
				DrawCircle(890,180,10, colors[BLACK]);
				DrawCircle(890,210,10, colors[BLACK]);
				DrawCircle(890,240,10, colors[BLACK]);
				DrawCircle(930,210,10, colors[BLACK]);
				DrawCircle(930,240,10, colors[BLACK]);
				DrawCircle(930,180,10, colors[BLACK]);
				break;
		}
	}
	
	//players gitis
	{

		/* for every giti
		- Black one is the border of each giti
		- Respective colors are to show whose giti it is
		- Letter to point each giti for each player
		*/

		switch (no_of_players)
		{
			case 2:
					DrawCircle(player_data[0][2], player_data[0][3], 20, colors[BLACK]);
					DrawCircle(player_data[0][4], player_data[0][5], 20, colors[BLACK]);
					DrawCircle(player_data[0][6], player_data[0][7], 20, colors[BLACK]);
					DrawCircle(player_data[0][8], player_data[0][9], 20, colors[BLACK]);
					DrawCircle(player_data[0][2], player_data[0][3], 15, colors[RED]);
					DrawCircle(player_data[0][4], player_data[0][5], 15, colors[RED]);
					DrawCircle(player_data[0][6], player_data[0][7], 15, colors[RED]);
					DrawCircle(player_data[0][8], player_data[0][9], 15, colors[RED]);
					DrawString((player_data[0][2]-8), (player_data[0][3]-8), "A", colors[WHITE]);
					DrawString((player_data[0][4]-8), (player_data[0][5]-8), "B", colors[WHITE]);
					DrawString((player_data[0][6]-8), (player_data[0][7]-8), "C", colors[WHITE]);
					DrawString((player_data[0][8]-8), (player_data[0][9]-8), "D", colors[WHITE]);

					DrawCircle(player_data[1][2], player_data[1][3], 20, colors[BLACK]);
					DrawCircle(player_data[1][4], player_data[1][5], 20, colors[BLACK]);
					DrawCircle(player_data[1][6], player_data[1][7], 20, colors[BLACK]);
					DrawCircle(player_data[1][8], player_data[1][9], 20, colors[BLACK]);
					DrawCircle(player_data[1][2], player_data[1][3], 15, colors[BLUE]);
					DrawCircle(player_data[1][4], player_data[1][5], 15, colors[BLUE]);
					DrawCircle(player_data[1][6], player_data[1][7], 15, colors[BLUE]);
					DrawCircle(player_data[1][8], player_data[1][9], 15, colors[BLUE]);
					DrawString((player_data[1][2]-8), (player_data[1][3]-8), "A", colors[WHITE]);
					DrawString((player_data[1][4]-8), (player_data[1][5]-8), "B", colors[WHITE]);
					DrawString((player_data[1][6]-8), (player_data[1][7]-8), "C", colors[WHITE]);
					DrawString((player_data[1][8]-8), (player_data[1][9]-8), "D", colors[WHITE]);

					break;
			case 3:
					DrawCircle(player_data[0][2], player_data[0][3], 20, colors[BLACK]);
					DrawCircle(player_data[0][4], player_data[0][5], 20, colors[BLACK]);
					DrawCircle(player_data[0][6], player_data[0][7], 20, colors[BLACK]);
					DrawCircle(player_data[0][8], player_data[0][9], 20, colors[BLACK]);
					DrawCircle(player_data[0][2], player_data[0][3], 15, colors[RED]);
					DrawCircle(player_data[0][4], player_data[0][5], 15, colors[RED]);
					DrawCircle(player_data[0][6], player_data[0][7], 15, colors[RED]);
					DrawCircle(player_data[0][8], player_data[0][9], 15, colors[RED]);
					DrawString((player_data[0][2]-8), (player_data[0][3]-8), "A", colors[WHITE]);
					DrawString((player_data[0][4]-8), (player_data[0][5]-8), "B", colors[WHITE]);
					DrawString((player_data[0][6]-8), (player_data[0][7]-8), "C", colors[WHITE]);
					DrawString((player_data[0][8]-8), (player_data[0][9]-8), "D", colors[WHITE]);

					DrawCircle(player_data[1][2], player_data[1][3], 20, colors[BLACK]);
					DrawCircle(player_data[1][4], player_data[1][5], 20, colors[BLACK]);
					DrawCircle(player_data[1][6], player_data[1][7], 20, colors[BLACK]);
					DrawCircle(player_data[1][8], player_data[1][9], 20, colors[BLACK]);
					DrawCircle(player_data[1][2], player_data[1][3], 15, colors[BLUE]);
					DrawCircle(player_data[1][4], player_data[1][5], 15, colors[BLUE]);
					DrawCircle(player_data[1][6], player_data[1][7], 15, colors[BLUE]);
					DrawCircle(player_data[1][8], player_data[1][9], 15, colors[BLUE]);
					DrawString((player_data[1][2]-8), (player_data[1][3]-8), "A", colors[WHITE]);
					DrawString((player_data[1][4]-8), (player_data[1][5]-8), "B", colors[WHITE]);
					DrawString((player_data[1][6]-8), (player_data[1][7]-8), "C", colors[WHITE]);
					DrawString((player_data[1][8]-8), (player_data[1][9]-8), "D", colors[WHITE]);

					DrawCircle(player_data[2][2], player_data[2][3], 20, colors[BLACK]);
					DrawCircle(player_data[2][4], player_data[2][5], 20, colors[BLACK]);
					DrawCircle(player_data[2][6], player_data[2][7], 20, colors[BLACK]);
					DrawCircle(player_data[2][8], player_data[2][9], 20, colors[BLACK]);
					DrawCircle(player_data[2][2], player_data[2][3], 15, colors[YELLOW]);
					DrawCircle(player_data[2][4], player_data[2][5], 15, colors[YELLOW]);
					DrawCircle(player_data[2][6], player_data[2][7], 15, colors[YELLOW]);
					DrawCircle(player_data[2][8], player_data[2][9], 15, colors[YELLOW]);
					DrawString((player_data[2][2]-8), (player_data[2][3]-8), "A", colors[WHITE]);
					DrawString((player_data[2][4]-8), (player_data[2][5]-8), "B", colors[WHITE]);
					DrawString((player_data[2][6]-8), (player_data[2][7]-8), "C", colors[WHITE]);
					DrawString((player_data[2][8]-8), (player_data[2][9]-8), "D", colors[WHITE]);
					break;
			case 4:
					DrawCircle(player_data[0][2], player_data[0][3], 20, colors[BLACK]);
					DrawCircle(player_data[0][4], player_data[0][5], 20, colors[BLACK]);
					DrawCircle(player_data[0][6], player_data[0][7], 20, colors[BLACK]);
					DrawCircle(player_data[0][8], player_data[0][9], 20, colors[BLACK]);
					DrawCircle(player_data[0][2], player_data[0][3], 15, colors[RED]);
					DrawCircle(player_data[0][4], player_data[0][5], 15, colors[RED]);
					DrawCircle(player_data[0][6], player_data[0][7], 15, colors[RED]);
					DrawCircle(player_data[0][8], player_data[0][9], 15, colors[RED]);
					DrawString((player_data[0][2]-8), (player_data[0][3]-8), "A", colors[WHITE]);
					DrawString((player_data[0][4]-8), (player_data[0][5]-8), "B", colors[WHITE]);
					DrawString((player_data[0][6]-8), (player_data[0][7]-8), "C", colors[WHITE]);
					DrawString((player_data[0][8]-8), (player_data[0][9]-8), "D", colors[WHITE]);

					DrawCircle(player_data[1][2], player_data[1][3], 20, colors[BLACK]);
					DrawCircle(player_data[1][4], player_data[1][5], 20, colors[BLACK]);
					DrawCircle(player_data[1][6], player_data[1][7], 20, colors[BLACK]);
					DrawCircle(player_data[1][8], player_data[1][9], 20, colors[BLACK]);
					DrawCircle(player_data[1][2], player_data[1][3], 15, colors[BLUE]);
					DrawCircle(player_data[1][4], player_data[1][5], 15, colors[BLUE]);
					DrawCircle(player_data[1][6], player_data[1][7], 15, colors[BLUE]);
					DrawCircle(player_data[1][8], player_data[1][9], 15, colors[BLUE]);
					DrawString((player_data[1][2]-8), (player_data[1][3]-8), "A", colors[WHITE]);
					DrawString((player_data[1][4]-8), (player_data[1][5]-8), "B", colors[WHITE]);
					DrawString((player_data[1][6]-8), (player_data[1][7]-8), "C", colors[WHITE]);
					DrawString((player_data[1][8]-8), (player_data[1][9]-8), "D", colors[WHITE]);

					DrawCircle(player_data[2][2], player_data[2][3], 20, colors[BLACK]);
					DrawCircle(player_data[2][4], player_data[2][5], 20, colors[BLACK]);
					DrawCircle(player_data[2][6], player_data[2][7], 20, colors[BLACK]);
					DrawCircle(player_data[2][8], player_data[2][9], 20, colors[BLACK]);
					DrawCircle(player_data[2][2], player_data[2][3], 15, colors[YELLOW]);
					DrawCircle(player_data[2][4], player_data[2][5], 15, colors[YELLOW]);
					DrawCircle(player_data[2][6], player_data[2][7], 15, colors[YELLOW]);
					DrawCircle(player_data[2][8], player_data[2][9], 15, colors[YELLOW]);
					DrawString((player_data[2][2]-8), (player_data[2][3]-8), "A", colors[WHITE]);
					DrawString((player_data[2][4]-8), (player_data[2][5]-8), "B", colors[WHITE]);
					DrawString((player_data[2][6]-8), (player_data[2][7]-8), "C", colors[WHITE]);
					DrawString((player_data[2][8]-8), (player_data[2][9]-8), "D", colors[WHITE]);

					DrawCircle(player_data[3][2], player_data[3][3], 20, colors[BLACK]);
					DrawCircle(player_data[3][4], player_data[3][5], 20, colors[BLACK]);
					DrawCircle(player_data[3][6], player_data[3][7], 20, colors[BLACK]);
					DrawCircle(player_data[3][8], player_data[3][9], 20, colors[BLACK]);
					DrawCircle(player_data[3][2], player_data[3][3], 15, colors[GREEN]);
					DrawCircle(player_data[3][4], player_data[3][5], 15, colors[GREEN]);
					DrawCircle(player_data[3][6], player_data[3][7], 15, colors[GREEN]);
					DrawCircle(player_data[3][8], player_data[3][9], 15, colors[GREEN]);
					DrawString((player_data[3][2]-8), (player_data[3][3]-8), "A", colors[WHITE]);
					DrawString((player_data[3][4]-8), (player_data[3][5]-8), "B", colors[WHITE]);
					DrawString((player_data[3][6]-8), (player_data[3][7]-8), "C", colors[WHITE]);
					DrawString((player_data[3][8]-8), (player_data[3][9]-8), "D", colors[WHITE]);
					break;
		}
	}

	/*showing whose turn
	this makes a small box on the left of player name whose turn is going on
	*/
	{
		if(turn==0)
			DrawRectangle( 755, 650, 10,16, colors[RED]);
		else if(turn==1)
			DrawRectangle( 755, 620, 10,16, colors[BLUE]);
		else if(turn==2)
			DrawRectangle( 755, 590, 10,16, colors[YELLOW]);
		else if(turn==3)
			DrawRectangle( 755, 560, 10,16, colors[GREEN]);
		else
			DrawRectangle( 755, 650, 10,16, colors[RED]);
	}

	if(start==1) // menu to select no. of players
	{
		DrawRectangle(0,0,1100,750,colors[BLACK]); // background

		DrawString(50,650, "Back", colors[WHITE]); // back button

		DrawString(448,460, "How many players?", colors[WHITE]);

		int x=285;
		for (int i=0;i<3;i++)
		{
			DrawRectangle(x, 260, 130, 60, colors[SLATE_BM]);
			x+=200;
		}

		DrawString(350,280, "2", colors[WHITE]);
		DrawString(550,280, "3", colors[WHITE]);
		DrawString(750,280, "4", colors[WHITE]);
	}
	else if(start==0) // start menu
	{
		DrawRectangle(0,0,1100,750,colors[BLACK]); // background color

		// title
		DrawString(490,550, "L", colors[RED]); 
		DrawString(525,550, "U", colors[BLUE]);
		DrawString(560,550, "D", colors[YELLOW]);
		DrawString(595,550, "O", colors[GREEN]);

		// options
		DrawString(520,480, "Start", colors[WHITE]);
		DrawString(482,440, "Leaderboard", colors[WHITE]);
		DrawString(480,400, "Instructions", colors[WHITE]);
		DrawString(522,360, "Quit", colors[WHITE]);

	}
	else if(start==3) // instructions menu
	{
		DrawRectangle(0,0,1100,750,colors[BLACK]); // background color
		DrawString(50,650, "Back", colors[WHITE]); // back button
		DrawString(438,550, "Instructions", colors[WHITE]); // title

		//content
		DrawString(100,480, "- Press Spacebar to roll dice, then press a letter to select it (to move it),", colors[WHITE]);
		DrawString(100,460, "   then press Spacebar again to actually move the giti.", colors[WHITE]);
		DrawString(100,430, "- Bar on left of player names in scoreboard shows whose turn it is.", colors[WHITE]);
		DrawString(100,400, "- Before quiting the game, press 'u' to update the leaderboard. ", colors[WHITE]);
		DrawString(100,370, "- To quit the game press Esc button.", colors[WHITE]);
		DrawString(100,350, "  ", colors[WHITE]);
		DrawString(400,300, " Enjoy your game!", colors[WHITE]);

	}
	else if(start==2) // leaderboard menu
	{
		DrawRectangle(0,0,1100,750,colors[BLACK]); // background color
		DrawString(50,650, "Back", colors[WHITE]); // back button
		DrawString(438,550, "Leaderboard", colors[WHITE]); // title

		// printing the values from highscore.txt
		int y=475;
		for(int i=0; i<10 ; i++)
		{
			DrawString(400,y, HighScore_Array[i], colors[WHITE]);
			y-=30;
		}
	}
	

	glutSwapBuffers(); // do not modify this line..
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */
void NonPrintableKeys(int key, int x, int y) {
	if (key
			== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...

	} else if (key
			== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	} else if (key
			== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {

	}

	else if (key
			== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {

	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}

	// rolling dice, and all the other things that come along
	if(key== ' ') // Space key
	{
		static bool firstturnchecker; // static so if once it is true, it remains like that throughout the program

		/* This following condition is to only check whose turn will be first it'll only be true for the first round
		*/
        if(firstturnchecker==false)
        {
            //cout << "run" << endl;
			firstturnchecker=FirstTurnCheck();
        }
		else 
		{
		static int count=0; /*static so that it's value isn't lost while the program is running 
		will make sure the dice isn't rolled twice in the same turn without getting a six */

		static int sixes_checker=0; // to count number of sixes in a row

		/* following array is to store all the data of every player before anyone's turn incase 3 sixes come, so to undo it
		initially it is the exact same as that of player_data array */
		static int player_position[4][12] = {
			{1,0,119,119,199,119,119,199,199,199,0,0}, 
			{2,0,199,631,119,631,119,551,199,551,0,0},
			{3,0,551,551,631,631,631,551,551,631,0,0},
			{4,0,631,199,631,119,551,119,551,199,0,0}
		}; 

		if(player_data[turn][11]==4) // to skip turn of a player who has won
		{
			turn++;
			TurnCheck();
		}
		
		if(count==0) // this condition makes sure that the dice is not rolled again in the same turn without getting a six
		{
			DiceRollGenerator(); // rolls a dice to give random number
			count=1;
		}	
		
		// main working
		{	
			if(move_giti!=20) // to make sure user selects a giti to move before executing the code ahead
			{
				
				Movement(); // to move the giti according to the dice roll

				// condition to make sure that a giti doesn't move if another is taken out and vice versa
				if(player_data[turn][move_giti]==player_position[turn][move_giti])
					GitiOutsideHouse(); // checks if any giti is inside the base (name is house, it refers to the base)

				PlayerDeadChecker(); // sees if any giti can be killed in this turn

				BlockChecker(); // to check if a double giti block was made in this turn or not

				ScoreCalculator(); // updates the score board

				if(dice_roll==6) // condition for dice roll=6
				{
					sixes_checker++;
					if(sixes_checker==3) // if 3 sixes in a row
					{
						for(int i=0;i<4;i++)
							for(int j=0;j<11;j++)
								player_data[i][j]=player_position[i][j]; // reverts the previous 3 moves

						turn++;
						sixes_checker=0;
						dice_roll=1; // this is to make the condition of (dice_roll!=6) true
					}
				}
				else
				{
					turn++;
					sixes_checker=0; 
				}

				takenout=0;
				count=0;
			}
		}

		if(dice_roll!=6)
		{
			TurnCheck(); // sees whose turn would be next
			for(int i=0;i<4;i++) // copying data from array to another
				for(int j=0;j<12;j++)
					player_position[i][j]=player_data[i][j];
		}
		move_giti=20; // to reset the value so that next player needs to enter again
		}
	}

	// all key inputs are mentioned here
	switch(key)
	{
		case 'A':
				move_giti=2;
				break;
		case 'B':
				move_giti=4;
				break;
		case 'C':
				move_giti=6;
				break;
		case 'D':
				move_giti=8;
				break;
		case 'a':
				move_giti=2;
				break;
		case 'b':
				move_giti=4;
				break;
		case 'c':
				move_giti=6;
				break;
		case 'd':
				move_giti=8;
				break;
		case 'u':
				Leaderboard();
				break;
		case 'U':
				Leaderboard();
				break;
	}


	glutPostRedisplay();
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {

	// implement your functionality here

	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(1000.0, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {

	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		//cout << x << " " << y << endl;

		if(x>285 && x<415 && y<490 && y>430) // 2 players
		{
			no_of_players=2;
			//cout << "2 players\n";
			start=10;
		}

		if(x>485 && x<615 && y<490 && y>430) // 3 players
		{
			no_of_players=3;
			//cout << "3 players\n";
			start=10;
		}

		if(x>685 && x<815 && y<490 && y>430) // 4 players
		{
			no_of_players=4;
			//cout << "4 players\n";
			start=10;
		}

		if(x>520 && x<570 && y>250 && y<270) //start
		{
			start=1;
		}			

		if(x>480 && x<600 && y>290 && y<310) // leaderboard
		{
			Leaderboard();
			start=2;
		}

		if(x>480 && x<600 && y>330 && y<350) // instructions
		{
			start=3;
		}
			
		if(x>520 && x<570 && y>370 && y<390) // quit
		{
			exit(1);
		}

		if(x>50 && x<100 && y>80 && y<100) // back 
		{
			start=0;
		}
			
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{

	}
	glutPostRedisplay();
}

// main function
int main(int argc, char*argv[]) {

	int width = 1100, height = 750; // i have set my window size to be 1100 x 750
	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("PF Project Ludo Game"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();

	return 1;
}
#endif /* AsteroidS_CPP_ */
