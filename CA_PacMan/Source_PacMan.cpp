//---------------------------Опис--------------------------------------
/*
Гра з одним рівнем
персонажі:
 - пакман
 - блінкі
 - пінкі
 - інкі
 - клайд
можливості:
 - пакман має три життя
 - присутні 4 енерджайзери (по 500 поінтів)
 - три фрукти (по 1000 поінтів)
 - точка (по 5 поінтів)
Рух:
 -Стрілками (вверх, вниз, вліво, вправо)
*/

//---------------------Налаштування------------------------------------
//Файл "logo_pacman.txt" повинен бути у папці з проектом або екзешником
//Файл "scores.txt" повинен бути у папці з проектом або екзешником
//Файли "map_level_*.txt" повинен бути у папці з проектом або екзешником
//---------------------------------------------------------------------
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <direct.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <iomanip>

using namespace std;

//-------------------------------------------------------------------------------------------------
//Елементи налаштувань
const int MAP_SIZE_COL = 43; //кількість стовпців в карті
const int MAP_SIZE_ROW = 29; //кількість рядків в карті
//Пакман
const int StartX_map = 56; //Стартові координати для екрану
const int StartY_map = 27;
const int StartX_array = 24; //Стартові координати для масиву
const int StartY_array = 27;

const int GHOST_SPEED = 100;//100; //Швидкість привидів
const int PACMAN_SPEED = 5; //Швидкість пакмана

const int PACMAN_LIVES = 3; //Кількість життів у Пакмана
//Blinky
const int StartX_map_Blinky = 50; //Стартові координати для екрану
const int StartY_map_Blinky = 11;
const int StartX_array_Blinky = 18; //Стартові координати для масиву
const int StartY_array_Blinky = 11;
//Pinky
const int StartX_map_Pinky = 48; //Стартові координати для екрану
const int StartY_map_Pinky = 13;
const int StartX_array_Pinky = 16; //Стартові координати для масиву
const int StartY_array_Pinky = 13;
//Inky
const int StartX_map_Inky = 50; //Стартові координати для екрану
const int StartY_map_Inky = 13;
const int StartX_array_Inky = 18; //Стартові координати для масиву
const int StartY_array_Inky = 13;
//Clyde
const int StartX_map_Clyde = 54; //Стартові координати для екрану
const int StartY_map_Clyde = 13;
const int StartX_array_Clyde = 22; //Стартові координати для масиву
const int StartY_array_Clyde = 13;

//Кількість точок 
int const DOTS = 382;

//-------------------------------------------------------------------------------------------------
//Додакові функції
void LoadLogoScreen(); //Завантажує заставку Пак_ман
//MENU function-----------------------------------
int choise_menu(); //Вибір пункту меню (вертає номер пункту)
//Для роботи з кольором---------------------------
void SetColor(int text, int background); //Колір тесту і фону
void SetTextColor(int color); //Колір тексту
//Позиція курсора в консолі-----------------------
void SetPos(int x, int y);
//-------------------------------------------------------------------------------------------------

//-----------ФУНКЦІЇ ДЛЯ РОБОТИ ЗКОЛЬОРОМ В СКОНСОЛІ-----------------------------------------------
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

//Колір тексту------------------------------------
void SetTextColor(int color)
{
	HANDLE hcom = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcom, color);
}
//-------------------------------------------------------------------------------------------------

//Позиція курсора в консолі-----------------------
void SetPos(int x, int y)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(h, coord);
}
//-------------------------------------------------------------------------------------------------

void LoadLogoScreen() //Завантажує заставку Пак_ман
{
	system("cls");

	char current_work_dir[FILENAME_MAX];
	_getcwd(current_work_dir, sizeof(current_work_dir)); //директорія екзешніка

	string filename = "\\logo_pacman";
	string fileformat = ".txt";
	string allpath = current_work_dir + filename + fileformat;

	ifstream file(allpath);

	try
	{
		if (!file.is_open())
		{
			throw "File not opened!";
		}

		char buf[255];
		SetTextColor(Yellow);
		cout << endl;
		while (!file.eof())
		{
			file.getline(buf, sizeof(buf));
			cout << "\t\t\t\t\t" << buf << endl;
			if (file.eof()) break; // return;
		}
		cout << "\t PAC-MAN" << endl;
		SetTextColor(LightGray);
		file.close();
	}
	catch (char* ex)
	{
		cout << ex << endl;
		file.close();
		return;
	}
}

//-------------------------------------------------------------------------------------------------
//Клас ігрового поля
class Game_field
{
private:
	char field_array[MAP_SIZE_ROW][MAP_SIZE_COL]; //ігрове поле
public:
	Game_field()
	{ }

	void LoadMapField(int row, int col, char value)
	{
		this->field_array[row][col] = value;
	}

	char GetObj(int row, int col) const
	{
		return this->field_array[row][col];
	}

	void SetObj_Null(int row, int col)
	{
		this->field_array[row][col] = '0';
	}

	void ShowAllMap()
	{
		for (size_t i = 0; i < MAP_SIZE_ROW; i++)
		{
			for (size_t j = 0; j < MAP_SIZE_COL; j++)
			{
				cout << this->field_array[i][j] << " ";
			}
			cout << endl;
		}
	}

	~Game_field()
	{ }
};
//-------------------------------------------------------------------------------------------------
class Pacman;
//Клас виведення інфорації про стан гри
class info
{
private:
	static int _Score;
	int _HighScore;
	int _level;
	int _energy;
	int _fruits;
	int _pacman_lives;
	info() {};
public:
	void Score();
	void AddToScore(int bonus);
	void Energy();
	void Fruits();
	void Pacman_lives();
	int Get_Pacman_Lives() const;
	void Show();
	void Clear();
	void SetHightScore(int hscore);
	int GetScore() const;
	friend Pacman;
};

int info:: _Score = 0;

void info::Score()
{
	this->_Score += 5;
}

void info::AddToScore(int bonus)
{
	this->_Score += bonus;
}

void info::Energy()
{
	this->_energy--;
}

void info::Fruits()
{
	this->_fruits--;
}

void info::Pacman_lives()
{
	this->_pacman_lives--;
}

int info::Get_Pacman_Lives() const
{
	return _pacman_lives;
}

