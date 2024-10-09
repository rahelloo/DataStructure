#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // Library untuk fungsi-fungsi khusus seperti getch() yang digunakan dalam program ini
#include <ctype.h> // Library untuk fungsi-fungsi karakter seperti isdigit() yang digunakan dalam program ini

typedef struct node{
	char namaProduk[21];
	int jumlahBarang;
	int hargaBarang;
	unsigned int key;
	struct node *left;
	struct node *right;
}node;

node *root = NULL;
#define MAX 1000
int done=0;

void deleteTree(node *curr){ // menghapus simpul kanan dan kiri pada b tree menggunakan free(curr)
	if(curr!=NULL){
		deleteTree(curr->left);
		deleteTree(curr->right);
		free(curr);
	}
}

void printHeader(){ // cetak header tabel produk di toko grosir
	printf("                ================================================================\n");
	printf("	        || Nama Produk          || Jumlah Barang || Harga Barang      ||\n");
    printf("                ================================================================\n");
}

unsigned int hash(char *str) { // mencari key, diambil dari angka ascii per char yang ada pada string 
	unsigned int hash_val = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		hash_val = hash_val + (unsigned int) str[i];
	}return hash_val % MAX;
}

void insert(node *curr, node *newnode, unsigned int key){ // mengelola penyisipan node baru dalam tree sesuai dengan nilai kunci yang ditentukan.
    if(root == NULL){
        root = newnode;
    }else if(key != curr->key){ // jika key kurang dari nilai kunci maka node tidak memiliki anak kiri
        if(key<curr->key && curr->left == NULL){
            node *temp = newnode;
            curr->left = temp; // jika key kurang dari nilai kunci kanan, maka ia tidak memiliki anak kanan
        }else if(key>curr->key && curr->right == NULL){
            node *temp = newnode;
            curr->right = temp;
        }else if(key<curr->key){
            insert(curr->left, newnode, key); //rekursif 
        }else{
            insert(curr->right, newnode, key);
        }
    }
}

void importData(){ 
	FILE *fp = fopen("data.txt", "r"); // membuka file txt
	if(fp==NULL){
		return;
	}
	
	node temp;
	while(fscanf(fp, "%[^;];%d;%d\n", temp.namaProduk, &temp.jumlahBarang, &temp.hargaBarang)!=EOF){ // membaca file txt
		node *newnode = (node*)malloc(sizeof(node));
		strcpy(newnode->namaProduk, temp.namaProduk);
		newnode->jumlahBarang = temp.jumlahBarang;
		newnode->hargaBarang = temp.hargaBarang;
		newnode->key = hash(newnode->namaProduk);
		newnode->left = NULL; newnode->right = NULL;
		insert(root, newnode, newnode->key); // panggil insert() agar newnode dapat disisipkan ke BST
	}
	fclose(fp);
	return;
}

int cekNamaProduk(char namaProduk[21]){
	unsigned int key = hash(namaProduk);
	node *curr = root;
	
	while(curr!=NULL){
		if(key == curr->key && strcmp(namaProduk, curr->namaProduk)==0){
			//printf("Nama sudah ada!!\n");
			return 0;
		}else if(key == curr->key && strcmp(namaProduk, curr->namaProduk)!=0 && strlen(namaProduk)!=strlen(curr->namaProduk)){
			return 2; // sudah ada data yang bernama itu
		}else if(key == curr->key && strcmp(namaProduk, curr->namaProduk)!=0){
			return 3; // untuk data yang hampir sama menghindari kesalahan pada typo (misal beras dengan bersa)
		}else if(key < curr->key){
			curr = curr->left;
		}else if(key > curr->key){
			curr = curr->right;
		}
	}return 1;
}

