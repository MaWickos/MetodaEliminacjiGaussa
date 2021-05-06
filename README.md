# Metoda Eliminacji Gaussa
Projekt zaliczeniowy z "Metod numerycznych", napisany w języku C++.

## Cel
Celem programu było wczytanie danych z pliku, wyboru metody rozwiązania układu i podania rozwiązań.

## Opcje
Program pozwala na wyznaczenie rozwiązań układów liniowych metodą eliminacji Gaussa. Istnieje możliwość wyznaczenia rozwiązań czterema metodami:

 1. Bez elementu podstawowego (rozkład A = LR)
 2. Z częściowym wyborem elementu podstawowego w wierszy (rozkład PA = LR)
 3. Z częściowym wyborem elementu podstawowego w kolumnie (rozkład QA = LR)
 4. Z pełnym wyborem elementu podstawowego (rozkład PAQ = LR)

## Dane wejściowe
Dane wejściowe, znajdują się w pliku `test.csv`. Struktura pliku jest następująca:

 - W pierwszej linijce pliku, znajduje się stopień układu równań.
 - Następnie podane są parametry stojące przy kolejnych niewiadomych.
 - Wyrazy wolne oddzielone są od współczynników pustym polem (po symbolu ;;).
