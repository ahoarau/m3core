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

#ifndef RT_SYSTEM_H
#define RT_SYSTEM_H

#include "m3rt/base/m3rt_def.h"
#include "m3rt/base/m3ec_def.h"
#include "m3rt/base/toolbox.h"
#include "m3rt/base/component.h"
#include "m3rt/base/component_ec.h"
#include "m3rt/base/component_factory.h"
#include "m3rt/base/component_base.pb.h" 
#include "m3rt/rt_system/rt_log_service.h"
//#include "m3rt/rt_system/rt_ros_service.h"
#include <string>
#include <vector>

#ifdef __RTAI__
#ifdef __cplusplus
extern "C" {
#endif 
#include <rtai.h>
#include "rtai_sem.h"
#ifdef __cplusplus
}  // extern "C"
#endif 
#endif
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>



namespace m3rt
{
class M3RtSystem
{
public:
    M3RtSystem(M3ComponentFactory * f):log_service(NULL),
        shm_ec(0),shm_sem(0),ext_sem(NULL),sync_sem(0),factory(f),logging(false),hard_realtime(true),ready_sem(NULL),
        safeop_required(false){GOOGLE_PROTOBUF_VERIFY_VERSION;}
    friend class M3RtDataService;
    ~M3RtSystem();
    bool Startup();
    bool StartupComponents();
    bool Shutdown();
    bool Step(bool safeop_only);
    void PrettyPrint();
    void PrettyPrintComponents();
    void PrettyPrintComponent(int idx);
    void PrettyPrintComponentNames();
    M3Component * 	GetComponent(std::string name){return factory->GetComponent(name);}
    M3Component *  	GetComponent(int idx){return factory->GetComponent(idx);}
    std::string  	GetComponentName(int idx){return factory->GetComponentName(idx);}
    std::string  	GetComponentType(int idx){return factory->GetComponentType(idx);}
    int 		GetNumComponents(){return factory->GetNumComponents();}
    int 		GetComponentIdx(std::string name){return factory->GetComponentIdx(name);}
    int			GetComponentState(int idx);
    bool SetComponentStateOp(int idx);
    bool SetComponentStateSafeOp(int idx);
	void SetComponentStateSafeOpAll(void);
	void SetComponentStateOpAll(void);
    bool IsOperational(){return !safeop_required;}
    bool IsHardRealTime(){return hard_realtime;}
#ifdef __RTAI__
    int GetEcCounter(){return shm_ec->counter;}
    SEM * ready_sem;
	SEM * sync_sem;
	SEM * shm_sem;
    SEM * ext_sem;
#else
    int GetEcCounter(){return 0;}
#endif
    void SetFactory(M3ComponentFactory * f){factory=f;}
    void AttachLogService(M3RtLogService * l){log_service=l;}

    void RemoveLogService(){log_service=NULL;M3_DEBUG("Log service stopped at %d\n",log_service);}
    bool ParseCommandFromExt(M3CommandAll & msg);  //Must be thread safe
    bool SerializeStatusToExt(M3StatusAll & msg, std::vector<std::string>& names); //Must be thread safe
    bool logging;
    int over_step_cnt;
private:
    void CheckComponentStates();
    M3ComponentFactory * factory;
    M3EcSystemShm *  shm_ec;
    bool safeop_required;
	bool hard_realtime;
    std::vector<M3ComponentEc *>	m3ec_list;
    std::vector<M3Component *>	m3rt_list;
#ifdef __RTAI__

    RTIME last_cycle_time;
#else
	sem_t * ready_sem;
    sem_t * shm_sem;
    sem_t * sync_sem;
    sem_t * ext_sem;
    long long last_cycle_time;
#endif
    M3RtLogService * log_service;

