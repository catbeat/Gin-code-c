#include<iostream>
#include<string>
#include<windows.h>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<cstdio>
#include<vector>

using namespace std;

#define strip1 0
#define strip2 1

#define square 2

#define L1 3
#define L2 4
#define L3 5
#define L4 6

#define AL1 7
#define AL2 8
#define AL3 9
#define AL4 10

#define LIGHT1 11
#define LIGHT2 12

#define ALIGHTl 13
#define ALIGHT2 14

#define T1 15
#define T2 16
#define T3 17
#define T4 18


void setPos(int x, int y)  //set the cursor to adjust the output position
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

class Tetris
{
public:
	explicit Tetris(int x = 6, int y = 1);
	void drawWall(void);        // display the out wall
	void drawShape(int id);           // display the fallings
	void eraseShape(int id);			// erase the fallings one second ago
	bool check_fall(int id);          // check whether the fallings can fall or not
	bool check_left(int id);		  // check whether the fallings can move left or not
	bool check_right(int id);		  // check whether the fallings can move right or not

	int Turn(int id);         // get the corresponding shape
	bool check_turn(int id);      // check if can turn or not
	void setting(int id);			// record the position of the fallings since it can no longer move
	
	void updateMap(void);         // to eliminate the finished row
	void printNext(int id);       // print next fallings
	void eraseNext(int id);       // erase next fallings to save space for the next next fallings
	void updateScore(void);

	bool check_top(int id);       // check whether the fallings arrive top
	void run(void);
private:
	const int shape[19][8] = {
		{0,0,0,1,0,2,0,3},     // strip type 1
		{0,0,1,0,2,0,3,0},	   // strip type 2

		{0,0,1,0,1,1,0,1},     // square

		{0,0,0,1,0,2,1,2},     // L type 1
		{0,1,0,0,1,0,2,0},     // L type 2
		{0,0,1,0,1,1,1,2},     // L type 3
		{0,1,1,1,2,1,2,0},     // L type 4

		{1,0,1,1,1,2,0,2},     // REV L type 1
		{0,0,0,1,1,1,2,1},	   // REV L type 2
		{0,0,1,0,0,1,0,2},     // REV L type 3
		{0,0,1,0,2,0,2,1},     // REV L type 4

		{0,0,0,1,1,1,1,2},     // lightning type 1
		{0,1,1,1,1,0,2,0},     // lightning type 2

		{1,0,1,1,0,1,0,2},     // REV lightning type 1
		{0,0,1,0,1,1,2,1},     // REV lightning type 2

		{0,1,1,1,2,1,1,0},     // T type 1
		{0,0,0,1,0,2,1,1},     // T type 2
		{0,0,1,0,2,0,1,1},     // T type 3
		{1,0,1,1,1,2,0,1}      // T type 4
	};
	int fallPoint[2];            // point to control where to draw the fallings
	int recordMap[18][24] = { 0 };
	int score = 0;
	bool gameStatus = true;
};

Tetris::Tetris(int x, int y)
{
	fallPoint[0] = x;
	fallPoint[1] = y;
}

void Tetris::drawShape(int id)
{
	for (int i = 0; i < 4; ++i) {
		setPos(fallPoint[0] + 2*shape[id][i * 2], fallPoint[1] + shape[id][i * 2 + 1]);
		cout << "¡ö";
	}
}

void Tetris::eraseShape(int id)
{
	for (int i = 0; i < 4; ++i) {
		setPos(fallPoint[0] + 2 * shape[id][i * 2], fallPoint[1] + shape[id][i * 2 + 1]);
		cout << " ";
	}
}

void Tetris::drawWall(void) 
{
	for (int i = 0; i < 28; ++i) {
		setPos(i * 2, 0);
		cout << "¡ö";
	}

	for (int i = 1; i <= 24; ++i) {
		setPos(0, i);
		cout << "¡ö";
	}

	for (int i = 1; i <= 24; ++i) {
		setPos(38, i);
		cout << "¡ö";
	}

	for (int i = 1; i <= 24; ++i) {
		setPos(54, i);
		cout << "¡ö";
	}

	for (int i = 0; i < 28; ++i) {
		setPos(i * 2, 25);
		cout << "¡ö";
	}
}

bool Tetris::check_fall(int id)
{
	int nx = fallPoint[0];
	int ny = fallPoint[1]+1;
	for (int i = 0; i < 4; ++i) {
		int newx = nx + 2 * shape[id][2 * i];
		int newy = ny + shape[id][i * 2 + 1];
		if ((recordMap[newx/2-1][newy-1] == 1) or (newy > 24)) {
			return false;
		}
	}
	return true;
}

bool Tetris::check_left(int id)
{
	int nx = fallPoint[0] - 2;
	int ny = fallPoint[1];
	for (int i = 0; i < 4; ++i) {
		int newx = nx + 2 * shape[id][2 * i];
		int newy = ny + shape[id][i * 2 + 1];
		if ((recordMap[newx / 2 - 1][newy - 1] == 1) or (newx < 2)) {
			return false;
		}
	}
	return true;
}

bool Tetris::check_right(int id)
{
	int nx = fallPoint[0] + 2;
	int ny = fallPoint[1];
	for (int i = 0; i < 4; ++i) {
		int newx = nx + 2 * shape[id][2 * i];
		int newy = ny + shape[id][i * 2 + 1];
		if ((recordMap[newx / 2 - 1][newy - 1] == 1) or (newx > 36)) {
			return false;
		}
	}
	return true;
}

