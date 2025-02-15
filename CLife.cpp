#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

const int sizeX = 650;
const int sizeY = 250;

void gotoxy(short x,short y) { 
	HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	COORD coord = {x,y};
	SetConsoleCursorPosition(StdOut, coord); 
}

class Life {
	private:
		int area[sizeX][sizeY];
		int updArea[sizeX][sizeY];
		int x;
		int y;
		int nearSum;
		
		int A1;
		int A2;
		int B;
		
		int bufferCounter;
		char bufferArea[sizeX*sizeX];
	public:
		Life(int A1, int A2,int B) : x(0), y(0), nearSum(0), bufferCounter(0), A1(A1), A2(A2), B(B) {}
		void resetArea() {
			for (y = 0; y < sizeY; y++) {
				for (x = 0; x < sizeX; x++) {
					area[x][y] = 0;
					updArea[x][y] = 0;
				}
			}
			
			for (int j = 0; j < sizeX*sizeY; j++){
				bufferArea[j] = ' ';
			}
		}
		
		void randomizeArea() {
			srand(time(NULL));
			for (y = 1; y < sizeY-1; y++) {
				for (x = 1; x < sizeX-1; x++) {
					if (rand() % 5 == 3) {
						area[x][y] = rand() % 2;
					}
				}
			}
		}
		
		/*
		void draw() {
			for (y = 1; y < sizeY-1; y++) {
				for (x = 1; x < sizeX-1; x++) {
					gotoxy(x,y);
					if (area[x][y] == 1) {
						printf("%c", 219);
					}
					else {
						printf(" ");
					}
				}
			}
		}
		*/
		
		void draw() {
			gotoxy(0,0);
			printf("%s", bufferArea);
		}

		void updateArea() {
			for (y = 1; y < sizeY-1; y++) {
				for (x = 1; x < sizeX-1; x++) {
					nearSum = area[x+1][y] + area[x-1][y] + area[x][y+1] + area[x][y-1] + area[x+1][y+1] + area[x-1][y-1] + area[x+1][y-1] + area[x-1][y+1];
					
					if (area[x][y] == 1 && nearSum == A1 || area[x][y] == 1 && nearSum == A2) {
						updArea[x][y] = 1;
					}
					else {
						if (area[x][y] != 0) {
							updArea[x][y] = 0;
						}
					}
					
					if (area[x][y] == 0 && nearSum == B) {
						updArea[x][y] = 1;
					}
					else {
						if (area[x][y] != 1) {
							updArea[x][y] = 0;
						}
					}
				}
			}
			
			bufferCounter = 0;
			for (y = 0; y < sizeY; y++) {
				for (x = 0; x < sizeX; x++) {
					area[x][y] = updArea[x][y];
					if (updArea[x][y] == 1){
						bufferArea[bufferCounter] = 219;
					}
					else {
						bufferArea[bufferCounter] = ' ';
					}
					bufferCounter++;
				}
			}
		}
};

int main() {
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle,&structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo( handle, &structCursorInfo );
	
	CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 2;
    cfi.dwFontSize.Y = 2;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	
	Life game(2,3,3); //s s b - rules. Default "Life" params is 2 3 3
	game.resetArea();
	game.randomizeArea();

	system("mode con cols=650 lines=250");
	system("title Life");
	
	while (true) {
		game.draw();
		game.updateArea();
		Sleep(25);
	}
	
	return 0;
}