void info::Show()
{
	Clear();
	SetPos(8, 3);
	SetTextColor(LightMagenta);
	printf("LEVEL:      %i", _level);
	SetPos(8, 5);
	SetTextColor(LightGreen);
	printf("SCORE:      %i", _Score);
	SetTextColor(LightBlue);
	SetPos(8, 7);
	printf("HIGH SCORE: %i", _HighScore);

	SetTextColor(LightCyan);
	SetPos(8, 9);
	printf("Energy:     ");
	for (size_t i = 0; i < _energy; i++)
	{
		cout << char(254) << " ";
	}

	SetTextColor(LightCyan);
	SetPos(8, 11);
	printf("Fruits:     ");
	for (size_t i = 0; i < _fruits; i++)
	{
		cout << char(253) << " ";
	}

	SetPos(8, 13);
	SetTextColor(LightRed);
	printf("LIVES:      ");
	for (size_t i = 0; i < _pacman_lives; i++)
	{
		cout << char(3) << " " ;
	}
	SetTextColor(LightGray);
}

void info:: Clear()
{
	SetPos(8, 3);
	cout << ("                    ");
	SetPos(8, 5);
	cout << ("                    ");
	SetPos(8, 7);
	cout << ("                    ");
	SetPos(8, 9);
	cout << ("                    ");
	SetPos(8, 11);
	cout << ("                    ");
	SetPos(8, 13);
	cout << ("                    ");
}

void info:: SetHightScore(int hscore)
{
	this->_HighScore = hscore;
}

int info:: GetScore() const
{
	return this->_Score;
}

//-------------------------------------------------------------------------------------------------

class Sound
{
public:
	//Pacman Sound
	void Pacman_Walk();
	void Pacman_Eat_Point();
	void Pacman_Eat_Energazer();
	void Pacman_Eat_Fruits();
	void Pacman_Eat_Ghost();
	//Ghost
	void Ghost_Eat_Pacman();
	//Music
	void Game_Music();
	//Menu
	void Menu_Choise();
	void Menu_Move();
};

void Sound:: Pacman_Walk()
{
	Beep(5000, 2);
}

void Sound:: Pacman_Eat_Point()
{
	Beep(800, 2);
	//MessageBeep(MB_ICONINFORMATION);
	//MessageBeep(MB_ICONASTERISK);
	//MessageBeep(MB_ICONEXCLAMATION);
	//MessageBeep(MB_ICONERROR);
	//MessageBeep(MB_ICONHAND);
	//MessageBeep(MB_ICONINFORMATION);
	//MessageBeep(MB_ICONQUESTION);
	//MessageBeep(MB_ICONSTOP);
	//MessageBeep(MB_ICONWARNING);
	//MessageBeep(MB_OK);
}
void Sound:: Pacman_Eat_Energazer()
{
	Beep(8000, 200);
}
void Sound:: Pacman_Eat_Fruits()
{
	Beep(7000, 400);
}
void Sound:: Pacman_Eat_Ghost()
{
	Beep(90000, 500);
	Beep(500, 300);
}
void Sound:: Ghost_Eat_Pacman()
{
	Beep(900, 400);
	Beep(600, 300);
	Beep(400, 200);
	Beep(100, 100);
}
void Sound:: Game_Music()
{
	Beep(1480, 200);
	Beep(1568, 200);
	Beep(1568, 200);
	Beep(1568, 200);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(783.99, 200);
	Beep(783.99, 200);
	Beep(369.99, 200);
	Beep(392, 200);
	Beep(369.99, 200);
	Beep(392, 200);
	Beep(392, 400);
	Beep(196, 400);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(783.99, 200);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(783.99, 200);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(880, 200);
	Beep(830.61, 200);
	Beep(880, 200);
	Beep(987.77, 400);
	Beep(880, 200);
	Beep(783.99, 200);
	Beep(698.46, 200);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(783.99, 200);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(783.99, 200);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(880, 200);
	Beep(830.61, 200);
	Beep(880, 200);
	Beep(987.77, 400);
	Sleep(200);
	Beep(1108, 10);
	Beep(1174.7, 200);
	Beep(1480, 10);
	Beep(1568, 200);
	Sleep(200);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(783.99, 200);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(783.99, 200);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(880, 200);
	Beep(830.61, 200);
	Beep(880, 200);
	Beep(987.77, 400);
	Beep(880, 200);
	Beep(783.99, 200);
	Beep(698.46, 200);
	Beep(659.25, 200);
	Beep(698.46, 200);
	Beep(784, 200);
	Beep(880, 400);
	Beep(784, 200);
	Beep(698.46, 200);
	Beep(659.25, 200);
	Beep(587.33, 200);
	Beep(659.25, 200);
	Beep(698.46, 200);
	Beep(784, 400);
	Beep(698.46, 200);
	Beep(659.25, 200);
	Beep(587.33, 200);
	Beep(523.25, 200);
	Beep(587.33, 200);
	Beep(659.25, 200);
	Beep(698.46, 400);
	Beep(659.25, 200);
	Beep(587.33, 200);
	Beep(493.88, 200);
	Beep(523.25, 200);
	Sleep(400);
	Beep(349.23, 400);
	Beep(392, 200);
	Beep(329.63, 200);
	Beep(523.25, 200);
	Beep(493.88, 200);
	Beep(466.16, 200);
	Beep(440, 200);
	Beep(493.88, 200);
	Beep(523.25, 200);
	Beep(880, 200);
	Beep(493.88, 200);
	Beep(880, 200);
	Beep(1760, 200);
	Beep(440, 200);
	Beep(392, 200);
	Beep(440, 200);
	Beep(493.88, 200);
	Beep(783.99, 200);
	Beep(440, 200);
	Beep(783.99, 200);
	Beep(1568, 200);
	Beep(392, 200);
	Beep(349.23, 200);
	Beep(392, 200);
	Beep(440, 200);
	Beep(698.46, 200);
	Beep(415.2, 200);
	Beep(698.46, 200);
	Beep(1396.92, 200);
	Beep(349.23, 200);
	Beep(329.63, 200);
	Beep(311.13, 200);
	Beep(329.63, 200);
	Beep(659.25, 200);
	Beep(698.46, 400);
	Beep(783.99, 400);
	Beep(440, 200);
	Beep(493.88, 200);
	Beep(523.25, 200);
	Beep(880, 200);
	Beep(493.88, 200);
	Beep(880, 200);
	Beep(1760, 200);
	Beep(440, 200);
	Beep(392, 200);
	Beep(440, 200);
	Beep(493.88, 200);
	Beep(783.99, 200);
	Beep(440, 200);
	Beep(783.99, 200);
	Beep(1568, 200);
	Beep(392, 200);
	Beep(349.23, 200);
	Beep(392, 200);
	Beep(440, 200);
	Beep(698.46, 200);
	Beep(659.25, 200);
	Beep(698.46, 200);
	Beep(739.99, 200);
	Beep(783.99, 200);
	Beep(392, 200);
	Beep(392, 200);
	Beep(392, 200);
	Beep(392, 200);
	Beep(196, 200);
	Beep(196, 200);
	Beep(196, 200);
	Beep(185, 200);
	Beep(196, 200);
	Beep(185, 200);
	Beep(196, 200);
	Beep(207.65, 200);
	Beep(220, 200);
	Beep(233.08, 200);
	Beep(246.94, 200);
}

