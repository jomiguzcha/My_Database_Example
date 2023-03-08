#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <chrono>
#include <thread>
#include "table.h"
#include "Tree.h"

#define SHELL_RL_BUFSIZE 1024
#define SHELL_TOK_BUFSIZE 64 
#define SHELL_TOK_DELIM " ;,()\t\r\n\a" 

using namespace std;

vector<table> tablas;
vector<Tree<int,Asc>> indices;

void shell_execute(vector<char*>& splited) {
	int n = -1;

	if (splited[0] == NULL) {
		return;
	}

	switch (splited[0][0]) {
	case 'C': {
		splited.erase(splited.begin());//create

		if (splited[0][0] == 'I') {
			splited.erase(splited.begin());//index
			string name = splited[0];
			splited.erase(splited.begin());//nombre del indice
			splited.erase(splited.begin());//on

			for (int i = 0; i < tablas.size(); i++) {//obtiene la tabla
				if (tablas[i].tablename.compare(splited[0]) == 0) {
					n = i;
					break;
				}
			}if (n == -1) {
				cout << "Tabla no existe" << endl;
				return;
			}
			splited.erase(splited.begin());//tablename

			Tree<int,Asc> arbol;//instancia un arbol
			indices.push_back(arbol);

			Tree<int, Asc>* ptr = &indices[indices.size()-1];//puntero que pasara el arbol a la tabla
			tablas[n].indexTable(ptr, name, string(splited[0]));

			cout << "El indice "<<name<<" fue creado con exito" << endl;
		}

		else if (splited[0][0] == 'T') {
			splited.erase(splited.begin());
			string tablename = splited[0]; //nombre de la tabla
			splited.erase(splited.begin());

			vector<pair<string, string>> meta; //metadata columna-tipo
			splited.push_back(NULL);

			while (splited[0] != NULL) {
				string aux = splited[0];
				string aux1 = splited[1];
				splited.erase(splited.begin()); splited.erase(splited.begin());

				meta.push_back(pair <string, string>(aux, aux1));
			}

			table aux(tablename);//instancia la tabla
			aux.insertMeta(meta);

			tablas.push_back(aux);
			cout << "La tabla "<<tablename<<" fue creada con exito" << endl;
		}

		break; }

	case 'I': {
		vector<bool> type;
		char aux[] = "VALUES";
		string row;
		int cont = 0;
		int conf = -1;
		string aux2;

		splited.erase(splited.begin());//insert
		splited.erase(splited.begin());//into

		for (int i = 0; i < tablas.size(); i++) {//obtiene la tabla
			if (tablas[i].tablename.compare(splited[0]) == 0){
				n = i;
				break;
			}
		}if (n == -1) {
			cout << "Tabla no existe" << endl;
			return;
		}
		splited.erase(splited.begin());//tablename

		while (strcmp(splited[0], aux) != 0) { //guarda los tipos de datos de las columnas de la tabla para compararlos
			for (int i = 0; i < indices.size(); i++) {//verifica si existe un indice para la columna
				if (indices[i].tabla.compare(tablas[n].tablename) == 0) {
					if (indices[i].columna.compare(splited[0]) == 0) {
						conf = cont;//guarda el numero de la columna donde existe indice
					}
				}
			}
			cont++;

			if (tablas[n].getType(splited[0]).compare("int") == 0) { //INT 1
				type.push_back(1);
				splited.erase(splited.begin());
			}
			else if (tablas[n].getType(splited[0]).compare("string") == 0) { //STRING 0
				type.push_back(0);
				splited.erase(splited.begin());
			}
			else {
				cout << "Columna incorrecta o no ubicada" << endl;
				return;
			}
		}
		splited.erase(splited.begin());//values

		for (int i = 0; i < type.size(); i++) { //validacion de tipos
			if (conf == i) {
				aux2 = splited[0];//guarda el dato de la columna con indice
			}

			string a = splited[0];

			if (type[i] == 1 && validarInt(a)) {
				row += a;
				row += " ";
			}
			else{
				row += a;
				row += " ";
			}
			splited.erase(splited.begin());
		}
		row.pop_back();

		if (aux2.compare(" ") != 0) {
			for (int i = 0; i < indices.size(); i++) {//inserta el nuevo dato al arbol 
				if (indices[i].tabla.compare(tablas[n].tablename) == 0) {
					indices[i].insert(stoi(aux2));

					Tree<int, Asc>* ptr = &indices[i];
					tablas[n].insert(row,ptr,aux2);
					cout << "Los datos fueron insertados con exito y el indice fue actualizado" << endl;
					return;
				}
			}
		}

		tablas[n].insert(row);

		cout << "Los datos fueron insertados con exito" << endl;
		break; }

	case 'S': {
		vector<string> cols;
		char aux[] = "FROM";
		char aux2[] = "WHERE";
		splited.erase(splited.begin());//select

		if (splited[0][0] == '*') {
			splited.erase(splited.begin());//*
		}
		else {
			while (strcmp(splited[0], aux) != 0) {//columnas especificas
				cols.push_back(splited[0]);
				splited.erase(splited.begin());
			}
		}
		splited.erase(splited.begin());//from

		for (int i = 0; i < tablas.size(); i++) {//obtiene la tabla
			if (tablas[i].tablename.compare(splited[0]) == 0) {
				n = i;
				break;
			}
		}
		if (n == -1) {
			cout << "Tabla no existe" << endl;
			return;
		}
		splited.erase(splited.begin());//tablename

		if (!splited.empty()){//verifica si hay where
			splited.erase(splited.begin());//where

			string x = splited[0]; //columna
			char y = splited[1][0]; //signo
			string z = splited[2];//valor

			for (int i = 0; i < indices.size(); i++) {//verifica si existe un indice para la columna
				if (indices[i].tabla.compare(tablas[n].tablename) == 0) {
					if (indices[i].columna.compare(x) == 0) {
						Tree<int, Asc>* ptr = &indices[i];
						tablas[n].select_index_where(ptr,cols,x,y,z);
						return;
					}
				}
			}

			tablas[n].select_where(cols,x, y, z);
			return;
		}
		
		for (int i = 0; i < indices.size(); i++) {//verifica si existe un indice para la consulta
			if (indices[i].tabla.compare(tablas[n].tablename) == 0) {
				if (cols.empty()) {//select *
					Tree<int, Asc>* ptr = &indices[i];
					tablas[n].select_index(ptr,cols);
					return;
				}
				else {//columnas especificas
					for (int j = 0; j < cols.size(); j++){
						if (indices[i].columna.compare(cols[j]) == 0) {
							Tree<int, Asc>* ptr = &indices[i];
							tablas[n].select_index(ptr,cols);
							return;
						}
					}
				}
			}
		}

		tablas[n].select(cols);
		break; }

	case 'U': {
		vector<pair<string, string>> datos;
		char aux[] = "WHERE";
		splited.erase(splited.begin());//update

		for (int i = 0; i < tablas.size(); i++) {//obtiene la tabla
			if (tablas[i].tablename.compare(splited[0]) == 0) {
				n = i;
				break;
			}
		}if (n == -1) {
			cout << "Tabla no existe" << endl;
			return;
		}
		splited.erase(splited.begin());//tablename
		splited.erase(splited.begin());//set

		while (!splited.empty() && strcmp(splited[0], aux) != 0) {//guarda los datos a modificar - colum/dato
			string x = splited[0];
			string y = splited[2];
			datos.push_back(pair<string, string>(x, y));

			splited.erase(splited.begin());//columna
			splited.erase(splited.begin());//signo
			splited.erase(splited.begin());//dato
		}

		if (!splited.empty()) {//verifica si hay where
			splited.erase(splited.begin());//where

			string x = splited[0]; //columna
			char y = splited[1][0]; //signo
			string z = splited[2];//valor

			tablas[n].update_where(datos, x, y, z);
			cout << "Los datos fueron actualizados con exito" << endl;
			return;
		}

		tablas[n].update(datos);
		cout << "Los datos fueron actualizados con exito" << endl;
		break; }

	case 'D': {
		splited.erase(splited.begin());//delete
		splited.erase(splited.begin());//from

		for (int i = 0; i < tablas.size(); i++) {//obtiene la tabla
			if (tablas[i].tablename.compare(splited[0]) == 0) {
				n = i;
				break;
			}
		}if (n == -1) {
			cout << "Tabla no existe" << endl;
			return;
		}
		splited.erase(splited.begin());//tablename

		if (!splited.empty()) {//verifica si hay where
			splited.erase(splited.begin());//where

			string x = splited[0]; //columna
			char y = splited[1][0]; //signo
			string z = splited[2];//valor

			for (int i = 0; i < indices.size(); i++) {//verifica si existe un indice para la columna
				if (indices[i].tabla.compare(tablas[n].tablename) == 0) {
					Tree<int, Asc>* ptr = &indices[i];
					tablas[n].delete_where(ptr,x, y, z);
					cout << "Los datos fueron eliminados con exito" << endl;
					return;
				}
			}

			tablas[n].delete_where(x,y,z);
			cout << "Los datos fueron eliminados con exito" << endl;
			return;
		}


		for (int i = 0; i < indices.size(); i++) {//verifica si existe un indice para la tabla
			if (indices[i].tabla.compare(tablas[n].tablename) == 0) {
				indices.erase(indices.begin() + i);
			}
		}

		tablas[n].deletef();
		cout << "Los datos fueron eliminados con exito" << endl;
		break; }

	case 'M': {
		vector<pair<int, int>> datos;
		char aux[] = "range";
		int num = 0;

		splited.erase(splited.begin());//massive
		splited.erase(splited.begin());//insert
		num = atoi(splited[0]);//num
		splited.erase(splited.begin());//num
		splited.erase(splited.begin());//into
		
		for (int i = 0; i < tablas.size(); i++) {//obtiene la tabla
			if (tablas[i].tablename.compare(splited[0]) == 0) {
				n = i;
				break;
			}
		}if (n == -1) {
			cout << "Tabla no existe" << endl;
			return;
		}
		splited.erase(splited.begin());//tablename

		while (!splited.empty()) {
			if (strcmp(splited[0], aux) == 0){//range
				splited.erase(splited.begin());
				datos.push_back(pair<int, int>(atoi(splited[1]) - atoi(splited[0]),atoi(splited[1])));

				splited.erase(splited.begin());//#1
				splited.erase(splited.begin());//#2
			}
			else {
				datos.push_back(pair<int, int>(0, 0));
				splited.erase(splited.begin());
			}
		}

		tablas[n].massive(datos,num);
		cout << "Columnas insertadas masivamente con exito" << endl;
		break; }

	default:
		cout << "Comando no reconocido :c" << endl;
		break;
	}

}

