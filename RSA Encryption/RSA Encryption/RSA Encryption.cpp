#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <string>
#include <sstream>
#include <cmath>
#include <stdio.h>
#include <iomanip>
using namespace std;


//this function is to check whether a number < 1850 is a prime or no. 1850 is 43*43+1
bool if_prime(int n){

	vector<int> prime = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43 };
	for (long long int i = 0; i < prime.size(); i++){
		if (n%prime[i] == 0){ return false; }
	}
	return true;
}

//this one generate prime within a certain range
long long int Generate_prime(int mini, int maxi){

	srand(time(0));     //This is to seed the random function in the compile, to not give you the same random numbers

	long long int n = 16;
	while (!if_prime(n)){
		n = mini + ((float)rand() / RAND_MAX) * (maxi - mini - 1);
	}
	return n;
}

// this one over here removes the spaces within a string
string no_space(string s){
	for (int i = 0; i < s.length(); i++){
		if (isspace(s[i])){

			s.erase(s.begin() + i);
		}


	}
	return s;
}

//this function here do the process of finding the remainder of (num)^pwr mod dg in a very efficient time
int remain_keys(int dg, long long int pwr, long long int num, long long int mod){
	long long int power = 1;
	long long result = 1;
	if (dg == 0){
		for (int i = 0; i < pwr; i++){
			result = (result%mod)*num;
		}
		return (result%mod);
	}
	else{
		for (int i = 0; i < dg; i++){
			power *= 10;
		}

		long long int rnd = pwr - (pwr%power);
		rnd = rnd / (power);

		long long int iterator = num;

		for (int j = 0; j < dg; j++){
			for (int i = 0; i < 10; i++){
				result = (result%mod)*iterator;

			}

			if (j == dg - 1){ break; }
			iterator = result%mod;
			result = 1;
		}

		result = (result%mod);
		long long int r2 = 1;
		for (int i = 0; i < rnd; i++){
			r2 = (r2%mod)*result;

		}


		long long int new_dg = floor(log10(pwr%power));


		return(r2%mod)*remain_keys(new_dg, pwr%power, num, mod) % mod;
	}


}

//as suggested by the name, it is for encrypting the message

string encryt_msg(long long int e, long long int n, string msg, int block){


	string M = "";
	int carrier = 0;
	int j = 0;
	int counter = 0;
	while (!isspace(msg[j]) && j != msg.length()){
		while (counter<block && j != msg.length()){
			carrier = carrier * 10 + (msg[j] - '0');
			counter++;

			j++;
		}

		M = M + ' ' + to_string(remain_keys(floor(log10(e)), e, carrier, n));
		counter = 0;
		carrier = 0;


	}
	return M;

}

string encryt_msg_hex(long long int e, long long int n, string msg){


	string M = "";
	int carrier = 0;
	int i = 1;
	
	while (i < msg.length()){

		while (!isspace(msg[i]) && i < msg.length()){

			carrier = carrier * 10 + (msg[i] - '0');
			
			i++;
		

		}
		if (carrier != 0){
			M = M + ' ' + to_string(remain_keys(floor(log10(e)), e, carrier, n));
		}
		
		i++;
		carrier = 0;
	}
		

	


	
	return M;

}
//Encrypt using hex



//----------------------------- Functions to find e from d ---------------------------------------------

//this is to find the reaminders in the euclidean algorithm used to find gcd(x,y)
vector<long long int> R(long long int x, long long int y){
	vector <long long int> r = { max(x, y), min(x, y) };
	long long int new_r = max(x, y) % min(x, y);
	while (new_r != 0){
		r.push_back(new_r);
		new_r = r[r.size() - 2] % new_r;
	}


	return r;
}
//this is to find the quotients in the euclidean algorithm used to find gcd(x,y)
vector< long long int> Q(long long int x, long long int y){
	vector <long long int> v = R(x, y);
	vector <long long int> q;
	for (long long int i = 0; i < v.size() - 2; i++){
		q.push_back(
			(v[i] - v[i + 2]) / v[i + 1]
			);
	}

	return q;
}

