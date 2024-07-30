#include <stdio.h>   // Standard input/output
#include <conio.h>   // Console input/output (getch())
#include <stdlib.h>  // Standard library functions (rand())
#include <time.h>    // Time related functions (srand(time(NULL)))
#include <windows.h> // Colour coding

// Dimensions of map, total winning score, and number of ghosts defined
#define ROWS 14
#define COLUMNS 24
#define WINNING_SCORE 184 // Equal to total number of pellets
#define TOTAL_GHOSTS 3    // Can be increased for higher difficulty

// Global variables
int pacmanX = ROWS / 2;        // Initial x-coordinate of Pacman
int pacmanY = COLUMNS / 2;     // Initial y-coordinate of Pacman
int score = 0;                 // Score counter
int lives = 3;                 // Lives counter
int pacmanFlag[ROWS][COLUMNS]; // Flag to mark Pacman's visited positions
int ghostsLoaded = 0;          // Flag to indicate that ghosts were loaded from saved game

// Ghost structure
struct Ghost
{
    int x;
    int y;
} ghosts[TOTAL_GHOSTS];

// Function to increment score using pointer
void incrementScore(int *score)
{
    *score = *score + 1;
}

// Function to decrement lives using pointer
void decrementLive(int *lives)
{
    *lives = *lives - 1;
}

// Function to set text color using Windows API function SetConsoleTextAttribute
void setTextColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    // 'color' parameter specifies the color to be set
    // GetStdHandle(STD_OUTPUT_HANDLE) gets the standard output handle, and SetConsoleTextAttribute sets its text color
}

// Function to display the game map
void displayMap(char map[ROWS][COLUMNS], int *pacmanX, int *pacmanY)
{
    // Clear the console screen
    system("cls");

    // Mark Pacman's current position on the map's centre
    map[*pacmanX][*pacmanY] = 'C';

    // Display the game map
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            switch (map[i][j])
            {
            case 'C':
                setTextColor(14); // Yellow for Pacman
                printf("%c ", map[i][j]);
                break;
            case 'G':
                setTextColor(12); // Red for ghosts
                printf("%c ", map[i][j]);
                break;
            case '.':
                setTextColor(7); // White for pellets
                printf("%c ", map[i][j]);
                break;
            case '#':
                setTextColor(10); // Green for walls
                printf("%c ", map[i][j]);
                break;
            default:
                printf("%c ", map[i][j]);
                break;
            }
            setTextColor(7); // Reset color to white for rest of the elements
        }
        printf("\n");
    }

    // Display score and lives counter
    printf("\nScore:%d\t\t\t\t\tLives:%d\n\n\n", score, lives);
}

// Function to initialise pellets on the map
void initialisePellets(char map[ROWS][COLUMNS])
{
    // Iterate through each position on map
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if (map[i][j] == ' ') // If it is empty
            {
                map[i][j] = '.'; // Place pellet
            }
        }
    }
}

