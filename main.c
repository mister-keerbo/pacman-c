#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define ROWS 33
#define COLS 28

void renderTable(char strArr[ROWS][COLS * 3 + 1]);
int kbhit(void);
int getch(void);

int main()
{	
	// Clear screen & Hide cursor
    printf("\e[2J\e[H\e[?25l");

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
        "|............||............|",
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
	int pacDir = 0; // 0 for horizontal 1 for vertical
	int pacX = 14;
	int pacY = 23;
	char pac = 'C';
	int score = 0;
	int lives = 3;
	
	// Ghosts settings
	

	printf("Press enter to start...");
	getchar();	
	printf("\e[2J\e[H");

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
			sleep(1);
			for (int i = 0; i < 6; i++)
				map[17][i + 11] = readyStr[i];
			renderTable(map);
			sleep(1);
			map[17][12] = '2';
			renderTable(map);
			sleep(1);
			map[17][12] = '1';
			renderTable(map);
			sleep(1);
			for (int i = 0; i < 6; i++)
                map[17][i + 11] = goStr[i];
			renderTable(map);
			map[12][13] = '|';
			map[12][14] = '|';
			start = 1;
		}
		char kb;
		if (kbhit())
			kb = getch();
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
		// Pacman Movement
		if (!pacDir)
		{
			map[pacY][pacX] = ' ';
			if (map[pacY][pacX + pacXDir] != '|' && map[pacY][pacX + pacXDir] != '_')
		    {
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
				if (map[pacY + pacYDir][pacX] == '.') score++;
				pacY += pacYDir;
			}
			map[pacY][pacX] = pac;
		}

		// Score text
		if (score >= 10)
		{
			if (score >= 100)
			{
				map[31][8] = score / 100 + '0';
				map[31][9] = score / 10 % 10 + '0';
				map[31][10] = score % 10 + '0';
			}
			else
			{
				map[31][8] = score / 10 + '0';
				map[31][9] = score % 10 + '0';
			}
		}
		else
			map[31][8] = score + '0';
		
		if (lives == 2) map[31][24] = ' ';
		else if (lives == 1) map[31][25] = ' ';
		// Game over
		else if (lives == 0)
		{
			map[31][26] = ' ';
			sleep(1);
			pac = '(';
			map[pacY][pacX] = pac;
			renderTable(map);
			sleep(1);
			pac = ' ';
			map[pacY][pacX] = pac;
			renderTable(map);
			for (int i = 0; i < 8; i++)
				map[17][10+i] = overStr[i];
				renderTable(map);
				sleep(1);
			quit = 1;
		}
		usleep(250000);
	}
	
	return 0;
}

void renderTable(char strArr[ROWS][COLS * 3 + 1]) {
	printf("\n");
	for (int i = 0; i < ROWS; i++)
    {
    	printf("%s\n", strArr[i]);
    }
	printf("\e[H\r");
}
int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
int getch(void)
{
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

