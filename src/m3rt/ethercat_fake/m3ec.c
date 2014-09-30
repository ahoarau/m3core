/*
Copyright � 2008, Meka Robotics
All rights reserved.
http://mekabot.com

Redistribution and use in source and binary forms, with or without
modification, are permitted. 


THIS SOFTWARE IS PROVIDED BY THE Copyright HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARtICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
Copyright OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIREcT, INDIREcT,
INCIDENTAL, SPEcIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORt (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#include <linux/module.h>

#include "m3rt/base/m3rt_def.h"
#include "m3rt/base/m3ec_def.h"

// Linux

#include <asm/io.h>
//#include <math.h>


// RtAI
#include <rtai.h>
#include <rtai_shm.h>
#include <rtai_sched.h>
#include <rtai_nam2num.h>
#include <rtai_sem.h>
#include <rtai_registry.h>
/*
// Not needed anymore
#include "ecrt.h"
#include "slave.h"
#include "slave_config.h"
*/


//Convenience printf-like macros for printing M3-specific information.
#define M3_INFO(fmt, args...) printk(KERN_INFO "M3 INFO: " fmt, ##args)
#define M3_ERR(fmt, args...) printk(KERN_ERR "M3 ERROR: " fmt, ##args)
#define M3_WARN(fmt, args...) printk(KERN_WARNING "M3 WARNING: " fmt, ##args)

//#define USE_DISTRIBUTED_CLOCKS //Version 1.0 and newer

#define NUM_EC_CYCLES_PER_RT 1
#define RT_KMOD_FREQUENCY (RT_TASK_FREQUENCY*NUM_EC_CYCLES_PER_RT*NUM_EC_DOMAIN)	//Frequency of rt kernel module (HZ) (3000)


typedef struct 
{
	M3EcSystemShm * shm;
	unsigned int offset_status[MAX_NUM_SLAVE];
	unsigned int offset_command[MAX_NUM_SLAVE];	
	uint8_t *domain_pd[NUM_EC_DOMAIN];
	int num_domain;
	int domain_idx;
		
} M3SystemEc;

/*
Unresolved issue. With many slaves (~35) the total size of a single domain spans
3 ethernet frames. When this happens, the bus starts to bomb. It works OK for two frames. Possible causes:
	* Ethernet NIC buffer size is too small (likely?)
	* Total bandwidth consumed (unlikely)
	* Etherlab Master has too much overhead (maybe)
If we split data into two domains, we can run the loop at 2khz and alternatively update either domain. This is effectively a 1khz update, however the time-stamping will be off by 500us on 1/2 the slaves. Because this works, the issue is not likely bandwidth (dropping to 100hz doesn't fix it either.

Solution (temporary): multiplex domains. We should be able to do all slaves a 1khz. Perhaps should look into the NIC specs.
*/
/*****************************************************************************/
M3SystemEc sys={};

//Things that need to be freed on completion
RT_TASK task;
SEM master_sem;
SEM shm_sem;
SEM sync_sem;

//A.Hoarau: Fix on deprecated SPIN_LOCK_UNLOCKED
//spinlock_t master_lock = __SPIN_LOCK_UNLOCKED();
//static DEFINE_SPINLOCK(master_lock) ;

cycles_t t_last_cycle;
cycles_t t_critical;
bool end=false;

/*****************************************************************************/