// Function to move the Pacman and event handling upon collision with ghost/pellet. Its parameter 'direction' is entered by user in main()
void movePacman(char map[ROWS][COLUMNS], int *pacmanX, int *pacmanY, char direction)
{
    int x = *pacmanX;
    int y = *pacmanY;
    map[x][y] = ' '; // Clear the current position of Pacman

    // Move the Pacman based on the input direction
    switch (direction)
    {
    case 'w':
    case 'W':

        if (map[x - 1][y] != '#') // Check if the next position is not a wall
            *pacmanX = x - 1;

        // Checking if collision is with ghost or pellet
        if (map[*pacmanX][*pacmanY] == 'G')
        {
            decrementLive(&lives); // Decrease lives when Pacman collides with a ghost
            *pacmanX = ROWS / 2;
            *pacmanY = COLUMNS / 2;
        }

        else if (map[*pacmanX][*pacmanY] == '.')
            incrementScore(&score); // Increase score when Pacman eats a pellet

        pacmanFlag[*pacmanX][*pacmanY] = 1; // Mark position as visited by Pacman
        map[*pacmanX][*pacmanY] = 'C';      // Update Pacman's position on the map
        break;

    // Similar event handling for each case
    case 's':
    case 'S':

        if (map[x + 1][y] != '#')
            *pacmanX = x + 1;

        if (map[*pacmanX][*pacmanY] == 'G')
        {
            decrementLive(&lives);
            *pacmanX = ROWS / 2;
            *pacmanY = COLUMNS / 2;
        }

        else if (map[*pacmanX][*pacmanY] == '.')
            incrementScore(&score);

        pacmanFlag[*pacmanX][*pacmanY] = 1;
        map[*pacmanX][*pacmanY] = 'C';
        break;

    case 'a':
    case 'A':
        if (map[x][y - 1] != '#')
            *pacmanY = y - 1;

        if (map[*pacmanX][*pacmanY] == 'G')
        {
            decrementLive(&lives);
            *pacmanX = ROWS / 2;
            *pacmanY = COLUMNS / 2;
        }

        else if (map[*pacmanX][*pacmanY] == '.')
            incrementScore(&score);

        pacmanFlag[*pacmanX][*pacmanY] = 1;
        map[*pacmanX][*pacmanY] = 'C';
        break;

    case 'd':
    case 'D':
        if (map[x][y + 1] != '#')
            *pacmanY = y + 1;

        if (map[*pacmanX][*pacmanY] == 'G')
        {
            decrementLive(&lives);
            *pacmanX = ROWS / 2;
            *pacmanY = COLUMNS / 2;
        }

        else if (map[*pacmanX][*pacmanY] == '.')
            incrementScore(&score);

        pacmanFlag[*pacmanX][*pacmanY] = 1;
        map[*pacmanX][*pacmanY] = 'C';
        break;

    // Event handling when no valid input entered
    default:
        if (map[*pacmanX][*pacmanY] == 'G')
        {
            decrementLive(&lives);
            *pacmanX = ROWS / 2;
            *pacmanY = COLUMNS / 2;
        }

        else if (map[*pacmanX][*pacmanY] == '.')
            incrementScore(&score);

        pacmanFlag[*pacmanX][*pacmanY] = 1;
        map[*pacmanX][*pacmanY] = 'C';
        break;
    }
}

// Function to initialise ghosts on the map. Event handling done in case of game being loaded
void initialiseGhosts(char map[ROWS][COLUMNS], int pacmanX, int pacmanY)
{
    // Check if ghosts were loaded from file. If not, initialise them
    if (!ghostsLoaded)
    {
        // Initialise ghosts' positions
        for (int i = 0; i < TOTAL_GHOSTS; i++)
        {
            do
            {
                // Generate random x and y coordinates of ghost
                ghosts[i].x = rand() % ROWS;
                ghosts[i].y = rand() % COLUMNS;
            } while (map[ghosts[i].x][ghosts[i].y] == '#' || (ghosts[i].x == pacmanX && ghosts[i].y == pacmanY)); // Avoid generating ghosts on walls/Pacman

            map[ghosts[i].x][ghosts[i].y] = 'G'; // Place ghost on map
        }
    }
    else
    {
        // Ghosts were loaded, use the loaded positions
        for (int i = 0; i < TOTAL_GHOSTS; i++)
        {
            map[ghosts[i].x][ghosts[i].y] = 'G'; // Place ghost on map
        }
    }
}

