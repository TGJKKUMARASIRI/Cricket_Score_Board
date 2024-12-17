#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <random>

using namespace std;

class PlayerInitialization // class for store thr information of players that should be inputed
{
public:
    char name[64]; //Player name
    bool batsman; //Is player a Batsman
    bool bowler; //Is player a bowler
    bool wicketkeeper; //Is palyer a Wicketkeeper

    void GetData() // input function
    {
        cin.ignore();
        cin.getline(name, 64);
        cin >> batsman;
        cin >> bowler;
        cin >> wicketkeeper;
    };

    void ShowDeta() // output function
    {
        cout << name << endl;
        cout << batsman << endl;
        cout << bowler << endl;
        cout << wicketkeeper << endl
             << endl;
    }
};

class Player // class for players' other information
{
public:
    PlayerInitialization *playerInfo; // pointter to the playerInitialization class

    int NoOverBalled = 0; //For keep tracking on player's bawled number of overs
    int NoOfNoBalls = 0; //Number of no-balls
    int NoOfWideBalls = 0; //Number of wide-balls
    int NoOfWickets = 0; //Number of wickets
    int NoOfLBWs = 0; //Number of LBWs

    int NoOfCatches = 0; //Number of catches

    int BattedScore = 0; //Number of scores scored during batting
    bool out = false; //Is player out while batting
    bool onTheGround = false; //Is palyer on the ground now

    Player *NextPlayer; // pointer to the next player class
};

class Team // class for the team info
{
public:
    char TeamName[64]; //Team name
    Player *Head = NULL; //pointer for the first player
    Player *pointerP = NULL; //Pointer to go through the player linked list
    int TeamScore = 0; //Total team score
    int AllWickets = 0; //Team all wickets
    bool Batted = false; //Is team has batted
    int CurrentOver = 0; //Current over playing
    int CurrentBall = 0; //Current ball of the over

    void InsertFromFile(fstream &file) // geting the information from a binary input file
    {
        file.seekg(0);

        Head = new Player;
        Head->playerInfo = new PlayerInitialization;
        
        pointerP = Head;

        file.read(reinterpret_cast<char *>(pointerP->playerInfo), sizeof(PlayerInitialization));

        int i = 0;
        while (!file.eof() && i < 10)
        {
            i++;
            pointerP->NextPlayer = new Player;
            pointerP = pointerP->NextPlayer;
            pointerP->playerInfo = new PlayerInitialization;
            file.read(reinterpret_cast<char *>(pointerP->playerInfo), sizeof(PlayerInitialization));
        }

        pointerP->NextPlayer = Head; //Creating a circular linked list
    };
};

class Cricket // class for operate the game
{
public:
    Team *team1; //team 1 object pointer
    Team *team2; //team 2 object pointer
    float CurrentRunRate = 0;
    float RequiredRunRate = 0;

    void TeamNameInput(ifstream &file) // getting team names from a input text file
    {
        team1 = new Team;
        file.getline(team1->TeamName, 64);
        team2 = new Team;
        file.getline(team2->TeamName, 64);
    };

    int CheckingTheWinner(Team *teamBat, Team *TeamBall) //Use to check the the winner during the score chasing team batting
    {
        if (TeamBall->Batted == true)
        {
            if (teamBat->TeamScore > TeamBall->TeamScore)  //Batting team won
            {
                return 1;
            }
            else if (teamBat->TeamScore < TeamBall->TeamScore) //Balling team won
            {
                return 0;
            }
            else //Match draw situation
            {
                return -1;
            }
        }
        return 0;
    }