long long int U(long long int n, long long int x, long long int y);
//This is used to find the coefficients of the r with the smaller index while the process of inversing the eucledian algorithm
long long int W(long long int n, long long int x, long long int y){
	if (n == 1){

		return 1;
	}
	else{

		return U(n - 1, x, y);

	}

}
//This is used to find the coefficients of the r with the larger index while the process of inversing the eucledian algorithm
long long int U(long long int n, long long int x, long long int y){
	if (n == 1){

		return -Q(x, y)[Q(x, y).size() - 1];

	}
	else{



		return (W(n - 1, x, y) - U(n - 1, x, y)*Q(x, y)[Q(x, y).size() - n]);

	}
}
long long int D(long long int p, long long int q, long long int maximum){
	long long int n = Generate_prime(max(p, q), maximum);
	return n;
}


//find the e from d, p and q
long long int E(long long int d, long long int  p, long long int q){
	long long int O = (p - 1)*(q - 1);

	int s = Q(O, d).size();

	if (O > d){
		return U(s, O, d);
	}
	else {
		return W(s, O, d);
	}


}



//the decryption function
string decryt_msg(long long int d, long long int n, string E){

	string M = "";
	int carrier = 0;
	int j = 0;
	while (j < E.length()){
		while (!isspace(E[j]) && j < E.length()){

			carrier = carrier * 10 + (E[j] - '0');
			j++;
		}
		long long int add = remain_keys(floor(log10(d)), d, carrier, n);
		string S = to_string(remain_keys(floor(log10(d)), d, carrier, n));

		int checker = log10(remain_keys(floor(log10(d)), d, carrier, n)) + 1;
		if (checker % 2 == 1){
			M = M + " 0" + S;
		}

		else {
			M = M + ' ' + S;
		}





		j++;
		carrier = 0;

	}

	return M;
}



//this one encode the message (i.e. letters --> numbers(but also as a string)

string encode_msg(string s){
	string M = "";
	int k = 0;
	for (int i = 0; i < s.length(); i++){
		if (isspace(s[i])){ M = M + '2' + '7'; }//changed for spaces
		else{
			k = s[i] - 'a' + 1;
			if (k < 10){
				M = M + '0' + to_string(k);

			}
			else{
				M = M + to_string((k - (k % 10)) / 10) + to_string(k % 10);
			}

		}
	}

	return M;
}
//this one translates the message (i.e. number (as a string) --> letters)
string translate_msg(string s){
	int i = 0;
	string message = "";
	int two_char = 0;
	string letters = " abcdefghijklmnopqrstuvwxyz ";//changed for spaces

	while (i < s.length() - 1){
		two_char = 10 * (s[i] - 48) + (s[i + 1] - 48);

		if (two_char == 0){
			i += 2;
		}
		else{
			message = message + letters[two_char];
			i += 2;
		}





	}
	return message;
}
//convert hexadecimal to decimal


string dec_to_hex(string s){
	long long int n = 0;
	string F_M = "";
	
	string m = "";
	int j = 0;
while( j < s.length()){
		while (s[j] != ' ' && j != s.length()){
			n = n * 10 + (s[j] - '0');
			j++;
		}
		while (n){
			if (n % 16 < 10){
				m = to_string(n % 16) + m;
			}
			else{
				if (n % 16 == 10){ m = 'a' + m; }
				else if (n % 16 == 11){ m = 'b' + m; }
				else if (n % 16 == 12){ m = 'c' + m; }
				else if (n % 16 == 13){ m = 'd' + m; }
				else if (n % 16 == 14){ m = 'e' + m; }
				else if (n % 16 == 15){
					m = 'f' + m;
				}

			}
			n = (n - (n % 16)) / 16;


		}
		
		F_M = F_M + m;
		m = "";
		
		j++;
	}

	

	
	return F_M;
}
//charaters to hexademical
string char_to_hex(string s){
	int n = 0;
		string M = "";
	for (int i = 0; i < s.length(); i++){
		n = static_cast<int> (s[i]);
		M = M + dec_to_hex(to_string(n));
	}
	return M;
}
//letters of hex to numbers
int hTn(char s){
	if (s== 'a'){ return 10 ; }
	if (s == 'b'){ return 11; }
	if (s == 'c'){ return 12; }
	if (s == 'd'){ return 13; }
	if (s == 'e'){ return 14; }

	if (s == 'f'){ return 15; }
	else {
		return s - '0';
	}
}
//hex to ascii
string hex_to_ascii(string s){
	int i = 0;
	int n = 0;
	int carry = 0;
	char c;
	string M = "";

	i = 0;
	while (i + 1 < s.length()){
		carry = 16 * hTn(s[i]) + hTn(s[i+1]);
		
		
		c = carry;   // when you put character = to int, it gives you the char based on the ascii of the decimal 
		
		M = M + c;
		carry = 0;
		i+=2;
	}


	return M;
}

