#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>  
#include "fsf.h"
#include "Tree.h"

using namespace std;

struct Asc {
	bool operator()(int a, int b) {
		return a < b;
	}
};

struct table {
	string tablename;
	vector<pair<string, string>> metadata; //nombre de la columna,tipo de dato

	table(string t) {
		tablename = t;
		createDirectoryIfNotExists(fs::path(PRINCIPAL_FOLDER));
		createDirectoryIfNotExists(getTableDirectory(tablename));
	}

	void insertMeta(vector<pair<string, string>>& meta){ //crear columnas en la tabla
		/*ofstream fileTable = ::getOfstream(tablename, tablename + ".txt", ios::trunc);
		fileTable.close();*/
		ofstream fileMeta = ::getOfstream(tablename, "metadata.txt", ios::trunc);
		for (int i = 0; i < meta.size(); i++) {
			fileMeta << meta[i].first << " " << meta[i].second << '\n';
			metadata.push_back(meta[i]);
		}		
		fileMeta.close();
	}

	string getType(string col) { //devuelve el tipo de la columna dada
		for (int i = 0; i < metadata.size(); i++) {
			if (metadata[i].first.compare(col) == 0) {
				return metadata[i].second;
			}
		}
		return "No se encontro la columna ";
	}

	void printMetadata() { //imprime la metadata de la tabla
		for (int i = 0; i < metadata.size(); i++) {
			cout << metadata[i].first << " : " << metadata[i].second << endl;
		}
	}

	void insert(string fila){
		ofstream file = getOfstream(tablename, tablename + ".txt", ofstream::out | ofstream::app);
		file << fila << '\n';
		file.close();
	}

	void insert(string fila, Tree<int, Asc>* arbol,string dato) {
		ofstream file = getOfstream(tablename, tablename + ".txt", ofstream::out | ofstream::app);

		int s = sizefile(tablename, tablename + ".txt");
		Nodo<int>* ptr = arbol->find(stoi(dato));
		ptr->pos.push_back((long long)s);
		arbol->listado.insert(pair<int, Nodo<int>*>(stoi(dato), ptr));

		file << fila << '\n';
		file.close();
	}
	
	void select(vector<string>& cols){
		ifstream file = getIfstream(tablename, tablename + ".txt", ifstream::in);

		if (cols.empty()) {
			for (int i = 0; i < metadata.size(); i++) {//imprime los nombres de las columnas
				cout << metadata[i].first << " ";
			}
			cout << endl;

			for (string line; getline(file, line);) {//SELECT *
				cout << line << endl;
			}
		}

		else {
			vector<int> nums;
			for (int i = 0; i < cols.size(); i++) {//#s columnas especificas
				for (int j = 0; j < metadata.size(); j++) {
					if (cols[i].compare(metadata[j].first) == 0) {
						nums.push_back(j);
						break;
					}
				}
			}

			for (int i = 0; i < nums.size(); i++) {//imprime los nombres de las columnas
				cout << metadata[nums[i]].first << " ";
			}
			cout << endl;

			for (string line; getline(file, line);) {//SELECT columnas especificas
				vector<string> aux;
				aux = string_spaces(line);

				for (int i = 0; i < nums.size(); i++) {
					cout << aux[nums[i]] << " ";
				}
				cout << endl;
			}
		}

		file.close();
	}

