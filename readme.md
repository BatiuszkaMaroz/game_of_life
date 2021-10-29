## Wymagania:

1. Program będzie działał w pełni poprawnie tylko na systemach UNIX'owych. Na innych systemach może wystąpić problem z czyszczeniem konsoli, z racji iż jest tutaj wykorzystana funkcja `system("clear")` pochodząca z biblioteki `<unistd.h>`.

## Konfiguracja:

1. Do poprawnego działania program wymaga pliku o nazwie odpowiadającej tej znajdującej się w zmiennej `CONFIG_FILENAME` (podstawowo "config.txt").
2. W przypadku nie odnalezienia pliku program za zgodą użytkownika sam może utworzyć wymagany plik z podstawową konfiguracją.
3. W pliku konfiguracyjnym powinna się znajdować część planszy składająca się z wierszy o równej długości (równej ilości kolumn) złożonych ze znaków 0 oraz 1.
   - 0 odpowiada pustej komórce, a 1 żywej komórce.
   - Przykładowa konfiguracja:</br>
     00100</br>
     01010</br>
     10001</br>
     01010</br>
     00100</br>
4. Plik konfiguracyjny stanowi tylko część planszy, ta część zostanie umieszczona na środku planszy składającej się z samych zer.
5. W przypadku gdy plik konfiguracyjny posiada więcej wierszy/kolumn niż sama plansza, program zgłosi błąd.
6. Konfiguracja niektórych właściwości programu (rozmiar planszy, wyświetlane symbole, czas rundy) jest możliwa poprzez zmianę wartości zmiennych globalnych w pliku.
