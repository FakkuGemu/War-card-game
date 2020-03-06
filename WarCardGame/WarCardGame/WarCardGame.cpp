#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

using namespace std;

#define ILOSC_KART 52
#define ILOSC_KOLOROW 4
#define ILOSC_WARTOSCI 13
#define NIEPARZYSTA 1
#define PARZYSTA 0
#define PIERWSZA_KARTA 1
#define DRUGA_KARTA 2
#define PIERWSZY_WYBOR 49
#define DRUGI_WYBOR 50
#define TRZECI_WYBOR 51


typedef struct karta_t {
	int wartosc;
	const char* kolor;
};
typedef struct reka_t {
	struct karta_t karta;
	struct reka_t* nastepna_karta;
}gracz1, gracz2;

void stworz_talie(karta_t* deck, const char* NazwyKolorow[]) {  //tworzy talie 52 roznych kart w tablicy 
	int zmienna = 0;
	for (int kolor = 0; kolor < ILOSC_KOLOROW; kolor++) {
		for (int wartosc = 2; wartosc < ILOSC_WARTOSCI + 2; wartosc++) {	//wartosc 2 dla podstawowej talii
			deck[zmienna].wartosc = wartosc;							//jesli chcemy odjac wszystkie dwojki to zmieniamy wartosc na 3
			deck[zmienna].kolor = NazwyKolorow[kolor];
			zmienna++;
		}
	}

}
void tasuj_talie(karta_t* deck, int ilosc_kart_w_talii) {     //tasuje wartosci w tablicy
	struct karta_t* tymczasowa = (struct karta_t*) malloc(sizeof(struct karta_t));
	int jedynka = 0;
	for (int i = ILOSC_KART - 1; i > 0; i--) {
		int randomowa = rand() % (ilosc_kart_w_talii - jedynka);

		tymczasowa->wartosc = deck[i].wartosc;
		tymczasowa->kolor = deck[i].kolor;

		deck[i] = deck[randomowa];

		deck[randomowa].wartosc = tymczasowa->wartosc;
		deck[randomowa].kolor = tymczasowa->kolor;
		jedynka++;
	}
	free(tymczasowa);
}
//funkcje do tworzenia rak
void inicjalizuj_reke_gracza(reka_t* gracz) {
	gracz->nastepna_karta = NULL;     //inicjuje glowe kolejki
}
void dodaj_karte_z_talii(reka_t* gracz, karta_t* talia, int ktora_w_tabeli) {
	struct reka_t* kolejna_karta = (reka_t*)malloc(sizeof(reka_t));  //przypisuje wartosci kart kolejce

	kolejna_karta->karta = talia[ktora_w_tabeli];
	kolejna_karta->nastepna_karta = gracz->nastepna_karta;
	gracz->nastepna_karta = kolejna_karta;
}
void stworz_rece_graczy(reka_t* gracz1, reka_t* gracz2, karta_t* talia, int ilosc_kart) {
	for (int i = 0; i < ilosc_kart * 2; i++) {                                       //tworzy dwie oddzielne talie ktore sa kolejkami
		if (i % 2 == PARZYSTA)
			dodaj_karte_z_talii(gracz1, talia, i);
		else dodaj_karte_z_talii(gracz2, talia, i);
	}
}
void wypisz_reke(reka_t* gracz) {
	if (gracz->nastepna_karta != NULL) {
		gracz = gracz->nastepna_karta;                  //wypisuje wartosci i kolory kart kolejki
		while (gracz != NULL) {
			cout << gracz->karta.kolor << "|" << gracz->karta.wartosc << "  ";
			gracz = gracz->nastepna_karta;
		}
		cout << endl;
	}
}