node *newNode(int key, char namaProduk[21], int jumlahBarang, int hargaBarang){
	node *newnode = (node*)malloc(sizeof(node));
	newnode->key = key;
	strcpy(newnode->namaProduk, namaProduk);
	newnode->jumlahBarang = jumlahBarang;
	newnode->hargaBarang = hargaBarang;
	newnode->left = NULL;
	newnode->right = NULL;
	
	FILE *fp = fopen("data.txt", "a");
	fprintf(fp, "%s;%d;%d\n", newnode->namaProduk, newnode->jumlahBarang, newnode->hargaBarang);
	fclose(fp);
	return(newnode);
}

node *insertNode(node *curr, char namaProduk[21], int jumlahBarang, int hargaBarang){
	if (curr == NULL) // cek apakah node saat ini NULL, jika ya maka node baru dapat dimasukkan
    	return(newNode(hash(namaProduk), namaProduk, jumlahBarang, hargaBarang));
    	
  	if (hash(namaProduk) < curr->key) // ditempatkan sebelah kiri
    	curr->left = insertNode(curr->left, namaProduk, jumlahBarang, hargaBarang);
  	else if (hash(namaProduk) > curr->key) // ditempatkan sebelah kanan
    	curr->right = insertNode(curr->right, namaProduk, jumlahBarang, hargaBarang);
  	else
	    return curr;
}

void inputData(){
	char namaProduk[21];
	int jumlahBarang;
	int hargaBarang;
	system("cls");
	do{
		printf("Masukkan nama produk [5-20]: "); scanf("%[^\n]", namaProduk); getchar();
		if(strlen(namaProduk)<5 || strlen(namaProduk)>20) printf("Nama yang anda masukkan harus sepanjang 5-20 huruf!\n");
		else if(cekNamaProduk(namaProduk)==0) printf("Nama yang anda masukkan sudah ada di data\n");
	}while(cekNamaProduk(namaProduk)==0 || (strlen(namaProduk))<5 || (strlen(namaProduk))>20);
	
	do{
		printf("Masukkan jumlah barang [>0] : "); scanf("%d", &jumlahBarang); getchar();
		if(jumlahBarang<=0) printf("Jumlah barang yang anda masukkan harus lebih dari 0!\n");
	}while(jumlahBarang<=0);
	
	do{
		printf("Masukkan harga barang : "); scanf("%d", &hargaBarang); getchar();
		if(hargaBarang<=0) printf("Harga barang yang anda masukkan harus lebih dari 0!\n");
	}while(hargaBarang<=0);
	
	if(root==NULL){
		root = insertNode(root, namaProduk, jumlahBarang, hargaBarang);
	}else if(root!=NULL){
		insertNode(root, namaProduk, jumlahBarang, hargaBarang);
	}
	
	
	printf("Insert success!!\n");
	system("pause");
}

void view(node *curr){
	if(curr!=NULL){
		view(curr->left);
		printf("	        || %-20s || %-13d || Rp %-14d ||\n", curr->namaProduk, curr->jumlahBarang, curr->hargaBarang);
		printf("                ================================================================\n");
		view(curr->right);
	}
}

void validasiData(){ 
	if(root==NULL){
		system("cls");
		printf("There is no data !!\n");
	}else{
		system("cls");
		printHeader();
		view(root);
	}
}

