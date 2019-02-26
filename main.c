
// minesweeperbetatesting
// Created by Andreea Moraru on 25/11/18.
// Copyright © 2018 Andreea Moraru. All rights reserved.

// minesweeper

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

//// the user choose between a big 1 small board
// 2 medium board
///// 3 big board ( the bigger the board the harder is the level and the number of mines);
//display the welcome messages

void printBoard(char **board, int width, int height);
void Set_game(int size, int *height, int *width, int *mine);
void create_mines(int height,int width, char **board, int mines);
void Gboard(char **board, int width, int height);
void MainBoard(char **board, int width, int height, int mines);
char nearbyMines(char **mboard, int height,int width, int x, int y);
void reveal(char **board,char **mboard, int width, int height, int x, int y, int *openCells);
void game(char **board, char **mboard,int width, int height, int *openCells, int cells);


int main(){
    time_t start = 0;
    time_t end = 0;
    time_t elapsed = 0;
    
    start = time(NULL);  // grab start time
    int height = 0; //height of the board
    int width = 0; // width of the board
    int mines = 0;
    int size = 0; // difficulty of the game or size
    char **board = NULL;
    char **mboard = NULL;
    int openCells = 0; //the cells that the user opened
    int cells = 0; //the total number of cells
    //we ask the user for the size they want
    
    FILE *fp;
    char buff[255];
    fp = fopen("print.txt", "r");
    
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    
    while (fgets(buff, 10, fp)) {
        printf("%s", buff);
    };
    printf("\n");
    
    scanf("%d", &size);
    
    Set_game(size, &height, &width, &mines); ////we set the numeber of mines and the variables for the size;
    printf("mines: %d height %d width %d\n", mines, height, width);
    
    board = (char**)malloc(height* sizeof(char*)); //we allocate memory for the board like a bidemension array using double pointer
    mboard = (char**)malloc(height * sizeof(char*));
    
    
    for(int i = 0; i < height; i++){
        board[i] = (char*)malloc(width * sizeof(char)); //we allocate dinamicaly memory for every row
        mboard[i] =(char*)malloc(width * sizeof(char));
    }
    
    if(board == NULL || mboard == NULL) return 0 ;
    /// arr[i][j] = ++count; // OR *(*(arr+i)+j) = ++count
    cells = width * height - mines;
    
    
    Gboard(board, width, height);// we contruct the board that the user sees
    MainBoard(mboard, width, height, mines);
    
    printBoard(board, height, width);
    printBoard(mboard, height, width);
    //  reveal(board, mboard, width, height, 1, 1);
    //  printBoard(board, height, width);
    game(board, mboard, width, height, &openCells, cells);
    free(board);
    
    
    end = time(NULL);  // grab the end time
    elapsed = end - start;  // compute elapsed time
    // now show the elapsed time
    printf("the time you spent is: %ld seconds\n", elapsed);
    return 0;
}

void print_h_line(int width, wchar_t sep) {
    wchar_t line = 0x2550;//unicode character
    setlocale(LC_CTYPE,"UTF-8"); //
    for(int i = 0; i < width - 1; i++){
        wprintf(L"%lc%lc%lc%lc", line, line, line, sep); ///we print the column numbers;
    }
    wprintf(L"%lc", line);
    
    return;
}

void draw_top_line(int width) {
    wchar_t sep = 0x2566; //separator
    print_h_line(width, sep);
}

void draw_mid_line(int width) {
    wchar_t sep = 0x256C;
    print_h_line(width, sep);
}

void draw_bot_line(int width) {
    wchar_t sep = 0x2569;
    print_h_line(width, sep);
    
    
    
}
void printBoard(char **board, int width, int height){ //this funcion prints every value of a bidemensional array; its a general function that help us to print our board;
    wchar_t line = 0x2550;
    wchar_t vert_line = 0x2551;
    wchar_t lt_corner = 0x2554; //top lesft
    wchar_t lb_corner = 0x255A; //left bottom
    wchar_t rt_corner = 0x2557; //right top
    wchar_t rb_corner = 0x255D; //right bottom
    wchar_t l_line = 0x2560;
    wchar_t r_line = 0x2563;
    wchar_t bomb = 0x1F4A3;
    wchar_t flag = 0x1F6A9;
    // bomb = flag;
    setlocale(LC_CTYPE,"UTF-8");
    //    putwchar(hello);
    //    putchar('\n');
    
    
    printf(" ");
    for(int i = 0; i < width; i++){
        printf("%4d", i); ///we print the column numbers;
    }
    wprintf(L"\n  %lc", lt_corner);
    draw_top_line(width);
    wprintf(L"%lc%lc%lc\n", line, line, rt_corner);
    for( int i = 0; i < height; i++){
        wprintf(L"%2d%lc", i, vert_line);
        for(int j = 0; j < width; j++){
            wchar_t cell = board[i][j];
            if (board[i][j] == '*') cell = bomb; // we print the bombs usign unicode icons
            if(board[i][j] == 'F') cell = flag;
            if (cell == bomb) {
                wprintf(L"%1lc", cell);
            }
            else if(cell == flag){
                wprintf(L"%lc", cell);
            }
            else {
                wprintf(L"%2lc", cell);
            }
            if (j < width -1) {
                wprintf(L"%2lc", vert_line);
            }
        }
        wprintf(L"%2lc", vert_line);
        if (i == height -1) {
            wprintf(L"\n%3lc", lb_corner);
            draw_bot_line(width);
            wprintf(L"%lc%lc%lc\n", line, line, rb_corner);
        } else {
            wprintf(L"\n%3lc", l_line);
            draw_mid_line(width);
            wprintf(L"%lc%lc%lc\n", line, line, r_line);
        }
    }
    printf("\n");
}

