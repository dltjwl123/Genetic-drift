#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <stdlib.h>
using namespace std;
string arr[3000][6];
int arr_row = 3000, arr_col = 6;
void input(fstream& fs) {
	string str;
	int i = 0, j = 0;
	getline(fs, str, ',');
	while (!fs.eof()) {
		getline(fs, str, ',');
		if (str.size() > 10) {
			while (str.back() != '\n') str.pop_back();
			str.pop_back();
		}
		if (str.size() < 10) {
			int zero = 10 - str.size();
			string tmp;
			while (zero--) tmp.push_back('0');
			str = tmp + str;
		}
		arr[i][j++] = str;
		if (j == 6) {
			i++;
			j = 0;
		}
	}
	fs.close();
}
int get_p(string& str, const int p) {
	int count = 0;
	for (int i = 0; i < 10; i++) {
		if (str[i] - '0' < p) count++;
	}
	return count;
}
void initial_state(vector<pair<int, int>>& initial) {
	for (int i = 0; i < arr_row; i++) {
		for (int j = 0; j < arr_col; j++) {
			string tmp = arr[i][j];
			if (get_p(tmp, 5) == 5) {
				initial.push_back({ i,j });
			}
		}
	}
}
pair<int, int> random_state(vector<pair<int, int>>& initial) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, initial.size() - 1);
	return initial[dis(gen)];
}
void genetic_drift(const int initial_p, const pair<int,int> index) {
	int p = initial_p, total = 0;
	int row = index.first, col = index.second;
	string cur = arr[row][col];
	char col_alpha;
	if (row < 1000) col_alpha = 'A';
	else if (row >= 1000 && row < 2000) col_alpha = 'G';
	else col_alpha = 'M';
	col_alpha = col_alpha + col;
	if ((row + 1) % 10 != 0) cout << (row + 1) % 1000 << col_alpha << "\n";
	else cout << 1000 << col_alpha << "\n";
	while (p > 0 && p < 10) {
		cout << "t = " << total << "\n";
		total++;
		p = get_p(cur, p);
		if (p < 10) cout << "(" << cur << ", 0." << p << ")" << "\n";
		else  cout << "(" << cur << ", 1)" << "\n";
		col++;
		if (col == arr_col) {
			row++;
			if (row % 1000 == 0) row -= 1000;
			col = 0;
		}
		cur = arr[row][col];
	}
	cout << "총 세대 수: " << total - 1 << "\n";
}
void print() {
	for (int i = 0; i < arr_row; i++) {
		for (int j = 0; j < arr_col; j++) {
			cout << arr[i][j] << " ";
		}
		cout << "\n";
	}
}
int main() {
	fstream fs;
	fs.open("test.csv", ios::in);
	input(fs);
	vector<pair<int, int>> initial;
	initial_state(initial);
	bool check = true;
	while (check) {
		pair<int, int> initial_index = random_state(initial);
		genetic_drift(5, initial_index);
		cout << "1: 계속하기 0: 종료\n";
		cin >> check;
	}
	return 0;
}