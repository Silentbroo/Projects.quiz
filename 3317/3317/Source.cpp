#include <iostream>
using namespace std;
int main() {
	char letters;
	char answer;
	int question = 1;
	int sleeping = 0;
	int mulan = 0;
	int jazmin = 0;
	int tiana = 0;
	cout << "Which princess are you?" << endl;


	////////question 1/////////////////////////////
	cout << "What do you like to do in your free time?" << endl;
	cout << "r=read a book, s=sleep,l=listen to music,p=practice" << endl;
	cin >> answer;
	if (answer == 'r')
		jazmin = jazmin + 1;
	else if (answer == 's')
		sleeping = sleeping + 1;
	else if (answer == 'l')
		tiana = tiana + 1;
	else if (answer = 'p')
		mulan = mulan + 1;
	else
		cout << "invaild input" << endl;

	//question 2/////////////////

	cout << "What’s your favorite food?" << endl;
	cout << "f=fruits, v=vegis, c=chicken, n=noddles" << endl;
	cin >> answer;
	if (answer == 'f')
		jazmin = jazmin + 1;
	else if (answer == 'v')
		sleeping = sleeping + 1;
	else if (answer == 'c')
		tiana = tiana + 1;
	else if (answer = 'n')
		mulan = mulan + 1;
	else
		cout << "invaild input" << endl;

	////// question3/////////

	cout << "What pet would you like to have as a pet" << endl;
	cout << "t=tiger,c=cat,f=frog,d=dragon" << endl;
	cin >> answer;
	if (answer == 't')
		jazmin = jazmin + 1;
	else if (answer == 'c')
		sleeping = sleeping + 1;
	else if (answer == 'f')
		tiana = tiana + 1;
	else if (answer = 'd')
		mulan = mulan + 1;
	else
		cout << "invaild input" << endl;

	//question 4//////

	cout << "What’s your favorite color?" << endl;
	cout << "b=blue,p=pink,g=green,r=red" << endl;
	cin >> answer;
	if (answer == 'b')
		jazmin = jazmin + 1;
	else if (answer == 'p')
		sleeping = sleeping + 1;
	else if (answer == 'g')
		tiana = tiana + 1;
	else if (answer = 'r')
		mulan = mulan + 1;
	else
		cout << "invaild input" << endl;

	///question 5///
	cout << "What type of music do you like to listen to" << endl;
	cout << "r=R&B,c=clasics,h=hip-hop,e=edm" << endl;
	cin >> answer;
	if (answer == 'r')
		jazmin = jazmin + 1;
	else if (answer == 'c')
		sleeping = sleeping + 1;
	else if (answer == 'h')
		tiana = tiana + 1;
	else if (answer = 'e')
		mulan = mulan + 1;
	else
		cout << "invaild input" << endl;

	///question 6///

	cout << "What season do you enjoy?" << endl;
	cout << "s=summer,p=spring,f=fall,w=winter" << endl;
	cin >> answer;
	if (answer == 's')
		jazmin = jazmin + 1;
	else if (answer == 'p')
		sleeping = sleeping + 1;
	else if (answer == 'f')
		tiana = tiana + 1;
	else if (answer = 'w')
		mulan = mulan + 1;
	else
		cout << "invaild input" << endl;

	///question 7///

	cout << "Favorite sport?" << endl;
	cout << "s=soccer,t=tennis,b=basketball,a=archery" << endl;
	cin >> answer;
	if (answer == 's')
		jazmin = jazmin + 1;
	else if (answer == 't')
		sleeping = sleeping + 1;
	else if (answer == 'b')
		tiana = tiana + 1;
	else if (answer = 'a')
		mulan = mulan + 1;
	else
		cout << "invaild input" << endl;

	///question 8//

	cout << "What's your favorite holiday" << endl;
	cout << "c=christmas,j=4th of july,t=Thanksgiving,n=new years" << endl;
	cin >> answer;
	if (answer == 'c')
		jazmin = jazmin + 1;
	else if (answer == 'j')
		sleeping = sleeping + 1;
	else if (answer == 't')
		tiana = tiana + 1;
	else if (answer = 'n')
		mulan = mulan + 1;
	else
		cout << "invaild input" << endl;

}



cout << points << endl;
cout << "1-8 points is jazmine" << endl;
cout << "9-16 is sleeping beauty" << endl;
cout << "17-24 is tiana" << endl;
cout << "25-32 is Mulan" << endl;


