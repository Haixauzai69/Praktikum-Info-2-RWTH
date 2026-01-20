/*
 * Simulation.cpp
 *
 *  Created on: Jan 18, 2026
 *      Author: HaiPH
 */

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <iterator>
#include <memory>
#include "Simulation.h"
#include "Kreuzung.h"
#include "Simulationsobjekt.h"
#include "SimuClient.h"
#include "Car.h"
#include "Fahrrad.h"
#include "Fahrzeug.h"
#include "Tempolimit.h"

void Simulation::vEinlesen(std::istream &i)
{
	std::string key;
	std::string key2;
	int zeile = 0;
	double dStart;

	while (i >> key)
	{
		zeile++;
		try
		{
			if (key == "KREUZUNG")
			{
				auto kreuzung = std::make_shared<Kreuzung>();
				i >> *kreuzung;
				pMapKreuzungen[kreuzung->sGetName()] = std::move(kreuzung);
			}
			else if (key == "STRASSE")
			{
				std::string sNameK1;
				std::string sNameK2;
				i >> sNameK1;
				i >> sNameK2;

				std::string sNameW1;
				std::string sNameW2;
				i >> sNameW1;
				i >> sNameW2;

				int iStrassenlaenge;
				i >> key;
				iStrassenlaenge = stoi(key);

				Tempolimit eTempolimit;
				i >> key;
				if (stod(key) == 1) eTempolimit = Tempolimit::Innerorts;

				if (stod(key) == 2) eTempolimit = Tempolimit::Ausserorts;

				if (stod(key) == 3) eTempolimit = Tempolimit::Autobahn;

				bool bUeberholverbot = 0;
				i >> key;
				if (stod(key) == 1) bUeberholverbot = 1;

				auto it1 = pMapKreuzungen.find(sNameK1);
				auto it2 = pMapKreuzungen.find(sNameK2);

				if (it1 == pMapKreuzungen.end() || it2 == pMapKreuzungen.end())
				{
					throw std::runtime_error("Runtime Error:Gesuchte Kreuzung nicht vorhanden (Strasse)");
				}
				else
				{
					Kreuzung::vVerbinde(sNameW1, sNameW2, iStrassenlaenge, eTempolimit, bUeberholverbot, it1->second, it2->second);
				}
			}

			else if (key == "PKW")
			{
				std::unique_ptr<Car> car = std::make_unique<Car>();
				i >> *car;
				i >> key;
				i >> key2;
				dStart = stod(key2);

				auto it = pMapKreuzungen.find(key);
				if (it == pMapKreuzungen.end())
				{
					throw std::runtime_error("Runtime Error: Gesuchte Kreuzung nicht vorhanden (PKW)");
				}
				else
				{
					it->second->vAnnahme(std::move(car), dStart);
				}
			}
			else if (key == "FAHRRAD")
			{
			     std::unique_ptr<Fahrrad> fahrrad = std::make_unique<Fahrrad>();
			     i >> *fahrrad;
			     i >> key;
			     i >> key2;
			     dStart = stod(key2);

			     auto it = pMapKreuzungen.find(key);
			     if (it == pMapKreuzungen.end())
			     {
			     	 throw std::runtime_error("Runtime Error: Gesuchte Kreuzung nicht vorhanden (Fahrrad)");
			     }
			     else
			     {
			     	 it->second->vAnnahme(std::move(fahrrad), dStart);
			     }
			}

			else
			{
			    throw std::runtime_error("Unbekanntes Schlüsselwort: " + key);
			}

			i >> key;
		}

		catch (const std::exception& e)
		{
			std::cout << "In Zeile " << zeile << ":	" << e.what();
			exit(EXIT_FAILURE);
		}
	}
}

void Simulation::vSimulieren(double dDauer, double dZeitschritt)
{
	for (dGlobaleZeit = 0; dGlobaleZeit <= dDauer; dGlobaleZeit += dZeitschritt)
	{
		for (auto it = pMapKreuzungen.begin(); it != pMapKreuzungen.end(); it++)
		{
			vSetzeZeit(dGlobaleZeit);
			it->second->vSimulieren(dZeitschritt);
		}
	}
}