	void select_where(vector<string>& cols,string col,char sign, string sign2) {//columna,signo,valor
		ifstream file = getIfstream(tablename, tablename + ".txt", ifstream::in);
		int w = 0;

		for (int i = 0; i < metadata.size(); i++) {//# columna where
			if (col.compare(metadata[i].first) == 0) {
				w = i;
				break;
			}
		}

		if (cols.empty()) {//SELECT *
			for (int i = 0; i < metadata.size(); i++) {//imprime los nombres de las columnas
				cout << metadata[i].first << " ";
			}
			cout << endl;

			if (sign == '<') {
				for (string line; getline(file, line);) {//SELECT *
					vector<string> aux;
					aux = string_spaces(line);

					if (stoi(aux[w]) < stoi(sign2)){//condicion <
						cout << line << endl;
					}
					else {
						continue;
					}
				}
			}
			else if (sign == '>'){
				for (string line; getline(file, line);) {//SELECT *
					vector<string> aux;
					aux = string_spaces(line);

					if (stoi(aux[w]) > stoi(sign2)) {//condicion >
						cout << line << endl;
					}
					else {
						continue;
					}
				}
			}
			else{
				if (validarInt(sign2)) { // si es numero
					for (string line; getline(file, line);) {//SELECT *
						vector<string> aux;
						aux = string_spaces(line);

						if (stoi(aux[w]) == stoi(sign2)) {//condicion =
							cout << line << endl;
						}
						else {
							continue;
						}
					}
				}
				else {//si es string
					for (string line; getline(file, line);) {//SELECT *
						vector<string> aux;
						aux = string_spaces(line);

						if (aux[w].compare(sign2) == 0){//condicion =
							cout << line << endl;
						}
						else {
							continue;
						}

					}
				}
			}	
		}

		else{
			vector<int> nums;
			for (int i = 0; i < cols.size(); i++) {//#s columnas especificas
				for (int j = 0; j < metadata.size(); j++) {
					if (cols[i].compare(metadata[j].first) == 0) {
						nums.push_back(j);
						break;
					}
				}
			}

			for (int i = 0; i < nums.size(); i++) {//imprime los nombres de las columnas
				cout << metadata[nums[i]].first << " ";
			}
			cout << endl;

			if (sign == '<') {
				for (string line; getline(file, line);) {//SELECT columnas especificas
					vector<string> aux;
					aux = string_spaces(line);

					if (stoi(aux[w]) < stoi(sign2)) {//condicion <
						for (int i = 0; i < nums.size(); i++) {
							cout << aux[nums[i]] << " ";
						}
						cout << endl;
					}
					else {
						continue;
					}
				}
			}
			else if (sign == '>') {
				for (string line; getline(file, line);) {//SELECT columnas especificas
					vector<string> aux;
					aux = string_spaces(line);

					if (stoi(aux[w]) > stoi(sign2)) {//condicion >
						for (int i = 0; i < nums.size(); i++) {
							cout << aux[nums[i]] << " ";
						}
						cout << endl;
					}
					else {
						continue;
					}
				}
			}
			else {
				if (validarInt(sign2)) { // si es numero
					for (string line; getline(file, line);) {//SELECT columnas especificas
						vector<string> aux;
						aux = string_spaces(line);

						if (stoi(aux[w]) == stoi(sign2)) {//condicion =
							for (int i = 0; i < nums.size(); i++) {
								cout << aux[nums[i]] << " ";
							}
							cout << endl;
						}
						else {
							continue;
						}
					}
				}
				else {//si es string
					for (string line; getline(file, line);) {//SELECT columnas especificas
						vector<string> aux;
						aux = string_spaces(line);

						if (aux[w].compare(sign2) == 0) {//condicion =
							for (int i = 0; i < nums.size(); i++) {
								cout << aux[nums[i]] << " ";
							}
							cout << endl;
						}
						else {
							continue;
						}
					}
				}
			}
		}

		file.close();
	}