void Sound:: Menu_Choise()
{
	Beep(1000, 200);
}

void Sound:: Menu_Move()
{
	Beep(300, 100);
}

//-------------------------------------------------------------------------------------------------

//Вибір пункту меню (вертає номер пункту)--------
int choise_menu()
{
	char menu;
	int COUNTER = 0;

	Sound menu_sound;

	char menu_main[4][255];

	SetPos(47, 23);

	strcpy(menu_main[0], " >>> START GAME <<<\n");
	strcpy(menu_main[1], "   >>> SCORES <<<\n");
	strcpy(menu_main[2], "   >>> CREDITS <<<\n");
	strcpy(menu_main[3], "    >>> QUIT <<<\n");

	menu = -1;
	while (menu != char(13))//цикл для руху по меню.
	{
		menu_sound.Menu_Move();
		if (menu == char(224))
		{
			switch (_getch())
			{
			case(80):
			{
				COUNTER++;
				if (COUNTER > 3)COUNTER = 0;
				break;
			}
			case(72):
			{
				COUNTER--;
				if (COUNTER < 0)COUNTER = 3;
				break;
			}
			}
		}

		SetPos(47, 23);

		for (int i = 0; i < 4; i++)//виділяє кольором вибір
		{
			if (COUNTER == i)
			{
				SetColor(9, 0);
			}
			SetPos(47, 23 + i);
			cout << menu_main[i];
			SetColor(7, 0);
		}
		menu = _getch();
	}
	return COUNTER;
}
//-------------------------------------------------------------------------------------------------

//базовий клас для персонажів у грі
class Person
{
protected:
	COORD ObjectPos_map;
	COORD ObjectPos_array;

	int Direction;
	int Speed;
	int _color;
	char _face;
public:

	virtual void Move(int Speed, Game_field &obj) = 0;
	void DrawObject();
	void CleanPos();
	void CleanPosGhost(Game_field &obj);
	void SetDirection(char direction);
};

void Person::DrawObject()
{
	SetTextColor(_color);
	SetPos(ObjectPos_map.X, ObjectPos_map.Y);
	cout << _face;
	SetTextColor(7);
}

void Person::CleanPos()
{
	SetPos(ObjectPos_map.X, ObjectPos_map.Y);
	cout << " ";
}

void Person::CleanPosGhost(Game_field &obj)
{
	if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X) == '1')
	{
		SetPos(ObjectPos_map.X, ObjectPos_map.Y);
		SetTextColor(Yellow);
		cout << char(249);
		SetTextColor(LightGray);
	}
	else if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X) == '8')
	{
		SetPos(ObjectPos_map.X, ObjectPos_map.Y);
		SetTextColor(LightCyan);
		cout << char(254);
		SetTextColor(LightGray);
	}
	else if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X) == '9')
	{
		SetPos(ObjectPos_map.X, ObjectPos_map.Y);
		SetTextColor(LightMagenta);
		cout << char(253);
		SetTextColor(LightGray);
	}
	else CleanPos();
}

void Person::SetDirection(char direction)
{
	this->Direction = direction;
}

//-------------------------------------------------------------------------------------------------

//клас персонажа Pac-man
class Pacman :public Person
{
private:
	int lives; //кількість життів
	int dots; //кількість зідених точок
public:
	Pacman();
	void Move(int Speed, Game_field &obj) override;
	void Run(int Speed, Game_field &obj);
	COORD GetPosition();
	int GetLives() const;
	int GetCountDots() const;
	void ChangeLife();
	~Pacman();
	info *Info = new info;
	Sound *sound = new Sound;
};


Pacman::Pacman()
{
	//Життя Пакмана
	lives = PACMAN_LIVES;

	//Ініціалізація параметру кількість зідених точок
	dots = 0;

	//Координати стартові Пакмана на екрані
	ObjectPos_map.X = StartX_map;
	ObjectPos_map.Y = StartY_map;

	//Координати стартові Пакмана в масиві карти
	ObjectPos_array.X = StartX_array;
	ObjectPos_array.Y = StartY_array;

	//Колір та фігурка Пакмана
	_color = Yellow;
	_face = '>';// char(1);

	DrawObject();

	//Початкова інформація для класу інформації
	Info->_HighScore = 0;
	Info->_level = 1;
	Info->_pacman_lives = lives;
	Info->_Score = 0;
	Info->_energy = 4;
	Info->_fruits = 3;
	Info->Show();
}