//remove commas
string no_comma(string s){
	for (int i = 0; i < s.length(); i++){
		if (s[i] ==','){

			s.erase(s.begin() + i);
		}


	}
	return s;
}
string put_commas(string s){
	string m = "' , '";
	for (int i = 0; i < s.length(); i++){
		if (isspace(s[i])){
			s.insert(i, m);
			i = i + 5;
		}
	}
	return s;
}
string encode_for_hex(string msg, int block){
	int j = 0; 
	int i = 0;
	string carry = "";
	int n = 0;
	int carrier = 0;
	string M = "";
	while (i < msg.length()){
		while (j < block && i<msg.length()){
			carry = carry + msg[i];
			j++;
			i++;
		}
		for (int k = 0; k < carry.length(); k++){
			n = 16*n + 16 * hTn(carry[k]);
		}

		M = M + ' ' + to_string(n/16);
		carry = "";
		n = 0;
		j = 0;
	}
	
	
	return M;
}
int main(){
	cout << put_commas("291122 104015 530290 732838 575769 271820 444550 400435 74891 348260 367866 222765 307723 554364 130401 278828 29875 577577 348260 388563 103756 722628 249912 211784 52168 575769 367866 41954 393679 554364 645475 307480 36147 785477 647460 84535 395255 487096 106457 571131 47856 288749 393679 618793 348260 361456 376048 157017 369630 26478 554364 130401 278828 386863 722628 299571 555195 472797 452721 348260 361456 571131 474138 647460 211784 247290 80445 776242 259517 472797 679581 554364 405334 268123 617349 709583 444550 289943 348260 577347 177933 417429 278828 407352 10483 462436 702112 351363 577347 157017 285104 158119 791110 714355 229069 659974 530290 417429 278828 354779 57286 74891 714355 52168 497775 111408 100310 259517 472797 265704 289943 472797 416540 530290 417429 278828 277507 497775 111408 100310 259517 472797 265704 289943 389748 452721 412722 768209 82005 577577 308505 124970 195469 471371 307723 303268 704517 306384 444466 291096 308505 537672 768209 52168 211784 52168 82005 249912 259517 318969 417429 278828 716448 419804 471371 307723 303268 704517 306384 444466 804390 198339 412722 768209 785477 228789 124970 111408 100310 259517 472797 265704 289943 575769 707815 472797 194176 412775 802708 497775 124970 195469 471371 307723 303268 704517 306384 444466 351740 36147 211784 389748 454754 211784 52168 278030 222215 318969 417429 278828 124970 124970 213669 71927 307723 566874 732838 800028 69435 248263 335989 791110 388563 303268 433957 303268 794674 389748 265704 369630 124970 124970 766098 768209 303268 388563 537672 106457 571131 753229 487096 211784 352962 157017 369630 162877 318969 417429 644123 18090 348260 194176 575769 337049 348260 367866 514075 72829 631749 124970 532196 713717 365113 530290 753567 702180 791110 335989 29875 577577 709583 195379 106457 571131 753229 487096 472797 163219 775003 80445 566220 124970 104875 530290 29875 577577 348260 388563 472797 476430 530290 444550 549892 753567 104015 530290 704517 306384 444466 780731 707815 267028 397505") << endl;
	

	long long int a, p, q, d, e;
	char c;
	int enter = 0;
	p = /*Generate_prime(50,100); */ 953;
	q = /* Generate_prime(50, 100);*/ 853;
	while (p == q){
		q = Generate_prime(50, 101);
	}
	d = /*D(p, q, 101);*/ 1579;


	e = E(d, p, q);


	string msg;

	int block = 4/* floor(log10(p*q));*/;

	/*if (block % 2 != 0){
		block = block + 1;

	}
	*/

	do{
		cout << "--------------------------------" << endl;
		cout << "Good Morning, sir! Welcome Aboard to our Encryption Station. Please have a seat and make a selection" << endl;
		cout << "To encrypt a message, please enter 1" << endl;
		cout << "To decrypt a message, please enter 2" << endl;
		cout << "To translate an encoded message, please enter 3" << endl;
		cout << "To encode a message, please enter 4" << endl;
		cout << "To know the encryption and decryption info, please enter 5" << endl;
		cout << "To remove commas, please press 6" << endl;
		cout << "To convert you message into hexadecimal, please press 7" << endl;
		cout << "To convert you hex message into letters, please press 8" << endl;
		cout << "To encrypt your message by hex encryption, please press 9" << endl;
		cout << "To exit, please press 10" << endl;
		cin >> a;
		if (a == 1){
			cout << "Please enter your message" << endl;
			cin.ignore();
			getline(cin, msg);
			cout << "The encrypted message is " << encryt_msg(e, p*q, encode_msg(msg), block) << endl;
		}
		else if (a == 2){
			cout << "Please the encrypted message" << endl;
			cin.ignore();
			getline(cin, msg);
			cout << "The decrypted message is " << decryt_msg(d, p*q, msg) << endl;
		}
		else if (a == 3){
			cout << "Please the encoded message (without spaces)" << endl;
			cin.ignore();
			getline(cin, msg);
			cout << "The message is ' " << translate_msg(no_space(msg)) << " ' " << endl;
		}
		else if (a == 4){
			cout << "Please enter your message" << endl;
			cin.ignore();  //must be put before getline if you are getting inputs before it. Otherwise the program will ignore getline
			getline(cin, msg);
			cout << "The encoded msg is" << endl << encode_msg(msg) << endl;
		}
		else if (a == 5){
			cout << "The prime p = " << p << endl;
			cout << "The prime q = " << q << endl;
			cout << "The number n = " << p*q << endl;
			cout << "The encryption key (e) = " << e << endl;
			cout << "The decryption key (d) = " << d << endl;
			cout << "Please keep them secret, sir!" << endl;
		}
		else if (a == 6){
			cout << "Please enter your message" << endl;
			cin.ignore();  //must be put before getline if you are getting inputs before it. Otherwise the program will ignore getline
			getline(cin, msg);
			cout << "The msg is" << endl << no_comma(msg) << endl;
		}
		else if (a == 7){
			cout << "Please enter your message" << endl;
			cin.ignore();  //must be put before getline if you are getting inputs before it. Otherwise the program will ignore getline
			getline(cin, msg);
			cout << "The msg is" << endl << dec_to_hex(msg) << endl;
		}
		else if (a == 8){
			cout << "Please enter your message" << endl;
			cin.ignore();  //must be put before getline if you are getting inputs before it. Otherwise the program will ignore getline
			getline(cin, msg);
			cout << "The msg is" << endl << hex_to_ascii(msg) << endl;
		}
		else if (a == 9){
			cout << "Please enter your message" << endl;
			cin.ignore();  //must be put before getline if you are getting inputs before it. Otherwise the program will ignore getline
			getline(cin, msg);
			cout << "The encrypted message is" << endl << encryt_msg_hex(e, p*q, encode_for_hex(char_to_hex(msg), block)) << endl;
		}


		
		else if (a == 10){
			cout << "Thanks for dealing with us!" << endl;
			break;
		}
		else{
			cout << "Please make a valid selection, sir" << endl;
			break;
		}

	} while (1);




	system("pause");
	return 0;
}