void updateData(){
	system("cls");
	FILE *fp = fopen("data.txt", "r");
	if(root==NULL){
		printf("There is no data");
		system("pause");
		return;
	}
	
	FILE *temp = fopen("temp.txt", "w");
	int found = 0;
	
	validasiData();
	system("pause");
	
	char namaProduk[21];
	
	do{
		printf("Masukkan nama produk yang ingin di-update : "); scanf("%[^\n]", namaProduk); getchar();
	}while(cekNamaProduk(namaProduk)==2 || strlen(namaProduk)<5 || strlen(namaProduk)>20);
	
	unsigned int key = hash(namaProduk);
	int choice;
	node *tempData = (node*)malloc(sizeof(node));
	
	do{
		system("cls");
		printf("                ================================================================\n");
		printf("	        ||                   Pilih yang ingin anda ubah               ||\n");
	    printf("                ================================================================\n");
		printf("                ||                     1. Nama Produk                         ||\n");
		printf("                ================================================================\n");
		printf("                ||                     2. Jumlah Barang                       ||\n");
		printf("                ================================================================\n");
		printf("                ||                     3. Harga Barang                        ||\n");
		printf("                ================================================================\n");
		printf("                ||                     4. Semua detail                        ||\n");
		printf("                ================================================================\n");
		printf("                ||                     5. Exit                                ||\n");
		printf("                ================================================================\n");
		printf("                                 Your choice : "); scanf("%d", &choice); getchar();
	}while(choice<1 || choice>5);
	
	switch(choice){
		case 1:
			while(fscanf(fp, "%[^;];%d;%d\n", tempData->namaProduk, &tempData->jumlahBarang, &tempData->hargaBarang)!=EOF){
				if(strcmp(namaProduk, tempData->namaProduk)==0 && found==0){
					found = 1;
					char tempNama[21]; 
					do{
						printf("Masukkan update berupa nama [5-20] : "); scanf("%[^\n]", tempNama); getchar();
					}while(cekNamaProduk(tempNama)==2 || cekNamaProduk(tempNama)==0 || strlen(tempNama)<5 || strlen(tempNama)>20);
					
					// kita delete setelah user memasukkan 
					deleteTree(root);
					root = NULL;
					
					fprintf(temp, "%s;%d;%d\n", tempNama, tempData->jumlahBarang, tempData->hargaBarang);
				}else{
					fprintf(temp, "%s;%d;%d\n", tempData->namaProduk, tempData->jumlahBarang, tempData->hargaBarang);
				}
			}if(found==0){
				printf("Data not found!!\n");
				system("pause");
			}else if(found==1){
				printf("Data updated !!\n");
				system("pause");
			}
			fclose(temp); fclose(fp);
			remove("data.txt");
			rename("temp.txt", "data.txt");
			break;
		case 2:
			while(fscanf(fp, "%[^;];%d;%d\n", tempData->namaProduk, &tempData->jumlahBarang, &tempData->hargaBarang)!=EOF){
				if(strcmp(namaProduk, tempData->namaProduk)==0 && found==0){
					found = 1;
					int tempJumlah; 
					
					deleteTree(root);
					root = NULL;
					
					do{
						printf("Masukkan update jumlah barang : "); scanf("%d", &tempJumlah); getchar();
					}while(tempJumlah<=0);
					
					fprintf(temp, "%s;%d;%d\n", tempData->namaProduk, tempJumlah, tempData->hargaBarang);
				}else{
					fprintf(temp, "%s;%d;%d\n", tempData->namaProduk, tempData->jumlahBarang, tempData->hargaBarang);
				}
			}if(found==0){
				printf("Data not found!!\n");
				system("pause");
			}else if(found==1){
				printf("Data updated !!\n");
				system("pause");
			}
			fclose(temp); fclose(fp);
			remove("data.txt");
			rename("temp.txt", "data.txt");
			break;
		case 3:
			while(fscanf(fp, "%[^;];%d;%d\n", tempData->namaProduk, &tempData->jumlahBarang, &tempData->hargaBarang)!=EOF){
				if(strcmp(namaProduk, tempData->namaProduk)==0 && found==0){
					found = 1;
					int tempHarga;
				
					// kita delete setelah user memasukkan 
					deleteTree(root);
					root = NULL;
					
					do{
						printf("Masukkan update harga barang : "); scanf("%d", &tempHarga); getchar();
					}while(tempHarga<=0);
					fprintf(temp, "%s;%d;%d\n", tempData->namaProduk, tempData->jumlahBarang, tempHarga);
				}else{
					fprintf(temp, "%s;%d;%d\n", tempData->namaProduk, tempData->jumlahBarang, tempData->hargaBarang);
				}
			}if(found==0){
				printf("Data not found!!\n");
				system("pause");
			}else if(found==1){
				printf("Data updated !!\n");
				system("pause");
			}
			fclose(temp); fclose(fp);
			remove("data.txt");
			rename("temp.txt", "data.txt");
			break;
		case 4:
			while(fscanf(fp, "%[^;];%d;%d\n", tempData->namaProduk, &tempData->jumlahBarang, &tempData->hargaBarang)!=EOF){
				if(strcmp(namaProduk, tempData->namaProduk)==0 && found==0){
					found = 1;
					char tempNama[21]; 
					int tempJumlah; 
					int tempHarga;
					do{
						printf("Masukkan update berupa nama [5-20] : "); scanf("%[^\n]", tempNama); getchar();
					}while(cekNamaProduk(tempNama)==2 || cekNamaProduk(tempNama)==0 || strlen(tempNama)<5 || strlen(tempNama)>20);
					
					// kita delete setelah user memasukkan 
					deleteTree(root);
					root = NULL;
					
					do{
						printf("Masukkan update jumlah barang : "); scanf("%d", &tempJumlah); getchar();
					}while(tempJumlah<=0);
					
					do{
						printf("Masukkan update harga barang : "); scanf("%d", &tempHarga); getchar();
					}while(tempHarga<=0);
					fprintf(temp, "%s;%d;%d\n", tempNama, tempJumlah, tempHarga);
				}else{
					fprintf(temp, "%s;%d;%d\n", tempData->namaProduk, tempData->jumlahBarang, tempData->hargaBarang);
				}
			}if(found==0){
				printf("Data not found!!\n");
				system("pause");
			}else if(found==1){
				printf("Data updated !!\n");
				system("pause");
			}
			fclose(temp); fclose(fp);
			remove("data.txt");
			rename("temp.txt", "data.txt");
			break;
		case 5:
			exit(0);
			break;
	}
	
}

