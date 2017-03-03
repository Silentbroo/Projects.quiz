#include <iostream>
#include <windows.h>
using namespace std;
int main() {
	double num;
	double num2;
	double num3;

	cout << "Put your first number ugly cx" << endl;
	cin >> num;
	cout << " " << endl;
	cout << "Put the second number rawr xD" << endl;
	cin >> num2;
	cout << " " << endl;
	cout << "Put your third number plz" << endl;
	cin >> num3;
	cout << " " << endl;

	if (num3< num2< num)
		cout << "The numbers in order are " << num3 << ", " << num2 << ", " << num << "." << endl;
	cout<<  endl;
	if (num2< num3< num)
		cout << "The numbers in order are " << num2 << ", " << num3 << ", " << num << "." << endl;
	cout << endl;
	if (num3< num< num2)
		cout << "The numbers in order are " << num3 << ", " << num << ", " << num2 << "." << endl;
	cout << endl;
	if (num< num3< num2)
		cout << "The numbers in order are " << num << ", " << num3 << ", " << num2 << "." << endl;
	cout << endl;
	if (num< num2< num3)
		cout << "The numbers in order are " << num << ", " << num2 << ", " << num3 << "." << endl;
	cout << endl;
	if (num2< num< num3)
		cout << "The numbers in order are " << num2 << ", " << num<< ", " << num3 << "." << endl;



}