	void select_index(Tree<int, Asc>* arbol, vector<string>& cols){
		ifstream file = getIfstream(tablename, tablename + ".txt", ifstream::in);

		if (cols.empty()) {
			for (int i = 0; i < metadata.size(); i++) {//imprime los nombres de las columnas
				cout << metadata[i].first << " ";
			}
			cout << endl;

			for (auto& x : arbol->listado) {
				for (int i = 0; i < x.second->pos.size(); i++) {
					file.seekg(x.second->pos[i], ios::beg);
					string line; 
					getline(file, line);
					cout << line << endl;
				}	
			}
		}

		else {
			vector<int> nums;
			for (int i = 0; i < cols.size(); i++) {//#s columnas especificas
				for (int j = 0; j < metadata.size(); j++) {
					if (cols[i].compare(metadata[j].first) == 0) {
						nums.push_back(j);
						break;
					}
				}
			}

			for (int i = 0; i < nums.size(); i++) {//imprime los nombres de las columnas
				cout << metadata[nums[i]].first << " ";
			}
			cout << endl;

			for (auto& x : arbol->listado) {
				for (int i = 0; i < x.second->pos.size(); i++) {
					file.seekg(x.second->pos[i], ios::beg);
					string line;
					getline(file, line);

					vector<string> aux;
					aux = string_spaces(line);

					for (int i = 0; i < nums.size(); i++) {
						cout << aux[nums[i]] << " ";
					}
					cout << endl;
				}
			}

		}

		file.close();
	}

	void select_index_where(Tree<int, Asc>* arbol,vector<string>& cols, string col, char sign, string sign2) {
		ifstream file = getIfstream(tablename, tablename + ".txt", ifstream::in);
		int w = 0;

		for (int i = 0; i < metadata.size(); i++) {//# columna where
			if (col.compare(metadata[i].first) == 0) {
				w = i;
				break;
			}
		}

		if (cols.empty()) {//SELECT *
			for (int i = 0; i < metadata.size(); i++) {//imprime los nombres de las columnas
				cout << metadata[i].first << " ";
			}
			cout << endl;

			if (sign == '<') {
				for (auto& x : arbol->listado) {
					if (x.first < stoi(sign2)) {//condicion <
						for (int i = 0; i < x.second->pos.size(); i++) {
							file.seekg(x.second->pos[i], ios::beg);
							string line;
							getline(file, line);
							cout << line << endl;
						}
					}
					else {
						continue;
					}					
				}
			}
			else if (sign == '>') {
				for (auto& x : arbol->listado) {
					if (x.first > stoi(sign2)) {//condicion >
						for (int i = 0; i < x.second->pos.size(); i++) {
							file.seekg(x.second->pos[i], ios::beg);
							string line;
							getline(file, line);
							cout << line << endl;
						}
					}
					else {
						continue;
					}
				}
			}
			else {
				map<int, Nodo<int>*>::iterator it;
				it = arbol->listado.find(stoi(sign2));//busca el elemento

				if (it != arbol->listado.end()) {//si lo encontro
					for (int i = 0; i < (it->second)->pos.size() ; i++) {
						file.seekg((it->second)->pos[i], ios::beg);
						string line;
						getline(file, line);
						cout << line << endl;
					}
				}
			}
		}

		else {
			vector<int> nums;
			for (int i = 0; i < cols.size(); i++) {//#s columnas especificas
				for (int j = 0; j < metadata.size(); j++) {
					if (cols[i].compare(metadata[j].first) == 0) {
						nums.push_back(j);
						break;
					}
				}
			}

			for (int i = 0; i < nums.size(); i++) {//imprime los nombres de las columnas
				cout << metadata[nums[i]].first << " ";
			}
			cout << endl;

			if (sign == '<') {
				for (auto& x : arbol->listado) {
					if (x.first < stoi(sign2)) {//condicion <
						for (int i = 0; i < x.second->pos.size(); i++) {
							file.seekg(x.second->pos[i], ios::beg);
							string line;
							getline(file, line);
							
							vector<string> aux;
							aux = string_spaces(line);

							for (int i = 0; i < nums.size(); i++) {
								cout << aux[nums[i]] << " ";
							}
							cout << endl;
						}
					}
					else {
						continue;
					}
				}
			}
			else if (sign == '>') {
				for (auto& x : arbol->listado) {
					if (x.first > stoi(sign2)) {//condicion >
						for (int i = 0; i < x.second->pos.size(); i++) {
							file.seekg(x.second->pos[i], ios::beg);
							string line;
							getline(file, line);

							vector<string> aux;
							aux = string_spaces(line);

							for (int i = 0; i < nums.size(); i++) {
								cout << aux[nums[i]] << " ";
							}
							cout << endl;
						}
					}
					else {
						continue;
					}
				}
			}
			else {
				map<int, Nodo<int>*>::iterator it;
				it = arbol->listado.find(stoi(sign2));//busca el elemento

				if (it != arbol->listado.end()) {//si lo encontro
					for (int i = 0; i < (it->second)->pos.size(); i++) {
						file.seekg((it->second)->pos[i], ios::beg);
						string line;
						getline(file, line);

						vector<string> aux;
						aux = string_spaces(line);

						for (int i = 0; i < nums.size(); i++) {
							cout << aux[nums[i]] << " ";
						}
						cout << endl;
					}
				}
			}
		}

		file.close();

	}

