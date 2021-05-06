/*
Projekt na Metody Numeryczne - Metody Eliminacjii Gaussa
Autor: Maciej Wicha
Grupa: Czwartek 18.15 (3.8/15)
*/
#include <iostream>
#include <fstream>
#include <vector>       // w aktualnej wersji programu zbedne (wczesniej byly wektory)
#include <iomanip>      // uzycie funkcjii setw(odleglosc);
#include <cmath>
using namespace std;

///*** PROTOTYPY FUNKCJI
void wczytajDane(string, float**&, float *&, int&, bool&);
void kopiujDane(float**, float*, float**&, float*&, int);
void wyswietlDane(float**, float*, int);
void wyswietlMacierz(float**, int, char);
void wyswietlWektor(float*, int, char);

bool czyZeroNaPrzekatnej(float);
bool zerowanieKolumny(float**&, float*&, int, int);
float* wyznaczNiewiadome(float**, float*, int);

void szukajNajwiekszegoKolumna(float**, float*, int, int, float&, int&);
void szukajNajwiekszegoWiersz(float**, float*, int, int, float&, int&);
void szukajNajwiekszegoMacierz(float**, float*, int, int, float&, int&, int&);
void zamienKolumny(float**&, float *&, int, int, int);
void zamienWiersze(float**&, float*&, int, int, int);

int GaussBezWyboru(float**, float*, int);
int GaussCzesciowyWyborWiersze(float**, float*, int);
int GaussCzesciowyWyborKolumny(float**, float*, int);
int GaussPelnyWybor(float**, float*, int);

/// Funkcja MAIN
int main()
{
    string nazwa_pliku="test.csv";                                                              //*** Deklaracje zmiennych
    bool czyWczytano=false;
    int wybor;

    float **wspolczynniki, **kopia_wspolczynniki;
    float *wyrazyWolne, *kopia_wyrazyWolne;
    int ile_x;
                                                                                                    //*** Wczytanie i skopiowanie danych (bo dzialamy na wskaznikach)
    wczytajDane(nazwa_pliku, wspolczynniki, wyrazyWolne, ile_x, czyWczytano);
    kopiujDane(wspolczynniki, wyrazyWolne, kopia_wspolczynniki, kopia_wyrazyWolne, ile_x);
    system("cls");

    do                                                                                              ///*** Menu wielokrotnego wybory
    {
        cout << "Metoda eliminacji Gaussa - Projekt" << endl;
        cout << "***MENU***" << endl;
        cout << "Metoda Gaussa..." << endl;
        cout << "1) ... bez wyboru elementu podstawoego (A=LR)" << endl;
        cout << "2) ... z czesciowym wyborem elementu podstawowego w wierszu (PA=LR)" << endl;
        cout << "3) ... z czesciowym wyborem elementu podstawowego w kolumnie (QA=LR)" << endl;
        cout << "4) ... z pelnym wyborem elementu podstawowego (PAQ=LR)" << endl;
        cout << "5) Wyswietl dane" << endl;
        cout << "6) Wyjscie" << endl;
        cout << "Wybieram -> ";
        cin >> wybor;

        switch(wybor)                                                                                    ///*** SWITCH
        {
            case 1: if(czyWczytano) GaussBezWyboru(wspolczynniki, wyrazyWolne, ile_x); break;
            case 2: if(czyWczytano) GaussCzesciowyWyborWiersze(wspolczynniki, wyrazyWolne, ile_x); break;
            case 3: if(czyWczytano) GaussCzesciowyWyborKolumny(wspolczynniki, wyrazyWolne, ile_x); break;
            case 4: if(czyWczytano) GaussPelnyWybor(wspolczynniki, wyrazyWolne, ile_x); break;
            case 5: if(czyWczytano) wyswietlDane(wspolczynniki, wyrazyWolne, ile_x); break;
        }
        kopiujDane(kopia_wspolczynniki, kopia_wyrazyWolne, wspolczynniki, wyrazyWolne, ile_x);          ///***  Przywrocenie tablic
        system("pause");                                                                                //     Poniewaz dzialamy na wskaznikach, to zawartosc tablicy jest za kazdym razem zmieniana
        system("cls");                                                                                  //     Aby tego uniknac po kazdej jej modyfikacji nalezy przekopiowac poczatkowe elemeny spowrotem

    } while(wybor!=6);

    delete []wyrazyWolne;                                                                               ///*** Zwolnienie pamieci
    for(int i=0; i<ile_x; i++)
        delete []wspolczynniki[i];
    delete [] wspolczynniki;

    delete []kopia_wyrazyWolne;
    for(int i=0; i<ile_x; i++)
        delete []kopia_wspolczynniki[i];
    delete [] kopia_wspolczynniki;

    return 0;
}

