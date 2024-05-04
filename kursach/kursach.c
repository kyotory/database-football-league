#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

typedef struct team // структура, в которой хранятся данные о команде: название, очки, забитые и пропущенные мячи
{
	char name[100];
	int points;
	int scored_goals;
	int missed_goals;

} team;

void main_menu() { // функция, которая выводит главное меню на экран

	system("cls");
	printf("\n\n\t \t \t \t \t \t \t \t \t \t \tCписок доступных действий:\n\n\t");
	printf("\t \t \t \t \t \t \t \tВвод или изменение названий участников турнира (Нажмите клавишу 1)\n\n\t");
	printf("\t \t \t \t \t \t \t \tНачало или изменение футбольного турнира (Нажмите клавишу 2)\n\n\t");
	printf("\t \t \t \t \t \t \t \tВывод таблицы сыгранных матчей (Нажмите клавишу 3)\n\n\t");
	printf("\t \t \t \t \t \t \t \tВывод турнирной таблицы (Нажмите клавишу 4)\n\n\t");
	printf("\t \t \t \t \t \t \t \tРестарт турнира (Нажмите клавишу 5)\n\n\t");
	printf("\t \t \t \t \t \t \t \tВыход из программы (Нажмите клавишу 6)\n\n\t \t \t \t \t \t \t \t \t");

}

void team_sorting(team* arr_of_teams, int number_of_teams) // функция сортировки команд по принципу у кого больше очков, забитых мячей, пропущенных мячей
{
	for (int i = 0; i < number_of_teams - 1; i++)
	{
		for (int j = 0; j < number_of_teams - i - 1; j++)
		{
			if (arr_of_teams[j].points < arr_of_teams[j + 1].points)
			{
				team tmp = arr_of_teams[j];
				arr_of_teams[j] = arr_of_teams[j + 1];
				arr_of_teams[j + 1] = tmp;
			}
		}
	}

	for (int i = 0; i < number_of_teams - 1; i++)
	{
		for (int j = 0; j < number_of_teams - i - 1; j++)
		{
			if ((arr_of_teams[j].scored_goals < arr_of_teams[j + 1].scored_goals) && (arr_of_teams[j].points == arr_of_teams[j + 1].points))
			{
				team tmp = arr_of_teams[j];
				arr_of_teams[j] = arr_of_teams[j + 1];
				arr_of_teams[j + 1] = tmp;
			}
		}
	}

	for (int i = 0; i < number_of_teams - 1; i++)
	{
		for (int j = 0; j < number_of_teams - i - 1; j++)
		{
			if ((arr_of_teams[j].missed_goals > arr_of_teams[j + 1].missed_goals) && (arr_of_teams[j].points == arr_of_teams[j + 1].points) && (arr_of_teams[j].scored_goals == arr_of_teams[j + 1].scored_goals))
			{
				team tmp = arr_of_teams[j];
				arr_of_teams[j] = arr_of_teams[j + 1];
				arr_of_teams[j + 1] = tmp;
			}
		}
	}
}

void team_name_input(team* arr_of_teams, int count_of_teams, FILE* f) { // функция ввода названий команд

	getchar();
	for (int i = 0; i < count_of_teams; i++) {

		printf("\t \t \t \t \t \t \t \t \tКоманда %d: ", i + 1);
		gets(arr_of_teams[i].name);
		fwrite(&(arr_of_teams[i]), sizeof(team), 1, f);
		fflush(f);

		printf("\n\n");
	}
}

void team_name_edit(team* arr_of_teams, int count_of_teams, FILE* f) { // функция изменения названия команды

	int number_of_team;
	scanf("%d", &number_of_team);
	getchar();
	printf("\n\n\t \t \t \t \t \t \t \t \tВведите новое название команды %d: ", number_of_team);
	for (int i = 0; i < count_of_teams; i++) {

		if ((i + 1) == number_of_team) {
			gets(arr_of_teams[i].name);
			fseek(f, (i) * sizeof(team), SEEK_SET);
			fwrite(&(arr_of_teams[i]), sizeof(team), 1, f);
			fflush(f);
			fseek(f, 0, SEEK_END);
		}
	}
	printf("\n\n\t \t \t \t \t \t \t \t \tНазвание команды %d успешно изменено. Для выхода в меню - нажмите клавишу 1.", number_of_team);
	printf("\n\n\t \t \t \t \t \t \t \t \t");
}

