/*
 * Fahrrad.cpp
 *
 *  Created on: Oct 29, 2025
 *      Author: HaiPH
 */
#include <iostream>
#include <memory>
#include <string>
#include <limits>
#include <vector>
#include <iomanip>
#include <ios>
#include <cmath>
#include <algorithm>
#include "Fahrzeug.h"
#include "Fahrrad.h"

Fahrrad::Fahrrad() : Fahrzeug("", 0.0)
{

}

Fahrrad::Fahrrad(const std::string& name, double maxSpeed) : Fahrzeug(name, maxSpeed)
{

}

double Fahrrad::dGeschwindigkeit() const // add verhalten to bikes
{
	if (p_pVerhalten)
	{
		int streckenteil = p_dGesamtStrecke / 20.0;
		double speed = p_dMaxGeschwindigkeit;

		for (int i = 1; i < streckenteil; i++)
		{
			speed *= 0.9;
		}

		if (speed < 12.0)
		{
			speed = 12.0;
		}
		return speed;
	}
	else return this->getMaxGeschwindigkeit();
}

bool Fahrrad::bIstLeer() const
{
	return false;
}

//void Fahrrad::vSimulieren(double dTimeStep)
//{
//	double dCurrentSpeed = dGeschwindigkeit();
//	p_dGesamtStrecke += dCurrentSpeed * dTimeStep;
//}
//
//double dTanken(double dMenge = std::numeric_limits<double>::infinity())
//{
//	return 0.0;
//}

void Fahrrad::vAusgabe(std::ostream& ausgabe) const
{
	Simulationsobjekt::vAusgabe(ausgabe);
	ausgabe << std::setw(25) << std::fixed << std::setprecision(2) << Fahrzeug::getMaxGeschwindigkeit()
		    << std::setw(25) << std::fixed << std::setprecision(2) << Fahrzeug::getGesamtStrecke()
			<< std::setw(25) << "-"
			<< std::setw(25) << "-"
			<< std::setw(25) << std::fixed << std::setprecision(2) << dGeschwindigkeit()
			<< std::resetiosflags(std::ios::left) << std::endl;
}

void Fahrrad::vZeichen(const Weg& weg)
{
	double relPos = p_dAbschnittStrecke / weg.dGetLaenge();
	bZeichneFahrrad(this->sGetName(), weg.sGetName(), relPos, dGeschwindigkeit());
}

void Fahrrad::vEinlesen(std::istream& eingabe)
{
	Fahrzeug::vEinlesen(eingabe);
}

