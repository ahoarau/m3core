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

#include "m3rt/base/toolbox.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdarg.h>
#include <cstdlib>
#include <time.h>
#include <ios>
namespace m3rt
{
using namespace std;

void M3_WARN(const char *format, ...)
{
    //FILE * pFile = NULL;
    string path;

    /*	if (GetEnvironmentVar(M3_ROBOT_ENV_VAR, path))
    	{
    		path.append(LOG_FILE);
    		pFile = fopen (path.c_str(),"a");
    	}	*/

    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    printf("M3 WARNING: ");
    printf("%s", buffer);
    /*	 if (pFile)
    	 {
    	    fprintf(pFile,"M3 WARNING: ");
    	    fprintf(pFile,"%s", buffer);
    	    fclose (pFile);
    	 }*/
    va_end(args);
}

void M3_ERR(const char *format, ...)
{
    FILE *pFile = NULL;
    string path;

    /*if (GetEnvironmentVariable(M3_ROBOT_ENV_VAR, path))
    {
    	path.append(LOG_FILE);
    	pFile = fopen (path.c_str(),"a");
    }*/

    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    printf("M3 ERROR: ");
    printf("%s", buffer);
    if(pFile) {
        fprintf(pFile, "M3 ERROR: ");
        fprintf(pFile, "%s", buffer);
        fclose(pFile);
    }
    va_end(args);
}

void M3_DEBUG(const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    printf("M3 DEBUG: ");
    printf("%s", buffer);
    va_end(args);
}

void M3_INFO(const char *format, ...)
{
    //FILE * pFile = NULL;
    string path;

    /*	if (GetEnvironmentVar(M3_ROBOT_ENV_VAR, path))
    	{
    		path.append(LOG_FILE);
    		pFile = fopen (path.c_str(),"a");
    	}	*/

    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    printf("M3 INFO: ");
    printf("%s", buffer);
    /*	 if (pFile)
    	 {
    	    fprintf(pFile,"M3 INFO: ");
    	    fprintf(pFile,"%s", buffer);
    	    fclose (pFile);
    	 }*/
    va_end(args);
}

void BannerPrint(int width, const char *format, ...)
{
    char buf[81];
    width = width > 80 ? 80 : width;
    char out[] = "--------------------------------------------------------------------------------"; //len 80
    va_list args;
    va_start(args, format);
    snprintf(buf, 80, format, args);
    M3_PRINTF("\n");
    int n = (int)strlen(buf);
    if(n < width && ((width - n) / 2) > 0) {
        out[(width - n) / 2] = 0;
        M3_PRINTF("%s", out);
        M3_PRINTF("%s", buf);
        M3_PRINTF("%s", out);
    } else
        M3_PRINTF("%s", buf);
    va_end(args);
    M3_PRINTF("\n");
}

bool GetEnvironmentVariable(const char *var, vector<string>& result)
{
    //static vector<string> result;
    result.clear();
    if(!result.empty())
        return false;

    const std::string PATH = getenv(var);
    const char delimiter = ':';

    if(PATH.empty())
        return false;

    size_t previous = 0;
    size_t index = PATH.find(delimiter);
    while(index != string::npos) {
        result.push_back(PATH.substr(previous, index - previous));
        previous = index + 1;
        index = PATH.find(delimiter, previous);
    }
    result.push_back(PATH.substr(previous));

    return true;
}

bool GetEnvironmentVar(const char *var, string &s)
{
    char *p = getenv(var);
    if(p != NULL) {
        s.assign(p);
        return true;
    }
    return false;
}

vector<mReal> YamlReadVectorM(string s)
{
    size_t start = s.find_first_of("[", 0); //, size_type num );
    size_t end = s.find_first_of(",]", 0);
    vector<mReal> f;
    while(start < s.size() && end < s.size() && start != string::npos && end != string::npos) {
        string val = s.substr(start + 1, end - start - 1);
        f.push_back((mReal) strtod(val.c_str(), NULL));
        start = end;
        end = s.find_first_of(",]", start + 1);
    }
    return f;
};

vector<string> YamlReadVectorString(string s)
{
    size_t start = s.find_first_of("[", 0); //, size_type num );
    size_t end = s.find_first_of(",]", 0);
    vector<string> f;
    while(start < s.size() && end < s.size() && start != string::npos && end != string::npos) {
        string val = s.substr(start + 1, end - start - 1);
        f.push_back(val);
        start = end;
        end = s.find_first_of(",]", start + 1);
    }
    return f;
};

void operator >> (const YAML::Node &node, vector<mReal> & v)
{
    for(unsigned i = 0; i < node.size(); i++) {
        mReal x;
        node[i] >> x;
        v.push_back(x);
    }
}

vector<mReal> YamlReadVectorM(const YAML::Node &seq)
{
    vector<mReal> f;
    mReal val;
    for(size_t i = 0; i < seq.size(); i++) {
        seq[i] >> val;
        f.push_back(val);
    }

    return f;
};

// Converts a hexadecimal string to integer
// Returns 0 if not valid
unsigned int xtoi(const char *xs)
{
    unsigned int n;
    sscanf(xs, "%x", &n);
    return n;
}

void WriteYamlDoc(const char *filename, YAML::Emitter &doc, string sub_dir)
{
    string s(filename);
    string path;
    vector<string> vpath;
    if(GetEnvironmentVariable(M3_ROBOT_ENV_VAR, vpath)) {
        for(size_t i = 0; i < vpath.size(); i++) {
            vpath[i] += sub_dir;
            vpath[i] += s;
        }
    }
    path = vpath[0];
    std::ofstream fout(path.c_str());
    fout << doc.c_str();
    fout.close();
}
void GetYamlStream(const char *filename,YAML::Emitter& out, string sub_dir)
{
    string s(filename);
    string path;
    vector<string> vpath;
    if(GetEnvironmentVariable(M3_ROBOT_ENV_VAR, vpath)) {
        for(size_t i = 0; i < vpath.size(); i++) {
            vpath[i] += sub_dir;
            vpath[i] += s;
        }
    }
    YAML::Parser parser_in;
    YAML::Node node;
    //YAML::Emitter out;
    
    for(size_t i = 0; i < vpath.size(); i++) {
        
        path = vpath[i];
        ifstream fin(path.c_str());
        if(fin.fail()) {
            //M3_INFO("Could not read %s \n", path.c_str());
            continue;
        }
        parser_in.Load(fin);
        while(parser_in.GetNextDocument(node)) {
            out << node; 
        }
        fin.close();
    }
    assert(out.good());
    return ;//out;
}

void GetYamlDoc(const char *filename, YAML::Node &doc, string sub_dir)
{
    string s(filename);
    string path;
    vector<string> vpath;
    if(GetEnvironmentVariable(M3_ROBOT_ENV_VAR, vpath)) {
        for(size_t i = 0; i < vpath.size(); i++) {
            vpath[i] += sub_dir;
            vpath[i] += s;
        }
    }
    YAML::Parser parser;
    YAML::Node node;
    YAML::Emitter out;
    for(size_t i = 0; i < vpath.size(); i++) {
        path = vpath[i];
        //cout << "Loading file : " << path << endl;
        ifstream fin(path.c_str());
        if(fin.fail()) {
            //M3_ERR("Could not read %s \n", path.c_str());
            continue;
        }
        parser.Load(fin);
        while(parser.GetNextDocument(node)) {
            out << node; 
        }

        fin.close();
    }
    assert(out.good());
    std::stringstream stream(out.c_str());
    parser.Load(stream);
    parser.GetNextDocument(doc);
    
    return;
}

}
