# M3 -- Meka Robotics Real-Time Control System
# Copyright (c) 2010 Meka Robotics
# Author: edsinger@mekabot.com (Aaron Edsinger)
# 
# M3 is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# M3 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with M3.  If not, see <http://www.gnu.org/licenses/>.

## Process this file with automake to produce Makefile.in

#//////////////////////////////////////////////////////////////////

m3_dir = $(top_srcdir)/src
rtai_inc = $(RTAI_DIR)/include  
ros_dir = $(ROS_DIR)

EXTRA_DIST = \
	rt_system.h \
	rt_service.h \
	rt_data_service.h \
	rt_log_service.h \
	rt_ros_service.h \
	m3rt_system.i
  
clean-local:
	rm -f m3rt_system_wrap.cpp
	
_m3rt_system_la_includedir = $(includedir)/m3rt/rt_system
_m3rt_system_la_include_HEADERS = \
	rt_data_service.h \
	rt_log_service.h \
	rt_ros_service.h \
	rt_service.h \
	rt_system.h


#lib_LTLIBRARIES = libm3rt_system.la
BUILT_SOURCES = m3rt_system_wrap.cpp 
pkgpython_PYTHON = m3rt_system.py 
pkgpyexec_LTLIBRARIES = _m3rt_system.la 

_m3rt_system_la_SOURCES = \
	rt_service.cpp \
	rt_system.cpp \
	rt_data_service.cpp \
	rt_log_service.cpp \
	m3rt_system_wrap.cpp \
	m3rt_system.i 

if RTAI
RT = -D__RTAI__
else
RT = 
endif

if RTAI
LRT = -llxrt
else
LRT = 
endif
	
_m3rt_system_la_CPPFLAGS = \
	-pipe -msse2 -ffast-math -mhard-float \
	$(RT) \
	$(SWIG_PYTHON_CPPFLAGS) \
	$(ETHERCAT_CFLAGS) \
	-I$(m3_dir) \
	-I$(rtai_inc) \
	-I$(ros_dir)/include
	
	
_m3rt_system_la_LDFLAGS = -module -version-info 1:1:0 -mhard-float \
	-L$(RTAI_DIR)/lib \
	-L$(top_srcdir)/src/m3rt/base \
	-L$(ros_dir)/lib
	

_m3rt_system_la_LIBADD = $(LRT) $(PROTOBUF_LIBS) $(YAMLCPP_LIBS) -lm3base -lpthread

m3rt_system_wrap.cpp : m3rt_system.i
	$(SWIG) $(SWIG_PYTHON_OPT) -o $@ -I$(m3_dir) $<
	
