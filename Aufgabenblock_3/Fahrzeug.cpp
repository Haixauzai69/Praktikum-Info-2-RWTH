/*
 * Fahrzeug.cpp
 *
 *  Created on: Oct 23, 2025
 *      Author: HaiPH
 */
#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <vector>
#include <iomanip>
#include <ios>
#include "Fahrzeug.h"

Fahrzeug::~Fahrzeug()
{
	std::cout << "Name des gelöschten: " << Simulationsobjekt::sGetName() << std::endl;
	std::cout << "ID des gelöschten: " << Simulationsobjekt::iGetID() << std::endl;
}

Fahrzeug::Fahrzeug(const std::string name, double max_geschwindigkeit)
{
	vSetName(name);
	p_dMaxGeschwindigkeit = (max_geschwindigkeit < 0) ? 0 : max_geschwindigkeit;
}

double Fahrzeug::getMaxGeschwindigkeit() const
{
	return p_dMaxGeschwindigkeit;
}

double Fahrzeug::getGesamtStrecke() const
{
	return p_dGesamtStrecke;
}

bool Fahrzeug::bIstLeer() const
{
	return false;
}

void Fahrzeug::vSimulieren(double dTimeStep)
{
	double dS = p_pVerhalten->dStrecke(*this, dTimeStep);

	std::cout << " Fahrzeug::vSimulieren '" << sGetName()
	          << "' dS=" << dS << " (before update abs=" << p_dAbschnittStrecke << ")\n";

//	vAddStrecke(dS);
	p_dAbschnittStrecke += dS;
//	p_dGesamtStrecke += dS;
}

double Fahrzeug::dGeschwindigkeit() const
{
	return p_dMaxGeschwindigkeit;
}

double Fahrzeug::dGetTank()
{
	return 0.0;
}

bool Fahrzeug::operator<(const Fahrzeug& other) const
{
    return this->p_dGesamtStrecke < other.p_dGesamtStrecke;
}

Fahrzeug& Fahrzeug::operator=(const Fahrzeug& other)
{
    if (this != &other)
    {
        this->p_dMaxGeschwindigkeit = other.p_dMaxGeschwindigkeit;
        this->p_dGesamtStrecke = 0.0;
        this->p_dGesamtZeit = 0.0;
    }
    return *this;
}

void Fahrzeug::vKopf()
{
	std::cout << std::setiosflags(std::ios::left)
					<< std::setw(5) << "ID"
					<< std::setw(10) << "Name"
					<< std::setw(25) << "MaxGeschwindigkeit"
					<< std::setw(25) << "Gesamtstrecke"
					<< std::setw(25) << "Gesamtverbrauch"
					<< std::setw(25) << "Aktuelle Tankinhalt"
					<< std::setw(25) << "Aktuelle Geschwindigkeit"
					<< std::resetiosflags(std::ios::left) << std::endl
					<< std::string(140,'-') << std::endl;
}

void Fahrzeug::vAusgabe(std::ostream& ausgabe) const
{
	Simulationsobjekt::vAusgabe(ausgabe);
	      ausgabe << std::setw(25) << std::fixed << std::setprecision(2) << p_dMaxGeschwindigkeit
	              << std::setw(25) << std::fixed << std::setprecision(2) << p_dGesamtStrecke
				  << std::setw(25) << "-"
				  << std::setw(25) << "-"
				  << std::setw(25) << std::fixed << std::setprecision(2) << dGeschwindigkeit()
				  << std::resetiosflags(std::ios::left) << std::endl;
}

void Fahrzeug::vNeueStrecke(Weg& weg) // always create a street before a vehicle can move on it
{
	p_pVerhalten = std::make_shared<Fahren>(weg);
	p_dAbschnittStrecke = 0.0;
}

void Fahrzeug::vNeueStrecke(Weg& weg, double start)
{
	p_pVerhalten = std::make_shared<Parken>(weg, start);
	p_dAbschnittStrecke = 0.0;
	p_dStartZeit = start;
}

double Fahrzeug::getStreckenabschn()
{
	return p_dAbschnittStrecke;
}

void Fahrzeug::vAddStrecke(double strecke)
{
	p_dAbschnittStrecke += strecke;
}

void Fahrzeug::vZeichen(const Weg& weg)
{

}

double Fahrzeug::getStartZeit()
{
	return p_dStartZeit;
}

double Fahrzeug::dGetSchranke() const
{
	return p_dAbschnittStrecke;
}

void Fahrzeug::vEinlesen(std::istream& eingabe)
{
	Simulationsobjekt::vEinlesen(eingabe);
	eingabe >> p_dMaxGeschwindigkeit;
}