	void update(vector<pair<string, string>>& datos){
		ifstream file = getIfstream(tablename, tablename + ".txt", ifstream::in);//lector
		ofstream aux = getOfstream(tablename, "temp.txt", ofstream::out | ofstream::app);//escritor

		vector<int> nums;
		for (int i = 0; i < datos.size(); i++) {//#s columnas especificas a modificar
			for (int j = 0; j < metadata.size(); j++) {
				if (datos[i].first.compare(metadata[j].first) == 0) {
					nums.push_back(j);
					break;
				}
			}
		}

		for (string line; getline(file, line);) {//UPDATE
			vector<string> aux2;
			aux2 = string_spaces(line);
			string aux3;

			for (int i = 0,j = 0; i < aux2.size(); i++){//guarda la linea modificada
				if (j < nums.size() && i == nums[j]) {
					aux3 += datos[j].second;
					aux3 += " ";
					j++;
				}
				else {
					aux3 += aux2[i];
					aux3 += " ";
				}				
			}

			aux3.pop_back();
			aux << aux3 << '\n';//escribe la linea modificada
		}

		file.close(); aux.close();

		/*string filedelete = tablename + ".txt";
		string fileupdate = "temp.txt";
		remove(filedelete.c_str());
		rename(fileupdate.c_str(), filedelete.c_str());*/

	}

