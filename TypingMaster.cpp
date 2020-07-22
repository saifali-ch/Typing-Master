#include <iostream>
#include <time.h>
#include <Windows.h>
#include <random>
#include <conio.h>
#include <iomanip>

using namespace std;

class LinkedList
{
    struct Node
    {
        int data;
        int xPos;
        Node *next;
    };

    Node *createNode(int data, int xPos)
    {
        len++;
        Node *node = new Node;
        node->data = data;
        node->xPos = xPos;
        node->next = nullptr;
        return node;
    }
    int len = 0;
    Node *first = nullptr, *last = nullptr;

public:
    //Utility Functions
    bool isEmpty()
    {
        if (first)
            return false;
        cout << "List is empty\n";
        return true;
    }

    //User Functions
    void append(int data, int xPos)
    {
        Node *temp = createNode(data, xPos);
        if (!first)
        {
            last = first = temp;
            return;
        }
        last->next = temp;
        last = temp;
    }

    void deleteNode(int value)
    {
        if (isEmpty())
            return;
        len--;
        Node *temp = first;
        if (first->data == value)
        {
            first = first->next;
            delete temp;
            return;
        }
        Node *pre = nullptr;
        while (temp)
        {
            if (temp->data == value)
            {
                pre->next = temp->next;
                if (!temp->next)
                    last = pre;
                delete temp;
                return;
            }
            pre = temp;
            temp = temp->next;
        }
    }

    int size()
    {
        return len;
    }

    void show()
    {
        if (isEmpty())
            return;
        Node *copy = first;
        while (copy)
        {
            cout << copy->data << endl;
            copy = copy->next;
        }
    }

    bool exists(int value)
    {
        if (isEmpty())
            return false;
        Node *copy = first;
        while (copy)
        {
            if (copy->data == value)
                return true;
            copy = copy->next;
        }
        return false;
    }

    void clear()
    {
        Node *temp = first;
        first = last = nullptr;
        delete temp;
        len = 0;
    }

    void iterate(int &xPos, int &ascii)
    {
        len--;
        Node *temp = first;
        xPos = first->xPos;
        ascii = first->data;
        first = first->next;
        delete temp;
        append(ascii, xPos);
    }
};

struct Position
{
    short x;
    short y;
};

Position chancePosition{18, 29}; // Chance position
Position scoresPosition{56, 1};  // Score position
Position missedPosition{11, 1};  // Missed position
Position timePosition{102, 1};   // Clock position
Position histPosition{61, 29};   // Hits position
Position savedPosition{0, 0};    // Used to Save Cursor Position at return to previous locatoin

LinkedList list;       // The game strictly follows FIFO order
string mode = "Digit"; // Used to display different game modes in bottom right of the main screen

int timerValue = 0; // keep track of timer value
int chances = 0;    // Keep track of chance
int missed = 0;     // Keep track of missed
int scores = 0;     // Keep track of scores
int hits = 0;       // Keep track of total hits

void gameMode(string mode, int seconds, int chances); // Heart of Program
void initializeValues(int seconds, int chanceCount);  // Initialize Everthing to Default Values
void rectangle(int x1, int y1, int x2, int y2);       // Just to print border around game screen
void gameOverMenu(string str, int x, int y);          // Game Over Menu
void updateScreen(Position any, int value);           // Update Scores, Missed, Total Chances, Total Hits
void updateTime(int no_of_seconds);                   // Used to Update time in realtime
void gotoxy(short x, short y);                        // Change cursor position
void saveCurrentPosition();                           // When invoked it saves the most recent cursor position
void cursor(bool visible);                            // Set cursor visibility
void drawGameScreen();                                // All filler elements in game screen
void printChars();                                    // To print character on screen
void clear();                                         // Clear the character portion of screen
string timeFormat(int no_of_seconds);                 // It returns the 00:00 time format which is used in main screen



int main()
{
    int choice;
    while (true)
    {
        system("cls");
        cursor(false);
        char menuString[] =
            "                <<==============Welcome To Typing Tutor============>>\n"
            "                <<=================================================>>\n"
            "                |Project Name : Typing Tutor v1.0.0                 |\n"
            "                |Developed By : Saif Ali                            |\n"
            "                |Reg #        : SP19-BSE-036                        |\n"
            "                |Version      : 1.0.0                               |\n"
            "                <<=================================================>>\n"
           
            "<==========><==========><==========><==========><==========><==========><==========>\n"
            "1) Digit Mode\n"
            "2) Alphabet Mode\n"
            "3) Alphanumeric Mode\n"
            "4) Exit\n\n";
        for (int i = 0; i < sizeof menuString / sizeof menuString[0]; i++)
        {
            cout << menuString[i];
            // Sleep(1);
        }
        cursor(true);
        cout << "Enter Your Choice : ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            gameMode("Digit", 100, 3);
            break;
        case 2:
            gameMode("Alphabetic", 110, 3);
            break;
        case 3:
            gameMode("Alphamumeric", 120, 3);
            break;
        case 4:
            exit(-1);
            break;
        }
    }
    cout << "\nHalted\n";
}

