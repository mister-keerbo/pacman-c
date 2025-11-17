#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define ROWS 33
#define COLS 28

void renderTable(char strArr[ROWS][COLS * 3 + 1]);
int kbhit(void);
int getch(void);

// Function to clear screen
void clearScreen() {
    system("cls");
}

// Function to hide cursor
void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Function to show cursor
void showCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

int main()
{	
    // Clear screen & Hide cursor
    clearScreen();
    hideCursor();

    // Table
    char map[ROWS][COLS * 3 + 1] = {
        " ____________  ____________ ",
        "|............||............|",
        "|. __ . ___ .||. ___ . __ .|",
        "|O|  |.|   |.||.|   |.|  |O|",
        "|.|__|.|___|.||.|___|.|__|.|",
        "|..........................|",
        "|. __ .  . ______ .  . __ .|",
        "|.|__|.||.|__  __|.||.|__|.|",
        "|......||....||....||......|",
        "|____ .||__  ||  __||. ____|",
        "     |.| __|    |__ |.|     ",
        "     |.||          ||.|     ",
        "     |.||  __==__  ||.|     ",
        "_____|.|| |      | ||.|_____",
        "      .   | MMMM |   .      ",
        "_____ .   |______|   . _____",
        "     |.||          ||.|     ",
        "     |.||  Ready!  ||.|     ",
        "     |.||  ______  ||.|     ",
        " ____|.|| |__  __| ||.|____ ",
        "|............||.M..........|",
        "|. __ . ___ .||. ___ . __ .|",
        "|.|_ |.|___|.||.|___|.| _|.|",
        "|O .||................||. O|",
        "|_ .||.  . ______ .  .||. _|",
        " _|.||.||.|__  __|.||.||.|_ ",
        "|......||....||....||......|",
        "|. ____||__ .||. __||____ .|",
        "|.|________|.||.|________|.|",
        "|..........................|",
        "|__________________________|",
        " Score: 0               CCC "
    };
	
    // Player settings
    int pacXDir = 1;
    int pacYDir = 1;
    int pacDir = 0; // 0 for horizontal, 1 for vertical
    int pacX = 14;
    int pacY = 23;
    char pac = 'C';
    int score = 0;
    int lives = 3;
	
    // Ghosts settings (not included here, but you can add as needed)
	
    printf("Press enter to start...");
    getchar();	
    clearScreen();

    int quit = 0;
    int start = 0;
    char readyStr[] = " 3... ";
    char goStr[] = " Go!! ";
    char overStr[] = "GameOver";

    while (!quit)
    {
        renderTable(map);
        if (!start)
        {
            Sleep(1000);
            for (int i = 0; i < 6; i++)
                map[17][i + 11] = readyStr[i];
            renderTable(map);
            Sleep(1000);
            map[17][12] = '2';
            renderTable(map);
            Sleep(1000);
            map[17][12] = '1';
            renderTable(map);
            Sleep(1000);
            for (int i = 0; i < 6; i++)
                map[17][i + 11] = goStr[i];
            renderTable(map);
            map[12][13] = '|';
            map[12][14] = '|';
            start = 1;
        }

        if (_kbhit())
        {
            char kb = _getch();
            if (kb == 'w')
            {
                pacDir = 1;
                pacYDir = -1;
                pac = 'U';
            }
            else if (kb == 'a')
            {
                pacDir = 0;
                pacXDir = -1;
                pac = '>';
            }
            else if (kb == 's')
            {
                pacDir = 1;
                pacYDir = 1;
                pac = 'A';
            }
            else if (kb == 'd')
            {
                pacDir = 0;
                pacXDir = 1;
                pac = 'C';
            }
        }

        // Pacman Movement
        if (!pacDir)
        {
            map[pacY][pacX] = ' ';
            if (map[pacY][pacX + pacXDir] != '|' && map[pacY][pacX + pacXDir] != '_')
            {
                if (map[pacY][pacX + pacXDir] == 'M') quit = 1;
                if (map[pacY][pacX + pacXDir] == '.') score++;
                pacX += pacXDir;
            }
            map[pacY][pacX] = pac;
        }
        else
        {
            map[pacY][pacX] = ' ';
            if (map[pacY + pacYDir][pacX] != '|' && map[pacY + pacYDir][pacX] != '_')
            {
                if (map[pacY + pacYDir][pacX] == 'M') quit = 1;
                if (map[pacY + pacYDir][pacX] == '.') score++;
                pacY += pacYDir;
            }
            map[pacY][pacX] = pac;
        }

        // Score display
        if (score >= 10)
        {
            if (score >= 100)
            {
                map[31][8] = score / 100 + '0';
                map[31][9] = (score / 10) % 10 + '0';
                map[31][10] = score % 10 + '0';
            }
            else
            {
                map[31][8] = score / 10 + '0';
                map[31][9] = score % 10 + '0';
            }
        }
        else
        {
            map[31][8] = score + '0';
        }

        // Lives display
        if (lives == 2) map[31][24] = ' ';
        else if (lives == 1) map[31][25] = ' ';
        else if (lives == 0)
        {
            map[31][26] = ' ';
            Sleep(1000);
            pac = '(';
            map[pacY][pacX] = pac;
            renderTable(map);
            Sleep(1000);
            pac = ' ';
            map[pacY][pacX] = pac;
            renderTable(map);
            for (int i = 0; i < 8; i++)
                map[17][10 + i] = overStr[i];
            renderTable(map);
            Sleep(1000);
            quit = 1;
        }

        Sleep(250);
    }

    // Show cursor again before exiting
    showCursor();

    return 0;
}

void renderTable(char strArr[ROWS][COLS * 3 + 1]) {
    clearScreen();
    printf("\n");
    for (int i = 0; i < ROWS; i++)
    {
        printf("%s\n", strArr[i]);
    }
}