    void ScoreBoard(Team *TeamBat, Team *TeamBall) //Use to display the score board
    {
        Player *pointerBat; //pointer to go through the Batting team players
        Player *pointerBall; //pointer to go through the Balling team players
        cout << "\n\n\t" << TeamBall->TeamName << "\t\tVs.\t\t" << TeamBat->TeamName << endl;
        cout << left << setw(20) << "Bowlers"
             << left << setw(3) << "O"
             << left << setw(3) << "W"
             << left << setw(6) << "Catch"
             << left << setw(4) << "LBW"
             << left << setw(3) << "NB"
             << left << setw(3) << "WB"
             << left << setw(20) << "Batsmans"
             << left << setw(10) << " "
             << left << setw(6) << "Score"
             << endl;
        
        pointerBat = TeamBat->Head;
        pointerBall = TeamBall->Head;

        for (int i=0 ; i<11 ; i++) //for loop for displaying the status of all the players
        {
            cout << left << setw(20) << pointerBall->playerInfo->name;
            cout << left << setw(3) << pointerBall->NoOverBalled;
            cout << left << setw(3) << pointerBall->NoOfWickets;
            cout << left << setw(6) << pointerBall->NoOfCatches;
            cout << left << setw(4) << pointerBall->NoOfLBWs;
            cout << left << setw(3) << pointerBall->NoOfNoBalls;
            cout << left << setw(3) << pointerBall->NoOfWideBalls; //Until this line carries information about bawllers
            cout << left << setw(20) << pointerBat->playerInfo->name; //From hear Batsman information carries

            if (pointerBat->onTheGround == true)
            {
                cout << left << setw(10) << "On Ground"
                     << setw(6) << pointerBat->BattedScore
                     << endl;
            }
            else 
            {
                if (pointerBat->out == true)
                {
                    cout << left << setw(10) << "Out"
                     << setw(6) << pointerBat->BattedScore
                     << endl;
                }
                else
                {
                    cout << left << setw(10) << " "
                     << left << setw(6) << " " << endl;
                }
            }

            pointerBat = pointerBat->NextPlayer; //go to next Batsman
            pointerBall = pointerBall->NextPlayer; //go to next bowler
            
        }

        if (TeamBall->Batted) //TeamBall total should display only if they had allready played
        {
            cout << left << setw(7) << "Total: " << left << setw(35) << TeamBall->TeamScore;
        }
        else //else nothing display
        {
            cout << setw(42) << " ";
        }

        cout << left << setw(7) << "Total: " << TeamBat->TeamScore << endl;

        cout << left << setw(14) << "Current Over: " << left << setw(28) << TeamBall->CurrentOver
                 << left << setw(14) << "Current Ball: " << TeamBall->CurrentBall
                 << endl;

        cout << left << setw(18) << "Current Run Rate: " << setw(24)<< left << CurrentRunRate;

        if (TeamBall->Batted) //Required Run Rate should display only if they had allready played
        {
            cout << left << setw(19) << "Required Run Rate: " << RequiredRunRate;
        }
    }

    void CalCurrentRunRate(int i, int j, Team *TeamBat) //Function for calculating the current run rate
    {
        int ballBawled = i * 6 + j;
        if (ballBawled == 0)
        {
            return;
        }
        CurrentRunRate = (float(TeamBat->TeamScore) / float(ballBawled))*6;
    };

    void CalRequiredRunRate(int i, int j, Team *TeamBat, Team *TeamBall)  //Function for calculatinf the Required Run Rate
    {
        int restBalls = 120 - (i * 6 + j);
        RequiredRunRate = (float(TeamBall->TeamScore + 1 - TeamBat->TeamScore) / float(restBalls))*6;
        if (RequiredRunRate < 0)
        {
            RequiredRunRate = 0;
        }
    };

