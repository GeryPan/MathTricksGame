#include <iostream>
#include <random>
#include <windows.h> 
#include <cstdlib>  
#include <ctime>//for time() function
#include <fstream> // for file operations

using namespace std;

#define MAX_LENGTH 200
#define MAX_WIDTH 200


int visit_length[MAX_LENGTH] = { 0 };
int visit_width[MAX_WIDTH] = { 0 };
char table[MAX_LENGTH][MAX_WIDTH * 2] = {};
char allowed_moves[] = { 'A', 'S' , 'D' , 'W' , 'Q' , 'E' , 'Z' , 'X' , 'a', 's','d','w','q','e','z','x' };

void setColor(int text, int background)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (background << 4) | text);
}

bool allowed_move(char move)
{
    for (int i = 0; i <= 16; i++)
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
    move = tolower(move);

    switch (move)
    {
        case 'w':  
        case 'q':
        case 'e': 
            i--; 
            break;

        case 's':
        case 'z':
        case 'x':
            i++;
            break;

        default:
            break;
    }

    return i;
}


int relocation_w(char move, int move_w)
{
    int j = move_w;
    move = tolower(move);

    switch (move)
    {
        case 'a':  
        case 'q': 
        case 'z':  
            j -= 2;  
            break;

        case 'd':  
        case 'e':  
        case 'x':  
            j += 2;  
            break;
        default:
            break;
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

bool check_can_you_step_there(int l, int w,int i, int move_l, int move_w, char move)
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
    for (int k = 0; k < 8; k++) {
        char move = allowed_moves[k];

        if (move_check(length, width, move, current_l, current_w) &&
            check_can_you_step_there(length, width, i, current_l, current_w, move)) {
            return true; 
        }
    }

    return false; 
}

int check_winner(int points1, int points2)
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
    else if (check_can_you_step_there(l, w, i, move1l, move1w, move) == false)
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
            else if (i == place_l_2 && j == place_w_2)
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

//  Function for writing the state of a game in file 
//TODO fix it 
void serialize_game(const string& filename, int length, int width, int points_1, int points_2, int l_start1, int w_start1, int l_start2, int w_start2) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    file.write((char*)&length, sizeof(length));
    file.write((char*)&width, sizeof(width));
    file.write((char*)&points_1, sizeof(points_1));
    file.write((char*)&points_2, sizeof(points_2));
    file.write((char*)&l_start1, sizeof(l_start1));
    file.write((char*)&w_start1, sizeof(w_start1));
    file.write((char*)&l_start2, sizeof(l_start2));
    file.write((char*)&w_start2, sizeof(w_start2));


    for (int i = 0; i < length; i++) {
        file.write((char*)&visit_length[i], sizeof(int));
    }

    for (int i = 0; i < width; i++) {
        file.write((char*)&visit_width[i], sizeof(int));
    }

    for (int i = 0; i < length; i++) {
        file.write((char*)table[i], width * 2 * sizeof(char)); 
    }

    file.close();
    cout << "Game state successfully saved to " << filename << endl;
}


// Function for reading the state of a game from file 
//TODO fix it 
void deserialize_game(const string& filename, int& length, int& width, int& points_1, int& points_2, int& l_start1, int& w_start1, int& l_start2, int& w_start2) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    file.read((char*)&length, sizeof(length));
    file.read((char*)&width, sizeof(width));
    file.read((char*)&points_1, sizeof(points_1));
    file.read((char*)&points_2, sizeof(points_2));
    file.read((char*)&l_start1, sizeof(l_start1));
    file.read((char*)&w_start1, sizeof(w_start1));
    file.read((char*)&l_start2, sizeof(l_start2));
    file.read((char*)&w_start2, sizeof(w_start2));

    cout << typeid(length).name() << endl;

    for (int i = 0; i < length; i++) {
        file.read((char*)&visit_length[i], sizeof(int));
    }

    for (int i = 0; i < width; i++) {
        file.read((char*)&visit_width[i], sizeof(int));
    }

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width * 2; j++) {
            file.read((char*)&table[i][j], sizeof(char));
        }
    }

    file.close();
    cout << "Game state successfully loaded from " << filename << endl;
}


int main()
{
    int length, width;
    int choice_number;
    int l_start1 = 0, w_start1 = 1, l_start2 = 0, w_start2 = 0;
    int points_1 = 0, points_2 = 0;


    cout << "Welcome to the game!" << endl;
    cout << "1. Start new game" << endl;
    cout << "2. Load saved game" << endl;

    cin >> choice_number;

    if (choice_number == 2) {
        deserialize_game("savegame.bin", length, width, points_1, points_2, l_start1, w_start1, l_start2, w_start2);
    }
    else {
        cout << "Enter the game dimensions (two positive numbers defining the length and width of the playing field). " << endl;
        cout << "Length: ";
        cin >> length;
        cout << "Width: ";
        cin >> width;

        l_start1 = 0, w_start1 = 1;

        l_start2 = length - 1;
        w_start2 = 2 * width - 1;
        while (size_check(length, width) == false)
        {
            cout << "The minimum game size is 4x4! ";
            cin >> length >> width;
        }

        cout << "A-left, D-right, W-up, S-down, E-top right diagonal, Q-top left diagonal, X-bottom right diagonal, Z-bottom left diagonal" << endl;

        make_the_table(length, width);
    }

    for (int i = 2; i < (length * width); i++)
    {
        system("cls");
        cout << "Allowed moves" << endl;
        cout << "A-left, D-right, W-up, S-down, E-top right diagonal, Q-top left diagonal, X-bottom right diagonal, Z-bottom left diagonal" << endl;
        cout << "Blue=" << points_1 << " VS Green=" << points_2 << endl;
        print_table(length, width, i, l_start1, w_start1, l_start2, w_start2);

        char choice;
        cout << "Type 'Y' or 'y' to save the game: ";
        cin >> choice;

        if (choice == 'Y' || choice == 'y') {
            serialize_game("savegame.bin", length, width, points_1, points_2, l_start1, w_start1, l_start2, w_start2);
            cout << "Game state saved successfully!" << endl;
            i--;  // this prevent missing a move 
            system("pause");
            continue; // continue with current move
        }

        cout << "Enter where you want to move." << endl;
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
     
        
    }
    if (points_1 == points_2) cout << "Equal";
    cout << "The winner is Player " << check_winner(points_1, points_2);

    return 0;   
}