void consultorMeta() {
	for (int i = 0; i < tablas.size(); i++){
		cout << tablas[i].tablename << endl;
		tablas[i].printMetadata();

		for (int i = 0; i < indices.size(); i++) {
			if (indices[i].tabla.compare(tablas[i].tablename) == 0) {
				cout << "Existe un indice " << indices[i].nombre << " en esta tabla, el cual indexa " << indices[i].columna << endl;
				indices[i].print_endl();
			}
		}
		cout << endl;
	}
}

vector<char*> shell_split_line(string command) {
	char* token;
	char* next_token = NULL;
	vector<char*> palabras;

	char* c_command = new char[command.length() + 1];//copia el comando en char*
	strcpy_s(c_command, command.length() + 1, command.c_str());
	//const char* delim = " \t\r\n\a";

	token = strtok_s(c_command, SHELL_TOK_DELIM, &next_token);
	while (token != NULL) {
		palabras.push_back(token);

		token = strtok_s(NULL, SHELL_TOK_DELIM, &next_token);
	}

	return palabras;
}

void loop() {
	string command;
	vector<char*> splited;

	while (1){
		splited.clear();
		cout << "Query:";
		getline(cin, command);

		if (command.compare("exit") == 0){//salir
			exit(0);
		}

		else if (command.compare("metadata") == 0){//metadata
			consultorMeta();
			continue;
		}

		splited = shell_split_line(command);

		for (int i = 0; i < splited.size(); i++){//Consulta parseada
			cout << splited[i] << endl;
		}

		auto start = chrono::system_clock::now();{
		shell_execute(splited);
		}auto end = chrono::system_clock::now();

		chrono::duration<double> elapsed = end - start;
		cout << "Tardo : " << elapsed.count() << " s"<<endl;
	}
}

int main(){
	loop();

	return 0;
}