void deleteData(){ // hapus data dari BST dan memperbarui data yang ada
	system("cls");
	FILE *fp = fopen("data.txt", "r"); // buka data yang ada 
	if(root==NULL){
		printf("There is no data");
		system("pause");
		return;
	}
	
	FILE *temp = fopen("temp.txt", "w"); 
	int found = 0;
	
	validasiData();
	system("pause");
	
	char namaProduk[21];
	
	do{
		printf("Masukkan nama produk yang ingin di-delete : "); scanf("%[^\n]", namaProduk); getchar();
	}while(cekNamaProduk(namaProduk)==2 || strlen(namaProduk)<5 || strlen(namaProduk)>20);
	
	unsigned int key = hash(namaProduk);
	int choice;
	node *tempData = (node*)malloc(sizeof(node));
	
	do{
		system("cls");
		printf("                ================================================================\n");
		printf("	        ||                  Pilih yang ingin anda delete              ||\n");
	    printf("                ================================================================\n");
		printf("                ||                     1. Jumlah Barang                       ||\n");
		printf("                ================================================================\n");
		printf("                ||                     2. Semua detail                        ||\n");
		printf("                ================================================================\n");
		printf("                ||                     3. Exit                                ||\n");
		printf("                ================================================================\n");
		printf("                                 Your choice : "); scanf("%d", &choice); getchar();
	}while(choice<1 || choice>3);
	
	char ch;
	
	switch(choice){
		case 1:
			printf("Jumlah barang anda akan dibuat menjadi 0 apabila ditemukan, lanjutkan?\n");
			printf("Y/N : "); scanf("%c", &ch); getchar();
			if(ch=='Y'){
				while(fscanf(fp, "%[^;];%d;%d\n", tempData->namaProduk, &tempData->jumlahBarang, &tempData->hargaBarang)!=EOF){
					if(strcmp(namaProduk, tempData->namaProduk)==0 && found==0){
						found = 1; 
						deleteTree(root);
						root = NULL;
						
						fprintf(temp, "%s;0;%d\n", tempData->namaProduk, tempData->hargaBarang);
					}else{
						fprintf(temp, "%s;%d;%d\n", tempData->namaProduk, tempData->jumlahBarang, tempData->hargaBarang);
					}
				}
				fclose(temp); fclose(fp);
				remove("data.txt");
				rename("temp.txt", "data.txt");
			}
			break;
		case 2:
			printf("Semua detail barang anda akan dihapus apabila ditemukan datanya, lanjutkan?\n");
			printf("Y/N : "); scanf("%c", &ch); getchar();
			if(ch=='Y'){
				while(fscanf(fp, "%[^;];%d;%d\n", tempData->namaProduk, &tempData->jumlahBarang, &tempData->hargaBarang)!=EOF){
					if(strcmp(namaProduk, tempData->namaProduk)==0 && found==0){
						found = 1; // melacak apakah data yang ingin dihapus ditemukan
						deleteTree(root); // agar root menjadi NULL
						root = NULL;
					}else{
						fprintf(temp, "%s;%d;%d\n", tempData->namaProduk, tempData->jumlahBarang, tempData->hargaBarang);
					}
				}fclose(temp); fclose(fp);
				remove("data.txt"); // penghapusan data pada txt
				rename("temp.txt", "data.txt");
				
			}
			break;
		case 3:
			break;
			
		
	}if(found==0){
		printf("Data not found!!\n");
		system("pause");
	}else if(found==1){
		printf("Data deleted !!\n");
		system("pause");
	}
	
	
}