void usun_pierwsza_karte(reka_t* gracz) {
	if (gracz->nastepna_karta != NULL) {			//usuwa pierwsza karte
		void* tymczasowa = gracz->nastepna_karta;
		gracz->nastepna_karta = gracz->nastepna_karta->nastepna_karta;
		free(tymczasowa);
	}
}
void dodaj_karte_na_poczatek(reka_t* gracz, reka_t* wybor_kart) {
	if (wybor_kart->nastepna_karta != NULL) {		//dodaje karte na poczatek
		reka_t* wskaznik = (reka_t*)malloc(sizeof(reka_t));

		wskaznik->karta = wybor_kart->nastepna_karta->karta;
		usun_pierwsza_karte(wybor_kart);
		wskaznik->nastepna_karta = gracz->nastepna_karta;
		gracz->nastepna_karta = wskaznik;
	}
}
void przylacz_karte_na_koniec(reka_t* stos, reka_t* wskaznik) {
	while (stos->nastepna_karta != NULL)			//przylacza jedna karte 
		stos = stos->nastepna_karta;
	wskaznik->nastepna_karta = stos->nastepna_karta;
	stos->nastepna_karta = wskaznik;
}
void dodaj_karte_na_koniec_stosu(reka_t* gracz, reka_t* stos) {
	if (gracz->nastepna_karta != NULL) {						//dodaje pierwsza karte z jednej kolejki na koniec drugiej
		reka_t* wskaznik = (reka_t*)malloc(sizeof(reka_t));

		wskaznik->karta = gracz->nastepna_karta->karta;
		usun_pierwsza_karte(gracz);

		przylacz_karte_na_koniec(stos, wskaznik);
	}
}
void przylacz_liste_na_koniec(reka_t* stos1, reka_t* stos2) {
	while (stos1->nastepna_karta != NULL) {			//przylacza jedna liste na koniec drugiej
		stos1 = stos1->nastepna_karta;
	}
	stos1->nastepna_karta = stos2->nastepna_karta;
}
reka_t* dodaj_liste_na_koniec_listy(reka_t* stos1, reka_t* stos2) {
	przylacz_liste_na_koniec(stos1, stos2);
	return stos1;
}
int wartosc_ostatniej_karty(reka_t* stos) {
	while (stos->nastepna_karta->nastepna_karta != NULL) {
		stos = stos->nastepna_karta;				//zwraca wartosc ostatniej karty w kolejce
	}
	return stos->nastepna_karta->karta.wartosc;
}
int wartosc_pierwszej_karty(reka_t* stos) {
	return stos->nastepna_karta->karta.wartosc;  // zwraca wartosc pierwszej karty
}

int ilosc_kart_w_talii(reka_t* gracz) {
	int i = 0;											//sumuje ilosc kart w kolejce
	while (gracz->nastepna_karta != NULL) {
		gracz = gracz->nastepna_karta;
		i++;
	}
	return i;
}
void dodaj_karte_z_wyboru_na_koniec(reka_t* wybory_gracza, reka_t* stos_kart, int numer_karty) {
	int i = 1;												//dodaje n-ta karte na koniec innej kolejki
	while (i < numer_karty) {
		wybory_gracza = wybory_gracza->nastepna_karta;
		i++;
	}
	dodaj_karte_na_koniec_stosu(wybory_gracza, stos_kart);
}
void dodaj_na_koniec_potasowane(reka_t* gracz, reka_t* stos_kart1, reka_t* stos_kart2) {
	stos_kart1 = dodaj_liste_na_koniec_listy(stos_kart2, stos_kart1);	//dodaje losowe karty na koniec innej kolejki
	while (ilosc_kart_w_talii(stos_kart1) != 0) {
		int tymczasowa = rand() % ilosc_kart_w_talii(stos_kart1) + 1;
		dodaj_karte_z_wyboru_na_koniec(stos_kart1, gracz, tymczasowa);
	}
}

