#include <ctype.h>
#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <map>
#include <vector>

#define PRINCIPAL_FOLDER "Base de Datos TXT"
#define DBG(x) cout << #x << " = " << x << endl;

using namespace std;
namespace fs = std::experimental::filesystem;
typedef string::iterator itr_t;

void createDirectoryIfNotExists(fs::path p) {
	if (!fs::exists(p)) {
		bool isCreated = fs::create_directory(p);
		if (!isCreated)
			throw runtime_error(string("createDirectoryIfNotExists: No se pudo crear la carpeta ") + p.string());
	}
}

fs::path getTableDirectory(const string& tablename) {
	fs::path p(PRINCIPAL_FOLDER);
	p /= tablename;
	return p;
}

fs::path getFileDirectory(const string& tablename, const string& filename) {
	fs::path p = getTableDirectory(tablename);
	p /= filename;
	return p;
}

std::ifstream getIfstream(const string& tablename, const string& filename, ios_base::openmode flags) {
	fs::path p = getFileDirectory(tablename, filename);
	ifstream file(p.string(), flags);
	if (!file.is_open()) throw runtime_error("No se pudo abrir el archivo " + p.string());
	return file;
}

std::ofstream getOfstream(const string& tablename, const string& filename, ios_base::openmode flags) {
	fs::path p = getFileDirectory(tablename, filename);
	ofstream file(p.string(), flags);
	if (!file.is_open()) throw runtime_error("No se pudo abrir el archivo " + p.string());
	return file;
}

int sizefile(string tablename,string filename) {
	fs::path p = getFileDirectory(tablename, filename);
	p = fs::canonical(p);

	int a = fs::file_size(p);
	return a;
}

bool validarInt(string num) {
	for (int i = 0; i < (int)num.size(); i++) {
		if (!isdigit(num[i])) {
			return 0;
		}
	}
	return 1;
}

vector<string> string_spaces(string& str){
	vector<string> resultado;

	string::const_iterator itBegin = str.begin();
	string::const_iterator itEnd = str.end();

	int numItems = 1;
	for (string::const_iterator it = itBegin; it != itEnd; ++it)
		numItems += *it == ' ';

	resultado.reserve(numItems);

	for (string::const_iterator it = itBegin; it != itEnd; ++it){
		if (*it == ' '){
			resultado.push_back(string(itBegin, it));
			itBegin = it + 1;
		}
	}

	if (itBegin != itEnd)
		resultado.push_back(std::string(itBegin, itEnd));

	return resultado;
}