///*** FUNKCJE
/// Funkcje ogolne
void wczytajDane(string nazwaPliku, float **&wspolczynniki, float *&wyrazyWolne, int &ile_x, bool &czyWczytano)
{
    system("cls");
    cout << "***Wczytywanie pliku***\n" << endl;
    cout << "Trwa wczytywanie danych..." << endl;

    float wartosc;
    char sredniki;

    ifstream dane(nazwaPliku.c_str());                                                          ///*** Otworzenie pliku
    if(!dane.is_open())
    {
        cout << "Blad podczas otwierania pliku!" << endl;
        cout << "Koncze dzialanie programu!\n" << endl;
        czyWczytano=false;
        system("pause");
        exit(EXIT_FAILURE);
    }

    dane >> ile_x;
    for(int i=0; i<ile_x+1; i++)
        dane >> sredniki;
                                                                                                ///*** Deklaracja pamieci
    wyrazyWolne=new float[ile_x];
    wspolczynniki=new float*[ile_x];
    for(int i=0; i<ile_x; i++)
        wspolczynniki[i]=new float[ile_x];

    for(int w=0; w<ile_x; w++)                                                                  ///*** Odczytanie i zapisanie danych
    {
        for(int k=0; k<ile_x; k++)
        {
            dane >> wartosc;
            wspolczynniki[w][k]=wartosc;
            dane >> sredniki;
        }
        dane >> sredniki;
        dane >> wartosc;
        wyrazyWolne[w]=wartosc;
    }
    dane.close();
    czyWczytano=true;                                                                             ///*** Zamkniecie pliku
    cout << "Wczytywanie pliku zakonczone sukcesem!" << endl;
    cout << endl;
}

void kopiujDane(float **stare_A, float *stare_b, float **&nowe_A, float *&nowe_b, int ile_x)
{
    nowe_b=new float[ile_x];
    nowe_A=new float*[ile_x];
    for(int i=0; i<ile_x; i++)
        nowe_A[i]=new float[ile_x];

    for(int w=0; w<ile_x; w++)
    {
        for(int k=0; k<ile_x; k++)
            nowe_A[w][k]=stare_A[w][k];
        nowe_b[w]=stare_b[w];
    }
}

void wyswietlDane(float **macierz, float *wektor, int ile_x)
{
    system("cls");
    cout << "***Wyswietlanie danych***" << endl;
    wyswietlMacierz(macierz, ile_x, 'A');
    wyswietlWektor(wektor, ile_x, 'b');
}

/// Dzialania na macierzach
void wyswietlMacierz(float **macierz, int ile_x, char znak)
{
    //cout << "Wyswietlam macierz " << znak << ":" << endl;
    cout << znak << "=" << "|" << setw(10);
    for(int w=0; w<ile_x; w++)
    {
        if(w!=0)
            cout << setw(3) << "|" << setw(10);
        for(int k=0; k<ile_x; k++)
            cout << macierz[w][k] << setw(10);
        cout << "|" << endl;
    }
    cout << endl;
}

void wyswietlWektor(float *wektor, int ile_x,  char znak)
{
    //cout << "Wyswietlam wektor " << znak << ":" << endl;
    cout << znak << "=[ ";
    for(int i=0; i<ile_x; i++)
        cout << wektor[i] << " ";                           /*printf("%18.17f ", wektor[i]);*/
    cout << "]." << endl;
    cout << endl;
}