// Function for ghost movement
void ghostsMovement(char map[ROWS][COLUMNS], int *pacmanX, int *pacmanY)
{
    // Variable to store direction of ghost
    int direction;
    for (int i = 0; i < TOTAL_GHOSTS; i++)
    {
        // Check if Pacman has visited current position of ghost
        if (pacmanFlag[ghosts[i].x][ghosts[i].y] == 1)
            map[ghosts[i].x][ghosts[i].y] = ' '; // If Pacman has, ghost leaves empty space behind
        else
            map[ghosts[i].x][ghosts[i].y] = '.'; // If not, ghost leaves a pellet(default arrangement)

        // Generate random direction for ghost
        do
        {
            direction = rand() % 4; // 0=up, 1=down, 2=left, 3=right;
        }
        // Keep generating random direction until direction pointing towards is not a wall (Ensures non-stop movement)
        while (
            (direction == 0 && map[ghosts[i].x - 1][ghosts[i].y] == '#') ||
            (direction == 1 && map[ghosts[i].x + 1][ghosts[i].y] == '#') ||
            (direction == 2 && map[ghosts[i].x][ghosts[i].y - 1] == '#') ||
            (direction == 3 && map[ghosts[i].x][ghosts[i].y + 1] == '#'));

        // Update ghost's posiiton based upon generated direction
        if (direction == 0)
        {
            ghosts[i].x -= 1;

            // Event handling after collision with Pacman
            if ((ghosts[i].y == *pacmanY) && (ghosts[i].x == *pacmanX))
            {
                decrementLive(&lives); // Decrease lives when Pacman collides with a ghost
                *pacmanX = ROWS / 2;
                *pacmanY = COLUMNS / 2;
                map[*pacmanX][*pacmanY] = 'C';
            }
        }
        if (direction == 1)
        {
            ghosts[i].x += 1;
            if ((ghosts[i].y == *pacmanY) && (ghosts[i].x == *pacmanX))
            {
                decrementLive(&lives); // Decrease lives when Pacman collides with a ghost
                *pacmanX = ROWS / 2;
                *pacmanY = COLUMNS / 2;
                map[*pacmanX][*pacmanY] = 'C';
            }
        }
        if (direction == 2)
        {
            ghosts[i].y -= 1;
            if ((ghosts[i].y == *pacmanY) && (ghosts[i].x == *pacmanX))
            {
                decrementLive(&lives); // Decrease lives when Pacman collides with a ghost
                *pacmanX = ROWS / 2;
                *pacmanY = COLUMNS / 2;
                map[*pacmanX][*pacmanY] = 'C';
            }
        }

        if (direction == 3)
        {
            ghosts[i].y += 1;
            if ((ghosts[i].y == *pacmanY) && (ghosts[i].x == *pacmanX))
            {
                decrementLive(&lives); // Decrease lives when Pacman collides with a ghost
                *pacmanX = ROWS / 2;
                *pacmanY = COLUMNS / 2;
                map[*pacmanX][*pacmanY] = 'C';
            }
        }

        map[ghosts[i].x][ghosts[i].y] = 'G'; // Update ghost's position on the map
    }
}

// Function to save the game state to a file
void saveGame(char map[ROWS][COLUMNS])
{
    // Open file named "game_save.txt" for writing
    FILE *file = fopen("game_save.txt", "w");

    // Check if file not opened, print error message and exit the function
    if (file == NULL)
    {
        printf("Game could not be saved\n");
        Sleep(2000);
        return;
    }

    // Save player information and winning score (To ensure no error if WINNING_SCORE changes)
    fprintf(file, "%d %d %d %d %d\n", pacmanX, pacmanY, score, lives, WINNING_SCORE);

    // Save ghosts' positions
    for (int i = 0; i < TOTAL_GHOSTS; i++)
    {
        fprintf(file, "%d %d ", ghosts[i].x, ghosts[i].y);
    }

    // Save the arrangement of pellets
    for (int i = 0; i < ROWS; i++)
    {
        fprintf(file, "%s\n", map[i]); // Save each row as a string
    }

    // File closed after writing
    fclose(file);
}