	void update_where(vector<pair<string, string>>& datos, string col, char sign, string sign2){
		ifstream file = getIfstream(tablename, tablename + ".txt", ifstream::in);//lector
		ofstream aux = getOfstream(tablename, "temp.txt", ofstream::out | ofstream::app);//escritor

		int w = 0;
		for (int i = 0; i < metadata.size(); i++) {//# columna where
			if (col.compare(metadata[i].first) == 0) {
				w = i;
				break;
			}
		}

		vector<int> nums;
		for (int i = 0; i < datos.size(); i++) {//#s columnas especificas a modificar
			for (int j = 0; j < metadata.size(); j++) {
				if (datos[i].first.compare(metadata[j].first) == 0) {
					nums.push_back(j);
					break;
				}
			}
		}

		if (sign == '<') {
			for (string line; getline(file, line);) {//UPDATE
				vector<string> aux2;
				aux2 = string_spaces(line);

				if (stoi(aux2[w]) < stoi(sign2)) {//condicion <
					string aux3;

					for (int i = 0, j = 0; i < aux2.size(); i++) {//guarda la linea modificada
						if (j < nums.size() && i == nums[j]) {
							aux3 += datos[j].second;
							aux3 += " ";
							j++;
						}
						else {
							aux3 += aux2[i];
							aux3 += " ";
						}
					}

					aux3.pop_back();
					aux << aux3 << '\n';//escribe la linea modificada
				}
				else {
					aux << line << '\n';
				}
			}
		}
		else if (sign == '>') {
			for (string line; getline(file, line);) {//UPDATE
				vector<string> aux2;
				aux2 = string_spaces(line);

				if (stoi(aux2[w]) > stoi(sign2)) {//condicion >
					string aux3;

					for (int i = 0, j = 0; i < aux2.size(); i++) {//guarda la linea modificada
						if (j < nums.size() && i == nums[j]) {
							aux3 += datos[j].second;
							aux3 += " ";
							j++;
						}
						else {
							aux3 += aux2[i];
							aux3 += " ";
						}
					}

					aux3.pop_back();
					aux << aux3 << '\n';//escribe la linea modificada
				}
				else {
					aux << line << '\n';
				}
			}
		}
		else {
			if (validarInt(sign2)) { // si es numero
				for (string line; getline(file, line);) {//UPDATE
					vector<string> aux2;
					aux2 = string_spaces(line);

					if (stoi(aux2[w]) == stoi(sign2)) {//condicion =
						string aux3;

						for (int i = 0, j = 0; i < aux2.size(); i++) {//guarda la linea modificada
							if (j<nums.size() && i == nums[j]) {
								aux3 += datos[j].second;
								aux3 += " ";
								j++;
							}
							else {
								aux3 += aux2[i];
								aux3 += " ";
							}
						}

						aux3.pop_back();
						aux << aux3 << '\n';//escribe la linea modificada
					}
					else {
						aux << line << '\n';
					}
				}
			}
			else {//si es string
				for (string line; getline(file, line);) {//UPDATE
					vector<string> aux2;
					aux2 = string_spaces(line);

					if (aux2[w].compare(sign2) == 0) {//condicion =
						string aux3;

						for (int i = 0, j = 0; i < aux2.size(); i++) {//guarda la linea modificada
							if (j < nums.size() && i == nums[j]) {
								aux3 += datos[j].second;
								aux3 += " ";
								j++;
							}
							else {
								aux3 += aux2[i];
								aux3 += " ";
							}
						}

						aux3.pop_back();
						aux << aux3 << '\n';//escribe la linea modificada
					}
					else {
						aux << line << '\n';
					}
				}
			}
		}
	
		file.close(); aux.close();

		/*string filedelete = tablename + ".txt";
		char* ptr = new char[filedelete.length()];
		strcpy_s(ptr, filedelete.length() + 1, filedelete.c_str());

		string fileupdate = "temp.txt";
		char* ptr2 = new char[fileupdate.length()];
		strcpy_s(ptr2, fileupdate.length() + 1, fileupdate.c_str());

		int result = 0;
		result = remove(ptr);
		if (result == 0) {
			cout << "File successfully renamed";
		}
		else {
			perror("Error renaming file");
		}

		result = rename(ptr2, ptr);
		if (result == 0) {
			cout << "File successfully renamed";
		}
		else {
			perror("Error renaming file");
		}
		
		delete[] ptr;
		delete[] ptr2;*/
	}

	void deletef(){
		ofstream fileTable = ::getOfstream(tablename, tablename + ".txt", ios::trunc);
		fileTable.close();
	}