void Gboard(char **board, int width, int height){
    for( int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            board[i][j] = 'X';
        }
    }
}
void MainBoard(char **board, int width, int height, int mines){ ///build the main board
    for( int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            board[i][j] = 'O';
        }
    }
    create_mines(height, width, board, mines);
}
void create_mines(int height, int width, char **board, int mines)///we randomly create the mines and put the on the board
{
    int randomx = 0;
    int randomy = 0;
    srand(time(0));//because for the computer is hard to make random values, usually it makes the same value without this
    
    for(int i = 0; i < mines; ){
        randomy = rand() % (height);
        randomx = rand() % (width);
        if(board[randomx][randomy] != '*'){
            board[randomx][randomy] = '*';
            i++;
        }
    }
}
void Set_game(int size, int *height, int *width, int *mine) {
    *height = 16;
    *width = 16;
    *mine = 25;
    
    //if the size is small
    if(size == 1){
        *height = 8;
        *width = 8;
        *mine = 15;
    } else if(size == 3){
        *height = 20;
        *width = 30;
        *mine = 30;
    }
}


int isMine(char **mboard, int x, int y, int height, int width) {
    if(x < 0 || y < 0 || y >= width || x >= height){
        return 0;
    }
    
    if( mboard[x][y] == '*') return 1;
    return 0;
}

char nearbyMines(char **mboard, int height,int width, int x, int y){
    if(x < 0 || y < 0 || y >= width || x >= height){
        return '*';
    }
    
    int count = 0;
    if( mboard[x][y] == '*' || mboard[x][y] == 'F' ) return '*';
    
    count += isMine(mboard, x+1, y, height, width);
    count += isMine(mboard, x, y+1, height, width);
    count += isMine(mboard, x+1, y-1, height, width);
    count += isMine(mboard, x, y - 1, height, width);
    count += isMine(mboard, x - 1, y - 1, height, width);
    count += isMine(mboard, x - 1, y + 1, height, width);
    count += isMine(mboard, x - 1, y, height, width);
    count += isMine(mboard, x + 1, y + 1, height, width);
    return count + '0';
}

void reveal(char **board,char **mboard, int width, int height, int x, int y,int *openCells){
    if(x < 0 || y < 0 || y >= width || x >= height){
        return;
    }
    
    if (board[x][y] != 'X' || board[x][y] == 'F' ) return; // if the cell is uncovered we return
    if (board[x][y] == '*') {
        return;
    }
    
    char minesCount = nearbyMines(mboard, height, width, x, y);
    board[x][y] = minesCount;
    *openCells = *openCells + 1;
    if(minesCount == '*' || minesCount != '0'){
        return;
    }
    
    reveal(board, mboard, width, height, x - 1, y + 1,openCells);
    reveal(board, mboard, width, height, x , y + 1 ,openCells);
    reveal(board, mboard, width, height, x + 1, y + 1, openCells);
    reveal(board, mboard, width, height, x + 1, y, openCells);
    reveal(board, mboard, width, height, x + 1, y - 1,openCells);
    reveal(board, mboard, width, height, x , y - 1,openCells);
    reveal(board, mboard, width, height, x - 1, y - 1, openCells);
    reveal(board, mboard, width, height, x - 1, y, openCells);
    
    return;
}

void game(char **board, char **mboard,int width, int height,int * openCells, int cells){
    while(1){
        int choice;
        int x = 0;
        int y = 0;
        // wchar_t flag = 0x1F6A9;
        //setlocale(LC_CTYPE,"UTF-8");
        printf("If you want to select a flag enter the first digit 1, if you want to select and sqare enter 0:\n ");
        scanf("%d", &choice);
        printf("Enter the coordinates you want to select\n");
        scanf("%d %d", &x, &y);
        if(choice == 1){
            if(board[x][y] == 'F'){
                mboard[x][y] = 'X';
                board[x][y] = 'X';
            }else{
                mboard[x][y] = 'F';
                board[x][y] = 'F';
            }
            
        }
        else{
            
            
            if( mboard[x][y] == '*'){
                printf("Yyu Lost the Game :/");
                break;
                return;
            }else{
                reveal(board, mboard, width, height, x, y, openCells);
            }
            
        }
        
        printBoard(mboard, height, width);
        printBoard(board, height, width);
        system("clear"); // clear on unix;
        //system("clr")// om windows;
        if(*openCells == cells){
            printf("CONGRATULATENS!!! YOU WON THIS GAME!!!!!\n");
            return;
        }
    }
}
