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
					
#ifndef M3RT_COMPONENT_H
#define  M3RT_COMPONENT_H

#include <string>
#include "m3rt/base/component_base.pb.h"
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <iostream>
#include "m3rt/base/toolbox.h"

namespace m3rt
{
class M3ComponentFactory;
class M3RtSystem;

/*
A component can be in one of 4 states:
		M3COMP_STATE_INIT = 0;
		M3COMP_STATE_ERR = 1;
		M3COMP_STATE_SAFEOP = 2;
		M3COMP_STATE_OP = 3;
	
	State INIT: On creation, Startup() not yet called.
	State OP: Initialized, running normally. Status and Command data is modified. A module must be placed in OP by an external process.
	State SAFEOP: Only Status data is modified. SAFEOP is set externally in case of system error.
	State ERR: A non-recoverable error or safety exception has been triggered. 
	
	Each component must implement  Startup, Shutdown, StepStateOp, StepStateSafeOp, StepStateError . 
	After successful Startup,     it should do a SetStateSafeOp else SetStateError
	After successful StepStateOp, it should do a SetStateOp else SetStateError
	After successful StepStateSafeOp, it should do a SetStateSafeOp else SetStateError
	After a StepStateError, 	  it should do a SetStateError
	
	The client process must manually place each SafeOp component in state Op.
*/

class M3Component{
	public:
		M3Component(int p=0):factory(NULL),priority(p),version_id(-1){GOOGLE_PROTOBUF_VERIFY_VERSION;}
		virtual ~M3Component(){}
		friend class M3RtSystem;
		friend class M3RtLogService;
		std::string GetName(){return GetBaseStatus()->name();}
		int  GetState(){return (int)GetBaseStatus()->state();}
		int  GetPriority(){return priority;}
		void  SetPriority(int p){priority=p;}
		void SetStateError(){GetBaseStatus()->set_state(M3COMP_STATE_ERR);}
		void SetStateOp(){if (!IsStateError()) GetBaseStatus()->set_state(M3COMP_STATE_OP);}
		void SetStateSafeOp(){if (!IsStateError()) GetBaseStatus()->set_state(M3COMP_STATE_SAFEOP);}
		bool IsStateError(){return GetBaseStatus()->state() == M3COMP_STATE_ERR;}
		bool IsStateSafeOp(){return GetBaseStatus()->state() == M3COMP_STATE_SAFEOP;}
		bool IsStateOp(){return GetBaseStatus()->state() == M3COMP_STATE_OP;}
		bool IsRateFast(){return GetBaseStatus()->rate()=="fast";}
		bool IsRateMedium(){return GetBaseStatus()->rate()=="medium";}
		bool IsRateSlow(){return GetBaseStatus()->rate()=="slow";}
		bool IsVersion(int id){return version_id==id;} 
		void RegisterVersion(const char * name, int id){version_names.push_back(name);version_ids.push_back(id);} 
		virtual void Startup()=0;
		virtual void Shutdown()=0;
		virtual void StepStatus()=0;
		virtual void StepCommand()=0;
		
		
		void SetTimestamp(int64_t ts){GetBaseStatus()->set_timestamp(ts);}
		void GetTimestamp(int64_t ts){GetBaseStatus()->timestamp();}
		void SetFactory(M3ComponentFactory * f){factory=f;}
		
		virtual void PrettyPrint();
		virtual google::protobuf::Message *  GetCommand()=0;
		virtual google::protobuf::Message *  GetStatus()=0;
		virtual google::protobuf::Message *  GetParam()=0;
		void ParseCommandTest(std::string & s){}
		virtual bool SerializeStatus(std::string & s);
	protected:
		void RegisterVersionID(const char * name, int id);
		virtual bool ParseCommand(std::string & s);
		virtual bool ParseParam(std::string & s);		
		virtual bool LinkDependentComponents(){return true;}
		virtual M3BaseStatus *  GetBaseStatus()=0;
	protected:
		virtual bool ReadConfig(const char * filename);
		M3ComponentFactory * factory;
		int priority;
		std::vector<std::string> version_names;
		std::vector<int> version_ids;
		int version_id;
		YAML::Node doc;
		//std::std::string doc_path;
};

//Factory defn.
typedef M3Component * create_comp_t();
typedef void destroy_comp_t(M3Component *);
extern std::map< std::string, create_comp_t *, std::less<std::string> > creator_factory;	//global
extern std::map< std::string, destroy_comp_t *, std::less<std::string> > destroyer_factory; //global

}

#endif

