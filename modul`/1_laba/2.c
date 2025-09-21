#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<time.h>
#define ALL_GOOD 0
#define MEMORY_ERROR -1
#define ERROR_OF_AUTHORIZATION -2
#define ERROR_OF_REGISTRATION -3
#define HOWMUCH_ERROR -4
#define DATA_FORMAT_ERROR -5
#define YEAR_BEFORE_1900 -6
#define UNKNOWN_FLAG -7
#define SANCTIONS_ERROR -8
#define INVALID_ARGUMENT -9
#define INVALID_ARGUMENT_MENU -10
#define USER_FOR_BLOCK_NOT_FOUND -11
#define NOT_NUMBER -12
#define TO_MENU 55555555
#define CLOSE_SHELL 82393149

typedef struct
{
	char** _logins;
	unsigned int *_passwords;
	int* block;

	size_t _capacity;
	size_t _size;
}users;

int init_struct_users(users* arr, size_t capacity)
{
	arr->_logins = (char**)malloc(capacity * sizeof(char*));
	arr->_passwords = (unsigned int*)malloc(capacity * sizeof(unsigned int));
	arr->block = (int*)malloc(capacity * sizeof(int));

	if ((arr->_passwords == NULL) || (arr->_logins == NULL) || (arr->block == NULL))
	{
		return MEMORY_ERROR; 
	}
	arr->_capacity = capacity;
	arr->_size = 0;
	return ALL_GOOD;
}

void clean(users* arr)
{
	//clean logins
	for (int i = 0; i < arr->_size; ++i) 
	{
		if (arr->_logins[i] != NULL) 
		{
			free(arr->_logins[i]);
			arr->_logins[i] = NULL;
		}
	}
	if (arr->_logins != NULL)
	{
		free(arr->_logins);
		arr->_logins = NULL;
	}

	//clean passwords
	if (arr->_passwords != NULL)
	{
		free(arr->_passwords);
		arr->_passwords = NULL;
	}

	if (arr->block != NULL)
	{
		free(arr->block);
		arr->block = NULL;
	}

	arr->_size = 0;
	arr->_capacity = 0;
}

//adding user
int add_user(users* arr_of_users, char const* login, int password)
{
	//if array is full
	if (arr_of_users->_size >= arr_of_users->_capacity)
	{
		size_t new_capacity = arr_of_users->_capacity << 1;
		char** new_logins = (char**)realloc(arr_of_users->_logins, new_capacity * sizeof(char*));
		unsigned int* new_passwords = (unsigned int*)realloc(arr_of_users->_passwords, new_capacity * sizeof(unsigned int));
		int* new_block = (int*)realloc(arr_of_users->block, new_capacity * sizeof(int));
		if (new_logins == NULL || new_passwords == NULL || new_block == NULL)
		{
			clean(arr_of_users);
			return MEMORY_ERROR;
		}

		arr_of_users->_logins = new_logins;
		arr_of_users->_passwords = new_passwords;
		arr_of_users->block = new_block;

		for (size_t i = arr_of_users->_capacity; i < new_capacity; i++)
		{
			arr_of_users->block[i] = 0;
		}

		arr_of_users->_capacity = new_capacity;
	}

	arr_of_users->_logins[arr_of_users->_size] = (char*)malloc((strlen(login) + 1) * sizeof(char));
	strcpy(arr_of_users->_logins[arr_of_users->_size], login);

	arr_of_users->_passwords[arr_of_users->_size] = password;
	arr_of_users->block[arr_of_users->_size] = 0;
	++(arr_of_users->_size);

	int index_of_user = arr_of_users->_size - 1;
	return index_of_user;
}

void print_time()
{
	time_t now = time(NULL);
	struct tm* local = localtime(&now);
	printf("\nTime: %02d:%02d:%02d\n", local->tm_hour, local->tm_min, local->tm_sec);
}

void print_date()
{
	time_t now = time(NULL);
	struct tm* local = localtime(&now);
	printf("\nDate: %02d:%02d:%02d\n", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
}

int print_howmuch(char const* date, char const* time_, char const* flag)
{
	time_t now = time(NULL);
	struct tm* local = localtime(&now);

	struct tm input_time = { 0 };
	int res_of_pars = parsing_time(date, time_, &input_time, local);
	if (res_of_pars == DATA_FORMAT_ERROR)
	{
		return DATA_FORMAT_ERROR;
	}
	if (res_of_pars == YEAR_BEFORE_1900)
	{
		return YEAR_BEFORE_1900;
	}

	time_t data1 = mktime(&input_time);
	time_t data2 = mktime(local);
	if (data1 == -1 || data2 == -1) 
	{
		return DATA_FORMAT_ERROR;
	}
	unsigned int second = difftime(data1, data2);

	if (strcmp("-s", flag) == 0)
	{
		printf("\n%u\n", second);
	}
	else if (strcmp("-m", flag) == 0)
	{
		printf("\n%u\n", second / 60);
	}
	else if (strcmp("-h", flag) == 0)
	{
		printf("\n%u\n", second / 3600);
	}
	else if (strcmp("-y", flag) == 0)
	{
		printf("\n%u\n", local->tm_year - input_time.tm_year);
	}
	else
	{
		return UNKNOWN_FLAG;
	}

	return ALL_GOOD;
}

int parsing_time(char const* date, struct tm* time, struct tm const* local)
{
	int day, month, year, hour, minute, second;
	if (sscanf(date, "%d:%d:%d", &day, &month, &year) != 3)
	{
		return DATA_FORMAT_ERROR;
	}
	if (sscanf(_time, "%d:%d:%d", &hour, &minute, &second) != 3)
	{
		return DATA_FORMAT_ERROR;
	}
	if ((day > 31 || day < 1) ||
		(month < 0 || month > 11) ||
		(year < 0 || year > local->tm_year) ||
		(hour < 0 || hour > 23) ||
		(minute < 0 || minute  > 59) ||
		(second < 0 || second > 60))
	{
		return DATA_FORMAT_ERROR;
	}

	time->tm_mday = day;
	time->tm_mon = month - 1;
	time->tm_year = year - 1900;
	time->tm_hour = hour;
	time->tm_min = minute;
	time->tm_sec = second;
	to_write->tm_isdst = -1;
	return ALL_GOOD;
}

int make_sanctions(char const* username, users* arr)
{
	int index = find_index_of_user_login(arr, username);
	if ( index >= 0)
	{
		arr->block[index] = 1;
		return ALL_GOOD;
	}
	if (index == -1)
	{
		return USER_FOR_BLOCK_NOT_FOUND;
	}
}

int menu(users* arr)
{
	char input[BUFSIZ];
	int have_an_acc = 0;
	int index_of_user;
	printf("Hi!! do you have acc?\n Yes - 1\n No - 0\n");
  	if(scanf("%d", &have_an_acc) != 1)
	{
		while (getchar() != '\n');
		return NOT_NUMBER;
	}

	if (have_an_acc == 0)
	{
		int res_of_registration = registration(arr);
		if (res_of_registration == ERROR_OF_REGISTRATION)
		{
			while (getchar() != '\n');
			return ERROR_OF_REGISTRATION;
		}
		else if (res_of_registration == MEMORY_ERROR)
		{
			return MEMORY_ERROR;
		}
		index_of_user = res_of_registration;
	}
	else if (have_an_acc == 1)
	{
		int res_of_authorization = authorization(arr);
		if (res_of_authorization == ERROR_OF_AUTHORIZATION)
		{
			while (getchar() != '\n');
			return ERROR_OF_AUTHORIZATION;
		}
		index_of_user = res_of_authorization;
	}
	else
	{
		return INVALID_ARGUMENT_MENU;
	}
	

	if (arr->block[index_of_user] == 0)
	{
		printf("\nYou can start using it!\n");
		
		printf("Available commands: \n");
		printf("Time\n");
		printf("Date\n");
		printf("Howmuch <datetime> flag\n");
		printf("Logout\n");
		printf("Sanctions <username>\n");

		while (strcmp("Logout", input) != 0)
		{
			while (getchar() != '\n');
			
			printf("\nEnter command: ");
			fgets(input, BUFSIZ, stdin);
			
			size_t len = strlen(input);
			while (len > 0 && isspace((unsigned char)input[len - 1])) {
				input[len - 1] = '\0';
				--len;
			}
			
			if (strcmp(input, "Time") == 0)
			{
				print_time();
			}
			else if (strcmp(input, "Date") == 0)
			{
				print_date();
			}
			else if (strncmp(input, "Howmuch", 7) == 0)
			{
				
				char _date[20];
				char _time[20];
				char flag[10];
				int parsed = sscanf(input + 8, "%19s %19s %9s", _date, _time, flag);

				if (parsed == 3)
				{
					int res_of_howmuch = print_howmuch(_date, _time, flag);
					if (res_of_howmuch == DATA_FORMAT_ERROR)
					{
						printf("Format of data is not right");
					}
					if (res_of_howmuch == YEAR_BEFORE_1900)
					{
						printf("The year must be more than 1900.");
					}
					if (res_of_howmuch == UNKNOWN_FLAG)
					{
						printf("Unknow flag.");
					}
				}
				else
				{
					printf("Unknow command.");
				}
			}
			else if (strncmp(input, "Sanctions", 9) == 0)
			{
				char username[8];
				if (sscanf(input + 10, "%7s", username) == 1)
				{
					if (strlen(username) > 6)
					{
						printf("The login must be not more than 6");
					}
					int res_of_sanction = make_sanctions(username, arr);
					if (res_of_sanction == ALL_GOOD)
					{
						printf("52\n");
					}
					if (res_of_sanction == USER_FOR_BLOCK_NOT_FOUND)
					{
						printf("\nUser not found\n");
					}
				}
				else
				{
					printf("Unknow command");
				}
			}
			else if (strcmp(input, "Logout") == 0)
			{
				break; 
			}
			else
			{
				printf("Unknown command\n");
			}
		}
		return TO_MENU;
	}
	else
	{
		printf("Your account is blocked!\n");
		return CLOSE_SHELL;
	}
}


int find_index_of_user_login(users const* arr, char const* login)
{
	for (int i = 0; i < arr->_size; ++i) 
	{
		if (strcmp(login, arr->_logins[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

int authorization(users const* arr)//return index of user
{
	char log[BUFSIZ];
	unsigned int pass;
	size_t index_of_log;

	printf("\nLogin: ");
	scanf("%s", log);
	if (strlen(log) > 6)
	{
		return ERROR_OF_AUTHORIZATION;
	}

	index_of_log = find_index_of_user_login(arr, log);
	if (index_of_log >= 0)
	{
		printf(" good!\nPassword: ");
		int res = scanf("%d", &pass);

		if (res != 1) //its not number
		{
			while (getchar() != '\n');
			return ERROR_OF_AUTHORIZATION;
		}
		else if (pass > 100000)
		{
			return ERROR_OF_AUTHORIZATION;
		}

		if (pass == arr->_passwords[index_of_log])
		{
			return index_of_log;
		}
		else
		{
			return ERROR_OF_AUTHORIZATION;
		}
	}
	else
	{
		return ERROR_OF_AUTHORIZATION;
	}
}

int registration(users* arr) //return index_of_user
{
	char log[BUFSIZ];
	unsigned int pass;
	printf("\nLogin(max size = 6): ");
	scanf("%s", &log);
	if (strlen(log) > 6)
	{
		return ERROR_OF_REGISTRATION;
	}

	printf("\nPassword(from 0 to 100000): ");
	int res = scanf("%d", &pass);
	if (res != 1)
	{
		return ERROR_OF_REGISTRATION;
	}
	else if (pass > 100000)
	{
		return ERROR_OF_REGISTRATION;
	}

	int res_of_add = add_user(arr, log, pass);
	if (res_of_add >= ALL_GOOD)
	{
		return res_of_add;
	}
	else
	{
		return MEMORY_ERROR;
	}
}


int main()
{
	
	users users_;                           
	if (init_struct_users(&users_, 1) == -1)  
	{
		printf("memory allocation error :(");
		return MEMORY_ERROR;
	}

	int lol = 1;
	while (lol) {
		switch (menu(&users_))
		{
		case ERROR_OF_REGISTRATION:
			printf("Error: invalid login or password. Try again:\n");
			break;

		case MEMORY_ERROR:
			printf("Error: memory error.\n");
			lol = 0;
			break;

		case ERROR_OF_AUTHORIZATION:
			printf("Error: invalid login or password. Try again:\n");
			break;

		case INVALID_ARGUMENT_MENU:
			printf("Error: invalid argument in menu. You must enter 1 or 0. Try again:\n");
			break;

		case TO_MENU:
			break;

		case CLOSE_SHELL:
			break;
		default:
			printf("idk bro hehe");
			break;
		}
	}
	clean(&users_);
	return ALL_GOOD;
}