void koniec_kart_podczas_wojny(reka_t* gracz1, reka_t* gracz2, reka_t* stos_kart1, reka_t* stos_kart2, reka_t* wybory_gracza1, reka_t* wybory_gracza2) {
	if (ilosc_kart_w_talii(gracz1) > ilosc_kart_w_talii(gracz2)) {
		cout << "KONIEC KART PODCZAS WOJNY" << endl << endl;
		dodaj_na_koniec_potasowane(gracz1, stos_kart1, stos_kart2);
		if (gracz2->nastepna_karta != NULL) {    //jezeli jest jakas karta w rece 
			gracz1 = dodaj_liste_na_koniec_listy(gracz1, gracz2);
			gracz2->nastepna_karta = gracz2->nastepna_karta->nastepna_karta;	// przerzuc wskaznik gracza 2 na NULL
		}
	}
	else if (ilosc_kart_w_talii(gracz1) < ilosc_kart_w_talii(gracz2)) {
		cout << "KONIEC KART PODCZAS WOJNY" << endl << endl;
		dodaj_na_koniec_potasowane(gracz2, stos_kart2, stos_kart1);
		if (gracz1->nastepna_karta != NULL) {
			gracz2 = dodaj_liste_na_koniec_listy(gracz2, gracz1);
			gracz1->nastepna_karta = gracz1->nastepna_karta->nastepna_karta;
		}
	}
	else cout << "REMIS";
}
void wojna(reka_t* gracz1, reka_t* gracz2, reka_t* stos_kart1, reka_t* stos_kart2, reka_t* wybory_gracza1, reka_t* wybory_gracza2, int ilosc_rund) {
	cout << "WOJNA" << endl << endl;
	if ((ilosc_kart_w_talii(gracz1) + ilosc_kart_w_talii(wybory_gracza1)) < 2 || (ilosc_kart_w_talii(gracz2) + ilosc_kart_w_talii(wybory_gracza2)) < 2)
		koniec_kart_podczas_wojny(gracz1, gracz2, stos_kart1, stos_kart2, wybory_gracza1, wybory_gracza2);
	else {
		if (wybory_gracza1->nastepna_karta != NULL) {
			dodaj_karte_na_koniec_stosu(wybory_gracza1, stos_kart1);
			dodaj_karte_na_koniec_stosu(gracz1, stos_kart1);
			dodaj_karte_na_koniec_stosu(gracz2, stos_kart2);
			dodaj_karte_na_koniec_stosu(gracz2, stos_kart2);				//jezeli sa jakiej karty w wyborach to 
		}																	// najpierw dodaje je do stosu kart a potem dopiero inne
		else if (wybory_gracza2->nastepna_karta != NULL) {
			dodaj_karte_na_koniec_stosu(wybory_gracza2, stos_kart2);
			dodaj_karte_na_koniec_stosu(gracz2, stos_kart2);
			dodaj_karte_na_koniec_stosu(gracz1, stos_kart1);
			dodaj_karte_na_koniec_stosu(gracz1, stos_kart1);
		}
		else {
			dodaj_karte_na_koniec_stosu(gracz1, stos_kart1);
			dodaj_karte_na_koniec_stosu(gracz1, stos_kart1);
			dodaj_karte_na_koniec_stosu(gracz2, stos_kart2);
			dodaj_karte_na_koniec_stosu(gracz2, stos_kart2);
		}
		if (ilosc_rund % 2 == 0) {
			cout << "karty gracza 1: ";
			wypisz_reke(stos_kart1);
			cout << "Karty gracza 2: ";
			wypisz_reke(stos_kart2);
			cout << endl;
		}

		else {
			cout << "karty gracza 1: ";
			wypisz_reke(stos_kart2);
			cout << "Karty gracza 2: ";
			wypisz_reke(stos_kart1);
			cout << endl;

		}


		if (wartosc_ostatniej_karty(stos_kart1) > wartosc_ostatniej_karty(stos_kart2)) {
			dodaj_na_koniec_potasowane(gracz1, stos_kart1, stos_kart2);
			cout << endl << "!!! PRZEGRALES WOJNE !!!" << endl << endl;
		}

		else if (wartosc_ostatniej_karty(stos_kart1) < wartosc_ostatniej_karty(stos_kart2)) {
			dodaj_na_koniec_potasowane(gracz2, stos_kart2, stos_kart1);
			cout << endl << "!!! WYGRALES WOJNE !!!" << endl << endl;
		}
		else wojna(gracz1, gracz2, stos_kart1, stos_kart2, wybory_gracza1, wybory_gracza2, ilosc_rund);
	}
}
void bitwa(reka_t* gracz1, reka_t* gracz2, reka_t* stos_kart1, reka_t* stos_kart2, reka_t* wybory_gracza1, reka_t* wybory_gracza2, int ilosc_rund) {
	if (wartosc_ostatniej_karty(stos_kart1) > wartosc_ostatniej_karty(stos_kart2)) {
		dodaj_na_koniec_potasowane(gracz1, stos_kart1, stos_kart2);
		cout << endl << "!!! PRZEGRALES BITWE !!!" << endl << endl;
	}

	else if (wartosc_ostatniej_karty(stos_kart1) < wartosc_ostatniej_karty(stos_kart2)) {
		dodaj_na_koniec_potasowane(gracz2, stos_kart2, stos_kart1);
		cout << endl << "!!! WYGRALES BITWE !!!" << endl << endl;
	}

	else wojna(gracz1, gracz2, stos_kart1, stos_kart2, wybory_gracza1, wybory_gracza2, ilosc_rund);
	_getch();
}

