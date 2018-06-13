#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

using namespace std;


//Deklaracja stalych
const int Glebokosc_rek = 3;
const int Rozmiar_planszy = 7;
const int Zwyciezca = 1<<30;
const int Przegrany = -Zwyciezca;
const int INF = 100000000;
const int Stan[] = {1,10,100,1000,10000,100000};
bool MAXI = false;
bool MINI = true;

class Plansza;

class Ruch {
	friend class Plansza;

	int Rzad;
	int Kolumna;

public:
	Ruch() {Rzad = 0; Kolumna = 0;}
	bool set(int p, int q);
	string toString() const;
};

class Plansza {
	int Znak[Rozmiar_planszy][Rozmiar_planszy];
	int ruch_skonczony;
	static char zmien_na_typ_char(int pozycja) {
		if ( pozycja == pusty ) return ' ';
		if ( pozycja == Gracz_X ) return 'X';
		return 'O';
	}

public:
	static const bool Gracz_X = true;
	static const bool Gracz_0 = false;
	static const int pusty = -1;

	Plansza();	
	int zliczaj_rzedy(int Rzad, bool GRACZ) const;
	int zliczaj_kolumny(int Kolumna, bool GRACZ) const;
	int zliczaj_przekatna_pier(bool GRACZ) const;
	int zliczaj_przek_drug(bool GRACZ) const;
	bool koniec_gry() const;
	bool remis() const;
	Plansza wykonaj_ruch(const Ruch &move) const;
	bool dozwolony_ruch(const Ruch &move) const;
	vector<Ruch> vec_doz_ruchow() const;
	string toString() const;
};

bool Ruch::set(int p, int q)
{
	if( p >= Rozmiar_planszy || q >= Rozmiar_planszy || p < 0 || q < 0) 
	{
		return false;
	} 
	else 
	{
		Rzad = p;
		Kolumna = q;
		return true;
	}
}

string Ruch::toString() const
{
	stringstream ss;
	ss << Rzad << " " << Kolumna;
	return ss.str();
}

Plansza::Plansza() 
	{
		ruch_skonczony = 0;
		for ( int i = 0 ; i < Rozmiar_planszy ; i++ ) 
		{
			for ( int j = 0 ; j < Rozmiar_planszy ; j++ ) 
			{
				Znak[i][j] = pusty;
			}
		}
	}

int Plansza::zliczaj_rzedy(int Rzad, bool GRACZ) const 
	{
		int tmp_licznik=0;
		for ( int j = 0 ; j < Rozmiar_planszy ; j++ ) 
		{
			if ( Znak[Rzad][j] == GRACZ ) 
			{
				tmp_licznik++;
			}
		}
		return tmp_licznik;
	}
int Plansza::zliczaj_kolumny(int Kolumna, bool GRACZ) const 
	{
		int tmp_licznik=0;
		for ( int i = 0;i<Rozmiar_planszy;i++) 
		{
			if(Znak[i][Kolumna] == GRACZ) 
			{
				tmp_licznik++;
			}
		}
		return tmp_licznik;
	}
int Plansza::zliczaj_przekatna_pier(bool GRACZ) const 
	{
		int tmp_licznik=0;
		for ( int i = 0 ; i < Rozmiar_planszy ; i++ ) 
		{
			if ( Znak[i][i] == GRACZ ) 
			{
				tmp_licznik++;
			}
		}
		return tmp_licznik;
	}
int Plansza::zliczaj_przek_drug(bool GRACZ) const 
	{
		int tmp_licznik=0;
		for ( int i = 0 ; i < Rozmiar_planszy ; i++ ) 
		{
			if ( Znak[i][Rozmiar_planszy-1-i] == GRACZ ) 
			{
				tmp_licznik++;
			}
		}
		return tmp_licznik;
	}

bool Plansza::koniec_gry() const 
	{
		int temp = 0;
		for ( int i = 0 ; i < Rozmiar_planszy ; i++ ) 
		{
			temp = max(zliczaj_rzedy(i,Gracz_X),temp);
			temp = max(zliczaj_rzedy(i,Gracz_0),temp);
			temp = max(zliczaj_kolumny(i,Gracz_X),temp);
			temp = max(zliczaj_kolumny(i,Gracz_0),temp);
		}
		temp = max(temp,zliczaj_przekatna_pier(Gracz_X));
		temp = max(temp,zliczaj_przekatna_pier(Gracz_0));
		temp = max(temp,zliczaj_przek_drug(Gracz_X));
		temp = max(temp,zliczaj_przek_drug(Gracz_0));
		return ((temp == Rozmiar_planszy) || (ruch_skonczony == Rozmiar_planszy*Rozmiar_planszy));
	}

