// ONP Kalkulator.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.

#include "pch.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>

using namespace std;

const int MAX_S = 100;  // definiuje rozmiar stosu

template <class Container>
void split(const std::string& str, Container& cont)
{
	std::istringstream iss(str);
	std::copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter(cont));
}

double CalcONP(string ONP) {
	double S[MAX_S];      // stos
	int p = 0;            // wskaźnik stosu
	string e;             // element wyrażenia ONP
	char f;
	int g;
	double v1, v2;         // argumenty operacji

	vector<string> words;
	split(ONP, words);


	for (int i = 0; words.size() > i; i++)           // w pętli przetwarzamy wyrażenie ONP
	{
		e = words[i];


		if (istringstream(e) >> g)        // konwertujemy na liczbę i sprawdzamy, czy nie było błędu
			S[p++] = g;      // umieszczamy ją na stosie
		else
		{                   // operator
			if ('~' == e[0]) {
				v1 = S[--p];
				v1 = -v1;
			}
			else
			{
				v2 = S[--p];      // pobieramy ze stosu dwa argumenty
				v1 = S[--p];
				switch (e[0])      // wykonujemy operacje wg operatora
				{
				case '^': v1 = pow(v1,v2); break;
				case '*': v1 *= v2; break;
				case '/': v1 /= v2; break;
				case '+': v1 += v2; break;
				case '-': v1 -= v2; break;
				}
			}
			S[p++] = v1;      // wynik umieszczamy na stosie
		}

	}

	return 	S[--p];
}

int prio(char c)
{
	switch(c)
	{
	  case '+':;
	  case '-': return 1;
	  case '*':;
	  case '/': return 2;
	  case '^': return 3;
	}
	return 0;
}

string ConvertONP(string normal) {
	string ONP;

	char S[MAX_S];                          // stos operatorów
	int p = 0;            // wskaźnik stosu
	char c;                                    // kolejny znak wyrażenia
	int i;

	string s;

	char previous = ' ';
	char next = ' ';

	vector<string> words;
	split(normal, words);

	for (int a = 0; words.size() > a; a++)           // w pętli przetwarzamy wyrażenie ONP
	{
		s = words[a];                                // czytamy znak

		if (istringstream(s) >> i) {
			ONP.append(s + " ");
			if ( p > 0 & S[p - 1] == '~')
			{
				ONP += S[--p];
				ONP += " ";
			}
			continue;
		}// konwertujemy na liczbę i sprawdzamy, czy nie było błędu
		c = s[0];

		if (words.size() > a)
		{
			next = words[a + 1][0];
		}
		else
		{
			next = ' ';
		}

		if (0 < a)
		{
			previous = words[a - 1][0];
		}
		else
		{
			previous = ' ';
		}

		if ((previous == ' ' && isdigit(next) && c == '-') || (!isdigit(previous) && isdigit(next) && c == '-')) {

			S[p++] = '~';
			continue;
		}

		switch(c)                              // analizujemy odczytany znak
		{
		  case '(': S[p++] = '(';         // nawias otwierający zawsze na stos
					 break;
		  case ')': while (S[p - 1] != '(')  // nawias zamykający
			  ONP += S[--p];
			  ONP += " ";	
			  p--;                     // usuwamy ze stosu nawias otwierający
			break;
		  case '+':;                           // operator
		  case '-':;
		  case '*':;
		  case '/':;
		  case '^': while (p)
					 {
					   if ((prio(c) == 3) || (prio(c) > prio(S[p - 1]))) break;
					   ONP += S[--p];
					   ONP += " ";// na wyjście przesyłamy ze stosu wszystkie
					 }                              // operatory o wyższych priorytetach
					 S[p++] = c;              // operator umieszczamy na stosie
					 break;
		  default:   
			ONP += c;
			ONP += " ";		// inaczej znak przesyłamy na wyjście
			break;
		}
	}

	while (p) {
		ONP += S[--p];
		ONP += " ";// jeśli tak, na wyjście przesyłamy
	}
	return ONP;
}

string getString() {
	string e;   

	getline(cin, e);

	return e;
}

int main()
{
	string previousONP = "";


	
	bool isEnd = false;

	while (!isEnd)
	{
		cout << "Wybierz co chcesz zrobic" << endl;
		cout << "1. Konwersja wyrazenia z notacji tradycyjnej do ONP" << endl;
		cout << "2. Obliczanie wartosci wyrazenia w ONP" << endl;
		cout << "3. Zakoncz program" << endl;

		int chose;
		string input = getString();
		
		if (!(stringstream(input) >> chose)) continue;

		switch (chose)
		{
		case 1:
			cout << "Wpisz wyrazenia z notacji tradycyjnej: ";
			previousONP = ConvertONP(getString());
			cout << "Wynik: " << previousONP << endl;
			break;
		case 2:
			cout << "1.Wpisz recznie wartosc ONP." << endl;
			cout <<	"2.Uzyj zapamietanego wyrazenia ONP." << endl;

			input = getString();
			if (!(stringstream(input) >> chose)) continue;
			cout << endl;
			switch (chose) {
			case 1:
				cout << "Wynik: " << fixed << CalcONP(getString()) << endl;
				break;
			case 2:
				if (previousONP != "")
					cout << "Wynik: " << fixed << CalcONP(previousONP) << endl;
				else
				{
					cout << "Brak zapamietanego wyrazenia." << endl;
				}
				break;
			}
			break;
		case 3:
			isEnd = true;
			break;
		default:
			break;
		}


	}
	return 0;
}