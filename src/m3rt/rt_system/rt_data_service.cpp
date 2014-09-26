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

#include "m3rt/rt_system/rt_data_service.h"
#include "m3rt/base/m3rt_def.h"
#include <unistd.h>
#ifdef __RTAI__
#ifdef __cplusplus
extern "C" {
#endif 
#include <rtai.h>
#include <rtai_shm.h>
#include <rtai_sched.h>
#include <rtai_nam2num.h>
#include <rtai_sem.h>
#include <rtai_lxrt.h>
#ifdef __cplusplus
}
#endif 
#endif

namespace m3rt
{
	
///////////////////////////////////////////////////////////
using namespace std;

void data_thread(void * arg)
{
	stringstream ss;
	string rt_name;	
	M3RtDataService * svc = (M3RtDataService *)arg;
	svc->data_thread_active=true;
	svc->data_thread_end=false;
		if (!svc->StartServer()) //blocks until connection
	{
		svc->data_thread_active=false;
		return;
	}
#ifdef __RTAI__
	RT_TASK *task;
        int cnt=0;
        RTIME tstart,dt;
        int printdt = 4;
        RTIME printdt_ns=printdt*1e9;
        RTIME printstart;
        RTIME requested_period = nano2count(4000000); // 250 Hz
        //RTIME requested_period=4000000;//1e9/250.;
	//Need to consider multiple threads, name conflict
	ss << "M3DSV" << svc->instances;
	ss >> rt_name;
	task = rt_task_init_schmod(nam2num(rt_name.c_str()), 0, 0, 0, SCHED_FIFO, 0xF); 
	//rt_allow_nonroot_hrt();
        //rt_make_soft_real_time();
        //rt_task_make_periodic(task,rt_get_time()+requested_period,requested_period);
	svc->instances++;
	if (task==NULL)
	{
		M3_ERR("Failed to create M3RtDataService RT Task\n",0);
		return;
	}
	//mlockall(MCL_CURRENT | MCL_FUTURE);
        printstart = rt_get_time_ns();
#endif 
	while(1)
	{       
                tstart = rt_get_time();
                if (svc->data_thread_end) break;
		if (!svc->Step())
		{
		   svc->data_thread_error=true;		   
		   break;
		}
#ifdef __RTAI__
                dt = rt_get_time()-tstart;
		//rt_task_wait_period(); //A.H Just go as fast as you can (No locks for now) TODO: test this more
		rt_sleep(MAX(0,requested_period-dt)); //250 Hz
                cnt++;
                if(rt_get_time_ns() - printstart >= printdt_ns){
                    printstart = rt_get_time_ns();
                    rt_printk("%s (TCP/IP Server) freq=%d (dt=%lldns)\n",rt_name.c_str(),cnt/printdt,count2nano(dt));
                    cnt=0;
                }
#else
		usleep(10000); //100hz
#endif
	}	
#ifdef __RTAI__
	rt_task_delete(task);
#endif
	if (svc->data_thread_error)
	  M3_INFO("Exiting M3 Data Server Thread Prematurely\n",0);
	else
	  M3_INFO("Exiting M3 Data Server Thread\n",0);
	svc->data_thread_active=false;
	return;
}
////////////////////////////////////////////////////////////

int M3RtDataService::instances = 0;

bool M3RtDataService::Startup()
{
	if (data_thread_active)
	{
		M3_ERR("M3RtDataService thread already active\n",0);
		return true;
	}
	M3_INFO("Startup of M3RtDataService, port %d...\n",portno);
	ext_sem=sys->GetExtSem();
#ifdef __RTAI__
	M3_INFO("Creating rt_threadDataService...\n");
	hdt=rt_thread_create((void*)data_thread,this,10000);  // wait until thread starts
#else
	long int hdt = pthread_create((pthread_t *)&hdt, NULL, (void *(*)(void *))data_thread, (void*)this);
#endif
	usleep(100000);
	if (!data_thread_active || !hdt)
	{
		M3_ERR("Unable to start M3RtDataSevice\n",0);
		return true;
	}
	return data_thread_active;
}

void M3RtDataService::Shutdown()
{
	M3_INFO("Shutdown of M3RtDataService , port %d\n",portno);
	data_thread_end=true;
#ifdef __RTAI__
	rt_thread_join(hdt);
#else
	pthread_join((pthread_t)hdt, NULL);
#endif
	if (data_thread_active)
		M3_WARN("M3RtDataService thread did not shut down correctly\n");
	server.Shutdown();
	
}


void M3RtDataService::ClientSubscribeStatus(string name)
{
	for (int i=0;i<status_names.size();i++)
		if (name.compare(status_names[i])==0)
			return;
	status_names.push_back(name);
}
	
bool M3RtDataService::Step()
{
	int nw,nr,res;

        res=server.ReadStringFromPort(sread, nr);
	if (res==-1) //error
	  return false;
	//If receive cmd data, parse and reply with status data (or just reply with status data if no cmd)
	if(res)
	{
		M3CommandAll * c=NULL;
		if (nr>0)
		{
			c = new M3CommandAll;
			c->ParseFromString(sread);
		}
		
#ifdef __RTAI__
		rt_sem_wait(ext_sem);
#else
		sem_wait(ext_sem);
#endif
		if (c!=NULL)
		{
			sys->ParseCommandFromExt(*c);
			delete c;
		}
		if (!sys->SerializeStatusToExt(status,status_names))
		{
#ifdef __RTAI__
			rt_sem_signal(ext_sem);
#else
			sem_post(ext_sem);
#endif
			return false;
		}
#ifdef __RTAI__
		rt_sem_signal(ext_sem);
#else
			sem_post(ext_sem);
#endif
		status.SerializeToString(&swrite);
		nw=server.WriteStringToPort(swrite);
		if(nw<0)
			return false;
	}
	return true;
}
////////////////////////////////////////////////////////////
}