/// Funkcje uzywane we wszystkich metodach
bool czyZeroNaPrzekatnej(float element)
{
    //if(element==0)
    if(abs(element)<0.0000001)      // zakres dokladnosci zmiennej float
    {
        cout << "***Warunek niespelniony!***" << endl;
        cout << "Podczas postepowania prostego Gaussa znaleziono 0 na glownej przekatnej!" << endl;
        cout << "Ta metoda nie zadziala, wyprobuj wybor czesciowy lub pelny." << endl;
        cout << endl;
        return true;
    }
    else
        return false;
}

bool zerowanieKolumny(float **&a, float *&b, int ile_x, int ktoraKolumne)
{
    float l=0.0;                                                                         ///*** Zerowanie elmenetow tablicy
    int k=ktoraKolumne;

    //wyswietlMacierz(a, ile_x, 'A');
    if(!czyZeroNaPrzekatnej(a[k][k]))                                                   ///*** Sprawdzenie czy na przekatnej nie ma 0
    {
        for(int w=k+1; w<ile_x; w++)                // odejmowanie wierszy
        {
            l=a[w][k]/a[k][k];                      // wspolczynnik zerowania
            a[w][k]=0.0;                            // aktualny element zamieniamy na zero

            for(int i=k+1; i<ile_x; i++)            // odejmowanie elementow
                a[w][i]-=l*a[k][i];
            b[w]-=l*b[k];
        }
        return true;
    }
    else
        return false;                               // nie licz dalej
                                                    // wylicz niewiadome
}

float* wyznaczNiewiadome(float **a, float *b, int ile_x)
{
    float *x=new float[ile_x];
    float suma;
                                                                                            ///***Postepowanie odwrotne Gaussa
    for(int n=ile_x-1; n>=0; n--)        //obliczenie pozostalych niewiadomych (od n-tej do pierwszej, czyli 0-wej)
    {
        suma=0.0;
        for(int m=n+1; m<ile_x; m++)
            suma+=a[n][m]*x[m];             //licznik to sumowanie niewyzerowanych wspolczynnikow "a" przemnozonych przez obliczone juz niewiadome "x"
                                            //podzielenie przez wspolczynnik a stojacy przy szukanym x, da nam odpowiedz
        x[n]=(float)((b[n]-suma)/a[n][n]);

        if(abs(x[n])<0.00000001)                //         Gdy przekroczymy zakres dokladnosci float dla zera moga sie pojawiac bardzo male liczby
            x[n]=0.0;                           //         Zapisujemy to jako zero, ze wzgledu na bledy obliczeniowe ten blad jest pomijalnie maly
    }
    return x;                                                                               ///*** Zwrocenie wskaznika do rozwiazan
                //wyciek pamieci?
}

/// Szukanie elementu
void szukajNajwiekszegoWiersz(float **a, float *b, int ile_x, int odKiedy, float &max, int &nr_max)
{
    max=a[odKiedy][odKiedy];                       // odKiedy = aktualny krok, lub aktualnie zerowana kolumna
    nr_max=odKiedy;

    for(int w=odKiedy+1; w<ile_x; w++)             //szukamy w wierszu
    {
        if(abs(a[w][odKiedy])>abs(max))            //porownania (musza byc moduly)!!!!
        {
            max=a[w][odKiedy];
            nr_max=w;
        }
    }
}

void szukajNajwiekszegoKolumna(float **a, float *b, int ile_x, int odKiedy, float &max, int &nr_max)
{
    max=a[odKiedy][odKiedy];                        // odKiedy = aktualny krok, lub aktualnie zerowana kolumn
    nr_max=odKiedy;

    for(int w=odKiedy+1; w<ile_x; w++)              // szukamy w kolumnie
    {
        if(abs(a[odKiedy][w])>abs(max))
        {
            max=a[odKiedy][w];
            nr_max=w;
        }
    }
}