void played_games_table_output(team* arr_of_teams, int count_of_teams, int*** arr_of_games) // функция вывода таблицы сыгранных матчей
{
	int max_team_name_len = 0;

	for (int i = 0; i < count_of_teams; i++)
	{
		int team_name_len = strlen(arr_of_teams[i].name);
		if (team_name_len > max_team_name_len)
		{
			max_team_name_len = team_name_len;
		}
	}
	if (max_team_name_len < 14) max_team_name_len = 14;
	max_team_name_len += 10;

	printf("%-*c", max_team_name_len * 2 + 8, ' ');
	for (int i = 0; i < count_of_teams; i++)
	{
		printf("%-*s", max_team_name_len, arr_of_teams[i].name);
	}
	printf("\n\n");

	for (int i = 0; i < count_of_teams; i++)
	{
		printf("\t\t\t\t%-*s", max_team_name_len, arr_of_teams[i].name);

		for (int j = 0; j < count_of_teams; j++)
		{
			if ((arr_of_games[i][j][2] == 0) && (i != j))
			{
				printf("%-*s", max_team_name_len, "Матч не сыгран");
			}
			else if (i != j)
			{
				char result[10000];
				sprintf(result, "      %d:%d", arr_of_games[i][j][0], arr_of_games[i][j][1]);
				printf("%-*s", max_team_name_len, result);
			}
			else
			{
				printf("%-*s", max_team_name_len, "      ---");
			}
		}
		printf("\n\n");
	}
}

void full_table_output(team* arr_of_teams, int count_of_teams) // функция вывода турнирной таблицы
{
	printf("\t \t \t \t Место\t \t \t \t Команда\t \t \t \t Очки\t \t \t \t Забитые мячи\t \t \t \t Пропущенные мячи\n\n");
	for (int i = 0; i < count_of_teams; i++) {
		printf("%36d %35s %35d %35d %40d\n\n", i + 1, arr_of_teams[i].name, arr_of_teams[i].points, arr_of_teams[i].scored_goals, arr_of_teams[i].missed_goals);
	}
}

void tournament_restart(FILE* f_teams, FILE* f_games, team* arr_of_teams, int count_of_teams, int*** arr_of_games) { // функция, удаляющая данные и перезапускающая турнир

	fclose(f_teams); fclose(f_games); // очистка файлов с данными о командах и сыгранных матчах
	f_teams = fopen("database_football_teams.dat", "w");
	f_games = fopen("database_football_games.dat", "w");
	fclose(f_teams); fclose(f_games); 
	free(arr_of_teams); // освобождение памяти массивов
	for (int i = 0; i < count_of_teams; i++) {
		for (int j = 0; j < count_of_teams; j++) {
			free(arr_of_games[i][j]);
		}
		free(arr_of_games[i]);
	}
	free(arr_of_games);
}


