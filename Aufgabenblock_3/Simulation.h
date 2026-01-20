/*
 * Simulation.h
 *
 *  Created on: Jan 18, 2026
 *      Author: HaiPH
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <list>

class Kreuzung;

class Simulation
{
	protected:
	std::list<std::shared_ptr<Kreuzung>> p_pKreuzungen;
	std::map<std::string, std::shared_ptr<Kreuzung>> pMapKreuzungen;

	public:
	Simulation() = default;
	virtual ~Simulation() = default;

	void vEinlesen(std::istream& i);
	void vSimulieren(double dDauer, double dZeitschritt);
};




#endif /* SIMULATION_H_ */
