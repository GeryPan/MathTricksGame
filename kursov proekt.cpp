// kursov proekt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <windows.h> 
#include <cstdlib>  
#include <ctime>//for time() function
using namespace std;
char table[100][100];
int visit_length[2000];
int visit_width[2000];
char allowed_moves[] = { 'A', 'S' , 'D' , 'W' , 'Q' , 'E' , 'Z' , 'X' };
void setColor(int text, int background)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (background << 4) | text);
}

bool allowed_move(char move)
{
    for (int i = 0; i <= 8; i++)
    {
        if (allowed_moves[i] == move) return true;
    }
    return false;
}

bool size_check(int l, int w)
{ 
    if (l < 4 || w < 4)
    {
        return false;
    }
    return true;
}

void make_the_table(int l, int w)
{
    srand(static_cast<unsigned>(time(0)));
    char signs[] = { '+', '-', 'x','/'};
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < (w + w); j += 2)
        {
            int operation = rand() % 50;
            if (operation < 3)
            {
                table[i][j] = 'x';
                table[i][j + 1] = '0';
            }
            else if (operation < 9)
            { 
                table[i][j + 1] = '2';
                table[i][j] = 'x';
            }
            else if (operation < 13)
            {  
                table[i][j + 1] = '2';
                table[i][j] = '/';
            }
            else if(operation < 35)
            {  
                table[i][j + 1] = rand() % 9 + 1 + '0';
                table[i][j] = signs[rand() % 1];
            }
            else if (operation < 40)
            {  
                table[i][j + 1] = rand() % 9 + 1 + '0';
                table[i][j] = signs[rand() % 1+1];
            }
            else
            { 
                table[i][j + 1] = rand() % 9 + 1 + '0';
                table[i][j] = signs[rand() % 3 + 1];
            }
        }
    }
    table[0][0] = ' ';
    table[0][1] = '0';
    table[l - 1][w + w - 2] = ' ';
    table[l - 1][w + w - 1] = '0';
}

int relocation_l(char move, int move_l)
{
    int i = move_l;

    if (move == 'W')
    {
        i--;
    }
    else  if (move == 'S')
    {
        i++;
    }
    else  if (move == 'Q')
    {
        i--;
    }
    else  if (move == 'E')
    {
        i--;
    }
    else  if (move == 'Z')
    {
        i++;
    }
    else  if (move == 'X')
    {
        i++;
    }
    return i;
}

int relocation_w(char move, int move_w)
{
    int j = move_w;
    if (move == 'A')
    {
        j -= 2;
    }
    else  if (move == 'D')
    {
        j += 2;
    }
    else  if (move == 'Q')
    {
        j -= 2;
    }
    else  if (move == 'E')
    {
        j += 2;
    }
    else  if (move == 'Z')
    {
        j -= 2;
    }
    else  if (move == 'X')
    {
        j += 2;
    }
    return j;
}

bool move_check(int l, int w, char move, int move_l, int move_w)
{
    if (relocation_l(move, move_l) < 0 || relocation_l(move, move_l) >= l || relocation_w(move, move_w) < 0 || relocation_w(move, move_w) >= (w+w))
    {
        return false;
    }
    else return true;
}

int new_points(int points, int l, int w)
{
    int number = table[l][w] - '0';
    if (table[l][w - 1] == '+') points += number;
    else if (table[l][w - 1] == '-') points -= number;
    else if (table[l][w - 1] == 'x') points *= number;
    else if (table[l][w - 1] == '/') points /= number;
    return points;
}

void visited_place(int i, int move_l, int move_w)
{
    visit_length[i] = move_l;
    visit_width[i] = move_w;
}

bool can_you_step_there(int l, int w,int i, int move_l, int move_w, char move)
{
    int j = i-1;
    if (j <= 2) return true;
    while (j>= 0)
    {
        if (visit_length[j] == relocation_l(move, move_l) && visit_width[j] == relocation_w(move, move_w))
        {
            return false;
        }
        j--;
    }
    return true;
}
bool general_check(int length, int width, int i, int l, int w)
{
    int j = i - 1;
    while (j >= 0)
    {
        if (visit_length[j] == l && visit_width[j] == w)
        {
            return false;
        }
        j--;
    }
    if (l < 0 || w < 0 || l >= length || w >= width)
    {
        return false;
    }
    else return true;
}
bool stop_the_game(int length, int width, int i, int current_l, int current_w)
{
    char possible_moves[] = { 'A', 'D', 'W', 'S', 'Q', 'E', 'Z', 'X' };

    for (int k = 0; k < 8; k++) {
        char move = possible_moves[k];

        if (move_check(length, width, move, current_l, current_w) &&
            can_you_step_there(length, width, i, current_l, current_w, move)) {
            return true; 
        }
    }

    return false; 
}
int winner(int points1, int points2)
{
    if (points1 > points2) return 1;
    else return 2;
}
void process_player_move(int l, int w, int &points,int &move1l, int &move1w, int i)
{
    char move;
    cin >> move;
    if (allowed_move(move) == false)
    {
        cout << "Invalid move, try again." << endl;
        process_player_move(l, w, points, move1l, move1w, i);
    }

        else if (move_check(l, w, move, move1l, move1w) == false)
        {
            cout << "You can't leave the field." << endl << "Enter allowed move" << endl;
            process_player_move(l, w, points, move1l, move1w, i);
        }

        else if (can_you_step_there(l, w, i, move1l, move1w, move) == false)
        {
            cout << "You can't step on a cage which has already been passed once." << endl << "Enter allowed move" << endl;
            process_player_move(l, w, points, move1l, move1w, i);
        }

        else 
        {
            move1l = relocation_l(move, move1l);
            move1w = relocation_w(move, move1w);
            visited_place(i, move1l, move1w);
            points = new_points(points, move1l, move1w);
        }
}

