/* 
M3 -- Meka Robotics Real-Time Control System
Copyright (c) 2010 Meka Robotics
Author: edsinger@mekabot.com (Aaron Edsinger)

M3 is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

M3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with M3.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef  M3RT_COMPONENT_FACTORY_H
#define  M3RT_COMPONENT_FACTORY_H

#include "m3rt/base/component.h"
#include <string>
#include <vector>
#include <iterator>

namespace m3rt
{

//Because of Protocol Buffers implementation
//Only one instance of libprotobuf can be loaded
//So make this once on system startup, close only on exit
class M3ComponentFactory{
public:
	M3ComponentFactory(){}
	~M3ComponentFactory(){}
	bool Startup();												//Load libraries
	void Shutdown();											//Free libraries, release components
	M3Component * CreateComponent(std::string type);			//Instantiate a component of this type
	bool ReleaseComponent(M3Component * c);						//Safe delete of a component
	void ReleaseAllComponents();								//Safe delete of all components
	M3Component *  	GetComponent(int idx);
	std::string  	GetComponentType(int idx);
	int 			GetComponentIdx(std::string name); 			//Returns -1 if not found
	M3Component * 	GetComponent(std::string name);
	std::string  	GetComponentName(int idx);
	int 			GetNumComponents();
	M3MonitorStatus * GetMonitorStatus(){return &monitor_status;}
private:
	bool ReadConfig(const char * filename);
	bool AddComponentLibrary(std::string lib);
	std::vector<M3Component *>	m3_list;
	std::vector<std::string>	m3_types;
	std::vector<void *> 		dl_list; 						//handles for dynamic libs
	std::vector<std::string> 	dl_types;
	M3MonitorStatus  monitor_status; 					//Container for all component rt stats
};

}

#endif