void search(){
	if(root==NULL){
		system("cls");
		printf("There is no data!!\n");
		system("pause");
		return;
	}
	int choice;
	done = 0;
	do{
		system("cls");
		printf("                ================================================================\n");
		printf("	        ||   Pilih yang ingin anda gunakan sebagai detail pencarian   ||\n");
	    printf("                ================================================================\n");
		printf("                ||                     1. Nama Produk                         ||\n");
		printf("                ================================================================\n");
		printf("                ||                     2. Harga Barang                        ||\n");
		printf("                ================================================================\n");
		printf("                ||                     3. Keluar                              ||\n");
		printf("                ================================================================\n");
		printf("                                 Your choice : "); scanf("%d", &choice); getchar();
	}while(choice<1 || choice>3);
	
	switch(choice){
		case 1:
			searchName();
			system("pause");
			break;
		case 2:
			searchHarga();
			system("pause");
			break;
		case 3:
			break;
	}
}

void searchHarga(){
	system("cls");
	
	int harga;
	
	do{
		printf("Masukkan harga barang yang ingin dicari : "); scanf("%d", &harga); getchar();
	}while(harga<=0);
	
	searchHarga2(root, harga);
	
	if(done==0){
		printf("Data not found!!\n");
	}
}

void searchHarga2(node *curr, int harga){
	if(curr!=NULL){
		searchHarga2(curr->left, harga);
		if(harga == curr->hargaBarang){
			if(done==0){
				printHeader();
				done=1;	
			}
			printf("	        || %-20s || %-13d || Rp %-14d ||\n", curr->namaProduk, curr->jumlahBarang, curr->hargaBarang);
			printf("                ================================================================\n");
		}searchHarga2(curr->right, harga);
	}return;
}

void searchName(){
	system("cls");
	char namaProduk[21];
	do{
		printf("Masukkan nama produk yang ingin dicari : "); scanf("%[^\n]", namaProduk); getchar();
	}while((strlen(namaProduk))<5 || (strlen(namaProduk))>20);
	
	unsigned int key = hash(namaProduk);
	node *curr = root;
	int found=0;
	
	while(curr!=NULL){
		if(key == curr->key && strcmp(namaProduk, curr->namaProduk)==0){
			system("cls");
			printHeader();
			printf("	        || %-20s || %-13d || Rp %-14d ||\n", curr->namaProduk, curr->jumlahBarang, curr->hargaBarang);
			printf("                ================================================================\n");
			found = 1;
			return;
		}else if(key < curr->key){
			curr = curr->left;
		}else if(key > curr->key){
			curr = curr->right;
		}
	}if(found==0){
		printf("Data not found !!\n");
	}
}

