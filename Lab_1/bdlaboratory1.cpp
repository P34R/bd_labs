// bdlaboratory1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
int menus(int *ck_ind, int *or_ind);
void del_s(int *ck_ind, int *or_ind, int id, int size_or);
struct Order {
	int id;
	unsigned int price;
//	int cook;
	bool is_exist = 1;
	int next=-1;
};
struct Cook {
	int id;
	char name[20];
	int exp;
	int salary;
	int order_id; //head
	bool is_exist = 1;
};
int size_ck_find(int *ck_ind) {
	int i = 0;
	while (ck_ind[i] > -1) {
		i++;
	}
	return i;
}
int size_or_find(int *or_ind) {
	int i = 0;
	while (or_ind[i] > -1) {
		i++;
	}
	return i;
}
int my_realloc(int *arr, int size, int new_size) {
	int *new_arr = new int[new_size];
	if (size < new_size) {
		delete[] new_arr;
		return 0;
	}
	for (int i = 0; i < new_size; i++)
		new_arr[i] = -1;
	for (int i = 0; i < size; i++)
		new_arr[i] = arr[i];
	delete[] arr;

	arr = new_arr;
	return new_size;
}
int get_m(int *ck_ind, int *or_ind, int id,int size_ck) {
//	FILE *cki = fopen("cook.ind", "rb");

/*	fseek(cki, 0, 2);
	int k = ftell(cki) / (2*sizeof(int));
	if (k< id) {
		fclose(ck);
		menus(ck_ind, or_ind);
	}*/
	struct Cook cook;
	if (id >= size_ck || id<0) return 0;
	FILE *ck = fopen("cook.bin", "rb");
	fseek(ck, ck_ind[id] * sizeof(struct Cook), 0);
	fread(&cook, sizeof(struct Cook), 1, ck);
	if (cook.is_exist == 1) {
		cout << endl;
		cout << "Name: " << cook.name << endl;
		cout << "Experience: " << cook.exp << endl;
		cout << "Salary: " << cook.salary << endl;
	}
	else cout << "THERE IS NO COOK WITH THIS IS\n";
	fclose(ck);
//	menus(ck_ind, or_ind);
	return 0;
}
int get_s(int *ck_ind, int *or_ind, int id,int size_or) {

/*	fseek(or, 0, 2);
	int k = ftell(or) / sizeof(struct Order);
	if (k< id)
		{
		fclose(or);
		menus(ck_ind, or_ind);
	}*/
	if (id >= size_or || id<0) return 0;
		FILE *or = fopen("order.bin", "rb");
	struct Order order;
	fseek(or, or_ind[id] * sizeof(struct Order), 0);
	fread(&order, sizeof(struct Order), 1, or);
	if (order.is_exist == 1) {
		cout << endl;
		cout << "PRICE: " << order.price << endl;
//		cout << "COOK ID: " << order.cook << endl;
	}
	else cout << "THERE IS NO ORDER WITH THIS ID\n";
	fclose(or);
//	menus(ck_ind, or_ind);
	return 0;
}
int get_s_m(int *ck_ind, int *or_ind, int id,int size_ck) {
	FILE * or = fopen("order.bin", "rb");
	FILE *ck = fopen("cook.bin", "rb");
	struct Cook cook;
	if (size_ck <= id || id<0) {
		fclose(ck);
		fclose(or );
		return 0;
	} 
	fseek(ck, ck_ind[id] * sizeof(Cook), 0);
	fread(&cook, sizeof(Cook), 1, ck);
	fclose(ck);
	fseek(or , or_ind[cook.order_id]*sizeof(Order), 0);
/*	if (id > ftell(or ) / sizeof(struct Order)) {
		fclose(or );
		menus(ck_ind, or_ind);
	}*/
	struct Order order;
//	fseek(or , 0, 0);
	fread(&order, sizeof(Order), 1, or );

	while (order.next!=-1) {
		if (order.is_exist == 1) {
			cout << endl;
			cout << "ID: " << order.id << endl;
			cout << "PRICE: " << order.price;
		}
		fseek(or , or_ind[order.next] * sizeof(Order), 0);
		fread(&order, sizeof(Order), 1, or );
//		order = *pointer.next;
	}
	cout << endl;
	cout << "ID: " << order.id << endl;
	cout << "PRICE: " << order.price;
/*	while (fread(&order, sizeof(struct Order), 1, or )!=NULL) {
		if (order.cook==id && order.is_exist == 1) {
			cout << endl;
			cout << "ID: " << order.id << endl;
			cout << "PRICE: " << order.price << endl;
			cout << "COOK ID: " << order.cook << endl;
		}
	}*/
	fclose(or );
//	menus(ck_ind, or_ind);
	return 0;
}
void get_menu(int *ck_ind, int *or_ind) {
	cout << endl << "\n1-GET COOK\n2-GET ORDER\n3-GET ALL ORDERS OF ONE COOK\n";
	FILE *ck_i = fopen("cook.ind", "rb");
	FILE *or_i = fopen("order.ind", "rb");
	fseek(ck_i, 0, 2);
	fseek(or_i, 0, 2);
	int size_ck = ftell(ck_i) / (2 * sizeof(int));
	int size_or = ftell(or_i) / (2 * sizeof(int));
	fclose(ck_i);
	fclose(or_i);
	int choose;
	int id;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >3) menus(ck_ind, or_ind);
	if (choose == 3) cout << "\netner Cook ID: ";
	else
	cout << "\nenter ID\nID: ";
	cin >> id;
	cin.ignore();
	switch (choose) {
	case 1: get_m(ck_ind, or_ind, ck_ind[id],size_ck); break;
	case 2: get_s(ck_ind, or_ind, or_ind[id],size_or); break;
	case 3: get_s_m(ck_ind, or_ind, ck_ind[id],size_ck); break;
	}
}
void delete_s_m(int *ck_ind, int *or_ind, int id, int size_or) {
	FILE * or = fopen("order.bin", "rb+");
	FILE *swap = fopen("swap.bin", "wb");
	struct Order order;
	int idk = id;
	//	or_ind[id] = -1;
	fseek(or , or_ind[id] * sizeof(Order), 0);
/**/	int boka = 0;
	while (boka!=1) {
		fread(&order, sizeof(Order), 1, or );
		fseek(or , or_ind[idk] * sizeof(Order), 0);
		if (order.next != -1) {
			if (order.is_exist == 0) {
				idk = order.next;
				fseek(or , or_ind[idk] * sizeof(Order), 0);
			}
			else {
				order.is_exist = 0;
				fseek(or , 0, 0);
				fseek(or , or_ind[idk] * sizeof(Order), 0);
				fwrite(&order, sizeof(Order), 1, or );
				or_ind[idk] = -1;
				idk = order.next;
				fseek(or , or_ind[idk] * sizeof(Order), 0);
			}
		}
		else {
			order.is_exist = 0;
			
			fseek(or , 0, 0);
			fseek(or , or_ind[idk] * sizeof(Order), 0);
			or_ind[idk] = -1;
			fwrite(&order, sizeof(Order), 1, or );
			boka = 1;
		}
	}
	fclose(or );
/*	while (fread(&order, sizeof(struct Order), 1, or ) != NULL) {
		if (order.id == id && order.is_exist==1) {
			order.is_exist = 0;
			or_ind[id] = -1;
			if (order.next != -1)
				id = order.next;
		}
		fwrite(&order, sizeof(struct Order), 1, swap);
	}
	fclose(or ); fclose(swap);
	or = fopen("order.bin", "wb");
	swap = fopen("swap.bin", "rb");
	while (fread(&order, sizeof(struct Order), 1, swap) != NULL) {

		fwrite(&order, sizeof(struct Order), 1, or );
	}
	fclose(swap);
	fclose(or );*/
	FILE *or_i = fopen("order.ind", "wb");
	for (int i = 0; i < size_or; i++) {
		fwrite(&i, sizeof(int), 1, or_i);
		fwrite(&or_ind[i], sizeof(int), 1, or_i);
	}
	fclose(or_i);
//	fclose(swap);
}
void del_m(int *ck_ind, int *or_ind,int id, int size_ck,int size_or){
	FILE *ck = fopen("cook.bin", "rb+");
	struct Cook cook;
/*	while (fread(&order, sizeof(struct Order), 1, or )!=NULL) {
		if (order.cook == id && order.is_exist == 1) {
			cout << "\n YOU CANT DELETE COOK WHILE U HAVE ORDERS\n";
     fseek(or,0,0);
		FILE *swap=fopen("swap.bin","wb");
		while(fread(&order,sizeof(struct Order),1,or)!=NULL){
			if (order.cook==id && order.is_exist==1){
			order.is_exist=0;
			}
			fwrite(&order,sizeof(struct Order),1,swap);
		}
		fclose(or);
		fclose(swap);
		swap=fopen("swap.bin","rb");
		or=fopen("order.bin","wb");
		while (fread(&order,sizeof(struct Order),1,swap)!=NULL)
			fwrite(&order,sizeof(struct Order),1,or);
		fclose(swap);
			
			fclose(or);
			menus(ck_ind, or_ind);
		}
	}*/
	fseek(ck, ck_ind[id] * sizeof(Cook), 0);
	fread(&cook, sizeof(Cook), 1, ck);
	fseek(ck, ck_ind[id] * sizeof(Cook), 0);
	cook.is_exist = 0;
	fwrite(&cook, sizeof(Cook), 1, ck);
	delete_s_m(ck_ind, or_ind, cook.order_id,size_or);
	fclose(ck);
/*	FILE *swap = fopen("swap.bin", "wb");
	fseek(ck, 0, 0);
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
	fclose(swap);*/
	ck_ind[id] = -1;
	FILE *ck_i = fopen("cook.ind", "wb");
	for (int i = 0; i < size_ck; i++) {
		fwrite(&i, sizeof(int), 1, ck_i);
		fwrite(&ck_ind[i], sizeof(int), 1, ck_i);
	}
	fclose(ck_i);
//	menus(ck_ind, or_ind);
}
void del_s(int *ck_ind, int *or_ind, int id,int size_or) {
	FILE * or = fopen("order.bin", "rb+");
//	FILE *swap = fopen("swap.bin", "wb");
	struct Order order;
	fseek(or , or_ind[id] * sizeof(Order), 0);
	fread(&order, sizeof(Order), 1, or );
	fseek(or , or_ind[id] * sizeof(Order), 0);
	order.is_exist = 0;
	int ord_next = order.next;
	order.next = -1;
	fwrite(&order, sizeof(Order), 1, or );

		fseek(or , 0, 0);
		while (fread(&order, sizeof(Order), 1, or ) != NULL) {
			if (order.next == id) {
				fseek(or , or_ind[order.id], 0);
				order.next = ord_next;
				fwrite(&order, sizeof(Order), 1, or );
				fseek(or , 0, 2);
			}
		}
	FILE *ck = fopen("cook.bin", "rb+");
//	if (order.next != -1) {

		struct Cook cook;
		while (fread(&cook, sizeof(Cook), 1, ck)!=NULL) {
			if (cook.order_id == id && cook.is_exist==1) {
				cook.order_id = ord_next;
				fseek(ck, ck_ind[id] * sizeof(Cook), 0);
				fwrite(&cook, sizeof(Cook), 1, ck);
				fseek(ck , 0, 2);
			}
		}
//	}
	fclose(ck);
//	or_ind[id] = -1;
/*	while (fread(&order, sizeof(struct Order), 1, or ) != NULL) {
		fwrite(&order, sizeof(struct Order), 1, swap);
	}
	fclose(or ); fclose(swap);
	or = fopen("order.bin", "wb");
	swap = fopen("swap.bin", "rb");*/
//	while (fread(&order, sizeof(struct Order), 1, swap) != NULL) {
		/*	if (order.next != NULL) {
	order = *order.next;
	FILE *ck=fopen("cook.bin", "rb");
	FILE *sw2=fopen("swap2.bin", "wb");
	struct Cook cook;
	while (fread(&cook, sizeof(Cook), 1, ck) != NULL) {
		if (cook.id == order.cook) cook.order_id = order.id;
		fwrite(&cook, sizeof(Cook), 1, sw2);
			}	
	fclose(ck); ck = fopen("cook.bin", "wb");
	fclose(sw2); sw2 = fopen("swap2.bin", "rb");
	while (fread(&cook, sizeof(Cook), 1, sw2) != NULL)
		fwrite(&cook, sizeof(Cook), 1, ck);
	}*/
//		if (order.id == id)order.is_exist = 0;
//		fwrite(&order, sizeof(struct Order), 1, or);
//	}
	or_ind[id] = -1;
	fclose(or);
//	fclose(swap);
//	swap = fopen("swap.bin", "wb");
//	fclose(swap);
	FILE *or_i = fopen("order.ind", "wb");
	for (int i = 0; i < size_or; i++) {
		fwrite(&i, sizeof(int), 1, or_i);
		fwrite(&or_ind[i], sizeof(int), 1, or_i);
	}
	fclose(or_i);
//	menus(ck_ind, or_ind);
}
void del_menu(int *ck_ind, int *or_ind) {
	cout << endl << "\n1-DEL COOK\n2-DEL ORDER\n";
	FILE *ck_i = fopen("cook.ind", "rb");
	FILE *or_i = fopen("order.ind", "rb");
	fseek(ck_i, 0, 2);
	fseek(or_i, 0, 2);
	int size_ck = ftell(ck_i) / (2 * sizeof(int));
	int size_or = ftell(or_i) / (2 * sizeof(int));
	fclose(ck_i);
	fclose(or_i);
	int choose;
	int id;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >2) menus(ck_ind, or_ind);
	cout << "\nenter ID\nID: ";
	cin >> id;
	cin.ignore();
	switch (choose) {
	case 1: del_m(ck_ind,or_ind,id,size_ck,size_or); break;
	case 2: del_s(ck_ind,or_ind,id, size_or); break;
	}
}
void insert_m(int *ck_ind, int *or_ind, int size_ck, int size_or) {
	FILE *ck = fopen("cook.bin", "rb+");
	struct Cook cook;
	int id = 0;
	if (size_or == 0) {
		fclose(ck);
		menus(ck_ind, or_ind);
	}
	int point = -1;
	if (size_ck == 0) {
		my_realloc(ck_ind, size_ck, size_ck + 1);
		point = size_ck;
		size_ck++;
	}
	for (int i = 0; i < size_ck; i++) {
			if (i == point) {
				id = i;
				i = size_or;
				break;
			}
		if ((i == size_ck - 1 && ck_ind[i] > -1)) {
			my_realloc(ck_ind, size_ck, size_ck + 1);
			size_ck += 1;
			//ck_ind = (int*)realloc(ck_ind, size_ck*sizeof(int));
		}
		if (ck_ind[i] < 0) {
			id = i;
			i = size_ck;
		}
	}
	struct Cook cook_temp;
	cout << endl << endl << "asdasdasda" << id << endl << endl << endl;
	cook.id = id;
	cout << endl;
	cout << "\nENTER\nNAME: ";
	cin.getline(cook.name, 20);
	cout << "EXPERIENCE: ";
	cin >> cook.exp;
	cin.ignore();
	cout << "SALARY: ";
	cin >> cook.salary;
	cin.ignore();
	cout << "ORDER ID: ";
	cin >> cook.order_id;
	cin.ignore();
	int checker = 0;
	int counter_s = 0;
	while (fread(&cook_temp, sizeof(Cook), 1, ck) != NULL) {
		if (cook_temp.id == id) {
			fseek(ck, counter_s, 0);
			ck_ind[id] = ftell(ck) / sizeof(Cook);
			fwrite(&cook, sizeof(Cook), 1, ck);
			checker = 1;
		}
		counter_s++;
		if (checker == 1) break;
	}
	if (checker == 0) {
		ck_ind[id] = ftell(ck) / sizeof(Cook);
		fwrite(&cook, sizeof(struct Cook), 1, ck);
//		fseek(ck, id * sizeof(Cook), 0);
		
	}

	fclose(ck);
	FILE *ck_i = fopen("cook.ind", "wb");
	for (int i = 0; i < size_ck; i++) {
		fwrite(&i, sizeof(int), 1, ck_i);
		fwrite(&ck_ind[i], sizeof(int), 1, ck_i);
	}
	fclose(ck_i);
//	menus(ck_ind, or_ind);
}
void t_add_order_to(int *ck_ind, int *or_ind, int id, int cook_id) {
	FILE *ck = fopen("cook.bin", "rb");
	struct Cook cook;
	fseek(ck, ck_ind[cook_id] * sizeof(Cook), 0);
	fread(&cook, sizeof(Cook), 1, ck);
	FILE * or = fopen("order.bin", "rb");
	FILE *sw = fopen("swap.bin", "wb");
	struct Order order;
	struct Order pointer;
	int cod = cook.order_id;
	struct Order pointer2;
	while (fread(&order, sizeof(Order), 1, or ) != NULL) {
		if (cod == order.id && order.is_exist==1) {
			if (order.next != -1) {
				cod = order.next;
			}
			else {
				order.next = id;
			}
		}
		fwrite(&order, sizeof(Order), 1, sw);
	}
	fclose(or);
	fclose(sw);
	or = fopen("order.bin", "wb");
	sw = fopen("swap.bin", "rb");
	while (fread(&order, sizeof(order), 1, sw) != NULL) {
		fwrite(&order, sizeof(Order), 1, or );
	}
	fclose(or);
	fclose(sw);
	fclose(ck);
}
void add_order_to(int *ck_ind, int *or_ind, int id, int cook_id) {
	FILE *ck = fopen("cook.bin", "rb");
	struct Cook cook;
	fseek(ck, ck_ind[cook_id] * sizeof(Cook), 0);
	fread(&cook, sizeof(Cook), 1, ck);
	FILE * or = fopen("order.bin", "rb+");
//	FILE *sw = fopen("swap.bin", "wb");
	struct Order order;
	int beta = 0;
	int cod = cook.order_id;
	fseek(or , or_ind[cod] * sizeof(Order), 0);
	while (beta!=1) {
		fread(&order, sizeof(Order), 1, or );
		if (order.next != -1) {
			cod = order.next;
			fseek(or , or_ind[cod] * sizeof(Order), 0);
		}
		else {
			order.next = id;
			fseek(or , or_ind[cod] * sizeof(Order), 0);
			fwrite(&order, sizeof(Order), 1, or );
			beta = 1;
		}
	}
/*	while (fread(&order, sizeof(Order), 1, or ) != NULL) {
		if (cod == order.id && order.is_exist == 1) {
			if (order.next != -1) {
				cod = order.next;
			}
			else {
				order.next = id;
			}
		}
		fwrite(&order, sizeof(Order), 1, sw);
	}*/
	fclose(or );
	fclose(ck);
}
void insert_s(int *ck_ind, int *or_ind, int size_or, int size_ck) {
	FILE *or = fopen("order.bin", "rb+");
	struct Order order;
	int id = 0;
	int point = -1;
	if (size_or == 0) {
		my_realloc(or_ind, size_or, size_or + 1);
		point = size_or;
		size_or++;
	}
	for (int i = 0; i < size_or; i++) {
		if (i == point) {
			id = i;
			i = size_or;
			break;
		}
		if (i == size_or - 1  && or_ind[i] > -1) {
			my_realloc(or_ind, size_or, size_or + 1);
			size_or += 1;
			point = i+1;
			//or_ind = (int*)realloc(or_ind, size_or*sizeof(int));
		}
		if (i!=size_or && or_ind[i] < 0) {
			id = i;
			i = size_or;
		}
	}
	struct Order order_temp;
//	fseek(or , 0, 2);

	order.id = id;
	cout << endl;
	cout << "\nENTER\nPRICE: ";
	cin >> order.price;
	cin.ignore();
	cout << endl;
//	FILE *ck = fopen("cook.bin", "rb");
	int choosee=0;
	if (size_ck != 0) {
		cout << "\nDO U WANT TO CONNECT ORDER WITH COOK THAT ALREADY EXIST?\n1 IF YES, ANY OTHER NUMBER IF NO\n";
		cin >> choosee;
		cin.ignore();
	}
	int order_cook = -1;
	if (choosee == 1) {
		if (size_ck != 0) {
			cout << "COOK ID: ";
			cin >> order_cook;
			cin.ignore();

			while (ck_ind[order_cook] < 0) {
				cin >> order_cook;
				cin.ignore();
			}


			//  cout << "COOK ID: ";
			//	cin >> order_cook;
			//	cin.ignore();
			//	fseek(ck, ck_ind[order.cook] * sizeof(Cook), 0);
			//	fread(&cook, sizeof(Cook), 1, ck);
		}
	}
	int counter_s = 0;
	int locker = 0;
	while (fread(&order_temp, sizeof(Order), 1, or ) != NULL) {
		if (order_temp.id == order.id) {
			fseek(or , counter_s, 0);
			or_ind[id] = ftell(or ) / sizeof(struct Order);
			fwrite(&order, sizeof(Order), 1, or );
			locker = 1;
		}
		counter_s++;
		if (locker == 1) break;
	}
	if (locker != 1) {
		or_ind[id] = ftell(or ) / sizeof(struct Order);
		fwrite(&order, sizeof(struct Order), 1, or );
	}
	if (order_cook != -1) {
		fclose(or );
		add_order_to(ck_ind, or_ind, id, order_cook);
	}
//	fclose(ck);
	if (order_cook==-1)
	fclose(or);
	FILE *or_i = fopen("order.ind", "wb");
	for (int i = 0; i < size_or; i++) {
		fwrite(&i, sizeof(int), 1, or_i);
		fwrite(&or_ind[i], sizeof(int), 1, or_i);
	}
	fclose(or_i);
//	menus(ck_ind, or_ind);
}
void insert_menu(int *ck_ind, int *or_ind) {
	cout << endl << "\n1-INSERT COOK\n2-INSERT ORDER\n";
	FILE *ck_i = fopen("cook.ind", "rb");
	FILE *or_i = fopen("order.ind", "rb");
	fseek(ck_i, 0, 2);
	fseek(or_i, 0, 2);
	int size_ck = ftell(ck_i)/(2*sizeof(int));
	int size_or = ftell(or_i)/(2*sizeof(int));
	fclose(ck_i);
	fclose(or_i);
	int choose;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >2) menus(ck_ind, or_ind);
	switch (choose) {
	case 1: insert_m(ck_ind, or_ind,size_ck,size_or); break;
	case 2: insert_s(ck_ind, or_ind, size_or, size_ck); break;
	}
}
void m_update_m(int *ck_ind, int *or_ind, int id) {
	FILE *ck = fopen("cook.bin", "rb");
	struct Cook cook;
	fseek(ck, 0, 2);
	int k = ftell(ck) / sizeof(struct Cook);
	if (k< id)
		exit(0);
	fseek(ck, 0, 0);
	FILE *swap = fopen("swap.bin", "wb");
	while (fread(&cook, sizeof(struct Cook), 1, ck) != NULL) {
		if (cook.id == id) {
			if (cook.is_exist==1){
			cout << "\nNAME: " << cook.name << "\nEXP: " << cook.exp << "\nSALARY: " << cook.salary << endl;
			cout << "\nENTER\nNAME: ";
			cin.getline(cook.name, 20);
			cout << "EXPERIENCE: ";
			cin >> cook.exp;
			cin.ignore();
			cout << "SALARY: ";
			cin >> cook.salary;
			cout << "ORDER: ";
			cin >> cook.order_id;
			cin.ignore();
			//			cook.is_exist = 1;
		}
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
//	menus(ck_ind, or_ind);
}
void update_m(int *ck_ind, int *or_ind, int id) {
	FILE *ck = fopen("cook.bin", "rb+");
	struct Cook cook;
	fseek(ck, 0, 2);
	int k = ftell(ck) / sizeof(struct Cook);
	if (k < id)
		exit(0);
	fseek(ck, 0, 0);
	fseek(ck, ck_ind[id] * sizeof(Cook), 0);
	fread(&cook, sizeof(struct Cook), 1, ck);
	cout << "\nNAME: " << cook.name << "\nEXP: " << cook.exp << "\nSALARY: " << cook.salary << endl;
	cout << "\nENTER\nNAME: ";
	cin.getline(cook.name, 20);
	cout << "EXPERIENCE: ";
	cin >> cook.exp;
	cin.ignore();
	cout << "SALARY: ";
	cin >> cook.salary;
	cout << "ORDER: ";
	cin >> cook.order_id;
	cin.ignore();
	fseek(ck, ck_ind[id] * sizeof(Cook), 0);
	fwrite(&cook, sizeof(Cook), 1, ck);
/*	while (fread(&cook, sizeof(struct Cook), 1, ck) != NULL) {
		if (cook.id == id) {
			if (cook.is_exist == 1) {
				cout << "\nNAME: " << cook.name << "\nEXP: " << cook.exp << "\nSALARY: " << cook.salary << endl;
				cout << "\nENTER\nNAME: ";
				cin.getline(cook.name, 20);
				cout << "EXPERIENCE: ";
				cin >> cook.exp;
				cin.ignore();
				cout << "SALARY: ";
				cin >> cook.salary;
				cout << "ORDER: ";
				cin >> cook.order_id;
				cin.ignore();
				//			cook.is_exist = 1;
			}
		}
		fwrite(&cook, sizeof(struct Cook), 1, swap);
	}*/
	fclose(ck);
	//	menus(ck_ind, or_ind);
}
void s_update_s(int *ck_ind, int *or_ind, int id) {
	FILE * or = fopen("order.bin", "rb+");
	struct Order order;
	fseek(or, 0, 2);
	int k = ftell(or) / sizeof(struct Order);
	if (k< id)
		exit(0);
	fseek(or, 0, 0);
	FILE *swap = fopen("swap.bin", "wb");
	while (fread(&order, sizeof(struct Order), 1, or ) != NULL) {
		if (order.id == id) {
			if (order.is_exist == 1) {
				cout << "\nWAS\nPRICE: " << order.price /*<< "\nCOOK ID: " << order.cook */<< endl;
				cout << "\nENTER\nPRICE: ";
				cin >> order.price;
				cin.ignore();
				//			cout << "COOK ID: ";
				//			cin >> order.cook;
				//			cin.ignore();
			}
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
//	menus(ck_ind, or_ind);
}
void update_s(int *ck_ind, int *or_ind, int id) {
	FILE * or = fopen("order.bin", "rb+");
	struct Order order;
	fseek(or , 0, 2);
	int k = ftell(or ) / sizeof(struct Order);
	if (k< id)
		exit(0);
	fseek(or , or_ind[id]*sizeof(Order), 0);
	fread(&order, sizeof(Order), 1, or );
	cout << "\nWAS\nPRICE: " << order.price<<"\nID:"<<order.id << endl;
	cout << "\nENTER\nPRICE: ";
	fseek(or , or_ind[id] * sizeof(Order), 0);
	cin >> order.price;
	cin.ignore();
	fwrite(&order, sizeof(struct Order), 1, or );
//	FILE *swap = fopen("swap.bin", "wb");
/*	while (fread(&order, sizeof(struct Order), 1, or ) != NULL) {
		if (order.id == id) {
			if (order.is_exist == 1) {
				cout << "\nWAS\nPRICE: " << order.price << "\nCOOK ID: " << order.cook  << endl;
				cout << "\nENTER\nPRICE: ";
				cin >> order.price;
				cin.ignore();
				//			cout << "COOK ID: ";
				//			cin >> order.cook;
				//			cin.ignore();
			}
		}
		fwrite(&order, sizeof(struct Order), 1, swap);
	}*/
	fclose(or );
	//	menus(ck_ind, or_ind);
}
void update_menu(int *ck_ind, int *or_ind) {
	cout << endl << "\n1-UPDATE COOK\n2-UPDATE ORDER\n";
	int choose;
	int id;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >2) menus(ck_ind, or_ind);
	cout << "\nenter ID\nID: ";
	cin >> id;
	cin.ignore();
	switch (choose) {
	case 1: update_m(ck_ind, or_ind, id); break;
	case 2: update_s(ck_ind, or_ind, id); break;
	}
}
void ut_m(int *ck_ind, int *or_ind) {
	FILE *ck = fopen("cook.bin", "rb");
	struct Cook cook;
	while (fread(&cook, sizeof(struct Cook), 1, ck)!=NULL){
		cout << endl;
		cout << "ID: " << cook.id << endl;
		cout << "Name: " << cook.name << endl;
		cout << "Experience: " << cook.exp << endl;
		cout << "Salary: " << cook.salary << endl;
		cout << "Order id: " << cook.order_id << endl;
		cout << "Exist: " << cook.is_exist << endl;
	}
	fclose(ck);
//	menus(ck_ind, or_ind);
}
void ut_s(int *ck_ind, int *or_ind) {
	FILE * or = fopen("order.bin", "rb");
	struct Order order;
	while (fread(&order, sizeof(struct Order), 1, or ) != NULL) {
			cout << endl;
			cout << "ID: " << order.id << endl;
			cout << "PRICE: " << order.price << endl;
//			cout << "COOK ID: " << order.cook << endl;
			cout << "EXIST: " << order.is_exist << endl;
			cout << "NEXT: " << order.next << endl;
	}
	fclose(or);
//	menus(ck_ind, or_ind);
}
void ut_menu(int *ck_ind, int *or_ind) {
	cout << endl << "\n1-UT COOK\n2-UT ORDER\n";
	int choose;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >2) menus(ck_ind, or_ind);
	switch (choose) {
	case 1: ut_m(ck_ind,or_ind); break;
	case 2: ut_s(ck_ind,or_ind); break;
	}
}
void count_m(int *ck_ind, int *or_ind, int size_ck) {
	int count = 0;
	for (int i = 0; i < size_ck; i++) {
		if (ck_ind[i] > -1) count++;
	}
	cout << "there is " << count << " cooks\n";
//	menus(ck_ind, or_ind);
}
void count_s(int *ck_ind, int *or_ind, int size_or) {
	int count = 0;
	for (int i = 0; i < size_or; i++) {
		if (or_ind[i] > -1) count++;
	}
	cout << "there is " << count << " orders\n";
//	menus(ck_ind, or_ind);
}
void count_s_m(int *ck_ind, int *or_ind, int size_ck) {
	int id;
	struct Order order;
	struct Cook cook;
	FILE *ck = fopen("cook.bin", "rb");
	cout << "\nENTER ID\nID: ";
	cin >> id;
	cin.ignore();
	if (ck_ind[id] < 0) {
		fclose(ck);
		menus(ck_ind, or_ind);
	}
	while (id > size_ck) {
		cin >> id;
		cin.ignore();
		if (ck_ind[id] < 0) id = size_ck + 1;
	}
	FILE *or = fopen("order.bin", "rb");
	fseek(ck, ck_ind[id] * sizeof(Cook), 0);
	fread(&cook, sizeof(Cook), 1, ck);
	fseek(or , or_ind[cook.order_id]*sizeof(Order), 0);
	fread(&order, sizeof(Order), 1, or );
	int i = 1;
	while (order.next != -1) {
		i++;
		fseek(or , order.next * sizeof(Order), 0);
		fread(&order, sizeof(Order), 1, or );
	}
	cout << "there is " << i << " orders for this cook\n";
	fclose(or);
	fclose(ck);
//	menus(ck_ind, or_ind);
}
void count_menu(int *ck_ind, int *or_ind) {
	cout << endl << "\n1-NUMBER OF COOKS\n2-NUMBER OF ORDERS\n3-NUMBER OF ORDERS CONNECTED TO *id* cook\n";
	int choose;
	cin >> choose;
	cin.ignore();
	FILE *ck_i = fopen("cook.ind", "rb");
	FILE *or_i = fopen("order.ind", "rb");
	fseek(ck_i, 0, 2);
	fseek(or_i, 0, 2);
	int size_ck = ftell(ck_i) / (2 * sizeof(int));
	int size_or = ftell(or_i) / (2 * sizeof(int));
	fclose(ck_i);
	fclose(or_i);
	if (choose < 1 || choose >3) menus(ck_ind, or_ind);
	switch (choose) {
	case 1: count_m(ck_ind, or_ind, size_ck); break;
	case 2: count_s(ck_ind, or_ind, size_or); break;
	case 3: count_s_m(ck_ind, or_ind, size_ck); break;
	}
}
int exit_f(int *ck_ind, int *or_ind) {
	cout << "\nDO YOU WANT TO REWRITE FILES? 0 IF NO, ELSE IF YES\n";
	int choose;
	cin >> choose;
	cin.ignore();
	struct Cook cook;
	struct Order order;
	if (choose != 0) {
		FILE *ck = fopen("cook.bin", "rb");
		FILE *swap = fopen("swap.bin", "wb");
		while (fread(&cook, sizeof(Cook), 1, ck) != NULL) {
			if (cook.is_exist != 0) fwrite(&cook, sizeof(Cook), 1, swap);
		}
		fclose(ck); fclose(swap);
		ck = fopen("cook.bin", "wb");
		swap = fopen("swap.bin", "rb");
		while (fread(&cook, sizeof(Cook), 1, swap) != NULL) {
			 fwrite(&cook, sizeof(Cook), 1, ck);
		}
		fclose(ck); fclose(swap);
/*		FILE * or = fopen("order.bin", "rb");
		swap = fopen("swap.bin", "wb");
		while (fread(&order, sizeof(Order), 1, or) != NULL) {
			if (order.is_exist != 0) fwrite(&order, sizeof(Order), 1, swap);
		}
		fclose(or); fclose(swap);
		or = fopen("order.bin", "wb");
		swap = fopen("swap.bin", "rb");
		while (fread(&order, sizeof(Order), 1, swap) != NULL) {
			fwrite(&order, sizeof(Order), 1, or);
		}
		fclose(or ); fclose(swap);*/
	}
	exit(0);
}
int menus(int *ck_ind, int *or_ind) {
	cout << endl << "\n1-GET\n2-DEL\n3-INSERT\n4-UPDATE\n5-UT\n6-COUNT\n7-EXIT\n";
	int choose;
	cin >> choose;
	cin.ignore();
	if (choose < 1 || choose >8) {
		cout << "\n\nERROR choose from 1 to 7\n";
		menus(ck_ind, or_ind);
	}
	switch (choose) {
	case 1: get_menu(ck_ind, or_ind); break;
	case 2: del_menu(ck_ind, or_ind); break;
	case 3: insert_menu(ck_ind, or_ind); break;
	case 4: update_menu(ck_ind, or_ind); break;
	case 5: ut_menu(ck_ind, or_ind); break;
	case 6: count_menu(ck_ind, or_ind); break;
	case 7: exit_f(ck_ind, or_ind); break;
	}
	menus(ck_ind, or_ind);
	return 0;
}
//index files, delete, peredelat all s from m, delete m -> delete all s
int main()
{	
/*
	FILE *f = fopen("order.bin", "wb");
	fclose(f);
	f = fopen("order.ind", "wb");
	fclose(f);
	f = fopen("cook.bin", "wb");
	fclose(f);
	f = fopen("cook.ind", "wb");
	fclose(f);
	f = fopen("swap.bin", "wb");
	fclose(f);
	f = fopen("swap2.bin", "wb");
	fclose(f);*/
/**/
	FILE *ck_i = fopen("cook.ind", "rb");
	FILE *or_i = fopen("order.ind", "rb");
	fseek(ck_i, 0, 2);
	fseek(or_i, 0, 2);
	int a = ftell(ck_i) / (2 * sizeof(int));
	if (a == 0) a = 1;
	int c = ftell(or_i) / (2 * sizeof(int));
	if (c == 0) c = 1;
	int *ck_ind = new int[a];
	int *or_ind = new int[c];
	fseek(ck_i, 0, 0);
	fseek(or_i, 0, 0);
	int k = 0;
	int b = 0;
	while (fread(&k, sizeof(int), 1, ck_i) != NULL) {
		fread(&b, sizeof(int), 1, ck_i);
		if (b >= 0) ck_ind[k] = b;
	}
	while (fread(&k, sizeof(int), 1, or_i) != NULL) {

		fread(&b, sizeof(int), 1, or_i);
		if (b >= 0) or_ind[k] = b;
	}
	fclose(ck_i);
	fclose(or_i);
	menus(ck_ind, or_ind);
	//delete[] ck_ind;
	//delete[] or_ind;
//	free(ck_ind);
//	free(or_ind);
    return 0;
}