void tournament_play(team* arr_of_teams, int count_of_teams, int*** arr_of_games, FILE* f_games, FILE* f_teams) // функция, при помощи которой играется тур
{
	int* arr_of_used_teams = (int*)malloc(count_of_teams * sizeof(int));

	for (int i = 0; i < count_of_teams; i++) arr_of_used_teams[i] = -1;
	int cnt_games_played = 0;
	int team1 = 0, team2 = 0; int flag_played;
	int goals1 = 0, goals2 = 0;

	system("cls");

	for (int i = 0; i < count_of_teams; i++)
	{
		for (int j = 0; j < count_of_teams; j++)
		{
			if (arr_of_games[i][j][2] == 1) cnt_games_played++;
		}
	}

	if ((cnt_games_played / count_of_teams) != (count_of_teams - 1))
	{
		if (cnt_games_played == ((count_of_teams * count_of_teams - count_of_teams) / 2)) printf("\n\n\t \t \t \t \t \t \t \t \tПервый круг турнира сыгран, начинаем второй");

		printf("\n\n\t \t \t \t \t \t \t \t \tБыло сыграно %d туров, начинаем %dй тур\n\n\t \t \t \t \t \t \t \t \t", (cnt_games_played / (count_of_teams / 2)), (cnt_games_played / (count_of_teams / 2) + 1));

		for (int i = 0; i < count_of_teams / 2; i++) // если сыгран только первый круг турнира
		{
			if (cnt_games_played >= ((count_of_teams * (count_of_teams - 1))) / 2)
			{
				while (1)
				{
					flag_played = 0;
					srand(time(NULL)); // генератор пар на матчи при помощи времени
					team1 = rand() % count_of_teams;
					team2 = rand() % count_of_teams;

					for (int j = 0; j < count_of_teams; j++)
					{
						if ((team1 == arr_of_used_teams[j]) || (team2 == arr_of_used_teams[j])) flag_played = 1;
					}

					if ((team1 != team2) && (flag_played == 0) && (arr_of_games[team1][team2][2] == 0)) break;
				}

				printf("Матч %d: %s (Дома) - %s (Гости)", i + 1, arr_of_teams[team1].name, arr_of_teams[team2].name); // ввод забитых голов в матче
				printf("\n\n\t \t \t \t \t \t \t \t \tВведите забитые голы первой команды: ");
				scanf("%d", &goals1);
				printf("\n\n\t \t \t \t \t \t \t \t \tВведите забитые голы второй команды: ");
				scanf("%d", &goals2);
				printf("\n\n\t \t \t \t \t \t \t \t \tМатч между командами %s и %s сыгран. Итоговый счет - %d:%d\n\n\t \t \t \t \t \t \t \t \t", arr_of_teams[team1].name, arr_of_teams[team2].name, goals1, goals2);

				arr_of_teams[team1].missed_goals += goals2; arr_of_teams[team1].scored_goals += goals1;
				arr_of_teams[team2].missed_goals += goals1; arr_of_teams[team2].scored_goals += goals2;

				if (goals1 == goals2)
				{
					arr_of_teams[team1].points += 1;
					arr_of_teams[team2].points += 1;
				}
				else if (goals1 > goals2) arr_of_teams[team1].points += 3;
				else if (goals1 < goals2) arr_of_teams[team2].points += 3;

				arr_of_games[team1][team2][0] = goals1;
				arr_of_games[team1][team2][1] = goals2;
				arr_of_games[team1][team2][2] = 1;

				arr_of_used_teams[team1] = team1;
				arr_of_used_teams[team2] = team2;

				fseek(f_teams, (team1) * sizeof(team), SEEK_SET); // запись в файл массива структур и трехмерного массива
				fwrite(&(arr_of_teams[team1]), sizeof(team), 1, f_teams);
				fflush(f_teams);
				fseek(f_teams, 0, SEEK_END);

				fseek(f_teams, (team2) * sizeof(team), SEEK_SET);
				fwrite(&(arr_of_teams[team2]), sizeof(team), 1, f_teams);
				fflush(f_teams);
				fseek(f_teams, 0, SEEK_END);

				fclose(f_games);
				f_games = fopen("database_football_games.dat", "w");
				fclose(f_games);
				f_games = fopen("database_football_games.dat", "rb+");

				for (int i = 0; i < count_of_teams; i++)
				{
					for (int j = 0; j < count_of_teams; j++) {
						fwrite(arr_of_games[i][j], sizeof(int), 3, f_games); 
					}
				}

			}
			else // игра турнира в случае если играется уже второй круг
			{
				while (1)
				{
					flag_played = 0;
					srand(time(NULL));
					team1 = rand() % count_of_teams;
					team2 = rand() % count_of_teams;

					for (int j = 0; j < count_of_teams; j++)
					{
						if ((team1 == arr_of_used_teams[j]) || (team2 == arr_of_used_teams[j])) flag_played = 1;
					}

					if ((team1 != team2) && (flag_played == 0) && (arr_of_games[team1][team2][2] == 0) && (arr_of_games[team2][team1][2] == 0)) break;
				}

				printf("Матч %d: %s (Дома) - %s (Гости)", i + 1, arr_of_teams[team1].name, arr_of_teams[team2].name);
				printf("\n\n\t \t \t \t \t \t \t \t \tВведите забитые голы первой команды: ");
				scanf("%d", &goals1);
				printf("\n\n\t \t \t \t \t \t \t \t \tВведите забитые голы второй команды: ");
				scanf("%d", &goals2);
				printf("\n\n\t \t \t \t \t \t \t \t \tМатч между командами %s и %s сыгран. Итоговый счет - %d:%d\n\n\t \t \t \t \t \t \t \t \t", arr_of_teams[team1].name, arr_of_teams[team2].name, goals1, goals2);

				arr_of_teams[team1].missed_goals += goals2; arr_of_teams[team1].scored_goals += goals1;
				arr_of_teams[team2].missed_goals += goals1; arr_of_teams[team2].scored_goals += goals2;

				if (goals1 == goals2)
				{
					arr_of_teams[team1].points += 1;
					arr_of_teams[team2].points += 1;
				}
				else if (goals1 > goals2) arr_of_teams[team1].points += 3;
				else if (goals1 < goals2) arr_of_teams[team2].points += 3;

				arr_of_games[team1][team2][0] = goals1;
				arr_of_games[team1][team2][1] = goals2;
				arr_of_games[team1][team2][2] = 1;

				arr_of_used_teams[team1] = team1;
				arr_of_used_teams[team2] = team2;

				fseek(f_teams, (team1) * sizeof(team), SEEK_SET); // запись массив структур и трехмерного массива в файл
				fwrite(&(arr_of_teams[team1]), sizeof(team), 1, f_teams);
				fflush(f_teams);
				fseek(f_teams, 0, SEEK_END);

				fseek(f_teams, (team2) * sizeof(team), SEEK_SET);
				fwrite(&(arr_of_teams[team2]), sizeof(team), 1, f_teams);
				fflush(f_teams);
				fseek(f_teams, 0, SEEK_END);

				fclose(f_games);
				f_games = fopen("database_football_games.dat", "w");
				fclose(f_games);
				f_games = fopen("database_football_games.dat", "rb+");

				for (int i = 0; i < count_of_teams; i++)
				{
					for (int j = 0; j < count_of_teams; j++) {
						fwrite(arr_of_games[i][j], sizeof(int), 3, f_games); 
					}
				}

			}
		}
		if ((cnt_games_played / (count_of_teams / 2) + 1) == ((count_of_teams / 2 + 1) * 2)) printf("\n\n\t \t \t \t \t \t \t \t \tСыгран последний тур. Для просмотра результатов выйдите в главное меню");
		printf("\n\n\t \t \t \t \t \t \t \t \tТур %d успешно сыгран. Для выхода в меню - нажмите кнопку 1", (cnt_games_played / (count_of_teams / 2) + 1));
	}
	else // если все матчи турнира сыграны
	{
		printf("\n\n\t \t \t \t \t \t \t \t \tВсе матчи турнира уже сыграны"); 
		printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
	}

}

