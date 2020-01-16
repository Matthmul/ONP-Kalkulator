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

template <class Container>
void split(const std::string& str, Container& cont)
{
	std::istringstream iss(str);
	std::copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter(cont));
}

const int ROZMIAR = 100;  // rozmiar stosu

typedef int elementtype;
typedef int position;

class Stos
{
public:
	int S[ROZMIAR];
	position Top;   //szczyt stosu
public:
	Stos();
	void Push(elementtype x);
	void Pop();
	bool Empty();
	elementtype TopElem();
	void Makenull();
};

Stos::Stos()
{
	Top = -1;
}

void Stos::Makenull()
{
	Top = -1;
}

void Stos::Push(elementtype x)
{
	if (Top < ROZMIAR - 1)
	{
		Top++;
		S[Top] = x;
	}
}// PUSH

void Stos::Pop()
{
	if (Top >= 0) Top--;
}//POP

bool Stos::Empty()
{
	return (Top == -1);
}//Empty

elementtype Stos::TopElem()
{
	if (Top >= 0) return S[Top];
}

double CalcONP(string ONP) {
	Stos stos;

	string e;
	char f;
	int g;
	double v1, v2;

	vector<string> words;
	split(ONP, words);


	for (int i = 0; words.size() > i; i++)
	{
		e = words[i];


		if (istringstream(e) >> g)       // konwertujemy na liczbę i sprawdzamy, czy nie było błędu
			stos.Push(g);                // umieszczamy ją na stosie
		else
		{
			if ('~' == e[0]) {
				v1 = stos.TopElem();
				stos.Pop();
				v1 = -v1;
			}
			else
			{
				v2 = stos.TopElem();
				stos.Pop();
				v1 = stos.TopElem();
				stos.Pop();

				switch (e[0])      // wykonujemy operacje wg operatora
				{
				case '^': v1 = pow(v1,v2); break;
				case '*': v1 *= v2; break;
				case '/': v1 /= v2; break;
				case '+': v1 += v2; break;
				case '-': v1 -= v2; break;
				}
			}
			stos.Push(v1);
		}

	}

	return 	stos.TopElem();
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
	Stos stos;

	char c;
	int i;

	string s;

	char previous = ' ';
	char next = ' ';

	vector<string> words;
	split(normal, words);

	for (int a = 0; words.size() > a; a++)
	{
		s = words[a];

		if (istringstream(s) >> i) {
			ONP.append(s + " ");
			if ( stos.Top > 0 & stos.TopElem() == '~')
			{
				//ONP += S[--p];
				ONP += stos.TopElem();
				stos.Pop();
				ONP += " ";
			}
			continue;
		} // konwertujemy na liczbę i sprawdzamy, czy nie było błędu

		c = s[0];

		if (words.size() > a + 1)
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
			stos.Push('~');
			continue;
		}

		switch (c)
		{
		case '(':
			stos.Push('('); // nawias otwierający zawsze na stos
			break;
		case ')': 
			while (stos.TopElem() != '('){  // nawias zamykający
				ONP += stos.TopElem();
				stos.Pop();
			} 
			stos.Pop(); // usuwamy ze stosu nawias otwierający
			ONP += " ";	
			break;
		  case '+':;                           // operator
		  case '-':;
		  case '*':;
		  case '/':;
		  case '^': while (stos.Top)
					 {
					   if ((prio(c) == 3) || (prio(c) > prio(stos.TopElem()))) break;
					   ONP += stos.TopElem();
					   stos.Pop();
					   ONP += " "; // na wyjście przesyłamy ze stosu wszystkie
					 }             // operatory o wyższych priorytetach
					stos.Push(c);  // operator umieszczamy na stosie
					break;
		  default:   
			ONP += c;
			ONP += " ";
			break;
		}
	}

	while (stos.Top + 1) {
		ONP += stos.TopElem();
		stos.Pop();
		ONP += " ";
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