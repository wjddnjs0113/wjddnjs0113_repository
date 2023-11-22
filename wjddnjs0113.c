#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>   
#include <Windows.h>

#define KEY_ESC 27
#define KEY_UP (256 + 72)
#define KEY_DOWN (256 + 80)
#define KEY_LEFT (256 + 75)
#define KEY_RIGHT (256 + 77)

void Initialize(void);
int GetKey(void);
void SetCursorVisible(int visible);
void DrawBoard(int** p);
void Shuffle(int** p);
void Swap(int* x, int* y);
int CheckSuccess(void);

int size = 3;
int** target_board = NULL;
int** current_board = NULL;
int zero_row, zero_col;

int main(void)
{
    srand(time(NULL));
    SetCursorVisible(0);
    Initialize();
    Shuffle(current_board);
    DrawBoard(current_board);

    while (1)
    {
        if (CheckSuccess())
        {
            printf("성공!!!\n");
            break;
        }

        int key = GetKey();
        // 방향키 처리

        if (key == KEY_UP)
        {
            if (zero_row > 0)
            {
                Swap(&current_board[zero_row][zero_col], &current_board[zero_row - 1][zero_col]);
                zero_row--;
            }
        }
        else if (key == KEY_DOWN)
        {
            if (zero_row < size - 1)
            {
                Swap(&current_board[zero_row][zero_col], &current_board[zero_row + 1][zero_col]);
                zero_row++;
            }
        }
        else if (key == KEY_LEFT)
        {
            if (zero_col > 0)
            {
                Swap(&current_board[zero_row][zero_col], &current_board[zero_row][zero_col - 1]);
                zero_col--;
            }
        }
        else if (key == KEY_RIGHT)
        {
            if (zero_col < size - 1)
            {
                Swap(&current_board[zero_row][zero_col], &current_board[zero_row][zero_col + 1]);
                zero_col++;
            }
        }
        else if (key == KEY_ESC)
        {
            printf("프로그램 종료\n");
            return 0;
        }

        DrawBoard(current_board);
    }

    printf("프로그램 종료\n");

    return 0;
}

void Initialize(void)
{
    target_board = malloc(sizeof(int*) * size);
    for (int i = 0; i < size; i++)
        target_board[i] = malloc(sizeof(int) * size);

    int num = 1;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            target_board[i][j] = num;
            num++;
        }
    target_board[size - 1][size - 1] = 0;

    current_board = malloc(sizeof(int*) * size);
    for (int i = 0; i < size; i++)
        current_board[i] = malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            current_board[i][j] = target_board[i][j];

    zero_row = size - 1;
    zero_col = size - 1;
}

void DrawBoard(int** p)
{
    system("cls");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (p[i][j] == 0)
                printf("%3s", "   ");
            else
                printf("%3d", p[i][j]);
        }
        printf("\n");
    }
}

void Shuffle(int** p)
{
    int moves1 = 100;
    int moves2 = 0;

    while (moves2 < moves1)
    {
        int direction = rand() % 4;
        int zero_index = zero_row * size + zero_col;
        int zero_index2;

        if (direction == 0)
            zero_index2 = zero_index - size;
        else if (direction == 1)
            zero_index2 = zero_index + size;
        else if (direction == 2)
            zero_index2 = zero_index - 1;
        else if (direction == 3)
            zero_index2 = zero_index + 1;

        if (zero_index2 >= 0 && zero_index2 < size * size &&
            (zero_index / size == zero_index2 / size || zero_index % size == zero_index2 % size))
        {
            Swap(&p[zero_row][zero_col], &p[zero_index2 / size][zero_index2 % size]);
            zero_row = zero_index2 / size;
            zero_col = zero_index2 % size;
            moves2++;
        }
    }
}

void Swap(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int CheckSuccess(void)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (current_board[i][j] != target_board[i][j])
                return 0;
        }
    }
    return 1;
}

int GetKey(void)
{
    int ch = _getch();

    if (ch == 0 || ch == 224)
        // 방향키의 경우 0 또는 224의 값이 먼저 입력됨
        ch = 256 + _getch();
    // 그 다음에 해당 방향키에 따라 72(Up), 
    // 80(Down), 75(Left), 77(Right) 값이 입력됨
    return ch;
}

void SetCursorVisible(int visible)
{
    CONSOLE_CURSOR_INFO ci = { 100, visible };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}
