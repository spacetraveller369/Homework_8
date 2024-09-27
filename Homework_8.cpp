#include <iostream>
#include <windows.h> // окрашивание консоли и работа с координатами
#include <conio.h>
using namespace std;

// enumeration - перечисление
enum KeyCodes { ENTER = 13, ESCAPE = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACEBAR = 32 };
enum Colors { DARKGREEN = 2, RED = 12, YELLOW = 14, BLUE = 9, PURPLE = 13 };
enum Objects { HALL, WALL, COIN, ENEMY, DRUGS };

// const int ENTER = 13;
// const int ESCAPE = 27;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void print_message(int x, int y, int color, string message) // сообщение которое выводится при победе или сборе монеты
{
	setlocale(0, "");
	COORD position_text;
	position_text.X = x;
	position_text.Y = y;
	SetConsoleCursorPosition(h, position_text);
	SetConsoleTextAttribute(h, color);
	cout << message << "\n";
}

void print_message_coin(int x, int y, int color, int message) // сообщение которое выводится при победе
{
	setlocale(0, "");
	COORD position_text;
	position_text.X = x;
	position_text.Y = y;
	SetConsoleCursorPosition(h, position_text);
	SetConsoleTextAttribute(h, color);
	cout << message << "\n";
}

void hide_cursor() // скрыть стандартный мигающий курсор
{
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 100;
	SetConsoleCursorInfo(h, &info);
}

const int WIDTH = 50;
const int HEIGHT = 15;
int location[HEIGHT][WIDTH] = {};

int location_gen()	// генерация локации
{
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++) // перебор столбцов
		{
			// по дефолту пишется случайное число
			location[y][x] = rand() % 4; // 0 1 2 3

			// стены по краям
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				location[y][x] = WALL;

			// вход и выход
			if (x == 0 && y == 2 || x == WIDTH - 1 && y == HEIGHT - 3)
				location[y][x] = HALL;

			if (location[y][x] == ENEMY) {
				// определяется вероятность того, останется враг или нет
				// допустим, вероястность остаться на уровне - 10%
				int prob = rand() % 10; // 0-9
				if (prob != 0) // 1 2 3 4 5 6 7 8 9
					location[y][x] = HALL;
			}
		}
	}
	return location[HEIGHT][WIDTH];
}

void location_show() // отображение локации
{
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++)
		{
			// cout << location[y][x];
			switch (location[y][x]) {
			case HALL: // коридор
				cout << " ";
				break;
			case WALL: // стена
				SetConsoleTextAttribute(h, DARKGREEN); // 0-255
				cout << (char)177; //(char)219;
				break;
			case COIN: // монетки
				SetConsoleTextAttribute(h, YELLOW);
				cout << (char)15; // 249
				break;
			case ENEMY: // враги
				SetConsoleTextAttribute(h, RED);
				cout << (char)1;
				break;
			case DRUGS: //новый тип объектов лабиринта - "лекарство"
				SetConsoleTextAttribute(h, PURPLE);
				cout << (char)211;
				break;
			default:
				cout << location[y][x];
				break;
			}
		}
		cout << "\n";
	}
}

COORD position;


void hero()	// размещение ГГ (главного героя)
{

	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, BLUE);
	cout << (char)1;
}
void hero_erase() // стирание предыдущей позиции ГГ (главного героя)
{
	SetConsoleCursorPosition(h, position);
	cout << " ";
}

int code;
int switch_code(int code)
{
	switch (code)
	{
	case ENTER:
		// cout << "ENTER\n";
		break;
	case SPACEBAR:
		// cout << "SPACEBAR\n";
		break;
	case ESCAPE:
		// cout << "ESCAPE\n";
		break;
	case RIGHT:
		if (location[position.Y][position.X + 1] != WALL)
			position.X++;
		break;
	case LEFT:
		if (position.X > 0 && location[position.Y][position.X - 1] != WALL)
			position.X--;
		break;
	case UP:
		if (location[position.Y - 1][position.X] != WALL)
			position.Y--;
		break;
	case DOWN:
		if (location[position.Y + 1][position.X] != WALL)
			position.Y++;
		break;
	default:
		cout << code << "\n";
		break;
	}
	return position.X, position.Y;
}

int coins = 0;
void hero_coins()
{
	if (location[position.Y][position.X] == COIN)
	{
		coins++;
		print_message(1, 16, 14, "COIN COLLECTED!\n");
		print_message_coin(1, 17, 14, coins);
		location[position.Y][position.X] = HALL;
	}
}

void cursor()
{
	while (true)
	{
		int code = _getch(); // функция приостанавливает работу программы, ждёт реакции пользователя
		if (code == 224)
			code = _getch(); // если первый код был 224, то повторный вызов функции вернёт только один, нормальный код
		// стирание ГГ в старой позиции
		hero_erase();

		// пользователь может нажать любую кнопку (в том числе энтер, эскейп, пробел, стрелочки), после чего вернётся код нажатой клавиши
		switch_code(code);

		// показ ГГ в новой позиции
		hero();


		// взаимодействие ГГ с другими объектами в лабиринте
		hero_coins();


		if (position.Y == HEIGHT - 3 && position.X == WIDTH - 1)
		{
			cout << "победа - найден выход" << "\n";
			break;
		}
	}
}


int main()
{

	system("title Бомбер!");

	// запуск алгоритма генерации случайных чисел
	srand(time(0));
	rand();

	// скрыть стандартный мигающий курсор
	hide_cursor();

	// генерация локации	
	location_gen();

	// показ локации
	location_show();

	// размещение ГГ (главного героя)
	position.X = 1;
	position.Y = 2;
	hero();


	// игровой движок (интерактив с пользователем)
	cursor();
}