void player2(int l, int w, int &points, int &move2l, int &move2w, int i)
{
        char move;
        cin >> move;
        if (allowed_move(move) == false)
        {
            cout << "Invalid move, try again." << endl;
            player2(l, w, points, move2l, move2w, i);
        }

        else if (move_check(l, w, move, move2l, move2w) == false)
        {
            cout << "You can't leave the field." << endl << "Enter allowed move" << endl;
            player2(l, w, points, move2l, move2w, i);
        }

        else if (can_you_step_there(l, w, i, move2l, move2w, move) == false)
        {
            cout << "You can't step on a cage which has already been passed once." << endl << "Enter allowed move" << endl;
            player2(l, w, points, move2l, move2w, i);
        }

        else
        {
            move2l = relocation_l(move, move2l);
            move2w = relocation_w(move, move2w);
            visited_place(i, move2l , move2w);
            points = new_points(points, move2l, move2w);
        }
}

bool has_it_been_steped_BY_BLUE(int l, int w, int i)
{
    int k = 0;
    while (k <= i)
    {
        if (visit_length[k] == l && visit_width[k] == w && k % 2 == 0)
        {
            return false;
            break;
        }
        k++;
    }
    return true;
}

bool has_it_been_steped_BY_GREEN(int l, int w, int i)
{
    int k = 0;
    while (k <= i)
    {
        if (visit_length[k] == l && visit_width[k] == w && k % 2 == 1)
        {
            return false;
            break;
        }
        k++;
    }
    return true;
}

void print_table(int l, int w, int k, int place_l_1, int place_w_1, int place_l_2, int place_w_2)
{
    cout << "\t";
    visit_length[0] = 0;
    visit_width[0] = 1;
    visit_length[1] = l - 1;
    visit_width[1] = w + w - 1;
    for (int cols = 0; cols < w; cols++)
    {
        cout << " " << cols << "\t";
    }
    cout << endl << endl;
    for (int i = 0; i < l; i++) 
    {
        cout << i << "|" << "\t";
        for (int j = 1; j < (w + w); j+=2)
        {
            if (i == place_l_1 && j == place_w_1)
            {
                setColor(22, 25);
                cout << table[i][j - 1] << table[i][j];
            }
            else
                if (i == place_l_2 && j == place_w_2)
                {
                    setColor(38, 34);
                    cout << table[i][j - 1] << table[i][j];
                }
            else if (has_it_been_steped_BY_BLUE(i, j, k) == false)
            {
                setColor(15, 25);
                cout << table[i][j-1] << table[i][j];
            }
            else if (has_it_been_steped_BY_GREEN(i, j, k) == false)
            {
                setColor(15, 34);
                cout << table[i][j - 1] << table[i][j];
            }
            else
            {
                setColor(15, 0);
                cout << table[i][j - 1] << table[i][j];
            }
                cout << "\t";
                setColor(7, 0);
        }
        cout << endl;

    }
}

int main()
{
    int length, width;
    cout << "Enter the game dimensions (two positive numbers defining the length and width of the playing field). ";
    cin >> length >> width;
    while (size_check(length, width) == false)
    {
        cout << "The minimum game size is 4x4! ";
        cin >> length >> width;
    }
    int l_start1 = 0, w_start1 = 1, l_start2 = length-1, w_start2 = 2*width-1;
    int points_1 = 0, points_2 = 0;

    cout << "A-left, D-right, W-up, S-down, E-top right diagonal, Q-top left diagonal, X-bottom right diagonal, Z-bottom left diagonal" << endl;

    make_the_table(length, width);
    for (int i = 2; i < (length * width); i++)
    {
        cout << "Enter where you want to move." << endl;
        print_table(length, width, i, l_start1, w_start1, l_start2, w_start2);
        if (i % 2 == 0)
        {

            if (stop_the_game(length, width, i, l_start1, w_start1) == false)
            {
                i = length * width;
                
            }
            else 
            {
                cout << "Player 1: ";
                process_player_move(length, width, points_1, l_start1, w_start1, i);
            }
        }
        else
        {
            if ((length, width, i, l_start2, w_start2) == false)
            {
                break;
            }
            else 
            {
                cout << "Player 2: ";
                process_player_move(length, width, points_2, l_start2, w_start2, i);
            }
        }
        cout << "Blue=" << points_1 << " VS Green=" << points_2 << endl;
    }
    if (points_1 == points_2) cout << "Equal";
    cout << "The winner is Player " << winner(points_1, points_2);
} //Ostana ti da SE CHISTI KONZOLATA
        



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
