#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <stdlib.h>

int toMenuFlag;

void interrupt(*int_old_9)(...);

void interrupt int_new_9(...) {
	unsigned char code = inp(0x60);
	printf("%x\n", code);
	if (code == 129) {
		toMenuFlag = 1;
	}
	int_old_9();
}


void indicators() {
	printf("===========LEDs blinking===========\n");
	int attemptsAmount = 3;
	int canWriteFlag = 0;
	int i;
	char mask = 1;
	for (i = 0; i < 3; i++) {
		while (attemptsAmount > 0) {
			if ((inp(0x64) & 0x02) == 0x00) {
				canWriteFlag = 1;
				break;
			}
			attemptsAmount--;
		}

		if (!canWriteFlag) {
			printf("Cannot write to keyboard!\n");
			return;
		}
		outp(0x60, 0xED);
		if (inp(0x60) == 0xFA) {
			printf("Transfer success\n");
			attemptsAmount = 3;
			canWriteFlag = 0;
			while (attemptsAmount > 0) {
				if ((inp(0x64) & 0x02) == 0x00) {
					canWriteFlag = 1;
					break;
				}
				attemptsAmount--;
			}
			if (!canWriteFlag) {
				printf("Cannot write to keyboard!\n");
				return;
			}
			outp(0x60, mask);
		}
		mask *= 2;
		delay(1000);
		attemptsAmount = 3;
	}
	while (attemptsAmount > 0) {
		if ((inp(0x64) & 0x02) == 0x00) {
			canWriteFlag = 1;
			break;
		}
		attemptsAmount--;
	}
	
	if (!canWriteFlag) {
		printf("Cannot write to keyboard!\n");
		return;
	}
	outp(0x60, 0xED);
	if (inp(0x60) == 0xFA) {
		printf("Transfer success\n");
		attemptsAmount = 3;
		canWriteFlag = 0;
		while (attemptsAmount > 0) {
			if ((inp(0x64) & 0x02) == 0x00) {
				canWriteFlag = 1;
				break;
			}
			attemptsAmount--;
		}
		if (!canWriteFlag) {
			printf("Cannot write to keyboard!\n");
			return;
		}
		outp(0x60, 0x00);
	}
}


void returnCodes() {
	toMenuFlag = 0;
	int_old_9 = getvect(0x09);
	disable();
	setvect(0x09, int_new_9);
	enable();
	printf("===========Return codes===========\n");
	printf("Press ESC to exit\n");
	while (!toMenuFlag);
	disable();
	setvect(0x09, int_old_9);
	enable();
}


int main(){
	while (1) {
		system("cls");
		printf("===========Lab6===========\n");
		printf("1. LEDs blinking\n");
		printf("2. Buttons' codes\n");
		printf("3. Exit\n");
		printf("Choose the option\n");
		switch (getch()){
		case '1': 
			system("cls");
			indicators();
			system("pause");
			break;
		case '2':
			system("cls");
			returnCodes();
			system("pause");
			break;
		case '3':
			return 0;
		default:
			break;
		}
	}
}
