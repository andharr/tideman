#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool checkCycle(int loser, int n, int original_winner);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    //verify that name is in candidates array
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcasecmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count - 1; i++)
    {
        //Need to build options in preferences array, e.g.:
        //preferences[Alice][Bob] || preferences[ranks[0]ranks[1]]
        //preferences[Alice][Charlie] || preferences[ranks[0]ranks[2]]
        //preferences[Charlie][Bob] || preferences[ranks[2]ranks[1]]
        for (int j = i + 1; j < candidate_count; j++)
        {
            //may need to initialize entries first...
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    //Loop over the preferences[], add any pair where votes are not equal (preferences[i][j] != preferences[j][i])
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count += 1;
            }
        }
    }
    return;
}


// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    //how to find their strength?
    //preferences[pairs[i].winner][pairs[i].loser] => that will be the strength from the matchup recorded in preferences array
    //use )pair_count -1) because j is one higher and don't want to push it off into segmentation error
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            //if j is larger than i
            if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[i].winner][pairs[i].loser])
            {
                //swap them
                pair tempInt = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = tempInt;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int original_winner = pairs[i].winner;
        int loser = pairs[i].loser;
        int n = 0;
        int branch = 0;
        int test = checkCycle(loser, n, original_winner);

        if (!test)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        else
        {
            continue;
        }
    }
    return;
}

//return checkCycle(loser, n, original_winner)

bool checkCycle(int loser, int original_winner)
{
    for (int j = 0; j < candidate_count; j++)
    {
        //see if the loser is a winner in a locked square
        if (locked[loser][j] == true)
        {

            //if the loser is the same as the original winner it creates a cycle
            if (j == original_winner)
            {
                return true;
            }
            else
            {
                //This was the magic bullet. I WAS SO CLOSE!
                if(checkCycle(j, original_winner))
                {
                    return true;
                }
            }
            //I had this before which didn't work and I don't understand why:
            // else
            // {
            //     loser = j;
            //     checkCycle(loser, original_winner);
            // }
        }
    }
    return false;
}


// Print the winner of the election
void print_winner(void)
{
    // TODO
    //loop over locked array e.g. locked[i][j]
    // locked[i][j] means i is locked in over j
    //reverse this when there's a victory: locked[i][j] = false;
    //first pair automatic to true
    //if candidate_count = 2; first/only line wins
    //
    return;
}