int Tetris::Turn(int id)
{
	switch (id)
	{
	case strip1:
		return (id = strip2);
	case strip2:
		id = strip1;
		return id;

	case square:
		return id;

	case L1:
		return (id = L2);
	case L2:
		return (id = L3);
	case L3:
		return (id = L4);
	case L4:
		return (id = L1);

	case AL1:
		return (id = AL2);
	case AL2:
		return (id = AL3);
	case AL3:
		return (id = AL4);
	case AL4:
		return (id = AL1);

	case LIGHT1:
		return (id = LIGHT2);
	case LIGHT2:
		return (id = LIGHT1);

	case ALIGHTl:
		return (id = ALIGHT2);
	case ALIGHT2:
		return (id = ALIGHTl);


	case T1:
		return (id = T2);
	case T2:
		return (id = T3);
	case T3:
		return (id = T4);
	case T4:
		return (id = T1);
	}
}

bool Tetris::check_turn(int id)
{
	int nx = fallPoint[0];
	int ny = fallPoint[1];

	for (int i = 0; i < 4; ++i) {
		int newx = nx + 2 * shape[id][2 * i];
		int newy = ny + shape[id][i * 2 + 1];
		if ((recordMap[newx / 2 - 1][newy - 1] == 1) or (newx > 36) or (newx < 2) or (newy > 24) or (newy < 1)) {
			return false;
		}
	}
	return true;
}

void Tetris::setting(int id)
{
	for (int i = 0; i < 4; ++i) {
		int nx = (fallPoint[0] + 2 * shape[id][i * 2]) / 2-1;
		int ny = fallPoint[1] + shape[id][i * 2 + 1]-1;
		recordMap[nx][ny] = 1;
	}
}

void Tetris::printNext(int id)
{
	for (int i = 0; i < 4; ++i) {
		setPos(43 + 2 * shape[id][i * 2], 4 + shape[id][i * 2 + 1]);
		cout << "¡ö";
	}
}

void Tetris::eraseNext(int id)
{
	for (int i = 0; i < 4; ++i) {
		setPos(43 + 2 * shape[id][i * 2], 4 + shape[id][i * 2 + 1]);
		cout << " ";
	}
}

void Tetris::updateScore(void)
{
	setPos(42, 12);
	cout << "    ";
	setPos(42, 12);
	cout << " " << to_string(score);
	
}

void Tetris::updateMap(void)
{
	bool flag;
	vector<int> finish_row;
	
	for (size_t row = 0; row < 24; ++row) {
		for (size_t col = 0; col < 18; ++col) {
			if (recordMap[col][row] == 0) {
				flag = false;
				break;
			}
			flag = true;
		}

		if (flag == true) {
			finish_row.push_back(row);
		}
	}

	if (finish_row.size() != 0) {
		for (int x = 0; x < 18; ++x) {
			for (int y = 0; y < 24; ++y) {
				if (recordMap[x][y] == 1) {
					int new_x = (x + 1) * 2;
					int new_y = (y + 1);
					setPos(new_x, new_y);
					cout << " ";
				}
			}
		}


		for (auto row : finish_row) {
			for (int i = row; i > 0; --i) {
				for (int x = 0; x < 18; ++x) {
					recordMap[x][i] = recordMap[x][i-1];
				}
			}
		}

		for (int x = 0; x < 18; ++x) {
			for (int y = 0; y < 24; ++y) {
				if (recordMap[x][y] == 1) {
					int new_x = (x + 1) * 2;
					int new_y = (y + 1);
					setPos(new_x, new_y);
					cout << "¡ö";
				}
			}
		}

		score += 10 * finish_row.size();
		updateScore();
	}

	
}

bool Tetris::check_top(int id)
{
	for (int i = 0; i < 4; ++i) {
		int nx = (fallPoint[0] + 2 * shape[id][i * 2]) / 2 - 1;
		int ny = fallPoint[1] + shape[id][i * 2 + 1] - 1;
		if (recordMap[nx][ny] == 1) {
			return true;
		}
	}
	return false;

}

void Tetris::run(void)
{
	drawWall();
	setPos(42, 2);
	cout << "Next :";
	setPos(42, 10);
	cout << "Score :";
	setPos(42, 12);
	cout << " 0 ";
	int next_id = rand() % 19;
	while (gameStatus == true) {
		int id = next_id;
		next_id = rand() % 19;

		int time_cot = 0;
		printNext(next_id);
		while (gameStatus == true) {
			if (check_top(id)) {
				gameStatus = false;
				break;
			}
			drawShape(id);
			if (time_cot > 150) {
				time_cot = 0;

				if (check_fall(id)) {
					eraseShape(id);
					fallPoint[1] += 1;
				}
				else {
					setting(id);
					fallPoint[0] = 6;
					fallPoint[1] = 1;
					updateMap();
					eraseNext(next_id);
					break;
				}
			}

			if (_kbhit())
			{
				int key, key2;
				key = _getch();
				if (key == 224) {
					key2 = _getch();
					if (key2 == 75) {// left towards
						if (check_left(id)) {
							eraseShape(id);
							fallPoint[0] -= 2;
							drawShape(id);
						}
					}
					else if (key2 == 77) { // right towards
						if (check_right(id)) {
							eraseShape(id);
							fallPoint[0] += 2;
							drawShape(id);
						}
					}
					else if (key2 == 80) {   // down towards
						if (check_fall(id)) {
							eraseShape(id);
							fallPoint[1] += 1;
							drawShape(id);
						}
					}
					else if (key2 == 72) {
						int new_id = Turn(id);
						if (check_turn(new_id)) {
							eraseShape(id);
							id = new_id;
							drawShape(id);
						}
					}
				}
			}
			Sleep(1);
			time_cot += 1;
			
		}
	}
	system("cls");
	cout << "gameover";
}

int main(void)
{
	srand(time(NULL));
	Tetris main_game;
	main_game.run();
}