/*
 * main.cpp
 *
 *  Created on: Oct 6, 2025
 *      Author: HaiPH
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <list>
#include <fstream>
#include <ios>
#include <map>
#include "Simulation.h"
#include <algorithm>
#include <stdlib.h>
#include <iterator>
#include <stdexcept>
#include <exception>
#include <memory>
#include <cmath>
#include <random>
#include "Simulationsobjekt.h"
#include "Tempolimit.h"
#include "Car.h"
#include "Fahrzeug.h"
#include "Fahrrad.h"
#include "Weg.h"
#include "Fahrausnahme.h"
#include "Losfahren.h"
#include "Kreuzung.h"
#include "Streckenende.h"
#include "SimuClient.h"
#include "vertagt_liste.h"
#include "vertagt_aktion.h"

void vAufgabe1()
{
	std::unique_ptr<Fahrzeug> bmw = std::make_unique<Fahrzeug>("BMW", 130.0);
	std::unique_ptr<Fahrzeug> audi = std::make_unique<Fahrzeug>("Audi", 150.0);

	std::vector<std::unique_ptr<Fahrzeug>> fahrzeuge;
	fahrzeuge.push_back(std::move(bmw));
	fahrzeuge.push_back(std::move(audi));

	std::shared_ptr<Fahrzeug> bike1 = std::make_shared<Fahrzeug>("Mountain_Bike", 20.0);

	std::cout << bike1.use_count() << std::endl;

	std::vector<std::shared_ptr<Fahrzeug>> fahrrad;
	fahrrad.push_back(bike1);

	std::cout << bike1.use_count() << std::endl;

	std::shared_ptr<Fahrzeug> bike3;
	bike3 = bike1;

	std::cout << bike1.use_count() << std::endl;

	fahrrad.push_back(std::move(bike3));

	std::cout << bike1.use_count() << std::endl;

	fahrzeuge.clear();
	fahrrad.clear();
}

void vAufgabe1a()
{
	std::unique_ptr<Car> car = std::make_unique<Car>("Tesla", 200, 5, 55);
	std::unique_ptr<Fahrzeug> bike = std::make_unique<Fahrzeug>("Bike", 20);
	std::unique_ptr<Fahrzeug> tram = std::make_unique<Fahrzeug>("Tram", 100);

	std::vector<std::unique_ptr<Fahrzeug>> vehicles;
	vehicles.push_back(std::move(car));
	vehicles.push_back(std::move(bike));
	vehicles.push_back(std::move(tram));

	Fahrzeug::vKopf();
	for (auto& fzg : vehicles)
		{
		     std::cout << *fzg << std::endl;
		}

	for (int t = 0; t < 2; ++t)
	{
		  dGlobaleZeit++;
		  for (auto& i : vehicles)
		  {
			  i->vSimulieren(0.5);
		  }
	}

	std::cout << " " << std::endl;

	Fahrzeug::vKopf();
	for (auto& fzg : vehicles)
		{
			    std::cout << *fzg << std::endl;
		}
}

void vAufgabe_2()
{
	    int numCars = 0;
	    int numBikes = 0;
	    std::cout << "Enter number of cars: ";
	    std::cin >> numCars;
	    std::cout << "Enter number of bikes: ";
	    std::cin >> numBikes;

	    std::vector<std::unique_ptr<Fahrzeug>> vehicles;

	    for (int i = 0; i < numCars; ++i)
	    {
	        std::string name = "Car_" + std::to_string(i + 1);
	        double verbrauch = 5 + i;
	        double tankvolumen = 55;
	        vehicles.push_back(std::make_unique<Car>(name, 200, verbrauch, tankvolumen));
	    }

	    for (int i = 0; i < numBikes; ++i)
	    {
	        std::string name = "Bike_" + std::to_string(i + 1);
	        double maxSpeed = 25 + 2 * i;
	        vehicles.push_back(std::make_unique<Fahrrad>(name, maxSpeed));
	    }

	    // Simulation loop
	    double dTimeStep = 0.5;
	    double dTotalTime = 0.0;
	    double epsilon = 1e-6;

	    while (dTotalTime < 6.0)
	    {
	        dGlobaleZeit += dTimeStep;
	        dTotalTime += dTimeStep;

	        for (auto& fzg : vehicles)
	            fzg->vSimulieren(dTimeStep);

	        if (std::fabs(dTotalTime - 3.0) < epsilon)
	        {
	            for (auto& fzg : vehicles)
	                fzg->dTanken();
	        }

	        std::cout << "\nAfter " << dTotalTime << " hours:\n";
	        Fahrzeug::vKopf();
	        for (auto& fzg : vehicles)
	        {
	            std::cout << *fzg << std::endl;
	        }
	    }
}

void vAufgabe_3()
{
	auto car1 = std::make_unique<Fahrzeug>("Audi", 15.0);
	auto car2 = std::make_unique<Fahrzeug>("BMW", 13.0);

    Weg weg1("Weg1", 300, Tempolimit::Autobahn, false);

    weg1.vAnnahme(std::move(car1));
    weg1.vAnnahme(std::move(car2));

    weg1.vSimulieren(2.0);

    std::cout << *car1 << std::endl;
    std::cout << *car2 << std::endl;

    if (*car1 < *car2)
        std::cout << car1->sGetName() << " has driven less than " << car2->sGetName() << std::endl;
    else
        std::cout << car1->sGetName() << " has driven more than or equal to " << car2->sGetName() << std::endl;


    Fahrzeug car3("Mercedes", 200.0);
    std::cout << "Before assignment: " << car3 << std::endl;
    car3 = *car1;
    std::cout << "After assignment (car3 = car1): " << car3 << std::endl;
}

//double dEpsilon = 0.001;
//
//void vAufgabe_Probe() {
//    Car* pF1 = new Car(8, 55);
//    dGlobaleZeit = 0.0;
//    Fahrzeug::vKopf();
//    dGlobaleZeit = 3.0;
//    std::cout << std::endl << "Globalezeit = " << dGlobaleZeit << std::endl;
//    pF1->vSimulieren(3.0);
//    std::cout << *pF1 << std::endl;
//    delete pF1;
//    char c;
//    std::cin >> c;
//}

//double dEpsilon = 0.001;
//
//void vAufgabe_AB1() {
//
//    int l = 0; // Laufindex für gezielte AUsgabe
//    vector<int> ausgabe{15};
//    double dTakt = 0.3;
//
//    std::vector<unique_ptr<Fahrzeug>> vecFahrzeuge;
//    vecFahrzeuge.push_back(make_unique<Car>(10.7, 55.0));
//    vecFahrzeuge.push_back(make_unique<Fahrrad>("BMX", 21.4));
//    for (dGlobaleZeit = 0; dGlobaleZeit < 6; dGlobaleZeit += dTakt)
//    {
//        auto itL = find(ausgabe.begin(), ausgabe.end(), l);
//        if (itL != ausgabe.end()) {
//            std::cout << std::endl << l <<  " Globalezeit = " << dGlobaleZeit << std::endl;
//            Fahrzeug::vKopf();
//        }
//
//        for (int i = 0; i < vecFahrzeuge.size(); i++)
//        {
//            vecFahrzeuge[i]->vSimulieren(0.3);
//            if (fabs(dGlobaleZeit - 3.0) < dTakt/2)
//            {
//                vecFahrzeuge[i]->dTanken(55.0);
//            }
//            if (itL != ausgabe.end()) {
//                std::cout << *vecFahrzeuge[i] << endl;
//            }
//        }
//        l++;
//    }
//}

void vAufgabe4()
{
	std::unique_ptr<Fahrzeug> car1 = std::make_unique<Fahrzeug>("BMW", 210);
	std::unique_ptr<Weg> boxgraben = std::make_unique<Weg>("Boxgraben", 24, Tempolimit::Innerorts, true);

	std::unique_ptr<Fahrzeug> car2 = std::make_unique<Fahrzeug>("Audi", 240);
	boxgraben->vAnnahme(std::move(car2));
	boxgraben->vAnnahme(std::move(car1));
	boxgraben->vKopf();
	std::cout << *boxgraben << std::endl;
	std::cout << std::endl;
}

void vAufgabe_5()
{
	auto theaterstr = std::make_unique<Weg>("Theaterstr.", 50, Tempolimit::Innerorts, true);

	std::unique_ptr<Car> car1 = std::make_unique<Car>("Volkswagen", 200, 8, 55);
	std::unique_ptr<Fahrrad> bike1 = std::make_unique<Fahrrad>("Mountain bike", 30);

	theaterstr->vAnnahme(std::move(car1), 1.0); // startzeit hinzufügen bedeutet parken
	theaterstr->vAnnahme(std::move(bike1));

	theaterstr->vKopf();
	std::cout << *theaterstr << std::endl;

	for (int i = 0; i < 5; i++)
	{
		theaterstr->vSimulieren(2.0);
		dGlobaleZeit += 2.0; // from now on global zeit must be updated in the loop in main
	}

	theaterstr->vKopf();
	std::cout << *theaterstr << std::endl;
}

void vAufgabe_6()
{
	auto junkerstrasse = std::make_unique<Weg>("Weg1", 500, Tempolimit::Innerorts, true);

	std::unique_ptr<Car> car1 = std::make_unique<Car>("BMW", 100, 8, 55);
	std::unique_ptr<Fahrrad> bike1 = std::make_unique<Fahrrad>("Mountain_bike", 30);

    int koordinaten[4] = {100, 250, 700, 250};

    bInitialisiereGrafik(800, 500);

    vSleep(500);

    bZeichneStrasse(junkerstrasse->sGetName() + "1", junkerstrasse->sGetName() + "2", junkerstrasse->dGetLaenge(), 2, koordinaten);

	junkerstrasse->vAnnahme(std::move(car1)); // startzeit hinzufügen bedeutet parken
	junkerstrasse->vAnnahme(std::move(bike1));

	for(int i = 0; i < 30 ; i++)
	{

		junkerstrasse->vSimulieren(1.0);

		vSetzeZeit(dGlobaleZeit);

		dGlobaleZeit += 1.0;  // from now on global zeit must be updated in the loop in main

		if (i == 10)
		{
			std::unique_ptr<Car> car2 = std::make_unique<Car>("Audi", 200, 5, 55); // names must be different
			junkerstrasse->vAnnahme(std::move(car2), 12.0);
		}

		for (auto& i : junkerstrasse->getFahrzeuge())
		{
		     i->vZeichen(*junkerstrasse);
		}
		vSleep(600);
	}
	vSleep(1000);
	vBeendeGrafik();
}

void vTest()
{
    int koordinaten[4] = {100, 250, 700, 250};

    bInitialisiereGrafik(800, 500);

    vSleep(500);

    bool ok = bZeichneStrasse("W1", "W2", 500, 2, koordinaten);
    std::cout << "Draw result = " << ok << std::endl;

    bZeichnePKW("Auto1", "W1", 0.0, 40.0, 20.0);

    vSetzeZeit(5.0);

    vSleep(10000);
    vBeendeGrafik();
}

void vAufgabe_6a()
{
	static std::mt19937 device(0);
	std::uniform_int_distribution<int> dist(1, 10);

	vertagt::VListe<int> liste;

	for (int i = 0; i < 10; i++)
	{
		liste.push_back(dist(device));
	}

	liste.vAktualisieren(); // if i make this line disappear, the push back will not happen yet and can only be queued

	std::cout << "Random list" << std::endl;
	for (vertagt::VListe<int>::iterator i = liste.begin() ; i != liste.end() ; ++i)
	{
		std::cout << *i << " ";
	}

	std::cout << std::endl;

	for (vertagt::VListe<int>::iterator i = liste.begin() ; i != liste.end() ; ++i)
	{
		if (*i > 5)
		{
			liste.erase(i);
		}
	}

//	liste.vAktualisieren(); if this line appears the erase process will begin immediately and the numbers will be erased

	std::cout << "Random list after erase" << std::endl;
	for (vertagt::VListe<int>::iterator i = liste.begin() ; i != liste.end() ; ++i)
		{
			std::cout << *i << " ";
		}

	std::cout << std::endl;

	liste.vAktualisieren(); // all the actions can only be called when this calls

	std::cout << "Random list after vAktualisieren" << std::endl;
	for (vertagt::VListe<int>::iterator i = liste.begin() ; i != liste.end() ; ++i)
		{
			std::cout << *i << " ";
		}
	std::cout << std::endl;
}

void vAufgabe_7()
{
	bInitialisiereGrafik(1000, 800);

	std::shared_ptr<Kreuzung> Kr1 = std::make_shared<Kreuzung>("Kr1");
	std::shared_ptr<Kreuzung> Kr2 = std::make_shared<Kreuzung>("Kr2", 1000);
	std::shared_ptr<Kreuzung> Kr3 = std::make_shared<Kreuzung>("Kr3");
	std::shared_ptr<Kreuzung> Kr4 = std::make_shared<Kreuzung>("Kr4");

	Kreuzung::vVerbinde("W12", "W21", 40, Tempolimit::Innerorts, true, Kr1, Kr2);
	Kreuzung::vVerbinde("W23a", "W32a", 115, Tempolimit::Autobahn, false, Kr2, Kr3);
	Kreuzung::vVerbinde("W23b", "W32b", 40, Tempolimit::Innerorts, true, Kr2, Kr3);
	Kreuzung::vVerbinde("W24", "W42", 55, Tempolimit::Innerorts, true, Kr2, Kr4);
	Kreuzung::vVerbinde("W34", "W43", 85, Tempolimit::Autobahn, false, Kr3, Kr4);
	Kreuzung::vVerbinde("W44a", "W44b", 130, Tempolimit::Innerorts, false, Kr4, Kr4);

	int iCoords1[] = { 680, 40, 680, 300 };
	bZeichneStrasse("W12", "W21", 40, 2, iCoords1); //1

	int iCoords2[] = { 680, 300, 850, 300, 970, 390, 970, 500, 850, 570, 680, 570 };
	bZeichneStrasse("W23a", "W32a", 115, 6, iCoords2); //2

	int iCoords3[] = { 680, 300, 680, 570 };
	bZeichneStrasse("W23b", "W32b", 40, 2, iCoords3); //3

	int iCoords4[] = { 680, 300, 320, 300 };
	bZeichneStrasse("W24", "W42", 55, 2, iCoords4); //4

	int iCoords5[] = { 680, 570, 500, 570, 350, 510, 320, 420, 320, 300 };
	bZeichneStrasse("W34", "W43", 85, 5, iCoords5); //5

	int iCoords6[] = { 320, 300, 320, 150, 200, 60, 80, 90, 70, 250, 170, 300, 320, 300 };
	bZeichneStrasse("W44a", "W44b", 130, 7, iCoords6); //6

	std::unique_ptr<Car> bmw = std::make_unique<Car>("BMW", 10, 6, 55);
	std::unique_ptr<Car> audi = std::make_unique<Car>("Audi", 10, 6, 55);

	bZeichneKreuzung(680, 40); //kr1
	bZeichneKreuzung(680, 300); //kr2
	bZeichneKreuzung(680, 570); //kr3
	bZeichneKreuzung(320, 300); //kr4

	Kr1->vAnnahme(std::move(bmw), 1);
	Kr1->vAnnahme(std::move(audi), 5);

	std::list<std::shared_ptr<Kreuzung>> kreuzungen;
	kreuzungen.push_back(Kr1);
	kreuzungen.push_back(Kr2);
	kreuzungen.push_back(Kr3);
	kreuzungen.push_back(Kr4);

	for(int i = 0; i < 30 ; i++)
	{
		vSetzeZeit(dGlobaleZeit);
		dGlobaleZeit += 1.0;

		for (auto it = kreuzungen.begin(); it != kreuzungen.end(); it++)
		{
			(*it)->vSimulieren(1.0);
		}

		for (const auto& kr : kreuzungen)
		{
		    for (const auto& weg : kr->getWege())
		    {
		        for (const auto& fzg : weg->getFahrzeuge())
		        {
		            fzg->vZeichen(*weg);
		        }
		    }
		}

		vSleep(600);
	}
	vSleep(1000);
	vBeendeGrafik();
}

void vAufgabe_8()
{
	try
	{
	    std::ifstream f("VO.dat");
	        if (!f)
	        {
	        	throw std::runtime_error("Fehler beim Einlesen der Datei");
	        	return;
	        }

	        Car car;
	        Fahrrad fahrrad;
	        Kreuzung kreuzung;

	        f >> car;
	        f >> fahrrad;
	        f >> kreuzung;

	        std::cout << car << '\n';
	        std::cout << fahrrad << '\n';
	        std::cout << kreuzung << '\n';
	    }
	    catch (const std::exception& e)
	    {
	        std::cerr << e.what() << '\n';
	    }
}

void vAufgabe_9()
{
	Simulation sim;
	std::ifstream f("Simu.dat");
	if (!f) {
		throw std::runtime_error("Fehler beim Einlesen der Datei");
		return;
	}
	sim.vEinlesen(f);
}

void vAufgabe_9a()
{
	bInitialisiereGrafik(1500, 1500);
	Simulation sim;
	std::ifstream f("SimuDisplay.dat");
	if (!f)
	{
		throw std::runtime_error("Fehler beim Einlesen der Datei");
	}

	sim.vEinlesen(f, true);

	sim.vSimulieren(20, 0.1);

	vSleep(100000);
	vBeendeGrafik();
}


int main()
{
//	vAufgabe1();
//	vAufgabe1a();
//	vAufgabe_2();
//	vAufgabe_3();
//	vAufgabe_Probe();
//	vAufgabe_AB1();
//	vAufgabe4();
//	vAufgabe_5();
//	vAufgabe_6();
//	vTest();
//	vAufgabe_6a();
//	vAufgabe_7();
//	vAufgabe_8();
//	vAufgabe_9();
	vAufgabe_9a();
	return 0;
}