    int FirstSecondBatting(Team *teamBat, Team *TeamBall) // finction for the game
    {
        random_device rd; //random number generator
        mt19937 rng(rd());

        teamBat->Batted = true; //The way to confirm if a team bat once
        
        teamBat->pointerP = teamBat->Head; 
        teamBat->pointerP->onTheGround = true;
        Player *temp = teamBat->pointerP->NextPlayer; //Second Batsman on the ground
        temp->onTheGround = true;

        TeamBall->pointerP = TeamBall->Head;

        for (int i = 0; i < 20; i++) //For loop to iterate through the 20 overs 
        {
            TeamBall->pointerP->NoOverBalled++; //Increment Balled Over
            TeamBall->CurrentOver++;
            TeamBall->CurrentBall = 0;
            for (int j = 0; j < 6;) //Iterate through the 6 balls per over
            {
                int bawlingStatus = rng() % 100 + 1; //generating a random numeber to deside what hapen to the ball
                if (bawlingStatus < 75) // It's a normal ball or 4 or 6
                {
                    j++; //Increment the j in the loop
                    TeamBall->CurrentBall++;
                    if (bawlingStatus < 5) // It's a 6
                    {
                        teamBat->TeamScore += 6;
                        teamBat->pointerP->BattedScore += 6;
                    }
                    else if (bawlingStatus >= 5 && bawlingStatus < 20) // It's a 4
                    {
                        teamBat->TeamScore += 4;
                        teamBat->pointerP->BattedScore += 4;
                    }
                    else //It's get score through running
                    {
                        int running = rng() % 4; //Generate a randome numeber to deside the number of runns
                        teamBat->pointerP->BattedScore += running;
                        teamBat->TeamScore += running;
                        if (running == 1 || running == 3) //If it's a ood running then the batsman must shift their possition
                        {
                            Player *temptemp = teamBat->pointerP;
                            teamBat->pointerP = temp;
                            temp = temptemp;
                        }
                    }
                }
                else
                {
                    //Both WB abn NB doesn't increase the ball number (j)
                    if (bawlingStatus >= 75 && bawlingStatus < 80) // It's a no ball
                    {
                        TeamBall->pointerP->NoOfNoBalls++;
                        teamBat->TeamScore++;
                    }
                    else if (bawlingStatus >= 80 && bawlingStatus < 85) // It's a wide ball
                    {
                        TeamBall->pointerP->NoOfWideBalls++;
                        teamBat->TeamScore++;
                    }
                    else
                    {
                        j++;
                        TeamBall->CurrentBall++;
                        if (bawlingStatus >= 85 && bawlingStatus < 90) // It's a LBw
                        {
                            TeamBall->pointerP->NoOfLBWs++;
                        }
                        else if (bawlingStatus >= 90 && bawlingStatus < 94) // It's a catch
                        {
                            Player *tempPointer = TeamBall->Head;
                            int catcher = rng() % 10;
                            for (int k = 0; k < catcher; k++) // choosing the player who able to take the catch
                            {
                                tempPointer = tempPointer->NextPlayer;
                            }
                            tempPointer->NoOfCatches++; //Increment cathing count of the plyer
                        }
                        else // It's a wicket
                        {
                            TeamBall->pointerP->NoOfWickets++; //Increment wicket count of the player
                        }
                        teamBat->AllWickets++; //Increment wicket count of the overall team
                        teamBat->pointerP->onTheGround = false;
                        teamBat->pointerP->out = true; //Make the batsman out so he will not come to the ground again
                        if (teamBat->AllWickets < 10) //Make sure there are mo players that haven't played before
                        {
                            do //Select a batsmen for the one that out
                            {
                                teamBat->pointerP = teamBat->pointerP->NextPlayer;
                            } while (teamBat->pointerP->out == true || teamBat->pointerP->onTheGround == true); //Make sure the one selected in not ont the ground or get out
                            teamBat->pointerP->onTheGround = true;
                        }
                        else //All out status
                        {
                            if (TeamBall->Batted) //Checking if the batting team is the score chasing team
                            {
                                if (CheckingTheWinner(teamBat, TeamBall) == 0)
                                {
                                    ScoreBoard (teamBat, TeamBall);
                                    cout << "\n\nTeam " << TeamBall->TeamName << " won the game!";
                                }
                                else if (CheckingTheWinner(teamBat, TeamBall) == 1)
                                {
                                    ScoreBoard (teamBat, TeamBall);
                                    cout << "\n\nTeam " << teamBat->TeamName << " won the game!";
                                }
                                else
                                {
                                    ScoreBoard (teamBat, TeamBall);
                                    cout << "\n\nGame is a Draw!";
                                }
                            }
                            return 0;
                        }
                    }
                }
                if (CheckingTheWinner(teamBat, TeamBall) == 1) //After a ball checking the winner
                {
                    CalCurrentRunRate(i, j, teamBat); //Calculate the current Run Rate
                    CalRequiredRunRate(i, j, teamBat, TeamBall); //Calculate the required Run Rate
                    ScoreBoard (teamBat, TeamBall); //Display the Score board
                    cout << "\n\nTeam " << teamBat->TeamName << " won the game!"; //Announce the Winner if someone has won the game
                    return 0;
                }
                CalCurrentRunRate(i, j, teamBat); //Calculate the current Run Rate if no one has won the game yet
                if (TeamBall->Batted) //Calculate the current Run Rate if the score chasing team is playing
                {
                    CalRequiredRunRate(i, j, teamBat, TeamBall);
                }
                ScoreBoard (teamBat, TeamBall); //Display the score after every ball
            }
            do
            {
                TeamBall->pointerP = TeamBall->pointerP->NextPlayer; //After every over new bowler is selected
            } while (TeamBall->pointerP->playerInfo->bowler == false || TeamBall->pointerP->NoOverBalled >= 4); //Make sure selected player is a bowler and haven't balled more than 4 balls
        }
        if (CheckingTheWinner(teamBat, TeamBall) == 0) //After the game ended checking the winner
        {
            ScoreBoard (teamBat, TeamBall);
            cout << "\n\nTeam " << TeamBall->TeamName << " won the game!";
        }
        else if (CheckingTheWinner(teamBat, TeamBall) == 1)
        {
            ScoreBoard (teamBat, TeamBall);
            cout << "\n\nTeam " << teamBat->TeamName << " won the game!";
        }
        else
        {
            ScoreBoard (teamBat, TeamBall);
            cout << "\n\nGame is a Draw!";
        }
        return 0;
    };