void wybrales_karte(reka_t* wybory_gracza2, reka_t* stos_kart2, int wybrana_karta) {
	system("cls");
	if (wybrana_karta == PIERWSZA_KARTA) {
		dodaj_karte_z_wyboru_na_koniec(wybory_gracza2, stos_kart2, PIERWSZA_KARTA);
	}
	else {
		dodaj_karte_z_wyboru_na_koniec(wybory_gracza2, stos_kart2, DRUGA_KARTA);
	}
}
void manualny_wybor_karty(reka_t* wybory_gracza2, reka_t* stos_kart2) {
	int wybrana_karta, pobrana_wartosc_chara;

	cout << "Wybierz karte 1. lub 2.: ";
	pobrana_wartosc_chara = _getch();

	if (pobrana_wartosc_chara == 49)
		wybrana_karta = 1;
	else if (pobrana_wartosc_chara == 50)
		wybrana_karta = 2;

	wybrales_karte(wybory_gracza2, stos_kart2, wybrana_karta);
}
void randomowy_wybor_karty(reka_t* wybory_gracza2, reka_t* stos_kart2) {
	int wybrana_karta = rand() % 2;

	_getch();
	wybrales_karte(wybory_gracza2, stos_kart2, wybrana_karta);
}
void wsciekly_wybor_karty(reka_t* wybory_gracza2, reka_t* stos_kart2, reka_t* stos_kart1) {
	int wybrana_karta;
	if (wartosc_pierwszej_karty(wybory_gracza2) == wartosc_ostatniej_karty(stos_kart1))
		wybrana_karta = PIERWSZA_KARTA;

	else if (wartosc_ostatniej_karty(wybory_gracza2) == wartosc_ostatniej_karty(stos_kart1))
		wybrana_karta = DRUGA_KARTA;

	else if (wartosc_ostatniej_karty(wybory_gracza2) > wartosc_ostatniej_karty(stos_kart1) && wartosc_pierwszej_karty(wybory_gracza2) > wartosc_ostatniej_karty(stos_kart1)) {
		if (wartosc_pierwszej_karty(wybory_gracza2) > wartosc_ostatniej_karty(wybory_gracza2))
			wybrana_karta = DRUGA_KARTA;
		else wybrana_karta = PIERWSZA_KARTA;
	}

	else if (wartosc_ostatniej_karty(wybory_gracza2) < wartosc_ostatniej_karty(stos_kart1) && wartosc_pierwszej_karty(wybory_gracza2) < wartosc_ostatniej_karty(stos_kart1)) {
		if (wartosc_pierwszej_karty(wybory_gracza2) > wartosc_ostatniej_karty(wybory_gracza2))
			wybrana_karta = DRUGA_KARTA;
		else wybrana_karta = PIERWSZA_KARTA;
	}

	else {
		if (wartosc_pierwszej_karty(wybory_gracza2) > wartosc_ostatniej_karty(wybory_gracza2))
			wybrana_karta = PIERWSZA_KARTA;
		else wybrana_karta = DRUGA_KARTA;
	}
	_getch();
	wybrales_karte(wybory_gracza2, stos_kart2, wybrana_karta);
}
void pokojowy_wybor_karty(reka_t* wybory_gracza2, reka_t* stos_kart2, reka_t* stos_kart1) {
	int wybrana_karta;
	if (wartosc_pierwszej_karty(wybory_gracza2) < wartosc_ostatniej_karty(wybory_gracza2) && wartosc_pierwszej_karty(wybory_gracza2) != wartosc_ostatniej_karty(stos_kart1))
		wybrana_karta = PIERWSZA_KARTA;
	else wybrana_karta = DRUGA_KARTA;

	_getch();
	wybrales_karte(wybory_gracza2, stos_kart2, wybrana_karta);
}