int check_master_state(void)
{
	return 1;
}
/*****************************************************************************/
void check_domain_state(void)
{
	return;
}
/*****************************************************************************/
void check_slave_state(void)
{
	return;
}
/*****************************************************************************/
void run(long shm)
{
	static unsigned counter=0;
	int sidx,i;
	int rt_downsample = 0; // only signal rt server every 3 cycles
	M3EcSlaveShm * s;
	RTIME tstart;
	RTIME t_ecat_wait_rx;
	RTIME t_ecat_rx;
	RTIME t_ecat_wait_shm;
	RTIME t_ecat_shm;
	RTIME t_ecat_wait_tx;
	RTIME t_ecat_tx;
	RTIME ts0;
	RTIME ts1;
	RTIME ts2;
	RTIME ts3;
	RTIME ts4;
	RTIME ts5;
	RTIME ts6;
	int print_mult_dt=5;
	RTIME print_dt=print_mult_dt*1e9;
	RTIME print_start;
	RTIME dt;
	int tmp_cnt=0;
	M3_INFO("EtherCAT kernel loop starting...\n");
	sys.shm->counter=0;
	tstart=rt_get_time_ns();
	print_start = rt_get_time_ns();
	while (1) {
		if(end) break;
		ts0=rt_get_time_ns();
	/*	t_last_cycle = get_cycles();
	//Process Domain
		
		rt_sem_wait(&master_sem);
		ts1=rt_get_time_ns();
		//ecrt_master_receive(sys.master); //Get Status data
		//ecrt_domain_process(sys.domain[sys.domain_idx]);
		rt_sem_signal(&master_sem);
		ts2=rt_get_time_ns();
	//Exchange data with shared memory
		rt_sem_wait(&shm_sem);
		ts3=rt_get_time_ns();
		sys.shm->timestamp_ns=rt_get_time_ns()-tstart;
		for (sidx=0;sidx<sys.shm->slaves_responding;sidx++)
		{
			s=&(sys.shm->slave[sidx]);
			if (s->active && (sidx%sys.num_domain)==sys.domain_idx)
			{
				unsigned char * ps = sys.domain_pd[sidx%sys.num_domain]+sys.offset_status[sidx];
				unsigned char * pc = sys.domain_pd[sidx%sys.num_domain]+sys.offset_command[sidx];
				memcpy(s->status,ps, s->n_byte_status);
				memcpy(pc,s->cmd,s->n_byte_cmd);
			}
		}	
	
		rt_sem_signal(&shm_sem);
		if (sys.domain_idx%NUM_EC_DOMAIN==0)
		{		      
			sys.shm->counter++;
			rt_downsample++; 
			if (rt_downsample == NUM_EC_CYCLES_PER_RT)
			{
			  rt_sem_signal(&sync_sem);
			  rt_downsample = 0;
			}
		}
		ts4=rt_get_time_ns();
	//Send data out
		rt_sem_wait(&master_sem);
		ts5=rt_get_time_ns();
		//ecrt_domain_queue(sys.domain[sys.domain_idx]);
		//ecrt_master_send(sys.master);
		rt_sem_signal(&master_sem);
		ts6=rt_get_time_ns();
		// calc timing for monitor:		
		t_ecat_wait_rx = ts1 - ts0;
		t_ecat_rx = ts2 - ts1;
		t_ecat_wait_shm = ts3 - ts2;
		t_ecat_shm = ts4 - ts3;
		t_ecat_wait_tx = ts5 - ts4;
		t_ecat_tx = ts6 - ts5;
	      
		sys.shm->monitor[sys.domain_idx].t_ecat_wait_rx = t_ecat_wait_rx;
		sys.shm->monitor[sys.domain_idx].t_ecat_rx = t_ecat_rx;
		sys.shm->monitor[sys.domain_idx].t_ecat_wait_shm = t_ecat_wait_shm;
		sys.shm->monitor[sys.domain_idx].t_ecat_shm = t_ecat_shm;
		sys.shm->monitor[sys.domain_idx].t_ecat_wait_tx = t_ecat_wait_tx;
		sys.shm->monitor[sys.domain_idx].t_ecat_tx = t_ecat_tx;
		
		sys.domain_idx=(sys.domain_idx+1)%sys.num_domain;
	
		if (counter) 
			counter--;
		else 
		{
			counter=RT_STATUS_FREQUENCY;
			check_domain_state();
			check_slave_state();
			if (!check_master_state())
				goto run_cleanup; 
		}*/
		rt_task_wait_period();
		dt =rt_get_time_ns() -ts0;
		if (rt_get_time_ns() -print_start >= print_dt)
        {
            rt_printk("M3ec (Kernel) freq : %d (dt: %lld us / des period: %lld us)\n",tmp_cnt/print_mult_dt,(dt/1000),(RT_KMOD_TIMER_TICKS_NS/1000));
			tmp_cnt = 0;
			print_start = rt_get_time_ns();
			
        }
        tmp_cnt++;
	}
run_cleanup:
	M3_INFO("Entering Run Cleanup...\n");
	//Set all cmds to 0. By convention, all M3 products safe with this cmd.
	for (sidx=0;sidx<sys.shm->slaves_responding;sidx++)
	{
		s=&(sys.shm->slave[sidx]);
		if (s->active)
		{
			unsigned char * pc = sys.domain_pd[sidx%sys.num_domain]+sys.offset_command[sidx];
			memset(pc,0,MAX_PDO_SIZE_BYTES);
			s->active=0;

		}
	}
	for (i=0;i<sys.num_domain;i++)
	{
		rt_sem_wait(&master_sem);
		//ecrt_domain_queue(sys.domain[i]);
		//ecrt_master_send(sys.master);
		rt_sem_signal(&master_sem);
	}
}

/*****************************************************************************/