void Pacman:: Move(int Speed, Game_field &obj)
{
	CleanPos();
	enum arrow { left = 75, right = 77, up = 72, down = 80 };

	if (Direction == left)
	{
		_face = '>';
		if ((obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X) == '0') && (ObjectPos_array.X == 0))
		{

			ObjectPos_array.X = MAP_SIZE_COL - 1;
			ObjectPos_map.X = 32 + MAP_SIZE_COL - 1;
			Info->Score();
			sound->Pacman_Walk();
		}
		else if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) == '0')
		{
			ObjectPos_array.X--;
			ObjectPos_map.X--;
			sound->Pacman_Walk();
		}
		else if(obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) == '1')
		{
			ObjectPos_array.X--;
			ObjectPos_map.X--;
			dots++;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Point();
			Info->Score();
		}
		else if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) == '9')
		{
			ObjectPos_array.X--;
			ObjectPos_map.X--;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Fruits();
			Info->AddToScore(1000);
			Info->Fruits();
		}
		else if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) == '8')
		{
			ObjectPos_array.X--;
			ObjectPos_map.X--;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Energazer();
			Info->AddToScore(500);
			Info->Energy();
		}

	}

	if (Direction == right)
	{
		_face = '<';
		if ((obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X) == '0') && (ObjectPos_array.X == (MAP_SIZE_COL - 1)))
		{
			ObjectPos_array.X = 0;
			ObjectPos_map.X = 32;
			Info->Score();
			sound->Pacman_Walk();
		}
		else if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) == '0')
		{
			ObjectPos_array.X++;
			ObjectPos_map.X++;
			sound->Pacman_Walk();
		}
		else if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) == '1')
		{
			ObjectPos_array.X++;
			ObjectPos_map.X++;
			dots++;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Point();
			Info->Score();
		}
		else if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) == '9')
		{
			ObjectPos_array.X++;
			ObjectPos_map.X++;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Fruits();
			Info->AddToScore(1000);
			Info->Fruits();
		}
		else if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) == '8')
		{
			ObjectPos_array.X++;
			ObjectPos_map.X++;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Energazer();
			Info->AddToScore(500);
			Info->Energy();
		}
	}

	if (Direction == up)
	{
		_face = 'v';
		if (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) == '0')
		{
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			sound->Pacman_Walk();
		}
		else if (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) == '1')
		{
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			dots++;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Point();
			Info->Score();
		}
		else if (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) == '9')
		{
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Fruits();
			Info->AddToScore(1000);
			Info->Fruits();
		}
		else if (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) == '8')
		{
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Energazer();
			Info->AddToScore(500);
			Info->Energy();
		}

	}

	if (Direction == down)
	{
		_face = '^';
		if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) == '0')
		{
			ObjectPos_array.Y++;
			ObjectPos_map.Y++;
			sound->Pacman_Walk();
		}
		else if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) == '1')
		{
			ObjectPos_array.Y++;
			ObjectPos_map.Y++;
			dots++;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Point();
			Info->Score();
		}
		else if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) == '9')
		{
			ObjectPos_array.Y++;
			ObjectPos_map.Y++;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Fruits();
			Info->AddToScore(1000);
			Info->Fruits();
		}
		else if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) == '8')
		{
			ObjectPos_array.Y++;
			ObjectPos_map.Y++;
			obj.SetObj_Null(ObjectPos_array.Y, ObjectPos_array.X);
			sound->Pacman_Eat_Energazer();
			Info->AddToScore(500);
			Info->Energy();
		}
	}

	Info->Show();
	DrawObject();	
}

void Pacman::Run(int Speed, Game_field &obj)
{
	if (clock() % Speed)// accept move by tick
		return;

	srand(clock());
	if (_kbhit()) {
		//Гра пакманом
		char key = ' ';
			_getch();
			key = _getch();
			SetDirection(key);
			Move(Speed, obj);
	}
	else return;
}

COORD Pacman:: GetPosition()
{
	return this->ObjectPos_array;
}

int Pacman:: GetLives() const
{
	return lives;
}

int Pacman:: GetCountDots() const
{
	return dots;
}

void Pacman:: ChangeLife()
{
	lives--;
	Info->_pacman_lives = lives;

	Sleep(500);

	//Координати стартові Пакмана на екрані
	ObjectPos_map.X = StartX_map;
	ObjectPos_map.Y = StartY_map;

	//Координати стартові Пакмана в масиві карти
	ObjectPos_array.X = StartX_array;
	ObjectPos_array.Y = StartY_array;

	DrawObject();
}

Pacman::~Pacman()
{
	delete Info;
	delete sound;
}
//-------------------------------------------------------------------------------------------------
class Ghost :public Person
{
public:
	Ghost();
	virtual void Move(int Speed, Game_field &obj);
	void CheckPosition(Pacman &pacman);
	void Die();
	~Ghost();
	Sound *sounds = new Sound;
};

Ghost::Ghost()
{
	////Координати стартові Блінкі на екрані
	//ObjectPos_map.X = StartX_map_Blinki;
	//ObjectPos_map.Y = StartY_map_Blinki;

	////Координати стартові Блінкі в масиві карти
	//ObjectPos_array.X = StartX_array_Blinki;
	//ObjectPos_array.Y = StartY_array_Blinki;

	////Колір та фігурка Blinki
	//_color = LightRed;
	//_face = char(1);

	//DrawObject();
}

void Ghost::Move(int Speed,Game_field &obj)
{
	if (clock() % Speed)// accept move by tick
		return;
	
	srand(clock());

	enum arrow1 { UP_DIR = 1, RIGHT_DIR=2, DOWN_DIR=3, LEFT_DIR=4 };
	static int direction;
	int temp_dir;
	static bool freemove = true;
	static bool first_use = true;

	//Коли привиди в кімнаті (виходимо)  ворота 21 або 53
	if (ObjectPos_array.Y == 13 && ObjectPos_array.X >= 15 && ObjectPos_array.X <= 27)
	{
		if (ObjectPos_array.X < 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.X++;
			ObjectPos_map.X++;
			DrawObject();
		}
		if (ObjectPos_array.X > 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.X--;
			ObjectPos_map.X--;
			DrawObject();
		}
		if (ObjectPos_array.X == 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			DrawObject();

			CleanPosGhost(obj);
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			DrawObject();

			int temp = rand() % 2;
			if (temp = 1)
			{
				CleanPosGhost(obj);
				ObjectPos_array.X--;
				ObjectPos_map.X--;
				DrawObject();
			}
			if (temp = 0)
			{
				CleanPosGhost(obj);
				ObjectPos_array.X++;
				ObjectPos_map.X++;
				DrawObject();
			}
		}
		return;
	}

	if (first_use) {
		direction = 1 + rand() % (5 - 1);
		first_use = false;
	}
	//do {
		if (!freemove) {//if unite blocked, cycle don't work
			if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '0' &&
				obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '0' &&
				obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '0' &&
				obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '0' &&
				obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '1' &&
				obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '1' &&
				obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '1' &&
				obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '1' &&
				obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '8' &&
				obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '8' &&
				obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '8' &&
				obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '8' &&
				obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '9' &&
				obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '9' &&
				obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '9' &&
				obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '9')
			{
				return;
			}

			temp_dir = direction;
			do {
				direction = 1 + rand() % (5 - 1);
			} while (temp_dir == direction);
			freemove = true;
		}
		switch (direction) 
		{
		case UP_DIR:
		{
			if ((obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '0') && (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '1') &&
				(obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '8') && (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '9')) {
				freemove = false;
				break;
			}
			CleanPosGhost(obj);
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			DrawObject();
			break;
		}
		case RIGHT_DIR: 
		{
			if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '0' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '1' &&
				obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '8' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '9') {
				freemove = false;
				break;
			}
			CleanPosGhost(obj);
			ObjectPos_array.X++;
			ObjectPos_map.X++;
			DrawObject();
			break;
		}
		case DOWN_DIR:
		{
			if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '0' && obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '1' &&
				obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '8' && obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '9') {
				freemove = false;
				break;
			}
			CleanPosGhost(obj);
			ObjectPos_array.Y++;
			ObjectPos_map.Y++;
			DrawObject();
			break;
		}
		case LEFT_DIR: 
		{
			if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '0' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '1' &&
				obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '8' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '9') {
				freemove = false;
				break;
			}
			CleanPosGhost(obj);
			ObjectPos_array.X--;
			ObjectPos_map.X--;
			DrawObject();
			break;
		}
		/*default:
		{
			continue;
		}*/
		}
		//Sleep(15);
	//} while (!freemove);
}