void wybor_karty(reka_t* gracz1, reka_t* gracz2, reka_t* stos_kart1, reka_t* stos_kart2, reka_t* wybory_gracza1, reka_t* wybory_gracza2, char wybrana_opcja) {
	if (wybrana_opcja == 'G')
		manualny_wybor_karty(wybory_gracza2, stos_kart2);

	else if (wybrana_opcja == 'R')
		randomowy_wybor_karty(wybory_gracza2, stos_kart2);

	else if (wybrana_opcja == 'W')
		wsciekly_wybor_karty(wybory_gracza2, stos_kart2, stos_kart1);

	else if (wybrana_opcja == 'P')
		pokojowy_wybor_karty(wybory_gracza2, stos_kart2, stos_kart1);

	else cout << "WPISZ POPRAWNY TRYB BO NIE ZADZIALA";
}
void zapis_bitwy(reka_t* gracz1, reka_t* gracz2, reka_t* wybory_gracza1, reka_t* wybory_gracza2, reka_t* stos_kart1, reka_t* stos_kart2, int ilosc_rund, char wybrana_opcja) {
	if (ilosc_kart_w_talii(gracz2) >= 2) {
		dodaj_karte_na_koniec_stosu(gracz2, wybory_gracza2);
		dodaj_karte_na_koniec_stosu(gracz2, wybory_gracza2);

		wypisz_reke(wybory_gracza2);
		wybor_karty(gracz1, gracz2, stos_kart1, stos_kart2, wybory_gracza1, wybory_gracza2, wybrana_opcja);
		if (wybrana_opcja == 'G') {
			if (ilosc_rund % 2 == 0)
				cout << "-------------------------- Tura gracza 2: --------------------------" << endl << endl;
			else
				cout << "-------------------------- Tura gracza 1: --------------------------" << endl << endl;
		}
		else cout << "-------------------------- Tura komputera: -------------------------- " << endl << endl;
		cout << " " << endl;
		wypisz_reke(stos_kart1);
		cout << "  VS  " << endl;
		wypisz_reke(stos_kart2);
		cout << endl;

		bitwa(gracz1, gracz2, stos_kart1, stos_kart2, wybory_gracza1, wybory_gracza2, ilosc_rund);
		dodaj_karte_na_poczatek(gracz2, wybory_gracza2);
		free(stos_kart1);
		free(stos_kart2);
	}
	else {
		dodaj_karte_na_koniec_stosu(gracz2, stos_kart2);
		if (wybrana_opcja == 'G') {
			if (ilosc_rund % 2 == 0)
				cout << "-------------------------- Tura gracza 2: --------------------------" << endl << endl;
			else
				cout << "-------------------------- Tura gracza 1: --------------------------" << endl << endl;
		}
		else cout << "-------------------------- Tura komputera: -------------------------- " << endl << endl;
		cout << " " << endl << endl;
		wypisz_reke(stos_kart1);
		cout << endl << "VS" << endl << endl;
		wypisz_reke(stos_kart2);
		cout << endl;

		bitwa(gracz1, gracz2, stos_kart1, stos_kart2, wybory_gracza1, wybory_gracza2, ilosc_rund);
		free(stos_kart1);
		free(stos_kart2);
	}
}
void tury(reka_t* gracz1, reka_t* gracz2, reka_t* wybory_gracza1, reka_t* wybory_gracza2, int ilosc_rund, char wybrana_opcja1, char wybrana_opcja2) {
	reka_t* stos_kart1 = (reka_t*)malloc(sizeof(reka_t));
	reka_t* stos_kart2 = (reka_t*)malloc(sizeof(reka_t));

	inicjalizuj_reke_gracza(stos_kart1);
	inicjalizuj_reke_gracza(stos_kart2);

	if (ilosc_rund % 2 == 0) {
		dodaj_karte_na_koniec_stosu(gracz1, stos_kart1);
		if (wybrana_opcja2 == 'G')
			cout << "-------------------------- Tura gracza 2: --------------------------" << endl << endl;
		else cout << "-------------------------- Tura komputera: -------------------------- " << endl << endl;
		cout << "Ilosc kart w talii gracza1: " << ilosc_kart_w_talii(gracz1) << endl;
		cout << "Ilosc kart w talii gracza2: " << ilosc_kart_w_talii(gracz2);

		cout << endl << endl;
		cout << "Karta gracza 1 na stole:  ";
		wypisz_reke(stos_kart1);
		cout << endl;
		zapis_bitwy(gracz1, gracz2, wybory_gracza1, wybory_gracza2, stos_kart1, stos_kart2, ilosc_rund, wybrana_opcja2);
	}
	else {
		dodaj_karte_na_koniec_stosu(gracz2, stos_kart2);
		cout << "-------------------------- Tura gracza 1: --------------------------" << endl << endl;
		cout << "Ilosc kart w talii gracza1: " << ilosc_kart_w_talii(gracz1) << endl;
		cout << "Ilosc kart w talii gracza2: " << ilosc_kart_w_talii(gracz2);
		cout << endl << endl;
		cout << "Karta gracza 2 na stole:  ";
		wypisz_reke(stos_kart2);
		cout << endl;
		zapis_bitwy(gracz2, gracz1, wybory_gracza2, wybory_gracza1, stos_kart2, stos_kart1, ilosc_rund, wybrana_opcja1);
	}
}
void rozgrywka(reka_t* gracz1, reka_t* gracz2, char wybrana_opcja1, char wybrana_opcja2) {
	reka_t* wybory_gracza1 = (reka_t*)malloc(sizeof(reka_t));
	reka_t* wybory_gracza2 = (reka_t*)malloc(sizeof(reka_t));

	inicjalizuj_reke_gracza(wybory_gracza1);
	inicjalizuj_reke_gracza(wybory_gracza2);
	int ilosc_rund = 0;

	while (gracz1->nastepna_karta != NULL && gracz2->nastepna_karta != NULL && ilosc_rund < 100000) {
		tury(gracz1, gracz2, wybory_gracza1, wybory_gracza2, ilosc_rund, wybrana_opcja1, wybrana_opcja2);
		ilosc_rund++;
		system("cls");
	}


	if (gracz1->nastepna_karta == NULL) {
		if (wybrana_opcja2 == 'G')
			cout << endl << "Wygral gracz 2: ";
		else cout << endl << "Wygral komputer: ";

		wypisz_reke(gracz2);
	}
	else if (gracz2->nastepna_karta == NULL) {
		cout << endl << "Wygral gracz 1: ";
		wypisz_reke(gracz1);
	}
	else if (ilosc_rund > 99990) {
		cout << "REMIS";
	}

	cout << endl << "Ilosc rund wyniosla: " << ilosc_rund << endl << endl << "KONIEC GRY" << endl << endl;
}