	void delete_where(string col, char sign, string sign2){
		ifstream file = getIfstream(tablename, tablename + ".txt", ifstream::in);//lector
		ofstream aux = getOfstream(tablename, "temp.txt", ofstream::out | ofstream::app);//escritor

		int w = 0;
		for (int i = 0; i < metadata.size(); i++) {//# columna where
			if (col.compare(metadata[i].first) == 0) {
				w = i;
				break;
			}
		}

		if (sign == '<') {
			for (string line; getline(file, line);) {//DELETE
				vector<string> aux2;
				aux2 = string_spaces(line);

				if (stoi(aux2[w]) < stoi(sign2)) {//condicion <
					continue;
				}
				else {
					aux << line << '\n';
				}
			}
		}
		else if (sign == '>') {
			for (string line; getline(file, line);) {//DELETE
				vector<string> aux2;
				aux2 = string_spaces(line);

				if (stoi(aux2[w]) > stoi(sign2)) {//condicion >
					continue;
				}
				else {
					aux << line << '\n';
				}
			}
		}
		else {
			if (validarInt(sign2)) { // si es numero
				for (string line; getline(file, line);) {//DELETE
					vector<string> aux2;
					aux2 = string_spaces(line);

					if (stoi(aux2[w]) == stoi(sign2)) {//condicion =
						continue;
					}
					else {
						aux << line << '\n';
					}
				}
			}
			else {//si es string
				for (string line; getline(file, line);) {//DELETE
					vector<string> aux2;
					aux2 = string_spaces(line);

					if (aux2[w].compare(sign2) == 0) {//condicion =
						continue;
					}
					else {
						aux << line << '\n';
					}
				}
			}
		}

		file.close(); aux.close();

		/*string filedelete = tablename + ".txt";
		char* ptr = new char[filedelete.length()];
		strcpy_s(ptr, filedelete.length() + 1, filedelete.c_str());

		string fileupdate = "temp.txt";
		char* ptr2 = new char[fileupdate.length()];
		strcpy_s(ptr2, fileupdate.length() + 1, fileupdate.c_str());

		int result = 0;
		result = remove(ptr);
		if (result == 0) {
			cout << "File successfully renamed";
		}
		else {
			perror("Error renaming file");
		}

		result = rename(ptr2, ptr);
		if (result == 0) {
			cout << "File successfully renamed";
		}
		else {
			perror("Error renaming file");
		}

		delete[] ptr;
		delete[] ptr2;*/
	}

	void delete_where(Tree<int, Asc>* arbol,string col, char sign, string sign2) {
		ifstream file = getIfstream(tablename, tablename + ".txt", ifstream::in);//lector
		ofstream aux = getOfstream(tablename, "temp.txt", ofstream::out | ofstream::app);//escritor
		long long cont = 0;
		map<int, int> eliminados;

		int w = 0;
		for (int i = 0; i < metadata.size(); i++) {//# columna where
			if (col.compare(metadata[i].first) == 0) {
				w = i;
				break;
			}
		}

		int y = 0;
		for (int i = 0; i < metadata.size(); i++) {//# columna donde esta indexado
			if (arbol->columna.compare(metadata[i].first) == 0) {
				y = i;
				break;
			}
		}

		if (sign == '<') {
			for (string line; getline(file, line);) {//DELETE
				vector<string> aux2;
				aux2 = string_spaces(line);
				eliminados.insert(pair<int, int>(stoi(aux2[y]),0));

				if (stoi(aux2[w]) < stoi(sign2)) {//condicion <
					arbol->remove(stoi(aux2[y]));
					arbol->listado.erase(stoi(aux2[y]));
					continue;
				}
				else {
					map<int, int>::iterator it;
					it = eliminados.find(stoi(aux2[y]));//busca el elemento

					if (it == eliminados.end()) {
						Nodo<int>* ptr = arbol->find(stoi(aux2[y]));
						ptr->pos.clear();
					}

					Nodo<int>* ptr = arbol->find(stoi(aux2[y]));
					ptr->pos.push_back(cont);
					aux << line << '\n';
				}
				cont += (line.size() + 2);
			}
		}
		else if (sign == '>') {
			for (string line; getline(file, line);) {//DELETE
				vector<string> aux2;
				aux2 = string_spaces(line);
				eliminados.insert(pair<int, int>(stoi(aux2[y]), 0));

				if (stoi(aux2[w]) > stoi(sign2)) {//condicion >
					arbol->remove(stoi(aux2[y]));
					arbol->listado.erase(stoi(aux2[y]));
					continue;
				}
				else {
					map<int, int>::iterator it;
					it = eliminados.find(stoi(aux2[y]));//busca el elemento

					if (it == eliminados.end()) {
						Nodo<int>* ptr = arbol->find(stoi(aux2[y]));
						ptr->pos.clear();
					}

					Nodo<int>* ptr = arbol->find(stoi(aux2[y]));
					ptr->pos.push_back(cont);
					aux << line << '\n';
				}
				cont += (line.size() + 2);
			}
		}
		else {
			for (string line; getline(file, line);) {//DELETE
				vector<string> aux2;
				aux2 = string_spaces(line);
				eliminados.insert(pair<int, int>(stoi(aux2[y]), 0));

				if (stoi(aux2[w]) == stoi(sign2)) {//condicion =
					arbol->remove(stoi(aux2[y]));
					arbol->listado.erase(stoi(aux2[y]));
					continue;
				}
				else {
					map<int, int>::iterator it;
					it = eliminados.find(stoi(aux2[y]));//busca el elemento

					if (it == eliminados.end()) {
						Nodo<int>* ptr = arbol->find(stoi(aux2[y]));
						ptr->pos.clear();
					}

					Nodo<int>* ptr = arbol->find(stoi(aux2[y]));
					ptr->pos.push_back(cont);
					aux << line << '\n';
				}
				cont += (line.size() + 2);
			}
		}

		file.close(); aux.close();
	}

