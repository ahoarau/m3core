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

#ifndef  M3RT_TOOLBOX_H
#define  M3RT_TOOLBOX_H

#include <vector>
#include <string>
#include "m3rt/base/m3rt_def.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>
namespace m3rt
{

#define M3_PRINTF printf
void M3_WARN(const char * format, ...);
void M3_ERR(const char * format, ...);
void M3_INFO(const char * format, ...);
void M3_DEBUG(const char * format, ...);

inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

void BannerPrint(int width, const char *format, ...);
bool GetEnvironmentVariable(const char * var, std::vector<std::string>& result);
bool GetEnvironmentVar(const char * var, std::string &s);
std::vector<mReal> YamlReadVectorM(std::string s);
std::vector<std::string> YamlReadVectorString(std::string s);
#ifndef YAMLCPP_05
std::vector<mReal> YamlReadVectorM(const YAML::Node& seq);
#endif
unsigned int xtoi(const char* xs);
bool GetRobotConfigPath(std::vector< std::string >& vpath, std::string sub_dir = std::string(M3_CONFIG_DIR));

bool GetFileConfigPath(const char* filename, std::vector< std::string >& vpath);

void WriteYamlDoc(const char *filename, YAML::Emitter &doc, std::string sub_dir= std::string(M3_CONFIG_DIR));

std::string GetYamlDoc(const char* filename, YAML::Node& doc, void*);
bool GetYamlDoc(const char* filename, YAML::Node& doc);

bool GetYamlStream(const char* filename, YAML::Emitter& out);
#ifdef YAMLCPP_05	
template <typename _T >
void operator >>(const YAML::Node& input, _T& value) {
	value = input.as<_T>();
}
template <typename _T >
void operator >> (const YAML::Node &node, std::vector<_T> & v)
{
	for(unsigned i = 0; i < node.size(); i++){
		v.push_back(node[i].as<_T>());
	}
}
#else
inline void operator >> (const YAML::Node &node, std::vector<mReal> & v)
{
	for(unsigned i = 0; i < node.size(); i++) {
		mReal x;
		node[i] >> x;
		v.push_back(x);
	}
}
#endif 

inline bool ContainsString(const std::vector<std::string>& v_in, const std::string& s_in)
{
	assert(false == s_in.empty());
	for(std::vector<std::string>::const_iterator i=v_in.begin(); i!=v_in.end(); ++i)
	{
		if(*i== s_in)
			return true;
	}
	return false;
}

bool GetAllYamlDocs(const char* filename, std::vector< YAML::Node >& docs );
bool GetAllYamlDocs(std::vector< std::string > vpath, std::vector< YAML::Node >& docs );

}



#endif

