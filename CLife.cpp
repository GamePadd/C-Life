#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

const int sizeX = 960;
const int sizeY = 450;
const int randomChance = 5; //bigger value means a lower chance to spawn

void gotoxy(short x,short y) { 
	HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	COORD coord = {x,y};
	SetConsoleCursorPosition(StdOut, coord); 
}

struct ConsoleBuffer {
  HANDLE Console;
  DWORD BytesWritten;

  void Handle() {
    Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(Console);
    BytesWritten = 0;
  }
};

class Life {
	private:
		int** area;
		int** updArea;
		
		int x;
		int y;
		int nearSum;
		
		int A1;
		int A2;
		int B;
		
		int bufferCounter;
		char bufferArea[sizeX*sizeX];
		
		ConsoleBuffer cBuff;
	public:
		Life(int A1, int A2,int B) : x(0), y(0), nearSum(0), bufferCounter(0), A1(A1), A2(A2), B(B) {
			area = new int*[sizeY];
			updArea = new int*[sizeY];
			
			for (int i = 0; i < sizeY; i++) {
			    area[i] = new int[sizeX];
			    updArea[i] = new int[sizeX];
			}
			
			cBuff.Handle();
		}
		
		~Life() {
			for (int i = 0; i < sizeY; i++) {
			    delete[] area[i];
			    delete[] updArea[i];
			}
			
			delete[] area;
			delete[] updArea;
		}
		void resetArea() {
			for (y = 0; y < sizeY; y++) {
				for (x = 0; x < sizeX; x++) {
					area[y][x] = 0;
					updArea[y][x] = 0;
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
					if (rand() % randomChance == 1) {
						area[y][x] = rand() % 2;
					}
				}
			}
		}
		
		void draw() {
			WriteConsoleOutputCharacterA(cBuff.Console, bufferArea, sizeX * sizeY, { 0, 0 }, &cBuff.BytesWritten);
		}

		void updateArea() {
			for (y = 1; y < sizeY-1; y++) {
				for (x = 1; x < sizeX-1; x++) {
					nearSum = area[y+1][x] + area[y-1][x] + area[y][x+1] + area[y][x-1] + area[y+1][x+1] + area[y-1][x-1] + area[y+1][x-1] + area[y-1][x+1];
					
					if (area[y][x] == 1 && nearSum == A1 || area[y][x] == 1 && nearSum == A2) {
						updArea[y][x] = 1;
					}
					else {
						if (area[y][x] != 0) {
							updArea[y][x] = 0;
						}
					}
					
					if (area[y][x] == 0 && nearSum == B) {
						updArea[y][x] = 1;
					}
					else {
						if (area[y][x] != 1) {
							updArea[y][x] = 0;
						}
					}
				}
			}
			
			bufferCounter = 0;
			for (y = 0; y < sizeY; y++) {
				for (x = 0; x < sizeX; x++) {
					area[y][x] = updArea[y][x];
					if (updArea[y][x] == 1){
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
	//Do not change VVVV
	
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
    
    system("mode con cols=960 lines=450");
    system("title Life");
    
	//Do not change /\/\/\/\
	
	Life game(2,3,3); //s s b - rules. Default "Life" params is 2 3 3
	game.resetArea();
	game.randomizeArea();
	
	while (true) {
		game.draw();
		game.updateArea();
		Sleep(10);
	}
	
	return 0;
}
