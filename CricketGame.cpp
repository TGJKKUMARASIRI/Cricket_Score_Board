/************************************************************************************
Course             :   CO2203 Object Oriented Programming
Year               :   2023
Name               :   T.G.J.K. Kumarasiri
Index No           :   21_ENG_147
Mini Project
Date               :   25/09/2023
*************************************************************************************/
#include <iostream>
#include <fstream>
#include "Classes.h" //Classes header that has all the functions ans classes

using namespace std;

int main ()
{
    Cricket *WorldCup;  //Creating a worldcup object pointer
    ifstream file0("Team_Names.txt"); //creating a ifstream object for getting inputs for team names

    WorldCup = new Cricket;

    WorldCup->TeamNameInput(file0); 

    fstream file1;  //Creating a fstream object for getting inputs for team1 members information
    file1.open("Crecket_Team1.dat", ios::in | ios::out | ios::binary | ios::app);
    if (!file1.is_open()) {
        cout << "Could not open the file." << endl;
        return 1;
    }
    WorldCup->team1->InsertFromFile(file1); //calling team1 obeject InsertFromFile function
    
    file1.close(); 

    fstream file2; //Creating a fstream object for getting inputs for team 2 members informations
    file2.open("Crecket_Team2.dat", ios::in | ios::out | ios::binary | ios::app);
    if (!file2.is_open()) {
        cout << "Could not open the file." << endl;
        return 1;
    }
    WorldCup->team2->InsertFromFile(file2); //calling team2 obeject InsertFromFile function
    file2.close();

    WorldCup->WhoPlaywhat(); //Calling the WhoPlayWhat function fron WorldCup object

    delete WorldCup; //delete worldcup object pointer

    return 0;
}