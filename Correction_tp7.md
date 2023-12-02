# Polytechnique Montréal

Département de génie informatique et génie logiciel

INF1900: Projet initial de système embarqué

# Grille de correction des programmes:

Identification:
+ Travail    : Mise en commun du code et formation de librairies
+ Section #  : 1
+ Équipe #   : 1618
+ Correcteur : Charles De Lafontaine

# CODE

| Points importants                                        | Poids       |
| -------------------------------------------------------- | ----------- |
| La qualités et le choix de vos portions de code choisies | (4.5/5.0)      |
| La qualités de vos modifications aux Makefiles           | (2.5/5.0)      |
| __Résultat partiel__                                     | __(7/10.0)__ |

# RAPPORT

| Points importants                                                           | Poids      |
| --------------------------------------------------------------------------- | ---------- |
| Explications cohérentes par rapport au code retenu pour former la librairie | (1.6/2.0)     |
| Explications cohérentes par rapport aux Makefiles modifiés                  | (1.6/2.0)     |
| Explications claires avec un bon niveau de détails                          | (1.6/2.0)     |
| Bon français                                                                | (0/1.0)     |
| __Résultat partiel__                                                        | __(4.8/7.0)__ |

# GÉNÉRALITÉS
## Bonne soumission de l'ensemble du code et du rapport selon le format demandé 

| Pénalité par erreur      | -0.5       |
| ------------------------ | ---------- |
| compilation sans erreurs |            |
| etc                      |            |
| __Résultat partiel__     | __(2.5/3.0)__ |


# Résultat

__Total des points: 14.3/20__

# Commentaires du correcteur:

## >>> RAPPORT <<< 

- ＤÉＴＡＩＬＳ:      8️⃣/🔟
- 🇫​​🇴​​🇷​​🇲​​🇦​​🇹​:       ✅
- 𝔽ℝ𝔸ℕℂ𝔸𝕀𝕊 (𝕊𝕋ℝ𝕀ℂ𝕋): ❌

## >>> MAKEFILES <<< 

- Makefile commun:         ❌
- Makefiles synthétisés:   ❌

## >>> COMPILATION <<< 

- Compilation automatique par le grader:  ✅
- Aucun *warning*:                        ❌
- Aucune *error*:                         ✅
- Aucun fichiers indésirables:            ✅

## >>> CODE <<< 

- Préférez le PascalCase pour la nomenclature de vos noms de fichiers.
- Main.cpp:             revoir indentation.
- Button.cpp:           ✅
- Button.h:             ✅
- Comm.cpp:             ✅, le baud rate pourrait être un attribut de classe cela dit, voire être défini directement dans votre constructeur.
- Comm.h:               ✅
- Commons.h:            ✅
- Debug.cpp:            quelques variables magiques.
- Debug.h:              ✅
- GeneralInterrupt.cpp: ✅
- GeneralInterrupt.h:   ✅
- Led.cpp:              ✅
- Led.h:                ✅
- Navigation.cpp:       ✅
- Navigation.h:         ✅
- Pins.cpp:             ✅
- Pins.h:               ✅
- TimerCommons.h:       ✅
- Timer0.cpp:           ✅, typo 'Prescalar' => 'Prescal**e**r', à revoir pour tous vos fichiers.
- Timer0.h:             ✅
- Timer1.cpp:           ✅
- Timer1.h:             ✅
- Timer2.cpp:           ✅
- Timer2.h:             ✅
- Wheel.cpp:            ✅, quelques variables magiques à revoir.
- Wheel.h:              ✅
- Code très complet ! Félicitations !

💧 Watermark: 🄲🄳🄻, 𝟤𝟦/𝟣𝟣/𝟤𝟥 ✅

# Basé sur le commit suivant
```
commit b927605c2d06fd8e2e79ca34be1d4d7636881ff3
Author: Raissa Oumarou Petitot <raissa.oumarou-petitot@polymtl.ca>
Date:   Tue Oct 31 16:16:58 2023 -0400

    Suppression du doublon rapport.pdf
```

# Fichiers indésirables pertinents
Aucun

# Tous les fichiers indésirables
Aucun

# Sorties de `make` dans les sous-répertoires

## Sortie de `make` dans `tp/tp7/lib`
```
make : on entre dans le répertoire « /home/gigl/Bureau/WS/inf1900-grader/correction_tp7/1618/tp/tp7/lib »
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c button.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c can.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c comm.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c debug.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c general_interrupt.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c led.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c memoire_24.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c navigation.cpp
navigation.cpp: In constructor 'Navigation::Navigation()':
navigation.cpp:12:14: warning: variable 'pinEnaL' set but not used [-Wunused-but-set-variable]
   12 |     WritePin pinEnaL = WritePin(Port::B, PB3);
      |              ^~~~~~~
navigation.cpp:13:14: warning: variable 'pinEnaR' set but not used [-Wunused-but-set-variable]
   13 |     WritePin pinEnaR = WritePin(Port::B, PB4);
      |              ^~~~~~~
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c pins.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c timer0.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c timer1.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c timer2.cpp
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c wheel.cpp
avr-ar -crs librobot.a  button.o can.o comm.o debug.o general_interrupt.o led.o memoire_24.o navigation.o pins.o timer0.o timer1.o timer2.o wheel.o
make : on quitte le répertoire « /home/gigl/Bureau/WS/inf1900-grader/correction_tp7/1618/tp/tp7/lib »

```

## Sortie de `make` dans `tp/tp7/exec`
```
make : on entre dans le répertoire « /home/gigl/Bureau/WS/inf1900-grader/correction_tp7/1618/tp/tp7/exec »
avr-gcc -I. -I/usr/include/simavr -MMD -I ../lib -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=8000000UL -fno-exceptions -c main.cpp
main.cpp: In function 'int main()':
main.cpp:10:12: warning: variable 'button' set but not used [-Wunused-but-set-variable]
   10 |     Button button = Button(GeneralInterruptType::INT_0, true);
      |            ^~~~~~
main.cpp:12:9: warning: variable 'led' set but not used [-Wunused-but-set-variable]
   12 |     Led led = Led(Port::A, PA0, PA1);
      |         ^~~
avr-gcc -Wl,-Map,tp7-exec.elf.map -mmcu=atmega324pa -o tp7-exec.elf  main.o \
-lm -l robot -L ../lib
avr-objcopy -j .text -j .data -O ihex tp7-exec.elf tp7-exec.hex
make : on quitte le répertoire « /home/gigl/Bureau/WS/inf1900-grader/correction_tp7/1618/tp/tp7/exec »

```