void tournament_edit(team* arr_of_teams, int count_of_teams, int*** arr_of_games, FILE* f_games, FILE* f_teams) { // функция изменения выбранного пользователем матча

	fseek(f_games, 0, SEEK_END);
	int size_of_file = ftell(f_games); 
	rewind(f_games);

	if (size_of_file != 0) // если есть матчи для изменения
	{
		system("cls");
		printf("\n\n\t \t \t \t \t \t \t \t \tТаблица сыгранных матчей:\n\n\n");
		played_games_table_output(arr_of_teams, count_of_teams, arr_of_games); // вывод таблицы сыгранных матчей
		int line; int column; int goals1; int goals2; int prev_goals1; int prev_goals2;
		while (1) { // выбор матча для изменения

			printf("\n\n\t \t \t \t \t \t \t \t \tВведите номер строки и номер столбца матча, который хотите изменить\n\n\t \t \t \t \t \t \t \t \t");
			printf("Строка: ");
			scanf("%d", &line);
			printf("\n\t \t \t \t \t \t \t \t \t");
			printf("Столбец: ");
			scanf("%d", &column);

			if (line > count_of_teams || column > count_of_teams)
			{
				system("cls");
				printf("\n\n\t \t \t \t \t \t \t \t \tТаблица сыгранных матчей:\n\n\n");
				played_games_table_output(arr_of_teams, count_of_teams, arr_of_games);
				printf("\n\n\t \t \t \t \t \t \t \t \tВыберите действительный матч");
			}
			else if (line == column)
			{
				system("cls");
				printf("\n\n\t \t \t \t \t \t \t \t \tТаблица сыгранных матчей:\n\n\n");
				played_games_table_output(arr_of_teams, count_of_teams, arr_of_games);
				printf("\n\n\t \t \t \t \t \t \t \t \tВыберите действительный матч");
			}
			else if (arr_of_games[line - 1][column - 1][2] == 0)
			{
				system("cls");
				printf("\n\n\t \t \t \t \t \t \t \t \tТаблица сыгранных матчей:\n\n\n");
				played_games_table_output(arr_of_teams, count_of_teams, arr_of_games);
				printf("\n\n\t \t \t \t \t \t \t \t \tМатч еще не сыгран, выберите уже сыгранный матч");
			}
			else
			{
				system("cls");
				break;
			}
		}

		prev_goals1 = arr_of_games[line - 1][column - 1][0]; prev_goals2 = arr_of_games[line - 1][column - 1][1]; // новые голы команд
		printf("\n\n\t \t \t \t \t \t \t \tВведите новый результат матча:");
		printf("\n\n\t \t \t \t \t \tСчет команды 1: ");
		scanf("%d", &goals1);
		printf("\n\n\t \t \t \t \t \tСчет команды 2: ");
		scanf("%d", &goals2);
		arr_of_games[line - 1][column - 1][0] = goals1;
		arr_of_games[line - 1][column - 1][1] = goals2;
		// изменение очков, забитых мячей, пропущенных мячей в структуре
		if ((goals1 > goals2) && (prev_goals1 > prev_goals2))
		{
			arr_of_teams[line - 1].scored_goals -= prev_goals1;
			arr_of_teams[line - 1].scored_goals += goals1;
			arr_of_teams[line - 1].missed_goals -= prev_goals2;
			arr_of_teams[line - 1].missed_goals += goals2;

			arr_of_teams[column - 1].scored_goals -= prev_goals2;
			arr_of_teams[column - 1].scored_goals += goals2;
			arr_of_teams[column - 1].missed_goals -= prev_goals1;
			arr_of_teams[column - 1].missed_goals += goals1;
			printf("\n\n\t \t \t \t \t \t \t \t \tДанные о матче успешно изменены");
			printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
		}
		else if ((goals1 > goals2) && (prev_goals1 == prev_goals2))
		{
			arr_of_teams[line - 1].points += 2;
			arr_of_teams[line - 1].scored_goals -= prev_goals1;
			arr_of_teams[line - 1].scored_goals += goals1;
			arr_of_teams[line - 1].missed_goals -= prev_goals2;
			arr_of_teams[line - 1].missed_goals += goals2;

			arr_of_teams[column - 1].points -= 1;
			arr_of_teams[column - 1].scored_goals -= prev_goals2;
			arr_of_teams[column - 1].scored_goals += goals2;
			arr_of_teams[column - 1].missed_goals -= prev_goals1;
			arr_of_teams[column - 1].missed_goals += goals1;
			printf("\n\n\t \t \t \t \t \t \t \t \tДанные о матче успешно изменены");
			printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
		}
		else if ((goals1 > goals2) && (prev_goals1 < prev_goals2))
		{
			arr_of_teams[line - 1].points += 3;
			arr_of_teams[line - 1].scored_goals -= prev_goals1;
			arr_of_teams[line - 1].scored_goals += goals1;
			arr_of_teams[line - 1].missed_goals -= prev_goals2;
			arr_of_teams[line - 1].missed_goals += goals2;

			arr_of_teams[column - 1].points -= 3;
			arr_of_teams[column - 1].scored_goals -= prev_goals2;
			arr_of_teams[column - 1].scored_goals += goals2;
			arr_of_teams[column - 1].missed_goals -= prev_goals1;
			arr_of_teams[column - 1].missed_goals += goals1;
			printf("\n\n\t \t \t \t \t \t \t \t \tДанные о матче успешно изменены");
			printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
		}

		else if ((goals1 == goals2) && (prev_goals1 > prev_goals2))
		{
			arr_of_teams[line - 1].points -= 2;
			arr_of_teams[line - 1].scored_goals -= prev_goals1;
			arr_of_teams[line - 1].scored_goals += goals1;
			arr_of_teams[line - 1].missed_goals -= prev_goals2;
			arr_of_teams[line - 1].missed_goals += goals2;

			arr_of_teams[column - 1].points += 1;
			arr_of_teams[column - 1].scored_goals -= prev_goals2;
			arr_of_teams[column - 1].scored_goals += goals2;
			arr_of_teams[column - 1].missed_goals -= prev_goals1;
			arr_of_teams[column - 1].missed_goals += goals1;
			printf("\n\n\t \t \t \t \t \t \t \t \tДанные о матче успешно изменены");
			printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
		}
		else if ((goals1 == goals2) && (prev_goals1 == prev_goals2))
		{
			arr_of_teams[line - 1].scored_goals -= prev_goals1;
			arr_of_teams[line - 1].scored_goals += goals1;
			arr_of_teams[line - 1].missed_goals -= prev_goals2;
			arr_of_teams[line - 1].missed_goals += goals2;

			arr_of_teams[column - 1].scored_goals -= prev_goals2;
			arr_of_teams[column - 1].scored_goals += goals2;
			arr_of_teams[column - 1].missed_goals -= prev_goals1;
			arr_of_teams[column - 1].missed_goals += goals1;
			printf("\n\n\t \t \t \t \t \t \t \t \tДанные о матче успешно изменены");
			printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
		}
		else if ((goals1 == goals2) && (prev_goals1 < prev_goals2))
		{
			arr_of_teams[line - 1].points += 1;
			arr_of_teams[line - 1].scored_goals -= prev_goals1;
			arr_of_teams[line - 1].scored_goals += goals1;
			arr_of_teams[line - 1].missed_goals -= prev_goals2;
			arr_of_teams[line - 1].missed_goals += goals2;

			arr_of_teams[column - 1].points -= 2;
			arr_of_teams[column - 1].scored_goals -= prev_goals2;
			arr_of_teams[column - 1].scored_goals += goals2;
			arr_of_teams[column - 1].missed_goals -= prev_goals1;
			arr_of_teams[column - 1].missed_goals += goals1;
			printf("\n\n\t \t \t \t \t \t \t \t \tДанные о матче успешно изменены");
			printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
		}

		else if ((goals1 < goals2) && (prev_goals1 > prev_goals2))
		{
			arr_of_teams[line - 1].points -= 3;
			arr_of_teams[line - 1].scored_goals -= prev_goals1;
			arr_of_teams[line - 1].scored_goals += goals1;
			arr_of_teams[line - 1].missed_goals -= prev_goals2;
			arr_of_teams[line - 1].missed_goals += goals2;

			arr_of_teams[column - 1].points += 3;
			arr_of_teams[column - 1].scored_goals -= prev_goals2;
			arr_of_teams[column - 1].scored_goals += goals2;
			arr_of_teams[column - 1].missed_goals -= prev_goals1;
			arr_of_teams[column - 1].missed_goals += goals1;
			printf("\n\n\t \t \t \t \t \t \t \t \tДанные о матче успешно изменены");
			printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
		}
		else if ((goals1 < goals2) && (prev_goals1 == prev_goals2))
		{
			arr_of_teams[line - 1].points -= 1;
			arr_of_teams[line - 1].scored_goals -= prev_goals1;
			arr_of_teams[line - 1].scored_goals += goals1;
			arr_of_teams[line - 1].missed_goals -= prev_goals2;
			arr_of_teams[line - 1].missed_goals += goals2;

			arr_of_teams[column - 1].points += 2;
			arr_of_teams[column - 1].scored_goals -= prev_goals2;
			arr_of_teams[column - 1].scored_goals += goals2;
			arr_of_teams[column - 1].missed_goals -= prev_goals1;
			arr_of_teams[column - 1].missed_goals += goals1;
			printf("\n\n\t \t \t \t \t \t \t \t \tДанные о матче успешно изменены");
			printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
		}
		else if ((goals1 < goals2) && (prev_goals1 < prev_goals2))
		{
			arr_of_teams[line - 1].scored_goals -= prev_goals1;
			arr_of_teams[line - 1].scored_goals += goals1;
			arr_of_teams[line - 1].missed_goals -= prev_goals2;
			arr_of_teams[line - 1].missed_goals += goals2;

			arr_of_teams[column - 1].scored_goals -= prev_goals2;
			arr_of_teams[column - 1].scored_goals += goals2;
			arr_of_teams[column - 1].missed_goals -= prev_goals1;
			arr_of_teams[column - 1].missed_goals += goals1;
			printf("\n\n\t \t \t \t \t \t \t \t \tДанные о матче успешно изменены");
			printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
		}

		fseek(f_teams, (line - 1) * sizeof(team), SEEK_SET); // запись информации об измененной структуре в файл
		fwrite(&(arr_of_teams[line - 1]), sizeof(team), 1, f_teams); 
		fflush(f_teams);
		fseek(f_teams, 0, SEEK_END);

		fseek(f_teams, (column - 1) * sizeof(team), SEEK_SET); // запись информации об измененной структуре в файл
		fwrite(&(arr_of_teams[column - 1]), sizeof(team), 1, f_teams);
		fflush(f_teams);
		fseek(f_teams, 0, SEEK_END);

		fclose(f_games); // перезапись трехмерного массива в файл
		f_games = fopen("database_football_games.dat", "w");
		fclose(f_games);
		f_games = fopen("database_football_games.dat", "rb+");

		for (int i = 0; i < count_of_teams; i++)
		{
			for (int j = 0; j < count_of_teams; j++) {
				fwrite(arr_of_games[i][j], sizeof(int), 3, f_games); 
			}
		}

	}
	else // если ни одного матча еще не сыграно
	{
		system("cls");
		printf("\n\n\t \t \t \t \t \t \t \t \tВ базе данных нет информации о сыгранных матчах");
		printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
	}
}