void menu(){
	int choice;
	do{
		system("cls");
		printf("\n\n\n");
	    printf("                =============================================================================\n");
	    printf("                ||                                                                         ||\n");
	    printf("                ||                       PROGRAM GUDANG TOKO SEMBAKO 45                    ||\n");
	    printf("                ||                                                                         ||\n");
	    printf("                =============================================================================\n");
	    printf("                ||                                                                         ||\n");
	    printf("                ||                             1. INPUT DATA                               ||\n");
	    printf("                ||                             2. VIEW DATA                                ||\n");
	    printf("                ||                             3. UPDATE DATA                              ||\n");
	    printf("                ||                             4. DELETE DATA                              ||\n");
	    printf("                ||                             5. SEARCH DATA                              ||\n");
	    printf("                ||                             6. EXIT                                     ||\n");
	    printf("                ||                                                                         ||\n");
	    printf("                =============================================================================\n");
	    printf("                                         Input your choice [1-6] : "); scanf("%d", &choice); getchar();
	    printf("\n\n\n");
	}while(choice<1 || choice>6);
	importData();
	switch(choice){
		case 1:
			inputData();
			break;
		case 2:
			validasiData();
			system("pause");
			break;
		case 3:
			updateData();
			break;
		case 4:
			deleteData();
			break;
		case 5:
			search();
			break;
		case 6:
			exit(0);
			break;
	} 
}

int cekAdmin(char username[11], char password[11]){
	FILE *admin = fopen("user.txt", "r");
	char tempName[11];
	char tempPw[11];
	int found = 0;
	int foundpw = 0;
	int result;
	while(fscanf(admin, "%[^;];%[^\n]\n", tempName, tempPw)!=EOF){
		if(strcmp(username, tempName)==0){
			if(strcmp(password, tempPw)==0){
				result = 1; // berhasil masuk
				foundpw = 1;
			}found = 1;
		}
	}if(found==0){
		result = 0; // wrong username
	}else if(found==1 && foundpw==0){
		result = 2; // wrong password
	}
	fclose(admin);
	return result;
}

void loginMenu(){
	char username[11]; char password[11];
	do{
		system("cls");
		printf("\n\n\n");
	    printf("                =============================================================================\n");
	    printf("                ||                                                                         ||\n");
	    printf("                ||          Masukkan username dan password untuk masuk ke program          ||\n");
	    printf("                ||                                                                         ||\n");
	    printf("                =============================================================================\n");
	    printf("                                         Username : "); scanf("%[^\n]", username); getchar();
	    printf("                                         Password : "); scanf("%[^\n]", password); getchar();
	    if(cekAdmin(username, password)==2){
	    	printf("Wrong password!\n");
	    	system("pause");
		}else if(cekAdmin(username, password)==0){
			printf("Wrong username!\n");
			system("pause");
		}
	}while(cekAdmin(username, password)!=1);
	printf("Hello, %s \3\n", username);
	printf("Berhasil masuk !\n"); 
	system("pause");
	
}

int main() {
    system("COLOR 0C");
    printf("\n\n\n");
    printf("                =============================================================================\n");
    printf("                ||                                                                         ||\n");
    printf("                ||          Selamat Datang di Program Gudang Toko Sembako 45 !!! \2        ||\n");
    printf("                ||                                                                         ||\n");
    printf("                =============================================================================\n");
    printf("\n\n\n");
    getch();
    system("cls");
    loginMenu();

    while (1) {
        menu();
    }

    return 0;
}