void gameMode(string mode, int seconds, int chances)
{
    srand(time(NULL));
    initializeValues(seconds, chances);
    drawGameScreen();
    for (int y = 3; y < 1000; y++)
    {
        clear();
        printChars();
        Sleep(1000);
        updateTime(--timerValue);
        ///////////////random number generation///////////////
        int digit, alphabet, alphanumeric;
        digit = 48 + rand() % 10;
        alphabet = 97 + rand() % 26;
        alphanumeric = rand() % 2 ? digit : alphabet;
        //////////////////////////////////////////////////////

        int xPos = 2 + rand() % 106; // Random X Position
        if (mode.compare("Digit") == 0)
            list.append(digit, xPos);
        else if (mode.compare("Alphabetic") == 0)
            list.append(alphabet, xPos);
        else if (mode.compare("Alphamumeric") == 0)
            list.append(alphanumeric, xPos);
        if (kbhit())
        {
            cursor(false);
            int input = getch();
            if (input == 27)
                return;
            updateScreen(histPosition, ++hits);
            if (list.exists(input))
            {
                Beep(5000, 100);
                list.deleteNode(input);
                scores += 5;
                updateScreen(scoresPosition, scores);
            }
            else
            {
                Beep(1000, 100);
                updateScreen(missedPosition, ++missed);
            }
        }
        if (list.size() == 26)
        {
            y = 2;
            list.clear();
            updateScreen(chancePosition, --chances);
        }
        if (chances < 1 || scores > 300 || timerValue < 1 || missed > 19)
        {
            clear();
            string str = "You Loose the Game";
            if (timerValue < 1)
                str = "Time Over";
            if (scores > 300)
                str = "You Won the Game";
            if(missed > 19)
                str = "You Have Missed More Than 19 Characters";
            string con = "Back to the Game Menu";
            gameOverMenu(str, 43, 13);
            gameOverMenu(con, 43, 15);
            gotoxy(43, 17);
            system("pause");
            return;
        }
    }
}

void initializeValues(int seconds, int chanceCount)
{
    list.clear();
    chances = chanceCount;
    timerValue = seconds;
    missed = 0;
    scores = 0;
    hits = 0;
}

void rectangle(int x1, int y1, int x2, int y2)
{
    int i, m;
    gotoxy(x1, y1);
    cout << (char)201;
    for (i = x1 + 1; i < x2 - 1; i++)
    {
        gotoxy(i, y1);
        cout << (char)205;
    }
    gotoxy(i, y1);
    cout << (char)187;
    for (m = y1 + 1; m < y2; m++)
    {
        gotoxy(x1, m);
        for (i = x1; i < x2; i++)
        {
            if (i == x1 || i == x2 - 1)
            {
                gotoxy(i, m);
                cout << (char)186;
            }
        }
    }
    gotoxy(x1, m);
    cout << (char)200;
    for (i = x1 + 1; i < x2 - 1; i++)
    {
        gotoxy(i, m);
        cout << (char)205;
    }
    gotoxy(i, m);
    cout << (char)188;
}

void gameOverMenu(string str, int x, int y)
{
    gotoxy(x, y);
    for (int i = 0; i < str.length(); i++)
    {
        cout << str.at(i);
        Sleep(50);
    }
}

void updateScreen(Position any, int value)
{
    saveCurrentPosition();
    gotoxy(any.x, any.y);
    cout << to_string(value);
    gotoxy(savedPosition.x, savedPosition.y);
}

void updateTime(int no_of_seconds)
{
    saveCurrentPosition();
    gotoxy(timePosition.x, timePosition.y);
    cout << timeFormat(no_of_seconds);
    gotoxy(savedPosition.x, savedPosition.y);
}

void gotoxy(short x, short y)
{
    COORD coord{x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void saveCurrentPosition()
{ //gets current console pos x, y
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h, &csbi);
    savedPosition.x = csbi.dwCursorPosition.X;
    savedPosition.y = csbi.dwCursorPosition.Y;
}

void cursor(bool visible)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void drawGameScreen()
{
    system("cls");
    cout << "\n  Missed = " << missed;
    cout << "\t\t\t\t\tScore = " << scores;
    cout << "\t\t\t     Remaining Time - " << timeFormat(timerValue);
    cout << "\n  ----------------------------------------------------------------------------------------------------------";
    for (int i = 0; i < 25; i++)
        cout << endl;
    cout << "\n  ----------------------------------------------------------------------------------------------------------";
    cout << "\n  Total Chances = " << chances;
    cout << "\t\t\t\tTotal Hits = " << hits;
    cout << "\t\t" << setw(18) << mode << " Mode | " << timeFormat(timerValue) << " min\n";
    rectangle(0, 0, 110, 30);
}

void printChars()
{
    int j = list.size() + 2;
    while (j >= 3)
    {
        cursor(false);
        int xPos, ascii;
        list.iterate(xPos, ascii);
        gotoxy(xPos, j--);
        cout << (char)ascii;
    }
}

void clear()
{
    cursor(false);
    for (int y = 3; y < 28; y++)
    {
        gotoxy(2, y);
        cout << "                                                     ";
        cout << "                                                      ";
        cout << endl;
    }
}

string timeFormat(int no_of_seconds)
{
    string result = "";
    if (no_of_seconds == 120)
        result += "02:";
    else if (no_of_seconds > 59)
        result += "01:";
    else
        result += "00:";
    result += (no_of_seconds % 60 < 10) ? "0" : "";
    result += to_string(no_of_seconds % 60);
    return result;
}
