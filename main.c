#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <Windows.h>

struct Bus {
	int busNumber;
	char source[50];
	char destination[50];
	int totalSeats;
	int availableSeats;
	float fare;
};

struct Passenger {
	char name[50];
	int age;
	int seatNumber;
	int busNumber;
};

struct User {
	char username[50];
	char password[50];
};

void displayMainMenu()
{
	printf("-----Система бронювання автобусів-----\n");
	printf("1.Увійти\n");
	printf("2.Вийти\n");
	printf("Виберіть функцію: ");
}

// Function to display the user menu 
void displayUserMenu()
{
	printf("\n----- Меню користувача -----\n");
	printf("1. Замовити квиток\n");
	printf("2. Скасувати квиток\n");
	printf("3. Перевірити стан автобусу\n");
	printf("4. Вийти\n");
	printf("Виберіть функцію: ");
}

// Function to perform user login 
int loginUser(struct User users[], int numUsers,
	char username[], char password[])
{
	for (int i = 0; i < numUsers; i++) {
		if (strcmp(users[i].username, username) == 0
			&& strcmp(users[i].password, password) == 0) {
			return i; // Return the index of the logged-in 
			// user 
		}
	}
	return -1; // Return -1 if login fails 
}

// Function to book a ticket 
void bookTicket(struct Bus buses[], int numBuses,
	struct Passenger passengers[],
	int* numPassengers, int userId)
{
	printf("\nВведіть номер автобуса: ");
	int busNumber;
	scanf("%d", &busNumber);

	// Find the bus with the given busNumber 
	int busIndex = -1;
	for (int i = 0; i < numBuses; i++) {
		if (buses[i].busNumber == busNumber) {
			busIndex = i;
			break;
		}
	}

	if (busIndex == -1) {
		printf("Автобус з номером %d не знайдено.\n",
			busNumber);
	}
	else if (buses[busIndex].availableSeats == 0) {
		printf("На жаль, автобус повністю заброньовано.\n");
	}
	else {
		printf("Введіть ім'я пасажира: ");
		scanf("%s", passengers[*numPassengers].name);

		printf("Введіть вік пасажира: ");
		scanf("%d", &passengers[*numPassengers].age);

		// Assign a seat number to the passenger 
		passengers[*numPassengers].seatNumber
			= buses[busIndex].totalSeats
			- buses[busIndex].availableSeats + 1;

		// Update the passenger's bus number 
		passengers[*numPassengers].busNumber = busNumber;

		// Update available seats 
		buses[busIndex].availableSeats--;

		printf("Квиток успішно заброньовано!\n");
		(*numPassengers)++;
	}
}

// Function to cancel a ticket 
void cancelTicket(struct Bus buses[], int numBuses,
	struct Passenger passengers[],
	int* numPassengers, int userId)
{
	printf("\nВведіть ім'я пасажира: ");
	char name[50];
	scanf("%s", name);

	int found = 0;
	for (int i = 0; i < *numPassengers; i++) {
		if (strcmp(passengers[i].name, name) == 0
			&& passengers[i].busNumber
			== buses[userId].busNumber) {
			// Increase available seats 
			int busIndex = -1;
			for (int j = 0; j < numBuses; j++) {
				if (buses[j].busNumber
					== passengers[i].busNumber) {
					busIndex = j;
					break;
				}
			}
			buses[busIndex].availableSeats++;

			// Remove the passenger entry 
			for (int j = i; j < (*numPassengers) - 1; j++) {
				passengers[j] = passengers[j + 1];
			}
			(*numPassengers)--;
			found = 1;
			printf("Квиток успішно скасовано!\n");
			break;
		}
	}
	if (!found) {
		printf("Пасажира з ім'ям %s не знайдено в цьому автобусі.\n", name);
	}
}

// Function to check bus status 
void checkBusStatus(struct Bus buses[], int numBuses,
	int userId)
{
	printf("\nНомер автобуса: %d\n", buses[userId].busNumber);
	printf("Джерело: %s\n", buses[userId].source);
	printf("Пункт призначення: %s\n", buses[userId].destination);
	printf("Кількість місць: %d\n", buses[userId].totalSeats);
	printf("Доступні місця: %d\n",
		buses[userId].availableSeats);
	printf("Ціна: %.2f\n", buses[userId].fare);
}

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	struct User users[5] = {
		{ "Stanislav", "password1" }
	};
	int numUsers = 5;

	// Initialize bus data 
	struct Bus buses[3] = {
		{ 101, "Місто A", "Місто B", 50, 50, 25.0 },
		{ 102, "Місто C", "Місто D", 40, 40, 20.0 },
		{ 103, "Місто E", "Місто F", 30, 30, 15.0 },
	};
	int numBuses = 3;

	struct Passenger
		passengers[500]; // Array to store passenger 
	// information 
	int numPassengers = 0; // Number of passengers 

	int loggedInUserId = -1; // Index of the logged-in user 

	while (1) {
		if (loggedInUserId == -1) {
			displayMainMenu();
			int choice;
			scanf("%d", &choice);

			if (choice == 1) {
				char username[50];
				char password[50];

				printf("Введіть логін: ");
				scanf("%s", username);
				printf("Введіть пароль: ");
				scanf("%s", password);

				loggedInUserId = loginUser(
					users, numUsers, username, password);
				if (loggedInUserId == -1) {
					printf("Помилка! Будь ласка перевірьте "
						"ваш логін і пароль.\n");
				}
				else {
					printf(
						"Успішний вхід. Ласкаво просимо, %s!\n",
						username);
				}
			}
			else if (choice == 2) {
				printf("Вихід із системи.\n");
				break;
			}
			else {
				printf(
					"Неправильний вибір! Спробуйте ще раз.\n");
			}
		}
		else {
			displayUserMenu();
			int userChoice;
			scanf("%d", &userChoice);

			switch (userChoice) {
			case 1:
				bookTicket(buses, numBuses, passengers,
					&numPassengers, loggedInUserId);
				break;
			case 2:
				cancelTicket(buses, numBuses, passengers,
					&numPassengers,
					loggedInUserId);
				break;
			case 3:
				checkBusStatus(buses, numBuses,
					loggedInUserId);
				break;
			case 4:
				printf("Вихід.\n");
				loggedInUserId = -1;
				break;
			default:
				printf("Неправильний вибір! Спробуйте ще раз.\n");
			}
		}
	}

	return 0;
}