// Function to load a saved game from a file
void loadGame(char map[ROWS][COLUMNS])
{
    // Open file named "game_save.txt" for reading
    FILE *file = fopen("game_save.txt", "r");

    // Check if file not opened, print error message and exit the function
    if (file == NULL)
    {
        printf("\nNo saved game found. New game will start in a while\n");
        Sleep(2000);
        return;
    }

    // Variable to store WINNING_SCORE
    int loadedWinningScore;

    // Read the player information and winning score
    fscanf(file, "%d %d %d %d %d\n", &pacmanX, &pacmanY, &score, &lives, &loadedWinningScore);

    // Read ghosts' positions
    for (int i = 0; i < TOTAL_GHOSTS; i++)
    {
        fscanf(file, "%d %d ", &ghosts[i].x, &ghosts[i].y);
    }

    ghostsLoaded = 1; // Set flag to indicate ghosts were loaded

    // Read the arrangement of pellets
    for (int i = 0; i < ROWS; i++)
    {
        // Read each row as a string
        char row[COLUMNS + 1];
        fgets(row, COLUMNS + 1, file);

        // Check each position of the row for pellets and ghosts
        for (int j = 0; j < COLUMNS; j++)
        {
            // Flag to indicate if a ghost is found
            int ghostFound = 0;

            // Check if there is a ghost on the position
            for (int k = 0; k < TOTAL_GHOSTS; k++)
            {
                if (ghosts[k].x == i && ghosts[k].y == j) // Compare coordinates
                {
                    ghostFound = 1; // Set flag to true
                    break;          // Break the inner loop
                }
            }

            // If a ghost is found, skip copying the pellet at the position and move on to the next one
            if (ghostFound)
            {
                continue;
            }

            // If no ghost is found, update the map array with the pellet value
            else
            {
                map[i][j] = row[j];
            }
        }
    }

    // Update the pacmanFlag array
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if (map[i][j] == ' ' || map[i][j] == 'C' || map[i][j] == 'G')
            {
                pacmanFlag[i][j] = 1; // Mark as visited
            }
            else
            {
                pacmanFlag[i][j] = 0; // Mark as not visited
            }
        }
    }

    // File closed after reading
    fclose(file);
}

int main()
{

    // Initialise the game map outline with walls
    char map[ROWS][COLUMNS] = {
        "########################",
        "#   #             ######",
        "#   #####  ###### ######",
        "#                      #",
        "# #             #      #",
        "# # ########### ### ## #",
        "#                      #",
        "# # #         ### #    #",
        "# # #     ### #   ###  #",
        "# # #       # # # ###  #",
        "#                      #",
        "#   ###   ### #        #",
        "# #             ########",
        "########################"};

    // intiialise pellets on map
    initialisePellets(map);

    // Variable to store user's input
    char input;

    // Welcome message and developer names printed
    printf("\tWELCOME TO PAC-MAN\n\n");
    printf("MADE BY:\n");
    printf("SYED HAIDER MURTAZA\nMUJTABA KAMRAN\nMUHAMMAD RAYYAN\n\n");

    // User prompted with option to load a saved game
    printf("Do you want to load a saved game? (Y/N): ");
    input = getch();
    if (input == 'Y' || input == 'y')
    {
        loadGame(map);                           // Load a saved game
        initialiseGhosts(map, pacmanX, pacmanY); // Initialise ghosts based on the loaded game
    }
    else
    {
        initialiseGhosts(map, pacmanX, pacmanY); // Initialise ghosts based on a new game
    }

    // Seed the random number generator with the current time to ensure a different sequence in each program execution
    srand(time(NULL));

    // Main game loop
    do
    {
        // Display current map with score/lives counters, walls, and Pacman
        displayMap(map, &pacmanX, &pacmanY);

        // Control instructions along with game quit option printed
        printf("Control the Pac-Man using W, A, S, D  ");
        printf("\n\nPress Q to quit ");
        input = getch();
        printf("\n");

        // Check if user wants to quit game
        if (input == 'Q' || input == 'q')
        {
            // Ask user if they want to save the game before quitting
            printf("\nDo you want to save the game before quitting? (Y/N): ");
            input = getch();
            if (input == 'Y' || input == 'y')
            {
                saveGame(map); // Save the current game
            }
            break; // End program
        }

        movePacman(map, &pacmanX, &pacmanY, input); // Control Pacman based on user input
        ghostsMovement(map, &pacmanX, &pacmanY);    // Algorithm to move ghosts
    } while (score != WINNING_SCORE && lives > 0);  // Game continued until winning score reached or user runs out of lives

    // Clear the console screen
    system("cls");

    // Display ending message depending upon conclusion
    if (score == WINNING_SCORE)
    {
        // If WINNING_SCORE reached
        printf("\tYOU WON\n");
        remove("game_save.txt"); // Delete the saved file}
    }
    else if (lives == 0)
    {
        // If user runs out of lives
        printf("\tRAN OUT OF LIVES\n");
        remove("game_save.txt"); // Delete the saved file}
    }
    else
    {
        printf("\tWINNING SCORE NOT REACHED\n"); // If user saves games before either of above conditions met
    }

    // Display total score achieved
    printf("\nYour total score was: %d\n", score);
}