	void massive(vector<pair<int, int>>& datos,int num){
		ofstream file = getOfstream(tablename, tablename + ".txt", ofstream::out | ofstream::app);
		srand(time(NULL));
		string aux;

		for (int j = 0; j < num; j++){//massive
			aux.clear();

			for (int i = 0; i < datos.size(); i++) {//fila aux
				if (datos[i].first == 0 && datos[i].second == 0){//default
					if (metadata[i].second.compare("int") == 0) {
						aux += to_string(j);
						aux += " ";
					}
					else if (metadata[i].second.compare("string") == 0 && metadata[i].first.compare("fecha") == 0) {
						int d = rand() % 31 + 1;//dia
						int m = rand() % 12 + 1;//mes
						int a = rand() % 121 + 1900;//año

						aux += (to_string(d) + "/" + to_string(m) + "/" + to_string(a));
						aux += " ";
					}
					else {
						aux += (metadata[i].first + to_string(j));
						aux += " ";
					}
				}
				else {//por rango
					if (metadata[i].second.compare("int") == 0) {
						int a = rand() % datos[i].first + datos[i].second;

						aux += to_string(a);
						aux += " ";
					}
					else if (metadata[i].second.compare("string") == 0 && metadata[i].first.compare("fecha") == 0) {
						int d = rand() % 31 + 1;//dia
						int m = rand() % 12 + 1;//mes
						int a = rand() % datos[i].first + datos[i].second;//año

						aux += (to_string(d) + "/" + to_string(m) + "/" + to_string(a));
						aux += " ";
					}
					else {
						int a = rand() % datos[i].first + datos[i].second;

						aux += (metadata[i].first + to_string(a));
						aux += " ";
					}
				}
			}

			aux.pop_back();//espacio extra
			file << aux << '\n';
		}

		file.close();
	}

	void indexTable(Tree<int, Asc>* arbol, string nombre, string col) {
		arbol->nombre = nombre;//nombre del index
		arbol->columna = col;//columna que indexa
		arbol->tabla = tablename;//tablename

		long long cont = 0;
		ifstream file = getIfstream(tablename, tablename + ".txt", ifstream::in);

		int w = 0;
		for (int i = 0; i < metadata.size(); i++) {//# columna indexar
			if (col.compare(metadata[i].first) == 0) {
				w = i;
				break;
			}
		}

		for (string line; getline(file, line);) {
			vector<string> aux;
			aux = string_spaces(line);

			if (arbol->insert(stoi(aux[w]))) {//inserta el valor en el arbol
				Nodo<int>* ptr = arbol->find(stoi(aux[w]));
				ptr->pos.push_back(cont);//inserta su posicion en el archivo en bytes
				arbol->listado.insert(pair<int, Nodo<int>*>(stoi(aux[w]), ptr));
			}
			else {
				Nodo<int>* ptr = arbol->find(stoi(aux[w]));
				ptr->pos.push_back(cont);
			}

			cont += (line.size()+2);
			cout << "V:" << aux[w] << " ";
		}
		
		file.close();
	}

};