void database_reading(FILE* f_teams, int* count_of_teams, team** arr_of_teams, FILE* f_games, int**** arr_of_games) // функция, которая считывает данные из файла
{
	fseek(f_teams, 0, SEEK_END);
	long size_of_file = ftell(f_teams);
	rewind(f_teams);

	int num = size_of_file / sizeof(team); // получение информации о том, сколько команд было в турнире

	*count_of_teams = num;
	*arr_of_teams = (struct team*)calloc(num, sizeof(team)); // выделение памяти на массив структур и трехмерный массив

	*arr_of_games = (int***)calloc(num, sizeof(int**)); 
	for (int i = 0; i < num; i++)
	{
		(*arr_of_games)[i] = (int**)calloc(num, sizeof(int*));
		for (int j = 0; j < num; j++) {
			(*arr_of_games)[i][j] = (int*)calloc(3, sizeof(int));
		}
	}

	fread(*arr_of_teams, sizeof(team), *count_of_teams, f_teams); // считывание массива структур и трехмерного массива с играми
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++) {
			fread((*arr_of_games)[i][j], sizeof(int), 3, f_games); 
		}
	}
}

int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "RUS"); // локализация консоли для кириллицы
	 
	FILE* f_teams = fopen(argv[1], "rb+"); // открытие бинарного файла для записи и чтения
	FILE* f_games = fopen(argv[2], "rb+"); // открытие бинарного файла для записи и чтения

	team* arr_of_teams = NULL; // объявление массивов и переменной количества команд
	int count_of_teams = 0;
	int*** arr_of_games = NULL;

	fseek(f_teams, 0, SEEK_END);
	long file_size_teams = ftell(f_teams);

	int already_have_action = 0;
	int flag = 0;

	if (file_size_teams != 0) { // в случае если в базе данных есть какие-либо данные, программа даст выбор - удалить данные и начать турнир заново или считать данные

		while (flag == 0) { 

			printf("\n\t \t \t \t \t \t \tИмеется уже начатый турнир, если хотите продолжить его - нажмите 1. Если хотите начать новый - нажмите 2\n\n");
			printf("\t \t \t \t \t \t \t \t \t \t \t \t");
			int already_have_action = _getch();
			switch (already_have_action)
			{
			case '1': // кнопка чтобы считать данные из файла и продолжить играть уже созданный турнир
			{
				database_reading(f_teams, &count_of_teams, &arr_of_teams, f_games, &arr_of_games);
				flag = 1;
				break;
			}
			case '2': // кнопка чтобы удалить данные и начать новый турнир
			{
				tournament_restart(f_teams, f_games, arr_of_teams, count_of_teams, arr_of_games);
				while (1) {

					printf("\n\t \t \t \t \t \t \t \t \tВведите количество команд, принимающих участие в турнире\n\n\t \t \t \t \t \t \t \t \t");
					scanf("%d", &count_of_teams);

					if (count_of_teams % 2 != 0) {
						printf("\n\t \t \t \t \t \t \t \t \tВведите четное количество команд\n ");
					}

					if (count_of_teams == 0) {
						printf("\n\t \t \t \t \t \t \t \t \tВведите ненулевое количество команд\n ");
					}

					if (count_of_teams % 2 == 0 && count_of_teams != 0) {

						arr_of_teams = (struct team*)calloc(count_of_teams, sizeof(team)); // выделение памяти на массив структур

						arr_of_games = (int***)calloc(count_of_teams, sizeof(int**)); // выделение памяти на трехмерный массив
						for (int i = 0; i < count_of_teams; i++)
						{
							arr_of_games[i] = (int**)calloc(count_of_teams, sizeof(int*));
							for (int j = 0; j < count_of_teams; j++)
							{
								arr_of_games[i][j] = (int*)calloc(3, sizeof(int));
							}
						}
						break;
					}
				}
				flag = 1;

				FILE* f_teams = fopen(argv[1], "rb+"); // открытие бинарного файла для записи и чтения
				FILE* f_games = fopen(argv[2], "rb+"); // открытие бинарного файла для записи и чтения

				break;
			}
			default:
			{
				printf("\n\n\t \t \t \t \t \t \tТакой команды не существует. Нажмите клавишу действительной команды\n");
				break;
			}
			}
		}
		system("cls");

	}
	else // если в базе данных нет никаких данных
	{
		while (1) {

			printf("\n\t \t \t \t \t \t \t \t \tВведите количество команд, принимающих участие в турнире\n\n\t \t \t \t \t \t \t \t \t \t");
			scanf("%d", &count_of_teams);

			if (count_of_teams % 2 != 0) {
				printf("\n\t \t \t \t \t \t \t \t \tВведите четное количество команд\n ");
			}

			if (count_of_teams == 0) {
				printf("\n\t \t \t \t \t \t \t \t \tВведите ненулевое количество команд\n ");
			}

			if (count_of_teams % 2 == 0 && count_of_teams != 0) {

				arr_of_teams = (struct team*)calloc(count_of_teams, sizeof(team)); // выделение памяти на массив структур

				arr_of_games = (int***)calloc(count_of_teams, sizeof(int**)); // выделение памяти на трехмерный массив
				for (int i = 0; i < count_of_teams; i++)
				{
					arr_of_games[i] = (int**)calloc(count_of_teams, sizeof(int*));
					for (int j = 0; j < count_of_teams; j++) {
						arr_of_games[i][j] = (int*)calloc(3, sizeof(int));
					}
				}
				break;
			}
		}
	}

	main_menu();

	int action = _getch();

	while (action != '6') { // реализация управления при помощи кнопок

		switch (action)
		{
		case '1': // кнопка для ввода или изменения названий команд
		{
			system("cls");
			
			if (strlen(arr_of_teams[0].name) == 0) // если сведений о названиях нет то программа попросит их ввести
			{
				printf("\t \t \t \t \t \t \t \t \tВ базе данных нет сведений о названиях команд, введите их\n\n");
				team_name_input(arr_of_teams, count_of_teams, f_teams);
				printf("\t \t \t \t \t \t \t \t \tНазвания команд успешно введены. Для выхода в меню - нажмите клавишу 1.");
			}
			else // если сведения есть то доступно изменение названия команды
			{
				printf("\t \t \t \t \t \t \t \t \tВ базе данных есть сведения о названиях команд. Название какой команды вы хотите изменить?\n\n");
				printf("\t \t \t \t \t \t \t \t \t");
				team_name_edit(arr_of_teams, count_of_teams, f_teams);
			}
			_getch();
			system("cls");
			main_menu();
			break;
		}
		case '2': // кнопка игры или изменения матчей
		{
			system("cls");
			if (strlen(arr_of_teams[0].name) == 0) // сначала нужно ввести названия команд чтобы играть турнир
			{
				printf("\n\n\t \t \t \t \t \t \t \t \tДля начала турнира сначала введите названия команд");
				printf("\n\n\t \t \t \t \t \t \t \t \tДля выхода в меню - нажмите клавишу 1");
				_getch();
				system("cls");
				main_menu();
				break;
			}
			else // подраздел с выбором играть тур или изменить матч
			{
				printf("\n\n\t \t \t \t \t \t \t \t \t \t \tCписок доступных действий:\n\n\t");
				printf("\t \t \t \t \t \t \t \tНачало или продолжение турнира (Нажмите клавишу 1)\n\n\t");
				printf("\t \t \t \t \t \t \t \tИзменение результатов матчей (Нажмите клавишу 2)\n\n\t");
				printf("\t \t \t \t \t \t \t \tВыход в меню (Нажмите клавишу 3)\n\n\t \t \t \t \t \t \t \t \t");
				int flag_2 = 0;
				int flag_3 = 0;
				while (flag_2 == 0) {
					int action_tournament_start = _getch();
					switch (action_tournament_start)
					{
					case '1': // играть тур
					{
						tournament_play(arr_of_teams, count_of_teams, arr_of_games, f_games, f_teams);
						flag_2 = 1;
						break;
					}
					case '2': // изменить результат матча
					{
						tournament_edit(arr_of_teams, count_of_teams, arr_of_games, f_games, f_teams);
						flag_2 = 1;
						break;
					}
					case '3': // выйти в меню
					{
						flag_2 = 1;
						flag_3 = 1;
						break;
					}
					default:
					{
						printf("Такой команды не существует. Нажмите клавишу действительной команды\n");
						printf("\n\n\t \t \t \t \t \t \t \t \t");
						break;
					}
					}
				}
				if (flag_3 != 1) _getch();
				system("cls");
				main_menu();
				break;
			}
		}
		case '3': // кнопка вывода таблицы сыгранных матчей
		{
			system("cls");
			played_games_table_output(arr_of_teams, count_of_teams, arr_of_games);
			printf("\n\n\t \t \t \tДля выхода в меню - нажмите клавишу 1.");
			_getch();
			system("cls");
			main_menu();
			break;
		}
		case '4': // кнопка вывода полной турнирной таблицы
		{
			system("cls");
			team* arr_of_teams_copy = (struct team*)calloc(count_of_teams, sizeof(team));
			for (int i = 0; i < count_of_teams; i++) arr_of_teams_copy[i] = arr_of_teams[i];
			team_sorting(arr_of_teams_copy, count_of_teams);
			full_table_output(arr_of_teams_copy, count_of_teams);
			printf("\n\n\t \t \t \tДля выхода в меню - нажмите клавишу 1.");
			_getch();
			system("cls");
			free(arr_of_teams_copy);
			main_menu();
			break;
		}
		case '5': // кнопка рестарта турнира
		{
			system("cls");
			tournament_restart(f_teams, f_games, arr_of_teams, count_of_teams, arr_of_games);
			printf("\t \t \t \t \t \t \t \t \tДанные турнира очищены.\n\n\t \t \t \t \t \t \t \t \t");
			printf("Для начала нового турнира нажмите клавишу 1.\n\n\t \t \t \t \t \t \t \t \t");
			printf("Для выхода из программы нажмите клавишу 2.\n\n\t \t \t \t \t \t \t \t \t");
			int flag_1 = 0;
			while (flag_1 == 0) {
				int action_tournament_restart = _getch();
				switch (action_tournament_restart)
				{
				case '1': // если необходимо начать новый турнир
				{
					while (1) {

						printf("\n\t \t \t \t \t \t \t \t \tВведите количество команд, принимающих участие в турнире\n\n\t \t \t \t \t \t \t \t \t");
						scanf("%d", &count_of_teams);
						if (count_of_teams % 2 != 0) {
							printf("\n\t \t \t \t \t \t \t \t \tВведите четное количество команд\n ");
						}

						if (count_of_teams == 0) {
							printf("\n\t \t \t \t \t \t \t \t \tВведите ненулевое количество команд\n ");
						}

						if (count_of_teams % 2 == 0 && count_of_teams != 0) {

							arr_of_teams = (struct team*)calloc(count_of_teams, sizeof(team)); // выделение памяти на массив структур

							arr_of_games = (int***)calloc(count_of_teams, sizeof(int**)); // выделение памяти на трехмерный массив
							for (int i = 0; i < count_of_teams; i++)
							{
								arr_of_games[i] = (int**)calloc(count_of_teams, sizeof(int*));
								for (int j = 0; j < count_of_teams; j++) {
									arr_of_games[i][j] = (int*)calloc(3, sizeof(int));
								}
							}
							break;
						}
					}

					FILE* f_teams = fopen(argv[1], "rb+"); // открытие бинарного файла для записи и чтения
					FILE* f_games = fopen(argv[2], "rb+"); // открытие бинарного файла для записи и чтения
					flag_1 = 1;
					main_menu();
					break;
				}
				case '2': // если необходимо выйти из программы
				{
					exit(0);
					break;
				}
				default:
				{
					printf("Такой команды не существует. Нажмите клавишу действительной команды\n");
					printf("\n\n\t \t \t \t \t \t \t \t \t");
					break;
				}
				}
			}
			break;
		}
		default:
		{
			printf("Такой команды не существует. Нажмите клавишу действительной команды\n");
			printf("\n\n\t \t \t \t \t \t \t \t \t");
			break;
		}
		}
		action = _getch();
	}

	free(arr_of_teams); // освобождение памяти

	for (int i = 0; i < count_of_teams; i++) {
		for (int j = 0; j < count_of_teams; j++) {
			free(arr_of_games[i][j]);
		}
		free(arr_of_games[i]);
	}
	free(arr_of_games);

	return 0;
}