int main() {
	srand(time(NULL));
	const char* NazwyKolorow[] = { "P", "S", "K", "T" };
	karta_t talia[ILOSC_KART];
	int ilosc_kart_w_rece = 0;
	char wybrana_opcja1 = 0;
	char wybrana_opcja2 = 0;
	int ktory_gracz_zaczyna = 0;

	cout << "WYBIERZ TRYB GRY:" << endl << endl;
	cout << "Tryb jednoosobowy [1]" << endl;
	cout << "Tryb dwuosobowy [2]" << endl << endl;

	int tryb_gry = _getch();
	system("cls");

	if (tryb_gry == PIERWSZY_WYBOR) {
		wybrana_opcja1 = 'G';
		cout << "Przeciwko jakiemu botowi chcesz grac: " << endl << endl;
		cout << "Agresywny bot [1]" << endl;
		cout << "Pokojowy bot [2]" << endl;
		cout << "Losowy bot [3]" << endl;

		int rodzaj_bota = _getch();

		if (rodzaj_bota == PIERWSZY_WYBOR)
			wybrana_opcja2 = 'W';

		else if (rodzaj_bota == DRUGI_WYBOR)
			wybrana_opcja2 = 'P';

		else if (rodzaj_bota == TRZECI_WYBOR)
			wybrana_opcja2 = 'R';
		system("cls");

	}

	else if (tryb_gry == DRUGI_WYBOR) {
		wybrana_opcja1 = 'G';
		wybrana_opcja2 = 'G';

	}



	stworz_talie(talia, NazwyKolorow);
	cout << "Wcisnij [ENTER], aby zatwierdzic." << endl << endl;
	cout << "Podaj liczbe kart w talii jednego gracza (max 26): ";
	cin >> ilosc_kart_w_rece;
	system("cls");


	reka_t* gracz1 = (reka_t*)malloc(sizeof(reka_t));
	reka_t* gracz2 = (reka_t*)malloc(sizeof(reka_t));
	tasuj_talie(talia, ILOSC_KART);
	inicjalizuj_reke_gracza(gracz1);
	inicjalizuj_reke_gracza(gracz2);

	stworz_rece_graczy(gracz1, gracz2, talia, ilosc_kart_w_rece);

	rozgrywka(gracz1, gracz2, wybrana_opcja1, wybrana_opcja2);

	free(gracz1);
	free(gracz2);

	return 0;
}