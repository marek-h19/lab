# Subset Sum – AZA 2026/27

Autor: Marek Hrabkovský
Seed: **2026** (nastavený ako `SEED` na začiatku `experiments.cpp`)

## Súbory

| Súbor | Obsah |
|---|---|
| `baseline.cpp` | Algoritmus A – základný backtracking zober/preskoč, bez orezávania. `main` ukáže príklad s riešením a bez riešenia. |
| `improved.cpp` | Algoritmus B – backtracking s dvoma orezaniami, zoradením zostupne a predčasným ukončením. `main` ukáže rovnaké príklady. |
| `experiments.cpp` | Generátor testov (seed), načítanie uložených testov, meranie počtu volaní a času (medián z 3 behov, limit 10 s). |
| `plot.py` | Graf počtu volaní C(n) z `results.csv`. |
| `instances.txt` | Presné testovacie vstupy (12 inštancií: n = 10, 15, 20, 25; riešiteľná, náhodná, neriešiteľná). |
| `results.csv` | Namerané výsledky pre oba algoritmy. |
| `report.pdf` | Správa. |

Formát riadku v `instances.txt`: `meno S n a1 a2 ... an`

## Kompilácia

Potrebný je kompilátor C++17 (testované s g++ 13.3).

```
g++ -std=c++17 -O2 baseline.cpp -o baseline
g++ -std=c++17 -O2 improved.cpp -o improved

g++ -std=c++17 -O2 -c baseline.cpp -Dmain=baseline_main
g++ -std=c++17 -O2 -c improved.cpp -Dmain=improved_main
g++ -std=c++17 -O2 experiments.cpp baseline.o improved.o -o experiments
```

Prepínač `-Dmain=...` iba premenuje funkciu `main` v `baseline.cpp` a `improved.cpp`, aby sa dali pripojiť k experimentom bez úpravy zdrojového kódu.

## Spustenie

```
./baseline                      # príklady pre Algoritmus A
./improved                      # príklady pre Algoritmus B

./experiments                   # vygeneruje testy zo seedu -> instances.txt, results.csv
./experiments instances.txt     # zopakuje presne uložené testy -> results.csv

python3 plot.py                 # graf calls_plot.png (potrebuje matplotlib)
```

Počty rekurzívnych volaní sú deterministické a pri rovnakých vstupoch vyjdú vždy rovnako. Namerané časy závisia od počítača.