void request_lock(void *shm)
{
    // too close to the next real time cycle: deny access...
	if (get_cycles() - t_last_cycle > t_critical) return;
    // allow access
	rt_sem_wait(&master_sem);	
}

/*****************************************************************************/

void release_lock(void *shm)
{
	rt_sem_signal(&master_sem);
}

/*****************************************************************************/
int m3sys_startup(void)
{
	 
	//int sidx,i;
	//int pcode;
	//M3EcSlaveShm * s;
	//int found=0,ps=0;
	M3_INFO("Slaves Responding: %d\n",sys.shm->slaves_responding);

	sys.num_domain=MAX(1,MIN(sys.shm->slaves_responding,NUM_EC_DOMAIN));
	M3_INFO("Creating %d domains...\n",sys.num_domain);
	sys.domain_idx=0;
	M3_INFO("Registering PDOs...\n");
	sys.shm->slaves_active=0;
	M3_INFO("Successful Setup of all slaves\n");
	M3_INFO("Activating master...\n");
	return 1;
	
//out_release_master:
//		M3_ERR("Releasing master...\n");
	return 0;
	
}

int __init init_mod(void)
{
	RTIME tick_period, requested_ticks, now;

	rt_sem_init(&master_sem, 1);
	rt_sem_init(&shm_sem, 1);
	rt_sem_init(&sync_sem, 0); //Only signaled when ec update done
	rt_register(nam2num(SEMNAM_M3LSHM),&shm_sem,IS_SEM,0);
	rt_register(nam2num(SEMNAM_M3SYNC),&sync_sem,IS_SEM,0);
	M3_INFO("Starting...\n");
	sys.shm= rtai_kmalloc(nam2num(SHMNAM_M3MKMD), sizeof(M3EcSystemShm));
	memset(sys.shm,0,sizeof(M3EcSystemShm));
	M3_INFO("Allocated M3System shared memory of size: %u.\n",(int)sizeof(M3EcSystemShm));
	if (!m3sys_startup())
		goto out_return;
	t_critical = cpu_khz * 1000 / RT_KMOD_FREQUENCY - cpu_khz * RT_INHIBIT_TIME / 1000;
	M3_INFO("Starting cyclic sample thread...\n");
	requested_ticks = nano2count(RT_KMOD_TIMER_TICKS_NS); //
	tick_period = start_rt_timer(requested_ticks);
	M3_INFO("Rt timer started with %i/%i ticks.\n", (int) tick_period, (int) requested_ticks);
	if (rt_task_init(&task, run, 0, RT_STACK_SIZE, RT_TASK_PRIORITY+1, 1, NULL)) {
		M3_ERR("Failed to init RtAI task!\n");
		goto out_free_timer;
	}
	now = rt_get_time();
	if (rt_task_make_periodic(&task, now + tick_period, tick_period)) {
		M3_ERR("Failed to run RtAI task!\n");
		goto out_free_task;
	}
	M3_INFO("Initializion Done....\n");
	return 0;
 out_free_task:
		 rt_task_delete(&task);
 out_free_timer:
		 stop_rt_timer(); 
 out_return:
		 rt_sem_delete(&master_sem);
 rt_sem_delete(&shm_sem);
 rt_sem_delete(&sync_sem);
 rtai_kfree(nam2num(SHMNAM_M3MKMD));
 rt_drg_on_name(nam2num(SEMNAM_M3LSHM));
 rt_drg_on_name(nam2num(SEMNAM_M3SYNC));
 rt_drg_on_name(nam2num(SEMNAM_M3LEXT));
 M3_ERR("Failed to load. Aborting.\n");
 return -1;
}

/*****************************************************************************/

void __exit cleanup_mod(void)
{
	end=true;
	M3_INFO("Stopping...\n");
	rt_task_delete(&task);
	stop_rt_timer();
	//ecrt_release_master(sys.master);
	rt_sem_delete(&master_sem);
	rt_sem_delete(&shm_sem);
	rt_sem_delete(&sync_sem);
	rtai_kfree(nam2num(SHMNAM_M3MKMD));
	rt_drg_on_name(nam2num(SEMNAM_M3LSHM));
	rt_drg_on_name(nam2num(SEMNAM_M3SYNC));
	M3_INFO("Unloading.\n");
}

/*****************************************************************************/

module_init(init_mod);
module_exit(cleanup_mod);

/*****************************************************************************/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aaron Edsinger edsinger@mekabot.com");	
MODULE_DESCRIPTION("Meka Robotics M3 EtherCAT driver");	

/*****************************************************************************/
