//Include data from these libraries
#include <cs50.h>
#include <stdio.h>
#include <string.h>

//Max number of voters is 100 and candidates is 9
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

//preferences[i][j]: i is the voter, j is the preference.
int preferences[MAX_VOTERS][MAX_CANDIDATES];

//Candidate struct which includes name, vote counter, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

//Array of candidates as long as the Max number
candidate candidates[MAX_CANDIDATES];

//Numbers of voters and candidates
int voter_count;
int candidate_count;

//Function prototypes for later
bool vote(int voter, int rank, string name);    //check if vote is valid
void tabulate(void);                            //add up the votes
bool print_winner(void);                        //print the winner
int find_min(void);                             //find the candidate with the lowest votes
bool is_tie(int min);                           //tiebreaker check
void eliminate(int min);                        //eliminate the lowest candidate

//Main Body of Code////////////////////////////////////////////////////

//Command Line Argument (Entering the candidates)

int main(int argc, string argv[])
{
    //If argument counter is less than 2
    if (argc < 2)
    {
        //prompt for candidate
        printf("Usage: runoff [candidate ...]\n");
        //error 1
        return 1;
    }

    //Populate array of candidates
    candidate_count = argc - 1;

    //If there are too many candidates
    if (candidate_count > MAX_CANDIDATES)
    {
        //prompt for max number of candidates
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        //error 2
        return 2;
    }

    //Scan through candidate count
    for (int i = 0; i < candidate_count; i++)
    {
        //Assign these starting values to the candidates
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

///////////////////////////////////////////////////////////////////////

//Entering the number of voters and who they are voting for

    //Prompt for the number of voters and make it an value called voter count
    voter_count = get_int("Number of voters: ");

    //If there are more voters than the max allows
    if (voter_count > MAX_VOTERS)
    {
        //Prompt for the max number of voters
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        //error 3
        return 3;
    }

    //Keep querying for votes. Scan through the voter count
    for (int i = 0; i < voter_count; i++)
    {

        //Query for each rank. Scan through the candidate count
        for (int j = 0; j < candidate_count; j++)
        {
            //Prompt for the current candidate rank
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                //Prompt for invalid vote
                printf("Invalid vote.\n");
                //error 4
                return 4;
            }
        }

        //Print a space
        printf("\n");
    }

////////////////////////////////////////////////////////////////

    //Keep holding runoffs until winner exists
    while (true)
    {
        //Calculate votes given remaining candidates
        tabulate();

        //Check if election has been won
        bool won = print_winner();

        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        //If tie, everyone wins
        if (tie)
        {
            //Scan through the candidate count
            for (int i = 0; i < candidate_count; i++)
            {
                //If its not an eliminated candidate
                if (!candidates[i].eliminated)
                {
                    //Print the candidate name
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    //scan through the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //use strcmp to see if user input matches the candidates name
        int match = strcmp(candidates[i].name, name);

        //if the names match
        if (match == 0)
        {
            //assign rank to candidate
            preferences[voter][rank] = i;
            return true;
        }
    }

    //otherwise
    return false;
}

/////////////////////////////////////////////////////////////////////////////

//Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    //scan through voters
    for (int i = 0; i < voter_count; i++)
    {
        //scans through the candidate count
        for (int j = 0; j < candidate_count; j++)
        {
            //create an int called candidatearray to represent position in grid
            int candidatearray = preferences[i][j];

            //if voter's first preference is not eliminated
            if (candidates[candidatearray].eliminated == false)
            {
                //candidate vote + 1
                candidates[candidatearray].votes++;
                //skip
                break;
            }

        }
    }

    //otherwise
    return;
}

/////////////////////////////////////////////////////////////////////////////

// Print the winner of the election, if there is one
bool print_winner(void)
{
    //create an int to represent half of the voters
    int half = voter_count / 2;

    //scan through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //if vote is bigger than half
        if (candidates[i].votes > half)
        {
            printf("Winner: %s\n", candidates[i].name);
            return true;
        }
    }

    //otherwise
    return false;
}

/////////////////////////////////////////////////////////////////////////////

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    //create an int to represent half of the voters
    int half = voter_count / 2;

    //create an int to represent the lowest vote count
    int lowest = half;

    //scan through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //if candidate is not eliminated
        if (candidates[i].eliminated == false)
        {
            //if vote is smaller than lowest
            if (candidates[i].votes < lowest)
            {
                //lowest vote get replaced
                lowest = candidates[i].votes;
            }
        }
    }
    //return the lowest value
    return lowest;
}

/////////////////////////////////////////////////////////////////////////////

//Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    //scan through the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //check if candidate is not eliminated
        if (candidates[i].eliminated == false)
        {
            //if candidate votes is bigger than min
            if (candidates[i].votes > min)
            {
                //return false
                return false;
            }
        }
    }

    // TODO
    return true;
}

/////////////////////////////////////////////////////////////////////////////

//Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    //scan through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //if
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    //otherwise
    return;
}