    void GemaFlow(Team *teamBat, Team *TeamBall) // Fuction to desile the first half and second half of the game
    {
        FirstSecondBatting(teamBat, TeamBall);
        cout << "\nLet's teke 30 minutes break!\n\n\n\n\n" << endl;
        FirstSecondBatting(TeamBall, teamBat);
    };

    int WhoPlaywhat() //Function for deside who bat first and draw the toss
    {
        random_device rd; //Randome number Generator
        mt19937 rng(rd());

        cout << "Choose Head or tail for " << team1->TeamName << " (Head-1, Tail-0): ";
        int userInput;
        //cin >> userInput; //If you want to play as the team 1 and deside the coin side
        userInput = rng() % 2; //For automation Userinput has randomly generated
        cout << userInput << endl;
        if (userInput != 1 && userInput != 0)
        {
            cout << "Error: Invalid Input";
            return 0;
        }
        int CoinSide = rng() % 2;

        if (CoinSide == userInput) //If the team1 won the toss they can deside what they doing first
        {
            cout << "You won the toss! As " << team1->TeamName << " what will you choose? (Bowling-1 / Batting-0): ";
            int userInputB;
            //cin >> userInputB; //User can deside that 
            userInputB = rng() % 2; //But for the automation that has also generated from the random number generator
            cout << userInputB << endl;

            if (userInputB == 1)
            {
                GemaFlow(team2, team1); //Calling GameFlow Function
            }
            else
            {
                GemaFlow(team1, team2);
            }
        }
        else //If user loss the toss Team2 will batting first
        {
            cout << "You loss the toss! As " << team2->TeamName << " we will be batting first." << endl;
            GemaFlow(team2, team1);
        }
        return 0;
    };
};
