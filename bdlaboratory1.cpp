// bdlaboratory1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
void menus();
struct Order {
	int id;
	unsigned int price;
	int cook;
	bool is_exist = 1;
};
struct Cook {
	int id;
	char name[20];
	int exp;
	int salary;
	bool is_exist = 1;
};
void get_m(int id) {
	FILE *ck = fopen("cook.bin", "rb");
	fseek(ck, 0, 2);
	int k = ftell(ck) / sizeof(struct Cook);
	if (k< id) {
		fclose(ck);
		menus();
	}
	struct Cook cook;
	fseek(ck, id * sizeof(struct Cook), 0);
	fread(&cook, sizeof(struct Cook), 1, ck);
	if (cook.is_exist == 1) {
		cout << endl;
		cout << "Name: " << cook.name << endl;
		cout << "Experience: " << cook.exp << endl;
		cout << "Salary: " << cook.salary << endl;
	}
	else cout << "THERE IS NO COOK WITH THIS IS\n";
	fclose(ck);
	menus();
}
void get_s(int id) {
	FILE *or = fopen("order.bin", "rb");
	fseek(or, 0, 2);
	int k = ftell(or) / sizeof(struct Order);
	if (k< id)
		{
		fclose(or);
		menus();
	}
	struct Order order;
	fseek(or, id * sizeof(struct Order), 0);
	fread(&order, sizeof(struct Order), 1, or);
	if (order.is_exist == 1) {
		cout << endl;
		cout << "PRICE: " << order.price << endl;
		cout << "COOK ID: " << order.cook << endl;
	}
	else cout << "THERE IS NO ORDER WITH THIS ID\n";
	fclose(or);
	menus();
}
void get_s_m(int id) {
	FILE * or = fopen("order.bin", "rb");
/*	fseek(or , 0, 2);
	if (id > ftell(or ) / sizeof(struct Order)) {
		fclose(or );
		menus();
	}*/
	struct Order order;
//	fseek(or , 0, 0);
	while (fread(&order, sizeof(struct Order), 1, or )!=NULL) {
		if (order.cook==id && order.is_exist == 1) {
			cout << endl;
			cout << "ID: " << order.id << endl;
			cout << "PRICE: " << order.price << endl;
			cout << "COOK ID: " << order.cook << endl;
		}
	}
	fclose(or );
	menus();
}
void get_menu() {
	cout << endl << "\n1-GET COOK\n2-GET ORDER\n3-GET ALL ORDERS OF ONE COOK\n";
	int choose;
	int id;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >3) menus();
	cout << "\nenter ID\nID: ";
	cin >> id;
	cin.ignore();
	switch (choose) {
	case 1: get_m(id); break;
	case 2: get_s(id); break;
	case 3: get_s_m(id); break;
	}
}
void del_m(int id) {
	FILE * or = fopen("order.bin", "rb");
	struct Order order;
	while (fread(&order, sizeof(struct Order), 1, or )!=NULL) {
		if (order.cook == id && order.is_exist == 1) {
			cout << "\n YOU CANT DELETE COOK WHILE U HAVE ORDERS\n";
			fclose(or);
			menus();
		}
	}
	FILE *ck = fopen("cook.bin", "rb");
	FILE *swap = fopen("swap.bin", "wb");
	struct Cook cook;
	while (fread(&cook, sizeof(struct Cook), 1, ck) != NULL) {
		fwrite(&cook, sizeof(struct Cook), 1, swap);
	}
	fclose(ck); fclose(swap);
	ck = fopen("cook.bin", "wb");
	swap = fopen("swap.bin", "rb");
	while (fread(&cook, sizeof(struct Cook), 1, swap) != NULL) {
		if (cook.id == id)cook.is_exist = 0;
		fwrite(&cook, sizeof(struct Cook),1, ck);
	}
	fclose(ck);
	fclose(swap);
	swap = fopen("swap.bin", "wb");
	fclose(swap);
	menus();
}
void del_s(int id) {
	FILE * or = fopen("order.bin", "rb");
	FILE *swap = fopen("swap.bin", "wb");
	struct Order order;
	while (fread(&order, sizeof(struct Order), 1, or ) != NULL) {
		fwrite(&order, sizeof(struct Order), 1, swap);
	}
	fclose(or ); fclose(swap);
	or = fopen("order.bin", "wb");
	swap = fopen("swap.bin", "rb");
	while (fread(&order, sizeof(struct Order), 1, swap) != NULL) {
		if (order.id == id)order.is_exist = 0;
		fwrite(&order, sizeof(struct Order), 1, or);
	}
	fclose(or);
	fclose(swap);
	swap = fopen("swap.bin", "wb");
	fclose(swap);
	menus();
}
void del_menu() {
	cout << endl << "\n1-DEL COOK\n2-DEL ORDER\n";
	int choose;
	int id;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >2) menus();
	cout << "\nenter ID\nID: ";
	cin >> id;
	cin.ignore();
	switch (choose) {
	case 1: del_m(id); break;
	case 2: del_s(id); break;
	}
}
void insert_m() {
	FILE *ck = fopen("cook.bin", "ab");
	struct Cook cook;
	fseek(ck, 0, 2);
	cook.id = ftell(ck) / sizeof(struct Cook);
	cout << endl;
	cout << "\nENTER\nNAME: ";
	cin.getline(cook.name, 20);
	cout << "EXPERIENCE: ";
	cin >> cook.exp;
	cin.ignore();
	cout << "SALARY: ";
	cin >> cook.salary;
	cin.ignore();
	fwrite(&cook, sizeof(struct Cook), 1, ck);
	fclose(ck);
	menus();
}
void insert_s() {
	FILE *or = fopen("order.bin", "ab");
	struct Order order;
	fseek(or, 0, 2);
	order.id = ftell(or) / sizeof(struct Order);
	cout << endl;
	cout << "\nENTER\nPRICE: ";
	cin >> order.price;
	cin.ignore();
	cout << endl;
	FILE *ck = fopen("cook.bin", "rb");
	fseek(ck, 0, 2);
	cout << endl << "there is "<< ftell(ck) / sizeof(struct Cook)<< "cooks"<< endl;
	cout << "COOK ID: ";
	cin >> order.cook;
	cin.ignore();
	fwrite(&order, sizeof(struct Order), 1, or);
	fclose(ck);
	fclose(or);
	menus();
}
void insert_menu() {
	cout << endl << "\n1-INSERT COOK\n2-INSERT ORDER\n";
	int choose;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >2) menus();
	switch (choose) {
	case 1: insert_m(); break;
	case 2: insert_s(); break;
	}
}
void update_m(int id) {
	FILE *ck = fopen("cook.bin", "rb");
	struct Cook cook;
	fseek(ck, 0, 2);
	int k = ftell(ck) / sizeof(struct Cook);
	if (k< id)
		exit(0);
	fseek(ck, 0, 0);
	FILE *swap = fopen("swap.bin", "wb");
	while (fread(&cook, sizeof(struct Cook), 1, ck)!=NULL) {
		if (cook.id == id) {
			cout << "\nNAME: " << cook.name << "\nEXP: " << cook.exp << "\nSALARY: " << cook.salary << endl;
			cout << "\nENTER\nNAME: ";
			cin.getline(cook.name, 20);
			cout << "EXPERIENCE: ";
			cin >> cook.exp;
			cin.ignore();
			cout << "SALARY: ";
			cin >> cook.salary;
			cin.ignore();
			cook.is_exist = 1;
		}
		fwrite(&cook, sizeof(struct Cook), 1, swap);
	}
	fclose(ck);
	fclose(swap);
	swap = fopen("swap.bin", "rb");
	ck = fopen("cook.bin", "wb");
	while (fread(&cook, sizeof(struct Cook), 1, swap) != NULL) {
		fwrite(&cook, sizeof(struct Cook), 1, ck);
	}
	fclose(ck);
	fclose(swap);
	swap = fopen("swap.bin", "wb");
	fclose(swap);
	menus();
}
void update_s(int id) {
	FILE * or = fopen("order.bin", "rb");
	struct Order order;
	fseek(or, 0, 2);
	int k = ftell(or) / sizeof(struct Order);
	if (k< id)
		exit(0);
	fseek(or, 0, 0);
	FILE *swap = fopen("swap.bin", "wb");
	while (fread(&order, sizeof(struct Order), 1, or ) != NULL) {
		if (order.id == id) {
			cout << "\nWAS\nPRICE: " << order.price << "\nCOOK ID: " << order.cook << endl;
			cout << "\nENTER\nPRICE: ";
			cin >> order.price;
			cin.ignore();
			cout << endl;
			FILE *ck = fopen("cook.bin", "rb");
			fseek(ck, 0, 2);
			cout << endl << "there is " << ftell(ck) / sizeof(struct Cook)  << "cooks" << endl;
			fclose(ck);
			cout << "COOK ID: ";
			cin >> order.cook;
			cin.ignore();
			order.is_exist = 1;
		}
		fwrite(&order, sizeof(struct Order), 1, swap );
	}
	fclose(or);
	fclose(swap);
	swap = fopen("swap.bin", "rb");
	or = fopen("order.bin", "wb");
	while (fread(&order, sizeof(struct Order), 1, swap) != NULL) {
		fwrite(&order, sizeof(struct Order), 1, or);
	}
	fclose(or);
	fclose(swap);
	swap = fopen("swap.bin", "wb");
	fclose(swap);
	menus();
}
void update_menu() {
	cout << endl << "\n1-UPDATE COOK\n2-UPDATE ORDER\n";
	int choose;
	int id;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >2) menus();
	cout << "\nenter ID\nID: ";
	cin >> id;
	cin.ignore();
	switch (choose) {
	case 1: update_m(id); break;
	case 2: update_s(id); break;
	}
}
void ut_m() {
	FILE *ck = fopen("cook.bin", "rb");
	struct Cook cook;
	while (fread(&cook, sizeof(struct Cook), 1, ck)!=NULL){
		cout << endl;
		cout << "ID: " << cook.id << endl;
		cout << "Name: " << cook.name << endl;
		cout << "Experience: " << cook.exp << endl;
		cout << "Salary: " << cook.salary << endl;
		cout << "Exist: " << cook.is_exist << endl;
	}
	fclose(ck);
	menus();
}
void ut_s() {
	FILE * or = fopen("order.bin", "rb");
	struct Order order;
	while (fread(&order, sizeof(struct Order), 1, or ) != NULL) {
			cout << endl;
			cout << "ID: " << order.id << endl;
			cout << "PRICE: " << order.price << endl;
			cout << "COOK ID: " << order.cook << endl;
			cout << "EXIST: " << order.is_exist << endl;
	}
	fclose(or );
	menus();
}
void ut_menu() {
	cout << endl << "\n1-UT COOK\n2-UT ORDER\n";
	int choose;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >2) menus();
	switch (choose) {
	case 1: ut_m(); break;
	case 2: ut_s(); break;
	}
}
void count_m() {
	FILE *ck = fopen("cook.bin", "rb");
	int i = 0;
	struct Cook cook;
	while (fread(&cook, sizeof(struct Cook), 1, ck ) != NULL) {
		if (cook.is_exist == 1) i++;
	}
	cout << "there is " << i << " cooks\n";
	fclose(ck);
	menus();
}
void count_s() {
	FILE *or = fopen("order.bin", "rb");
	struct Order order;
	int i = 0;
	while (fread(&order, sizeof(struct Order), 1, or ) != NULL) {
		if (order.is_exist == 1) i++;
	}
	cout << "there is " << i << " orders\n";
	fclose(or);
	menus();
}
void count_s_m() {
	int id;
	struct Order order;
	cout << "\nENTER ID\nID: ";
	cin >> id;
	cin.ignore();
	FILE *or = fopen("order.bin", "rb");
	int i = 0;
	while (fread(&order, sizeof(struct Order), 1, or) != NULL) {
		if (order.cook == id && order.is_exist==1) i++;
	}
	cout << "there is " << i << " orders for this cook\n";
	fclose(or);
	menus();
}
void count_menu() {
	cout << endl << "\n1-NUMBER OF COOKS\n2-NUMBER OF ORDERS\n3-NUMBER OF ORDERS CONNECTED TO *id* cook\n";
	int choose;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >3) menus();
	switch (choose) {
	case 1: count_m(); break;
	case 2: count_s(); break;
	case 3: count_s_m(); break;
	}
}
void menus() {
	cout << endl << "\n1-GET\n2-DEL\n3-INSERT\n4-UPDATE\n5-UT\n6-COUNT\n7-EXIT\n";
	int choose;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >7) {
		cout << "\n\nERROR choose from 1 to 7\n";
		menus();
	}
	switch (choose) {
	case 1: get_menu(); break;
	case 2: del_menu(); break;
	case 3: insert_menu(); break;
	case 4: update_menu(); break;
	case 5: ut_menu(); break;
	case 6: count_menu(); break;
	case 7: exit(0); break;
	}
}
int main()
{	
	menus();
    return 0;
}

