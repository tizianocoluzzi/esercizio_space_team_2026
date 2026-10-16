# Esercizi
## Struttura dei file:
    -main: cartella con il codice sorgente
        -sensors: cartella con il codice sorgente dei sensori (non serve modificarla)
        -main.cpp: file in cui verrà effettuata l'esercitazione
        -CMakeLists.txt: file di compilazione standard di CMake, lo modificherete per includere altri file oltre al main
    -CMakeLists: come sopra
    -sdkconfig: file di configurazione, non serve modificarli per gli esercizi
## Esercizi:
0. Stampare su console, compilare ed eseguire sul simulatore
1. Leggere i valori dei sensori, Hint: aprite i file dei sensori per vedere le funzioni
2. Aggregare i valori delle funzioni in una struct in comune
3. Creare una piccola state machine con i dati forniti che rilevi almeno la fase di ascesa
4. Separare in più task (state machine e telemetry)

> Quando dichiarate l'ogetto del sensore dichiaratelo come static, altrimenti non compila