bool Plansza::remis() const 
	{
		int temp = Rozmiar_planszy;
		for ( int i = 0 ; i < Rozmiar_planszy ; i++ ) 
		{
			temp = min(zliczaj_rzedy(i,Gracz_X),temp);
			temp = min(zliczaj_rzedy(i,Gracz_0),temp);
			temp = min(zliczaj_kolumny(i,Gracz_X),temp);
			temp = min(zliczaj_kolumny(i,Gracz_0),temp);
		}
		temp = min(temp,zliczaj_przekatna_pier(Gracz_X));
		temp = min(temp,zliczaj_przekatna_pier(Gracz_0));
		temp = min(temp,zliczaj_przek_drug(Gracz_X));
		temp = min(temp,zliczaj_przek_drug(Gracz_0));
		return ((temp != 0) || (ruch_skonczony == Rozmiar_planszy*Rozmiar_planszy));
	}

bool Plansza::dozwolony_ruch(const Ruch &move) const 
	{
		return ( Znak[move.Rzad][move.Kolumna] == pusty );
	}

Plansza Plansza::wykonaj_ruch(const Ruch &move) const 
	{
		if ( !dozwolony_ruch(move) ) return *this;
		Plansza temp;
		for ( int i = 0 ; i < Rozmiar_planszy ; i++ ) 
		{
			for ( int j = 0 ; j < Rozmiar_planszy ; j++ ) 
			{
				if( i == move.Rzad && j == move.Kolumna ) 
				{
					if( ruch_skonczony%2 == 0 ) 
					{
						temp.Znak[i][j] = Gracz_X;
					} 
					else 
					{
						temp.Znak[i][j] = Gracz_0;
					}
					temp.ruch_skonczony = ruch_skonczony + 1;
				} 
				else 
				{
					temp.Znak[i][j] = Znak[i][j];
				}
			}
		}
		return temp;
	}

vector<Ruch> Plansza::vec_doz_ruchow() const 
	{
		vector<Ruch> movesList;
		Ruch temp;
		for ( int i = 0 ; i < Rozmiar_planszy ; i++ ) 
		{
			for ( int j = 0 ; j < Rozmiar_planszy ; j++ ) 
			{
				if( Znak[i][j] == pusty ) {
					temp.Rzad = i;
					temp.Kolumna = j;
					movesList.push_back(temp);
				}
			}
		}
		return movesList;
	}

string Plansza::toString() const 
	{
		stringstream ss;
		for ( int i = 0 ; i < Rozmiar_planszy ; i++ ) {
			ss << "	"; 
			for ( int j = 0 ; j < Rozmiar_planszy ; j++ ) {
				ss << ' ';
				ss << ' ';
				if (zmien_na_typ_char(Znak[i][j])==' ')
					ss << '.'; 
				else
					ss << zmien_na_typ_char(Znak[i][j]);
				ss << ' ';
				
			}
			ss << endl<< endl;
		}
		return ss.str();
	}

int zliczaj_stan(int x, int o) 
{
	int wynik = 0;
	if ( o == 0 ) wynik += Stan[x];
	if ( x == 0 ) wynik -= Stan[o];
	return wynik;
}

int zliczaj_stan(const Plansza &plansza_gry) 
{
	int wynik = 0;
	for ( int i = 0 ; i < Rozmiar_planszy ; i++ ) 
	{
		wynik += zliczaj_stan(plansza_gry.zliczaj_rzedy(i,MAXI),plansza_gry.zliczaj_rzedy(i,MINI));
		wynik += zliczaj_stan(plansza_gry.zliczaj_kolumny(i,MAXI),plansza_gry.zliczaj_kolumny(i,MINI));
	}
	wynik += zliczaj_stan(plansza_gry.zliczaj_przekatna_pier(MAXI),plansza_gry.zliczaj_przekatna_pier(MINI));
	wynik += zliczaj_stan(plansza_gry.zliczaj_przek_drug(MAXI),plansza_gry.zliczaj_przek_drug(MINI));
	return wynik;
}

Plansza temp_plansza;

int algorytm_alfa_beta(const int poziom_rek, const bool GRACZ, const Plansza &plansza_gry, int alfa, int beta, Ruch &move) 
{
	if ( plansza_gry.koniec_gry() || poziom_rek == 0 ) 
	{
		if ( !plansza_gry.koniec_gry() ) move = (plansza_gry.vec_doz_ruchow())[0];
		return (zliczaj_stan(plansza_gry));
	}

	vector<Ruch> kolejny_ruch = plansza_gry.vec_doz_ruchow();

	temp_plansza = plansza_gry;

	if ( GRACZ == MAXI ) 
	{
		for ( vector<Ruch>::iterator it = kolejny_ruch.begin() ; it != kolejny_ruch.end() ; it++ ) 
		{
			Plansza child = plansza_gry.wykonaj_ruch(*it);
			Ruch temp;
			int wynik = algorytm_alfa_beta(poziom_rek-1,!GRACZ,child,alfa,beta,temp);
			if ( wynik > alfa ) 
			{
				alfa = wynik; 
				move = *it;
			}
			if ( alfa >= beta ) // beta ciecie
			{
				move = *it;
				return alfa; 
			}
		}
		return alfa; 
	} 
	else 
	{
		for ( vector<Ruch>::iterator it = kolejny_ruch.begin() ; it != kolejny_ruch.end() ; it++ ) 
		{
			Plansza child = plansza_gry.wykonaj_ruch(*it);
			Ruch temp;
			int wynik = algorytm_alfa_beta(poziom_rek-1,!GRACZ,child,alfa,beta,temp);
			if ( wynik < beta ) 
			{
				beta = wynik; 
				move = *it;
			}
			if ( alfa >= beta ) // alfa ciecie
			{
				move = *it;
				return beta; 
			}
		}
		return beta; 
	}
}
Plansza plansza_gry;
Ruch prevMove;
int wyswietl_licz = 0;

