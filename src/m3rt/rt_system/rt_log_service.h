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

#ifndef RT_LOG_SERVICE_H
#define RT_LOG_SERVICE_H

#include "m3rt/base/component.h"
#include "m3rt/base/component_base.pb.h"
#include "m3rt/base/toolbox.h"
#include <string>

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


namespace m3rt
{

#define MAX_PAGE_QUEUE 300 //In case log service not stopped properly, force shutdown

/**
 * @brief
 *
 */
class M3RtLogService
{
public:
	M3RtLogService(M3RtSystem * s, std::string n, std::string p, mReal freq,int ps,int vb):
		sys(s),name(n),path(p),start_idx(0),page(NULL),entry(NULL),page_size(ps),verbose(vb),num_page_write(0),num_kbyte_write(0),num_kbytes_in_buffer(0),entry_idx(0),page_idx_read(0),page_idx_write(0),pages_written(0)
	{
		downsample_rate = MAX(0,((int)((mReal)RT_TASK_FREQUENCY)/freq)-1); 
		downsample_cnt=0;
	}
    /**
     * @brief
     *
     * @return bool
     */
    bool Startup();					//Called by M3RtService
    /**
     * @brief
     *
     */
    void Shutdown();				//Called by M3RtService
    /**
     * @brief
     *
     * @return bool
     */
    bool WritePagesToDisk();			//Called by M3RtLogService thread
    /**
     * @brief
     *
     * @param name
     */
    void AddComponent(std::string name);			//Called by M3RtService
    /**
     * @brief
     *
     * @return bool
     */
    bool Step();					//Called by M3RtSystem
    /**
     * @brief
     *
     * @param final
     * @return bool
     */
    bool WriteEntry(bool final);
    /**
     * @brief
     *
     * @return M3StatusLogPage
     */
    M3StatusLogPage * GetNextPageToRead();
    /**
     * @brief
     *
     * @return M3StatusLogPage
     */
    M3StatusLogPage * GetNextPageToWrite();
    /**
     * @brief
     *
     */
    void MarkPageEmpty();
    /**
     * @brief
     *
     */
    void MarkPageFull();
private:
	
    /**
     * @brief
     *
     * @param num_entry
     * @return std::string
     */
    std::string GetNextFilename(int num_entry);
    std::string name; /**< TODO */
    std::string path; /**< TODO */
    M3StatusAll * entry; /**< TODO */
    std::vector<M3StatusLogPage*> pages; /**< TODO */
    std::vector<bool> is_page_full; /**< TODO */
    std::vector<M3Component *> components; /**< TODO */
    int start_idx; /**< TODO */
    int downsample_cnt; /**< TODO */
    int downsample_rate; /**< TODO */
    M3StatusLogPage * page; /**< TODO */
    M3RtSystem * sys; /**< TODO */
    int page_size; /**< TODO */
    int hlt; /**< TODO */
    int verbose; /**< TODO */
    int num_page_write; /**< TODO */
    int num_kbyte_write; /**< TODO */
    int num_kbytes_in_buffer; /**< TODO */
    int entry_idx; /**< TODO */
    int page_idx_write; /**< TODO */
    int page_idx_read; /**< TODO */
    int pages_written; /**< TODO */
};

}
#endif