void szukajNajwiekszegoMacierz(float **a, float *b, int ile_x, int odKiedy, float &max, int &w_max, int &k_max)
{
    max=a[odKiedy][odKiedy];                        // odKiedy = aktualny krok, lub aktualnie zerowana kolumn
    w_max=k_max=odKiedy;

    for(int i=odKiedy; i<ile_x; i++)                // poruszaj sie po wierszach
    {
        for(int j=odKiedy; j<ile_x; j++)            // poruszaj sie po kolumnach
        {
            if(abs(a[i][j])>abs(max))
            {
                max=a[i][j];                //aktualnie najwiekszy element
                w_max=i;
                k_max=j;
            }
        }
    }
}

/// Zamiana
void zamienWiersze(float **&a, float *&b, int ile_x, int nr_max, int ktory)
{
    for(int i=0; i<ile_x; i++)
        swap(a[nr_max][i], a[ktory][i]);
    swap(b[nr_max], b[ktory]);
}

void zamienKolumny(float **&a, int *&q, int ile_x, int nr_max, int ktora)
{
    for(int i=0; i<ile_x; i++)
        swap(a[i][nr_max], a[i][ktora]);
    swap(q[nr_max], q[ktora]);              // zamiana kolumn == zmiana numerow niewiadomych
}

/// Metoda Eliminacji Gausa bez wyboru elemenntu podstawowego
int GaussBezWyboru(float **a, float *b, int ile_x)
{
    system("cls");
    cout << "***Metoda Eliminacji Gaussa bez wyboru elementu podstawowego***\n" << endl;
    cout << "Macierz przed eliminacja:" << endl;
    wyswietlMacierz(a, ile_x, 'A');

///===================================================================
    for(int k=0; k<ile_x-1; k++)
        if(!zerowanieKolumny(a, b, ile_x, k))                                                        ///*** Zerowanie kolumny
            return -1;
///===================================================================

    cout << "Macierz po eliminacji:" << endl;
    wyswietlMacierz(a, ile_x, 'A');
                                                                                                ///*** Wyliczenie niewiadomych
    cout << "Rozwiazanie ukladu:" << endl;
    wyswietlWektor(wyznaczNiewiadome(a, b, ile_x), ile_x, 'x');
    return 1;

}

/// Metoda Eliminacji Gaussa z czesciowym wyborem elementu podstawowego
int GaussCzesciowyWyborWiersze(float **a, float *b, int ile_x)
{
    system("cls");
    cout << "***Metoda eliminacji Gaussa z czesciowym wyborem elementu podstawowego w wierszu***\n" << endl;
    cout << "Macierz przed eliminacja:" << endl;
    wyswietlMacierz(a, ile_x, 'A');

    float max;
    int nr_max;

///===================================================================
    for(int k=0; k<ile_x-1; k++)
    {
        szukajNajwiekszegoWiersz(a, b, ile_x, k, max, nr_max);                             ///*** Wybranie elementu najwiekszego w wierszu
        zamienWiersze(a, b, ile_x, nr_max, k);                                             ///*** Zamiana elementow
        if(!zerowanieKolumny(a, b, ile_x, k))                                              ///*** Zerowanie kolumn
            return -1;
    }
///===================================================================
                                                                                        ///*** Wyliczenie niewiadomych
    cout << "Macierz po eliminacji:" << endl;
    wyswietlMacierz(a, ile_x, 'A');

    cout << "Rozwiazanie ukladu:" << endl;
    wyswietlWektor(wyznaczNiewiadome(a, b, ile_x), ile_x, 'x');             //bez tworzenia zbednej zmiennej, odrazu przekazac do funkcji
    cout << endl;

    return 1;
}