void wyswietl_plansza_gry() 
{
	cout<<"\n\t Obecny stan gry na planszy:\n\n";
	cout << plansza_gry.toString() << endl;
	wyswietl_licz++;
}

void wyswietl_wynik(){
	int x = 0, o = 0;
	for ( int i = 0 ; i < Rozmiar_planszy ; i++ ) 
	{
		x = max(x,plansza_gry.zliczaj_rzedy(i,plansza_gry.Gracz_X));
		x = max(x,plansza_gry.zliczaj_kolumny(i,plansza_gry.Gracz_X));
		o = max(o,plansza_gry.zliczaj_rzedy(i,plansza_gry.Gracz_0));
		o = max(o,plansza_gry.zliczaj_kolumny(i,plansza_gry.Gracz_0));
	}
	x = max(x,plansza_gry.zliczaj_przekatna_pier(plansza_gry.Gracz_X));
	x = max(x,plansza_gry.zliczaj_przek_drug(plansza_gry.Gracz_X));
	o = max(o,plansza_gry.zliczaj_przekatna_pier(plansza_gry.Gracz_0));
	o = max(o,plansza_gry.zliczaj_przek_drug(plansza_gry.Gracz_0));
	if ( x == Rozmiar_planszy ) {
		cout<<"\tGracz X wygrywa!!\n";
	} 
	else if ( o == Rozmiar_planszy ) 
	{
		cout<<"\tGracz O wygrywa!!\n";
	} else {
		cout<<"\tNa planszy jest remis!\n";
	}
}

void tura_czlowieka() {
	int r, c;
	Ruch move;
	do {
		bool isMoveInRange;
		do {
			cout << "Wprowadz współrzedne pola które chcesz zaznaczycz[rzad][kolumna]: ";
			cin >> r >> c;
			r=r-1;c=c-1;
			isMoveInRange = move.set(r,c);
		} while ( !isMoveInRange );
	} while ( !plansza_gry.dozwolony_ruch(move) );
	plansza_gry = plansza_gry.wykonaj_ruch(move);
	prevMove = move;
}

void tura_komputera() {
	Ruch move;
	if ( wyswietl_licz == 2 ) {
		move.set(3,4); // ruch poczatkowy ustawiony na sztywno
	} else {
		algorytm_alfa_beta(Glebokosc_rek,MAXI,plansza_gry,-INF,INF,move);
	}
	plansza_gry = plansza_gry.wykonaj_ruch(move);
	prevMove = move;
}

void gra_z_komputerem() {
	wyswietl_plansza_gry();
	int c;
	bool flag;
	do {
		cout << "Czy chcesz pierwszy wykonać ruch? [1-Tak  2-Nie]";
		cin >> c;
	} while ( c != 1 && c != 2 );
	if ( c == 1 ) {
		flag = true;
		MAXI = false; MINI = true;
	} else {
		flag = false;
		MAXI = true; MINI = false;
	}
	while ( !plansza_gry.koniec_gry() && !plansza_gry.remis() ) {
		wyswietl_plansza_gry();
		if ( flag ) tura_czlowieka();
		else tura_komputera();
		flag = !flag;
	}
	wyswietl_plansza_gry();
	wyswietl_wynik();
}

int main() {

int choice=0;
cout <<" WITAJ W GRZE KÓŁKO-KRZYŻYK O ROZMIARZE PLANSZY ";
cout << Rozmiar_planszy << "x" << Rozmiar_planszy;
cout <<" GRACZ KOMPUTEROWY JEST STWORZONY W OPARCIU O ALGORYTM ALFA BETA\n";
cout <<" WYNIKA STĄD, ŻE WSKAZANA JEST UWAŻNA GRA :)\n";
cout <<" POWODZENIA W GRZE!! \n\n";

	cout << "OPCJE GRY"<<endl<<endl;
	cout << "Graj przeciwko komputerowi   :  wybierz 1"<<endl;
	cout << "Wyjdz z programu             :  wybierz 2"<<endl;
	do {
		cout << "Twój wybór: ";
		cin >> choice;
	} while ( choice != 1 && choice != 2);

	switch(choice) {
		case 1:
			gra_z_komputerem();
			break;
		case 2:
			cout << "Koniec programu\n";
	}
}

