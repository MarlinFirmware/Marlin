BMP2C Version 1.17
==================

/**********************************************************
/* hk@holger-klabunde.de
/* Last Change: 30.03.2007
/**********************************************************

BMP2C erzeugt aus einfarbigen Bitmaps C-Quellcodedateien
oder Bin�rdateien mit und ohne Header.

Das Programm wurde entworfen um Bitmaps f�r Grafikdisplays
auf einfache Art in irgendeinen Speicher des Prozessors zu
bekommen. 

Funktioniert wunderbar mit Bitmaps die schwarz als Hintergrund
und wei� als Vordergrundfarbe haben. Ab Version 1.11 werden
Bitmaps mit wei�em Hintergrund und schwarzem Vordergrund
automatisch in das richtige Format umgewandelt.

C-Quellcodedateien erzeugen
===========================

Aufruf: bmp2c datei.bmp

In datei.c stehen die Bitmapdaten als Array, datei.h
ist die Headerdatei mit Prototyp des Arrays und der Angabe
von Breite und H�he in Pixeln. Jede Zeile in datei.c
enth�lt eine Zeile der Bitmap. Die erste Zeile enth�lt
die oberste Zeile der Bitmap. Das erste Byte ist links
oben in der Bitmap. D7 des Bytes ist das erste Pixel.
Diese Aufteilung ist optimal f�r T6963 Controller.

F�r KS108 kann man den Parameter v mit angeben:

Aufruf: bmp2c datei.bmp v

Ein Byte enth�lt dann 8 Pixel vertikal angeordnet.
D7 des Bytes ist das oberste Pixel. Eine Zeile im C-Code
besteht aus einem Streifen von 8 Pixeln H�he.


F�r T6963 kann man auch Arrays mit nur 6 Pixeln pro Byte
f�r den 6x8 Font Modus erzeugen:

Aufruf: bmp2c datei.bmp 6

D5 bis D0 eines Bytes enthalten die Pixeldaten.
D7 und D6 eines Bytes enthalten keine Daten.

Warnung ! Bei nur 6 Pixeln pro Byte wird der Speicherbedarf
f�r die Bitmap um ca. 30% gr��er als bei 8 Pixeln pro Byte.

Es werden Kommentarzeilen f�r verschiedene C-Compiler
eingef�gt. Die passenden bitte auskommentieren, und
die nicht benutzten zu Kommentaren machen oder l�schen !

Keil 		Kommerziell 8051
SDCC		Freeware 8051
AVR-GCC		Freeware Atmel AVR/ATMega/ATiny
MCC18      	Microchips PIC18Cxxxx und PIC18Fxxxx

Bin�rdateien
============

Dieses Format ist am besten geeignet um Daten in externem
Speicher abzulegen. Anordnung der Pixel wie oben beschrieben.

Bin�rdatei ohne Header erzeugen
===============================

Aufruf: bmp2c datei.bmp b

Schreibt datei.bin mit den reinen Bitmapdaten.

F�r den 6x8 Font Modus:

Aufruf: bmp2c datei.bmp 6 b

Bin�rdatei mit Header erzeugen
==============================

Aufruf: bmp2c datei.bmp b+

Schreibt datei.bin mit den Bitmapdaten und einem Header.
Vor den Bitmapdaten werden folgende Werte gespeichert:

Anzahl Pixel pro Byte 8 Bit
Orientierung 8 Bit 'v' oder 'h'
Breite der Bitmap in Pixeln 16 Bit
H�he der Bitmap in Pixeln 16 Bit
Anzahl Bytes pro Zeile 16 Bit

Alle 16 Bit Werte beginnen mit dem Low-Byte (little endian).

F�r den 6x8 Font Modus:

Aufruf: bmp2c datei.bmp 6 b+

Parameter 'm'
=============

Dreht die Reihenfolge der Pixel im Byte um.
Ohne Parameter 'm' : D7..D0
Mit Parameter  'm' : D0..D7

Parameter 'd'
=============

Gibt man d in der Befehlszeile mit an werden dezimale statt
hexadezimale Werte ins Array eingef�gt.

Gimmicks ;)
===========

F�ge mal den Parameter e mit an und schau in die Datei datei.asc

bmp2c datei.bmp e

Umrechnungen f�r T6963 mit 6x8 Font-Einstellung und 8x8 Daten:
==============================================================

Wenn BMP2C 8 Pixel in einem Byte speichert liegen sie f�r den
6x8 Modus quasi komprimiert vor. Das spart Speicher !
F�r eine 240x64 Bitmap werden 1920 Bytes ben�tigt. Legt man
nur 6 Pixel pro Byte ab braucht man 2560 Bytes f�r die
gleiche Bitmap.

Drei Bytes enthalten 24 Bits. F�r 6x8 muss man vier Bytes
daraus machen.

unsigned char q0,q1,q2;    //Quellbytes aus der Bitmap
unsigned char z0,z1,z2,z3; //Zielbytes zum schreiben in den T6963

-------------------------------------------------------------------------
|                             X- Koordinate                             |
|00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23|
-------------------------------------------------------------------------
|        q0             |          q1           |         q2            | 
|D7 D6 D5 D4 D3 D2 D1 D0|D7 D6 D5 D4 D3 D2 D1 D0|D7 D6 D5 D4 D3 D2 D1 D0|
------------------------------------------------------------------------- 
|         z0      |         z1      |         z2      |         z3      |
|D5 D4 D3 D2 D1 D0|D5 D4 D3 D2 D1 D0|D5 D4 D3 D2 D1 D0|D5 D4 D3 D2 D1 D0|
-------------------------------------------------------------------------

Alle drei Bytes (24 Bits) wiederholt sich das ganze:

//Lese q0..2 aus der Bitmap
z0 = q0 >> 2;
z1 = q0 << 4;
z1 = z1 + (q1 >> 4);
z2 = q1 << 2;
z2 = z2 + (q2 >> 6);
z3 = q2;
//Schreibe z0..3

Die oberen zwei Bits interessieren den T6963 im 6x8 Mode nicht.
Man kann sie ignorieren und muss sie nicht l�schen oder setzen.
Es ist egal was dort drin steht. Nur D5..0 werden gezeichnet.

Am rechten Rand der Bitmap muss man anhand der Breite der Bitmap
entscheiden ob z.B. q1 und q2 �berhaupt existieren.
Und dann muss man noch ermitteln wie viele Pixel des letzten
Bytes der Bitmap auch wirklich noch zur Bitmap geh�ren, und wie
viele Pixel nur F�llpixel sind. Die sollte man dann nicht mehr
zeichnen. Das erfordert aber eine SetPixel() Routine die nur einen
einzigen Pixel malen kann. Ansonsten k�nnten Bereiche die rechts
neben der Bitmap liegen mit F�llpixeln �berschrieben werden.

Die gleiche Routine mit nur zwei Variablen:

//Lese q0 aus der Bitmap
z0 = q0 >> 2;
//Schreibe z0
z0 = q0 << 4;
//Lese q0 aus der Bitmap
z0 = z0 + (q0 >> 4);
//Schreibe z0
z0 = q0 << 2;
//Lese q0 aus der Bitmap
z0 = z0 + (q0 >> 6);
//Schreibe z0
z0 = q0;
//Schreibe z0