    std::vector<int> idx_map_ec;
    std::vector<int> idx_map_rt;
    long hst;
    double test;
	
protected:
#ifdef __RTAI__
    SEM * GetExtSem(){return ext_sem;}
#else
    sem_t * GetExtSem(){return ext_sem;}
#endif
    //bool ReadConfigEc(const char * filename);
    //bool ReadConfigRt(const char * filename);
	template <class T>
	bool ReadConfig(const char* filename, const char* component_type, std::vector<T*>& comp_list, std::vector< int >& idx_map)
	{
		std::vector<std::string> vpath;
		GetFileConfigPath(filename,vpath);
		bool ret=false;
		for(std::vector<std::string>::iterator it=vpath.begin();it!=vpath.end();++it){
			if( ret=this->ReadConfigUnordered(*it,component_type,comp_list,idx_map) && comp_list.size()>0){
			  M3_WARN("Old config file detected, please update your %s\n",M3_CONFIG_FILENAME);
			continue;
			}
#if defined(YAMLCPP_05)

			try{
				ret = this->ReadConfigOrdered(*it,component_type,comp_list,idx_map);
			}catch(std::exception &e){
				M3_ERR("(Ordered) Error while reading %s config: %s\n",component_type,e.what());
			}
#endif
		}
		return ret;
	}
	template <class T>
	bool ReadConfigUnordered(const std::string& filename,const char * component_type,std::vector<T>& comp_list,std::vector<int>& idx_map)
	{
		try{
		YAML::Node doc;
#ifndef YAMLCPP_05
		std::ifstream fin(filename);
		YAML::Parser parser(fin);
		while(parser.GetNextDocument(doc)) {
#else
		doc = YAML::LoadFile(filename);
#endif

#ifndef YAMLCPP_05
			if(!doc.FindValue(component_type)) {
#else
			if(!doc[component_type]){
#endif
				M3_INFO("No %s key in %s. Proceeding without it...\n",component_type,M3_CONFIG_FILENAME);
				return true;
			}
			YAML::Node components = doc[component_type];
#ifndef YAMLCPP_05
			for(YAML::Iterator it = components.begin(); it != components.end(); ++it) {
				std::string dir;
				it.first() >> dir;
#else
			;
			for(YAML::const_iterator it_rt = components.begin();it_rt != components.end(); ++it_rt) {
				std::string dir = it_rt->first.as<std::string>();
#endif
				
#ifndef YAMLCPP_05
				for(YAML::Iterator it_dir = components[dir.c_str()].begin();
					it_dir != components[dir.c_str()].end(); ++it_dir) {
					string  name, type;
					it_dir.first() >> name;
					it_dir.second() >> type;
#else
				YAML::Node dir_comp = components[dir.c_str()];
				for(YAML::const_iterator it_dir = dir_comp.begin();it_dir != dir_comp.end(); ++it_dir) {
					std::string name=it_dir->first.as<std::string>();
					std::string type=it_dir->second.as<std::string>();
#endif
					T m = reinterpret_cast<T>(factory->CreateComponent(type));
					if(m != NULL) {
						m->SetFactory(factory);
						std::string f = dir + "/" + name + ".yml";
						try {
							std::cout <<"------------------------------------------"<<std::endl;
							std::cout <<"Component " << name<<std::endl;
							if(m->ReadConfig(f.c_str())) { //A.H: this should look first in local and to back to original if it exists
								comp_list.push_back(m);
								idx_map.push_back(GetNumComponents() - 1);
							} else {
								factory->ReleaseComponent(m);
								M3_ERR("Error reading config for %s\n", name.c_str());
							}
						} catch(...) {
							M3_WARN("Error while parsing config files for %s %s \n",component_type, name.c_str());
							factory->ReleaseComponent(m);
						}

					}
				}
			}
		return true;
		}catch(std::exception &e){
			//M3_ERR("(Unordered) Error while reading %s config (old config): %s\n",component_type,e.what());
			return false;
		}
		std::cout<<std::endl;
	}
#if defined(YAMLCPP_05)
	template <class T>
	bool ReadConfigOrdered(const std::string& filename,const char * component_type,std::vector<T>& comp_list,std::vector<int>& idx_map)
	{
		// New version with -ma17: -actuator1:type1 etc
		YAML::Node doc = YAML::LoadFile(filename);
		//for(std::vector<YAML::Node>::const_iterator it_doc=all_docs.begin(); it_doc!=all_docs.end();++it_doc){
			//doc = *it_doc;
			if(!doc[component_type]){
				M3_INFO("No %s keys in m3_config.yml. Proceeding without it...\n",component_type);
				return true;
			}
			const YAML::Node& components = doc[component_type];
			for(YAML::const_iterator it_rt = components.begin();it_rt != components.end(); ++it_rt) {
				const std::string dir =it_rt->begin()->first.as<std::string>();
				const YAML::Node& dir_comp = it_rt->begin()->second;
				for(YAML::const_iterator it_dir = dir_comp.begin();it_dir != dir_comp.end(); ++it_dir) {
					std::string name=it_dir->begin()->first.as<std::string>();
					std::string type=it_dir->begin()->second.as<std::string>();
					T m = reinterpret_cast<T>(factory->CreateComponent(type));
					if(m != NULL) {
						m->SetFactory(factory);
						std::string f = dir + "/" + name + ".yml";
						try {
							std::cout <<"------------------------------------------"<<std::endl;
							std::cout <<"Component " << name<<std::endl;
							if(m->ReadConfig(f.c_str())) { //A.H: this should look first in local and to back to original if it exists
								comp_list.push_back(m);
								idx_map.push_back(GetNumComponents() - 1);
							} else {
								factory->ReleaseComponent(m);
								M3_ERR("Error reading config for %s\n", name.c_str());
							}
						} catch(...) {
							M3_WARN("Error while parsing config files for %s %s \n",component_type, name.c_str());
							factory->ReleaseComponent(m);
						}

					}
				}
				//std::cout <<"------------------------------------------"<<std::endl;
			}
		std::cout<<std::endl;
		return true;
	}
#endif
};


}
#endif


