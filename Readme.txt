Projekt Voxe jest wokselowym silnikiem gier trójwymiarowych stworzonym w ramach pracy inżynierskiej.
Do uruchomienia aplikacji potrzebny jest komputer z systemem Windows oraz zainstalowane środowisko Visual Studio 2017.
Konfiguracja zapewniona została przez pliki konfiguracyjne generowane przez środowisko Visual Studio. 
Wspierany jest tryb Debug oraz Release w architekturze x64.

Kod źródłowy silnkia znajduje się w folderze DXProject.

Kod źródłowy aplikacji demonstracyjnej opisanej w pracy inżynierskiej znajduje się w pliku main.cpp oraz w folderze Game.
Dodatkowo sterowanie zostało zintegrowane z kodem encji kamery - GameCamera.
Aby uruchomić aplikację demonstracyjną należy otworzyć projekt w środowisku Visual Studio 2017 następnie skompilować oraz uruchomić program.

Sterowanie:
Ruch myszką - rozglądanie się
W - ruch do przodu
S - ruch do tyłu
D - ruch w prawo
A - ruch w lewo
SPACE - ruch do góry
Z - ruch w dół
C - destrukcja woksela wskazywanego przez środek ekranu

Wszelkie parametry mogą zostać zmienione w kodzie klasy GameStage lub w odpowiednich systemach (jeśli nie zostały zapewnione w tym celu interfejsy).