int GaussCzesciowyWyborKolumny(float **a, float *b, int ile_x)
{
    system("cls");
    cout << "***Metoda eliminacji Gaussa z czesciowym wyborem elementu podstawowego w kolumnie***\n" << endl;
    cout << "Macierz przed eliminacja:" << endl;
    wyswietlMacierz(a, ile_x, 'A');

    float max;
    int nr_max;
                                                                                            ///*** Wektor zamian kolumn
    int *q=new int[ile_x];
    for(int i=0; i<ile_x; i++)          // Wektor q zapamietuje prawidlowa kolejnosc
        q[i]=i;                         // niewiadomych jaka zostala obliczona

 ///===================================================================
    for(int k=0; k<ile_x-1; k++)        // wyzerowanie k-tej kolumny (3)
    {
        szukajNajwiekszegoKolumna(a, b, ile_x, k, max, nr_max);                             ///*** Wybranie elementu najwiekszego w kolumnie
        zamienKolumny(a, q, ile_x, nr_max, k);                                              ///*** Zamiana kolumn i kolejnosci niewiadomych
        if(!zerowanieKolumny(a, b, ile_x, k))                                               ///*** Zerowanie kolumny
            return -1;
    }
///===================================================================
                                                                                            ///*** Wyliczenie niewiadomych (zamiana mich miejsc)
    float *temp=new float[ile_x];           // tutaj wystraczylby chyba sam wskaznik na istniejaca (zwracana) tablice?
    float *x=new float[ile_x];

    temp=wyznaczNiewiadome(a, b, ile_x);
    for(int i=0; i<ile_x; i++)
        x[q[i]]=temp[i];            //obliczony x, trzeba zapisac pod odpowiednim miejscem

    cout << "Macierz po eliminacji:" << endl;
    wyswietlMacierz(a, ile_x, 'A');

    cout << "Rozwiazanie ukladu:" << endl;
    wyswietlWektor(x, ile_x, 'x');
    cout << endl;

    delete []q;                                                                 ///*** Zwolnienie pamieci przydzielanej dynamicznie
    delete []temp;
    delete []x;

    return 1;
}

/// Metoda Eliminacji Gaussa z pelnym wyborem elementu podstawowego
int GaussPelnyWybor(float **a, float *b, int ile_x)
{
    system("cls");
    cout << "***Metoda Eliminacji Gaussa z pelnym wyborem elementu podstawowego***\n" << endl;
    cout << "Macierz przed eliminacja:" << endl;
    wyswietlMacierz(a, ile_x, 'A');

    float max;
    int w_max,k_max;

    int *q=new int[ile_x];                    // zapamietanie zamian miejsc niewiadoamych
    for(int i=0; i<ile_x; i++)
        q[i]=i;

 ///===================================================================
    for(int k=0; k<ile_x-1; k++)             // zerowanie k-tej kolumny
    {
        szukajNajwiekszegoMacierz(a, b, ile_x, k, max, w_max, k_max);              ///*** Wybranie elementu najwiekszego w macierzy
        zamienWiersze(a, b, ile_x, w_max, k);                                      ///*** Zamiana wierszy
        zamienKolumny(a, q, ile_x, k_max, k);                                      ///*** Zamiana kolumn
        if(!zerowanieKolumny(a, b, ile_x, k))                                      ///*** Zerowanie elementow (odejmowane wierszy)
            return -1;
    }
///===================================================================
                                                                                    ///*** Wyliczenie niewiadomych z zamiana kolejnosci
    float *temp=new float[ile_x];           //ta sama uwaga co wyzej w wyborze kolum
    float *x=new float[ile_x];

    temp=wyznaczNiewiadome(a, b, ile_x);                                            ///*** Zamiana miejsc niewiadomych
    for(int i=0; i<ile_x; i++)
        x[q[i]]=temp[i];            //obliczony x, trzeba zapisac pod odpowiednim miejscem

    cout << "Macierz po eliminacji:" << endl;
    wyswietlMacierz(a, ile_x, 'A');

    cout << "Rozwiazanie ukladu:" << endl;
    wyswietlWektor(x, ile_x, 'x');
    cout << endl;

    delete []x;                                                                     ///*** Zwolnienie pamieci przydzielanej dynamicznie
    delete []temp;
    delete []q;

    return 1;
}