void Ghost:: CheckPosition(Pacman &pacman)
{
	if (pacman.GetPosition().X == ObjectPos_array.X && pacman.GetPosition().Y == ObjectPos_array.Y)
	{
		sounds->Ghost_Eat_Pacman();
		pacman.ChangeLife();
	}
}

void Ghost:: Die()
{
	sounds->Pacman_Eat_Ghost();
	Sleep(500);

	////Координати стартові Блінкі на екрані
	//ObjectPos_map.X = StartX_map_Blinki;
	//ObjectPos_map.Y = StartY_map_Blinki;

	////Координати стартові Блінкі в масиві карти
	//ObjectPos_array.X = StartX_array_Blinki;
	//ObjectPos_array.Y = StartY_array_Blinki;

	DrawObject();
}

Ghost::~Ghost()
{
	delete sounds;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class Blinky : public Ghost
{
public:
	Blinky();
};

Blinky::Blinky()
{
	//Координати стартові Блінкі на екрані
	ObjectPos_map.X = StartX_map_Blinky;
	ObjectPos_map.Y = StartY_map_Blinky;

	//Координати стартові Блінкі в масиві карти
	ObjectPos_array.X = StartX_array_Blinky;
	ObjectPos_array.Y = StartY_array_Blinky;

	//Колір та фігурка Blinki
	_color = LightRed;
	_face = char(1);

	DrawObject();
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class Pinky : public Ghost
{
public:
	Pinky();
	void Move(int Speed, Game_field &obj);
};

Pinky::Pinky()
{
	//Координати стартові Pinki на екрані
	ObjectPos_map.X = StartX_map_Pinky;
	ObjectPos_map.Y = StartY_map_Pinky;

	//Координати стартові Pinki в масиві карти
	ObjectPos_array.X = StartX_array_Pinky;
	ObjectPos_array.Y = StartY_array_Pinky;

	//Колір та фігурка Blinki
	_color = LightMagenta;
	_face = char(1);

	DrawObject();
}

void Pinky::Move(int Speed, Game_field &obj)
{
	if (clock() % Speed)// accept move by tick
		return;

	srand(clock());

	enum arrow1 { UP_DIR = 1, RIGHT_DIR = 2, DOWN_DIR = 3, LEFT_DIR = 4 };
	static int direction;
	int temp_dir;
	static bool freemove = true;
	static bool first_use = true;

	//Коли привиди в кімнаті (виходимо)  ворота 21 або 53
	if (ObjectPos_array.Y == 13 && ObjectPos_array.X >= 15 && ObjectPos_array.X <= 27)
	{
		if (ObjectPos_array.X < 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.X++;
			ObjectPos_map.X++;
			DrawObject();
		}
		if (ObjectPos_array.X > 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.X--;
			ObjectPos_map.X--;
			DrawObject();
		}
		if (ObjectPos_array.X == 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			DrawObject();

			CleanPosGhost(obj);
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			DrawObject();

			int temp = rand() % 2;
			if (temp = 1)
			{
				CleanPosGhost(obj);
				ObjectPos_array.X--;
				ObjectPos_map.X--;
				DrawObject();
			}
			if (temp = 0)
			{
				CleanPosGhost(obj);
				ObjectPos_array.X++;
				ObjectPos_map.X++;
				DrawObject();
			}
		}
		return;
	}

	if (first_use) {
		direction = 1 + rand() % (5 - 1);
		first_use = false;
	}
	//do {
	if (!freemove) {//if unite blocked, cycle don't work
		if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '0' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '0' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '0' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '0' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '1' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '8' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '8' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '8' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '8' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '9' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '9' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '9' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '9')
		{
			return;
		}

		temp_dir = direction;
		do {
			direction = 1 + rand() % (5 - 1);
		} while (temp_dir == direction);
		freemove = true;
	}
	switch (direction)
	{
	case UP_DIR:
	{
		if ((obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '0') && (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '1') &&
			(obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '8') && (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '9')) {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.Y--;
		ObjectPos_map.Y--;
		DrawObject();
		break;
	}
	case RIGHT_DIR:
	{
		if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '0' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '8' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '9') {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.X++;
		ObjectPos_map.X++;
		DrawObject();
		break;
	}
	case DOWN_DIR:
	{
		if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '0' && obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '1' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '8' && obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '9') {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.Y++;
		ObjectPos_map.Y++;
		DrawObject();
		break;
	}
	case LEFT_DIR:
	{
		if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '0' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '8' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '9') {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.X--;
		ObjectPos_map.X--;
		DrawObject();
		break;
	}
	/*default:
	{
	continue;
	}*/
	}
	//Sleep(15);
	//} while (!freemove);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class Inky : public Ghost
{
public:
	Inky();
	void Move(int Speed, Game_field &obj);
};

Inky::Inky()
{
	//Координати стартові Pinki на екрані
	ObjectPos_map.X = StartX_map_Inky;
	ObjectPos_map.Y = StartY_map_Inky;

	//Координати стартові Pinki в масиві карти
	ObjectPos_array.X = StartX_array_Inky;
	ObjectPos_array.Y = StartY_array_Inky;

	//Колір та фігурка Blinki
	_color = LightBlue;
	_face = char(1);

	DrawObject();
}

void Inky::Move(int Speed, Game_field &obj)
{
	if (clock() % Speed)// accept move by tick
		return;

	srand(clock());

	enum arrow1 { UP_DIR = 1, RIGHT_DIR = 2, DOWN_DIR = 3, LEFT_DIR = 4 };
	static int direction;
	int temp_dir;
	static bool freemove = true;
	static bool first_use = true;

	//Коли привиди в кімнаті (виходимо)  ворота 21 або 53
	if (ObjectPos_array.Y == 13 && ObjectPos_array.X >= 15 && ObjectPos_array.X <= 27)
	{
		if (ObjectPos_array.X < 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.X++;
			ObjectPos_map.X++;
			DrawObject();
		}
		if (ObjectPos_array.X > 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.X--;
			ObjectPos_map.X--;
			DrawObject();
		}
		if (ObjectPos_array.X == 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			DrawObject();

			CleanPosGhost(obj);
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			DrawObject();

			int temp = rand() % 2;
			if (temp = 1)
			{
				CleanPosGhost(obj);
				ObjectPos_array.X--;
				ObjectPos_map.X--;
				DrawObject();
			}
			if (temp = 0)
			{
				CleanPosGhost(obj);
				ObjectPos_array.X++;
				ObjectPos_map.X++;
				DrawObject();
			}
		}
		return;
	}

	if (first_use) {
		direction = 1 + rand() % (5 - 1);
		first_use = false;
	}
	//do {
	if (!freemove) {//if unite blocked, cycle don't work
		if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '0' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '0' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '0' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '0' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '1' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '8' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '8' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '8' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '8' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '9' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '9' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '9' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '9')
		{
			return;
		}

		temp_dir = direction;
		do {
			direction = 1 + rand() % (5 - 1);
		} while (temp_dir == direction);
		freemove = true;
	}
	switch (direction)
	{
	case UP_DIR:
	{
		if ((obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '0') && (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '1') &&
			(obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '8') && (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '9')) {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.Y--;
		ObjectPos_map.Y--;
		DrawObject();
		break;
	}
	case RIGHT_DIR:
	{
		if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '0' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '8' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '9') {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.X++;
		ObjectPos_map.X++;
		DrawObject();
		break;
	}
	case DOWN_DIR:
	{
		if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '0' && obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '1' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '8' && obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '9') {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.Y++;
		ObjectPos_map.Y++;
		DrawObject();
		break;
	}
	case LEFT_DIR:
	{
		if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '0' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '8' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '9') {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.X--;
		ObjectPos_map.X--;
		DrawObject();
		break;
	}
	/*default:
	{
	continue;
	}*/
	}
	//Sleep(15);
	//} while (!freemove);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class Clyde : public Ghost
{
public:
	Clyde();
	void Move(int Speed, Game_field &obj);
};

Clyde::Clyde()
{
	//Координати стартові Pinki на екрані
	ObjectPos_map.X = StartX_map_Clyde;
	ObjectPos_map.Y = StartY_map_Clyde;

	//Координати стартові Pinki в масиві карти
	ObjectPos_array.X = StartX_array_Clyde;
	ObjectPos_array.Y = StartY_array_Clyde;

	//Колір та фігурка Blinki
	_color = LightGreen;
	_face = char(1);

	DrawObject();
}

void Clyde::Move(int Speed, Game_field &obj)
{
	if (clock() % Speed)// accept move by tick
		return;

	srand(clock());

	enum arrow1 { UP_DIR = 1, RIGHT_DIR = 2, DOWN_DIR = 3, LEFT_DIR = 4 };
	static int direction;
	int temp_dir;
	static bool freemove = true;
	static bool first_use = true;

	//Коли привиди в кімнаті (виходимо)  ворота 21 або 53
	if (ObjectPos_array.Y == 13 && ObjectPos_array.X >= 15 && ObjectPos_array.X <= 27)
	{
		if (ObjectPos_array.X < 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.X++;
			ObjectPos_map.X++;
			DrawObject();
		}
		if (ObjectPos_array.X > 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.X--;
			ObjectPos_map.X--;
			DrawObject();
		}
		if (ObjectPos_array.X == 21)
		{
			CleanPosGhost(obj);
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			DrawObject();

			CleanPosGhost(obj);
			ObjectPos_array.Y--;
			ObjectPos_map.Y--;
			DrawObject();

			int temp = rand() % 2;
			if (temp = 1)
			{
				CleanPosGhost(obj);
				ObjectPos_array.X--;
				ObjectPos_map.X--;
				DrawObject();
			}
			if (temp = 0)
			{
				CleanPosGhost(obj);
				ObjectPos_array.X++;
				ObjectPos_map.X++;
				DrawObject();
			}
		}
		return;
	}

	if (first_use) {
		direction = 1 + rand() % (5 - 1);
		first_use = false;
	}
	//do {
	if (!freemove) {//if unite blocked, cycle don't work
		if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '0' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '0' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '0' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '0' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '1' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '8' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '8' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '8' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '8' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '9' &&
			obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '9' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '9' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '9')
		{
			return;
		}

		temp_dir = direction;
		do {
			direction = 1 + rand() % (5 - 1);
		} while (temp_dir == direction);
		freemove = true;
	}
	switch (direction)
	{
	case UP_DIR:
	{
		if ((obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '0') && (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '1') &&
			(obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '8') && (obj.GetObj(ObjectPos_array.Y - 1, ObjectPos_array.X) != '9')) {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.Y--;
		ObjectPos_map.Y--;
		DrawObject();
		break;
	}
	case RIGHT_DIR:
	{
		if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '0' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '8' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X + 1) != '9') {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.X++;
		ObjectPos_map.X++;
		DrawObject();
		break;
	}
	case DOWN_DIR:
	{
		if (obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '0' && obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '1' &&
			obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '8' && obj.GetObj(ObjectPos_array.Y + 1, ObjectPos_array.X) != '9') {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.Y++;
		ObjectPos_map.Y++;
		DrawObject();
		break;
	}
	case LEFT_DIR:
	{
		if (obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '0' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '1' &&
			obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '8' && obj.GetObj(ObjectPos_array.Y, ObjectPos_array.X - 1) != '9') {
			freemove = false;
			break;
		}
		CleanPosGhost(obj);
		ObjectPos_array.X--;
		ObjectPos_map.X--;
		DrawObject();
		break;
	}
	/*default:
	{
	continue;
	}*/
	}
	//Sleep(15);
	//} while (!freemove);
}

//-------------------------------------------------------------------------------------------------


//Загальний клас Гри-------------------------------------------
class Game
{
private:
	Game_field LoadMapLevel(int level); //Завантажує перший рівень карти для гри
public:
	void Menu();

	void Load_Ready();
	void Load_GameOver();
	void Load_Win();

	void SaveScores(string player, int score);
	void LoadScoresAndShow();
	int LoadScoresAndFindMax();

	bool Status(Pacman &pacman);
};

Game_field Game::LoadMapLevel(int level) //Завантажує перший рівень карти для гри
{
	system("cls");

	char current_work_dir[FILENAME_MAX];
	_getcwd(current_work_dir, sizeof(current_work_dir)); //директорія екзешніка

	char buffer[33];
	itoa(level, buffer, 10);
	string buf = buffer;
	string filename = "\\map_level_"+buf;
	string fileformat = ".txt";
	string allpath = current_work_dir + filename + fileformat;

	ifstream file(allpath);

	Game_field field;

	try
	{
		if (!file.is_open())
		{
			throw "File not opened!";
		}

		char buf[255];
		//cout << endl;
		cout << "\t\t\t\t";
		
		int i = 0, j = 0;

		while (!file.eof())
		{
			char temp = file.get();

			if (isdigit(temp))
				field.LoadMapField(i, j++, temp);


			switch (temp)
			{
			case '0':
			{
				cout << char(0);
				break;
			}
			case '1':
			{
				SetTextColor(Yellow);
				cout << char(249);
				SetTextColor(LightGray);
				break;
			}
			case '2':
			{
				SetTextColor(LightBlue);
				cout << char(205);
				SetTextColor(LightGray);
				break;
			}
			case '3':
			{
				SetTextColor(LightBlue);
				cout << char(186);
				SetTextColor(LightGray);
				break;			
			}
			case '4':
			{
				SetTextColor(LightBlue);
				cout << char(201);
				SetTextColor(LightGray);
				break;
			}
			case '5':
			{
				SetTextColor(LightBlue);
				cout << char(187);
				SetTextColor(LightGray);
				break;
			}
			case '6':
			{
				SetTextColor(LightBlue);
				cout << char(200);
				SetTextColor(LightGray);
				break;
			}
			case '7':
			{
				SetTextColor(LightBlue);
				cout << char(188);
				SetTextColor(LightGray);
				break;
			}
			case '8':
			{
				SetTextColor(LightCyan);
				cout << char(254);
				SetTextColor(LightGray);
				break;
			}
			case '9':
			{
				SetTextColor(LightMagenta);
				cout << char(253);
				SetTextColor(LightGray);
				break;
			}
			case '\n':
			{
				cout << endl;
				i++; j = 0;
				cout << "\t\t\t\t";
				break;
			}
			default:
			{
				break;
			}
			}
			if (file.eof()) break;
		}
		file.close();

		return field;
	}
	catch (char* ex)
	{
		cout << ex << endl;
		file.close();
		return field;
	}
}

void Game::Menu()
{
	LoadLogoScreen(); //Завантажує логотип Пакмана
	Sound Menu_Sound;
	int choise = choise_menu();
	while (choise != 3)
	{

		switch (choise)
		{
		case 0: //Початок гри
		{
			Menu_Sound.Menu_Choise();
			system("cls");
			cout << "Game" << endl;
			Game_field field;
			field = LoadMapLevel(1);

			//Напис про готовність
			Load_Ready();
			
			Pacman *pacman = new Pacman();

			//Знаходимо та завантажуємо з файлу найкращий рейтинг (HighScore)
			pacman->Info->SetHightScore(LoadScoresAndFindMax());
			pacman->Info->Show();

			Blinky *blinky = new Blinky();
			Pinky *pinky = new Pinky();
			Inky *inky = new Inky();
			Clyde *clyde = new Clyde();

			while (Status(*pacman))
			{
				//Pacman
				pacman->Run(PACMAN_SPEED, field);
				//Blinki
				blinky->Move(GHOST_SPEED, field);
				blinky->CheckPosition(*pacman);
				//Pinki
				pinky->Move(GHOST_SPEED, field);
				pinky->CheckPosition(*pacman);
				//Inki
				inky->Move(GHOST_SPEED, field);
				inky->CheckPosition(*pacman);
				//Clyde
				clyde->Move(GHOST_SPEED, field);
				clyde->CheckPosition(*pacman);
			}

			//Запис результатів гри у файл (scores)
			SetPos(8, 27);
			SetTextColor(LightGray);
			cout << "Name: ";
			string player_name;
			cin >> player_name;

			SaveScores(player_name, pacman->Info->GetScore());

			delete pacman;
			delete blinky;
			delete pinky;
			delete inky;
			delete clyde;
			break;
		}
		case 1: //Рейтинг досягнень
		{
			Menu_Sound.Menu_Choise();
			system("cls");

			LoadScoresAndShow();
			//cout << "Score" << endl;
			_getch();
			break;
		}
		case 2: //Про гру
		{
			Menu_Sound.Menu_Choise();
			system("cls");
			SetTextColor(LightRed);
			cout << "This is a Pacman game - a very simplistic version." << endl << endl;
			cout << "GAME HISTORY:" << endl << endl;
			SetTextColor(LightMagenta);
			cout << "The game was developed mainly by a Namco employee Toru Iwatani for eighteen months. In the original, it was called pakku-man [pakku-man] (yap. パ ッ ク マ ン). The name is derived from the Japanese descriptive expression pack - pack taberu (yap パ ク ク パ ク 食 べ る, eat, bite, repeatedly opening his mouth and slamming it). It is often mentioned that Ivatani was inspired by a piece of pizza. In an interview in 1986, he noted that it was half true: Iwatani rounded the kuti (yap 口 mouth) hieroglyph as a symbol of eating and gave it a semblance of a mouth. Iwatani also said that in many games of that time there was violence, and therefore they were not suitable for all categories of the population. He also wanted to create a fun game that would please the majority" <<

				"Iwatani tried to make the game interesting for wider sections of the population, besides ordinary boys and teenagers, and he added a maze element to the game.As a result, the game was called PUCK MAN.When the game appeared in Japan in 1980, it received an indifferent response, because games like Space Invaders or Defender were much more popular." <<

				"In the US, the release of the game took the company Midway, but under a changed name.This was due to the fact that the Latin letter P is quite easy to change to F, making the name obscene(English FUCK).To avoid possible vandalism, the joint decision of the publisher and developer, the name of the game PUCK MAN was changed to Pac-Man.After the release of the American version, the new name was used throughout the world.Midway has also changed the design of slot machines.Namco's style was more expensive and less suitable for the American market. While PUCK MAN was painted white and colorful with colorful designs, Pac-Man was made in yellow, and the drawings became simple and memorable." <<

				"The American public was well met with the game, which gave an alternative to Space Invaders, which was expressed in unprecedented popularity and income, surpassing its successful predecessor.Even Iwatani was surprised by sales in the US.The game soon became a worldwide phenomenon in the video industry, numerous sequels were released, the style of the game was often copied, but none of the clones could surpass the original." <<

				"In the game it was necessary to pass all 255 levels.The maximum number of points was registered on July 3, 1999 in Hollywood : Billy Mitchell scored 3, 333, 360 points.It took him 6 hours to do this.He collected all the points, all the energy - makers, all the fruits, ate all the ghosts at all 255 levels." <<

				"Manufacturers of video games were impressed by the success of the game.Its popularity exceeded the rating of Asteroids, arcades with giant sales.The machines of Pac - Man dispersed in the number of 350 000 copies.The game was so popular that on its basis a lot of fakes were born in the early 1980s." <<

				"The unique game design forced game manufacturers to reconsider the idea of ​​endless alien invasions.Introduced in the Pac-Man element of humor allowed the game to meet the interests of the broader population.The interest of teenagers to Pac-Man exceeded the interest in shooters.Many popular video games of the 1980s in one way or another due to the existence of Pac-Man : Q * bert, Donkey Kong, Frogger." <<

				"Site Killer List of Videogames considers Pac - Man the No. 1 game of all time in the list of the 10 most popular video games." << endl << endl;
			SetTextColor(Yellow);
			cout << "resource: https://ru.wikipedia.org/wiki/Pac-Man" << endl << endl;
			SetTextColor(LightBlue);
			cout << "Master-teacher: Pilaeva Anna" << endl;
			cout << "Kyrsach. Copyright Roma Lahodniuk" << endl;
			SetTextColor(LightGray);
			_getch();
			break;
		}
		default: //Вихід
		{
			Menu_Sound.Menu_Choise();
			cout << "Thank you! Bye." << endl;
			break;
		}
		}

		LoadLogoScreen(); //Завантажує логотип Пакмана
		choise = choise_menu();
	}
}


void Game:: Load_Ready()
{
	//Напис про готовність
	while (true)
	{
		if (!_kbhit())
		{
			SetPos(50, 13);
			SetTextColor(LightRed);
			cout << "READY!!!";
			Sleep(150);
			SetPos(50, 13);
			SetTextColor(Black);
			cout << "READY!!!";
			Sleep(150);
		}
		else
		{
			if (_getch() == 13)
			{
				SetPos(50, 13);
				SetTextColor(Black);
				cout << "READY!!!";
				break;
			}
		}
	}
}

void Game::Load_GameOver()
{
	//Напис про програну гру
	while (true)
	{
		if (!kbhit())
		{
			SetPos(49, 13);
			SetTextColor(LightRed);
			cout << "GAME OVER";
			Sleep(150);
			SetPos(49, 13);
			SetTextColor(Black);
			cout << "GAME OVER";
			Sleep(150);
		}
		else
		{
			if (_getch() == 13)
			{
				SetPos(49, 13);
				SetTextColor(Black);
				cout << "GAME OVER";
				break;
			}
		}
	}
}

void Game::Load_Win()
{
	//Напис про перемогу
	while (true)
	{
		if (!_kbhit())
		{

			SetPos(52, 13);
			SetTextColor(LightRed);
			cout << "WIN!!!";
			Sleep(150);
			SetPos(52, 13);
			SetTextColor(Black);
			cout << "WIN!!!";
			Sleep(150);
		}
		else
		{
			if (_getch() == 13)
			{
				SetPos(52, 13);
				SetTextColor(Black);
				cout << "WIN!!!";
				break;
			}
		}
	}
}

void Game:: SaveScores(string player, int score)
{
	char current_work_dir[FILENAME_MAX];
	_getcwd(current_work_dir, sizeof(current_work_dir)); //директорія екзешніка

	string filename = "\\scores";
	string fileformat = ".txt";
	string allpath = current_work_dir + filename + fileformat;

	ofstream file(allpath,ios_base::app);

	try
	{
		if (!file.is_open())
		{
			throw "File not opened!";
		}

		file << '\n' << player << ' ' << score;

		file.close();
	}
	catch (char* ex)
	{
		cout << ex << endl;
		file.close();
		return;
	}
}

void Game:: LoadScoresAndShow()
{
	system("cls");

	char current_work_dir[FILENAME_MAX];
	_getcwd(current_work_dir, sizeof(current_work_dir)); //директорія екзешніка

	string filename = "\\scores";
	string fileformat = ".txt";
	string allpath = current_work_dir + filename + fileformat;

	ifstream file(allpath);

	try
	{
		if (!file.is_open())
		{
			throw "File not opened!";
		}

		char buf[255];
		SetTextColor(LightGreen);
		cout << endl << endl;
		cout << "\t\t\t\tSCORES" << endl;
		cout << "\t\t\t";
		for (size_t i = 0; i < 25; i++)
		{
			cout << "-";//char(219);
		}
		cout << endl;

		string name, save_score;
		while (!file.eof())
		{

			file >> name;
			file>> save_score;
			cout <<"\t\t\t"<< char(221)<<setw(11)<< name <<char(221)<<setw(11) << save_score<<char(222) << endl;
			if (file.eof()) break;
		}
		cout << "\t\t\t";
		for (size_t i = 0; i < 25; i++)
		{
			cout << "-";//char(219);
		}
		SetTextColor(LightGray);
		file.close();
	}
	catch (char* ex)
	{
		cout << ex << endl;
		file.close();
		return;
	}
}

int Game:: LoadScoresAndFindMax()
{

	char current_work_dir[FILENAME_MAX];
	_getcwd(current_work_dir, sizeof(current_work_dir)); //директорія екзешніка

	string filename = "\\scores";
	string fileformat = ".txt";
	string allpath = current_work_dir + filename + fileformat;

	ifstream file(allpath);

	try
	{
		if (!file.is_open())
		{
			throw "File not opened!";
		}

		string name;
		int save_score = 0;
		int save_score_max = 0;

		while (!file.eof())
		{

			file >> name;
			file >> save_score;
			if (save_score_max < save_score) save_score_max = save_score;
			if (file.eof()) break;
		}
		return save_score_max;
		file.close();
	}
	catch (char* ex)
	{
		cout << ex << endl;
		file.close();
		return 0;
	}
}

bool Game::Status(Pacman &pacman)
{
	//Якщо закінчілтся життя - гра програна
	if (pacman.GetLives() == 0)
	{
		Load_GameOver();
		return false;
	}
	
	//Якщо зібрані всі точки гра виграна
	if (pacman.GetCountDots() == DOTS)
	{
		Load_Win();
		return false;
	}

	return true;
}

//Головна програма
void main()
{
	Game *pacman = new Game();
	pacman->Menu();
	delete pacman;
}