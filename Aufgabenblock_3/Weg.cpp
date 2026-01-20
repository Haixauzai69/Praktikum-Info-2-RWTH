/*
 * Weg.cpp
 *
 *  Created on: Nov 11, 2025
 *      Author: HaiPH
 */
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <ios>
#include <list>
#include <iterator>
#include "Weg.h"
#include "Tempolimit.h"
#include "Simulationsobjekt.h"
#include "Kreuzung.h"
#include "Fahrzeug.h"
#include "Car.h"
#include "Losfahren.h"
#include "Fahrausnahme.h"
#include "Streckenende.h"

Weg::Weg(std::string name, double laenge, Tempolimit tempolimit, bool ueberholen,
		std::shared_ptr<Kreuzung> zielkreuzung) : p_pZielkreuzung(zielkreuzung)
{
	Simulationsobjekt::vSetName(name);
	p_dLaenge = laenge;
	p_eTempolimit = tempolimit;
	p_bUeberholverbot = ueberholen;
	p_dVirtuelleSchranke = p_dLaenge;
}

enum Tempolimit Weg::getTempolimit()
{
	return p_eTempolimit;
}

bool Weg::bGetUeberhol()
{
	return p_bUeberholverbot;
}

double Weg::dGetVirtuelleSchranke() const
{
    if (p_bUeberholverbot == false)
    {
        return p_dLaenge;
    }

    if (p_pVorherFzg == nullptr)
    {
      	return p_dLaenge;
    }

    if (p_pVorherFzg->bIstLeer())
    {
    	return p_dLaenge;
    }

    return p_dVirtuelleSchranke;
}

void Weg::setVirtuelleSchranke(double dPosition)
{
	p_dVirtuelleSchranke = dPosition;
}

void Weg::vSimulieren(double dTimeStep) // heart of ueberholverbot
{
	std::vector<Fahrzeug*> sorted;

	for (auto& i : p_pFahrzeuge)
	{
		sorted.push_back(i.get());
	}

	std::sort(sorted.begin(), sorted.end(), [](Fahrzeug* a, Fahrzeug* b)
	{
		return a->getStreckenabschn() > b->getStreckenabschn();
	});

	p_pVorherFzg = nullptr;
	p_dVirtuelleSchranke = p_dLaenge;

	for (auto* i : sorted)
	{
		try
		{
			i->vSimulieren(dTimeStep);

			if (p_pVorherFzg)
			{
			     p_dVirtuelleSchranke = p_pVorherFzg->getStreckenabschn();
			}
			p_pVorherFzg = i;
		}

		catch(Fahrausnahme& error)
		{
			error.vBearbeiten();
		}
	}
	p_pFahrzeuge.vAktualisieren();
}

void Weg::vKopf() const
{
	std::cout << std::setiosflags(std::ios::left)
		<< std::setw(5) << "ID"
		<< std::setw(25) << "Name"
		<< std::setw(25) << "Laenge"
		<< std::setw(25) << "Fahrzeuge"
		<< std::resetiosflags(std::ios::left) << std::endl
		<< std::string(100,'-') << std::endl;
}

void Weg::vAusgabe(std::ostream& ausgabe) const
{
	Simulationsobjekt::vAusgabe(ausgabe);
	ausgabe << std::setw(20) << p_dLaenge
	<< std::setw(9);
	std::cout << std::setiosflags(std::ios::right) << std::setw(20);
	for (auto& i : p_pFahrzeuge)
	{
		std::cout << i->sGetName() << ", ";
	}
}

void Weg::vAnnahme(std::unique_ptr<Fahrzeug> fahrzeug)
{
	std::cout << fahrzeug->sGetName() << " befindet sich auf " << Weg::sGetName() << std::endl;

	fahrzeug->vNeueStrecke(*this);

	p_pFahrzeuge.push_back(std::move(fahrzeug));

	p_pFahrzeuge.vAktualisieren();
}

void Weg::vAnnahme(std::unique_ptr<Fahrzeug> fahrzeug, double start)
{
	std::cout << fahrzeug->sGetName() << " parkt auf " << Weg::sGetName() << std::endl;

	fahrzeug->vNeueStrecke(*this, start);

	p_pFahrzeuge.push_front(std::move(fahrzeug));

	p_pFahrzeuge.vAktualisieren();
}

double Weg::dGetLaenge() const
{
	return p_dLaenge;
}

const vertagt::VListe<std::unique_ptr<Fahrzeug>>& Weg::getFahrzeuge() const
{
	return p_pFahrzeuge;
}

std::unique_ptr<Fahrzeug> Weg::pAbgabe(const Fahrzeug& fahrzeug)
{
	for(auto i = p_pFahrzeuge.begin(); i != p_pFahrzeuge.end(); ++i)
	{
		if (*i == nullptr) continue;

		if (**i == fahrzeug)
		{
			std::unique_ptr<Fahrzeug> save = std::move(*i);
			p_pFahrzeuge.erase(i);
			return save;
		}
	}
	p_pFahrzeuge.vAktualisieren();
	std::cout << "Weg::pAbgabe returns nullptr" << std::endl;
	return nullptr;
}

std::shared_ptr<Kreuzung> Weg::pGetZielKreuzung() const
{
	return p_pZielkreuzung;
}

std::shared_ptr<Weg> Weg::pGetRueckweg()
{
	return p_pRueckweg.lock();
}

void Weg::setRueckweg(std::shared_ptr<Weg> pRueckweg)
{
	p_pRueckweg = pRueckweg;
}


