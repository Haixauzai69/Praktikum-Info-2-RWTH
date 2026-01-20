/*
 * PKW.cpp
 *
 *  Created on: Oct 24, 2025
 *      Author: HaiPH
 */

#include "Car.h"
#include <iostream>
#include <string>
#include "Fahrzeug.h"
#include "Simulationsobjekt.h"
#include <limits>
#include <iomanip>
#include <ios>
#include <algorithm>

Car::Car() : Fahrzeug("", 0.0)
{
	p_dVerbrauch = 0;
	p_dTankvolumen = 0;
	p_dTankinhalt = 0;
}

Car::Car(std::string name, double max_speed, double verbrauch, double tankvolumen) : Fahrzeug(name, max_speed)
{
	p_dVerbrauch = verbrauch;
	p_dTankvolumen = tankvolumen;
	p_dTankinhalt = tankvolumen/2;
}

double Car::dTanken(double dMenge)
{
	double vorher = p_dTankinhalt;
	if (dMenge == std::numeric_limits<double>::infinity())
	{
		p_dTankinhalt = p_dTankvolumen;
	}
	else
	{
		p_dTankinhalt = std::min(p_dTankvolumen, p_dTankinhalt + dMenge);
	}
	return p_dTankinhalt - vorher;
}

void Car::vSimulieren(double dTimeStep)
{
	double dS = p_pVerhalten->dStrecke(*this, dTimeStep);

	if (p_dTankinhalt <= 0.0)
	{
		std::cout << "Car is out of fuel" << std::endl;
		return;
	}

	double dVerbrauch = (dS / 100.0) * p_dVerbrauch; // liters pro 100km

	if (p_dTankinhalt > dVerbrauch)
	{
		p_dAbschnittStrecke += dS;
		p_dGesamtStrecke += dS;
	}
	else
	{
	    double dStreckeBisLeer = (p_dTankinhalt / p_dVerbrauch) * 100.0; // km until empty
	    double dZeitBisLeer = dStreckeBisLeer / p_dMaxGeschwindigkeit;   // hours until empty

	    p_dAbschnittStrecke += dStreckeBisLeer;
	    p_dTankinhalt = 0.0;

	    std::cout << "Car ran out of fuel after " << dZeitBisLeer << " hours ("
	              << dStreckeBisLeer << " km)." << std::endl;
	}
}

bool Car::bIstLeer() const
{
	return p_dTankinhalt <= 0;
}

double Car::dGeschwindigkeit() const
{
	if(p_pVerhalten)
	{
		Weg& weg = p_pVerhalten->getWeg();

		double limit = static_cast<double>(weg.getTempolimit());

		if (this->getMaxGeschwindigkeit() > limit)
		{
			std::cout << "Strafe wegen zu schnellem Fahren" << std::endl;
			return limit;
		}
	}
	else return this->getMaxGeschwindigkeit(); // another error
}

void Car::vAusgabe(std::ostream& ausgabe) const
{
	double dGesamtverbrauch = (p_dGesamtStrecke / 100.0) * p_dVerbrauch;

			Simulationsobjekt::vAusgabe(ausgabe); // printing id and name
	ausgabe << std::setw(25) << std::fixed << std::setprecision(2) << p_dMaxGeschwindigkeit
			<< std::setw(25) << std::fixed << std::setprecision(2) << p_dGesamtStrecke
			<< std::setw(25) << std::fixed << std::setprecision(2) << dGesamtverbrauch
			<< std::setw(25) << std::fixed << std::setprecision(2) << p_dTankinhalt
			<< std::setw(25) << std::fixed << std::setprecision(2) << dGeschwindigkeit()
			<< std::resetiosflags(std::ios::left) << std::endl;
}

double Car::dGetTank()
{
	return p_dTankinhalt;
}

void Car::vZeichen(const Weg& weg)
{
	double relPos = p_dAbschnittStrecke / weg.dGetLaenge();
	bZeichnePKW(this->sGetName(), weg.sGetName(), relPos, dGeschwindigkeit(), p_dTankinhalt);
}

void Car::vEinlesen(std::istream& eingabe)
{
	Fahrzeug::vEinlesen(eingabe);
	eingabe >> p_dVerbrauch >> p_dTankvolumen;
	p_dTankinhalt = p_dTankvolumen;
}

