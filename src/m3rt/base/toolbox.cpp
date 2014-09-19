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
#include <ctime>
#include <sstream>
#include <map>
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

void WriteYamlDoc(const char *filepath, YAML::Emitter &doc, string sub_dir)
{
	cout<<"Trying to write the file "<<filepath;
    std::ofstream fout(filepath);
    fout << doc.c_str();
    fout.close();
    cout<<" ...OK"<<endl;
}
void GetRobotConfigPath(vector<string>& vpath,string sub_dir)
{
        vpath.clear();
        if(GetEnvironmentVariable(M3_ROBOT_ENV_VAR, vpath)) {
        for(size_t i = 0; i < vpath.size(); i++) {
            vpath[i] += sub_dir;
        }
    }
    return;
}

void GetFileConfigPath(const char *filename,vector<string>& vpath)
{
        string s(filename);
        vpath.clear();
	GetRobotConfigPath(vpath);
        for(size_t i = 0; i < vpath.size(); i++) {
            vpath[i] += s;
        }
    return;
}

/*void GetYamlParser(const char *filename, YAML::Parser &parser )
{
    string path;
    YAML::Node node;
    vector<string> vpath; 
    YAML::Emitter out;
    GetFileConfigPath(filename,vpath);
    for(vector<string>::iterator it = vpath.begin(); it != vpath.end(); ++it) {
        ifstream fin((*it).c_str());
        if(fin.fail()) { continue;}
        parser.Load(fin);
        while(parser.GetNextDocument(node)) {
            out << node;
        }
        fin.close();
        fin.clear();
    }
    assert(out.good());
    parser.PrintTokens(cout);
    return;
}*/

/*std::auto_ptr<YAML::Node> GetYamlDocs(const char *filename)
{
    YAML::Parser parser;
    YAML::Node docs;
    vector<string> vpath; 
    GetFileConfigPath(filename,vpath);
    for(vector<string>::iterator it = vpath.begin(); it != vpath.end(); ++it) {
        ifstream fin((*it).c_str());
        if(fin.fail()) { continue;}
        parser.Load(fin);
        //parser.GetNextDocument(docs);
        fin.close();
        fin.clear();
    }
    parser.PrintTokens(cout);
    return docs.Clone();
}*/

void GetYamlStream(const char *filename, YAML::Emitter &out)
{
    string path;
    YAML::Node node;
    YAML::Parser parser;
    vector<string> vpath; 
    GetFileConfigPath(filename,vpath);
    for(vector<string>::iterator it = vpath.begin(); it != vpath.end(); ++it) {
        ifstream fin((*it).c_str());
        if(fin.fail()) { continue;}
        parser.Load(fin);
        while(parser.GetNextDocument(node)) {
            out << node;
        }
        fin.close();
        fin.clear();
    }
    assert(out.good());
    parser.PrintTokens(cout);
    return;
}
/*void GetYamlDoc(const char *filename, YAML::Node &doc, string sub_dir){
        YAML::Emitter out;
        m3rt::GetYamlStream(filename,out);
        std::stringstream stream(out.c_str());
        YAML::Parser parser(stream);
        while(parser.GetNextDocument(doc)) {}
        return;
}*/
bool GetYamlDoc(const char *filename, YAML::Node &doc, std::string *doc_root_path, const char *find_c)
{
    string s(filename);
    string find_str;
    
    find_c==NULL ? find_str=string():find_str=string(find_c);
    
    vector<string> vpath;
    GetFileConfigPath(filename,vpath);
    vector<string> vpath_root;
    GetRobotConfigPath(vpath_root);
    assert(vpath.size()==vpath_root.size());
    
    map<string, string> paths;
    std::map<string,string>::iterator it = paths.begin();
    for(size_t i=0;i<vpath.size();i++)
	    paths.insert(it,pair<string,string>(vpath[i],vpath_root[i]));
	    
    bool verbose = paths.size()>1;
    YAML::Parser parser;
    
    for(std::map<string,string>::reverse_iterator it = paths.rbegin(); it != paths.rend(); ++it) {
        //cout<<"Trying with path:"<<(*it).c_str()<<" find_str:"<<find_str<<endl;
        //A.H: Let's start by the very last one i.e a local version.
        //If the file exists, load it, otherwise go to previous path (down to original robot_config)
        //If the file is loaded, then check for an optional find_str provided to checkif this is the right file to load, otherwise go to previous path
        string path = it->first;
	string root_path = it->second;
	
        ifstream fin(path.c_str());
        if(fin.fail()) {
            //if(verbose)
            //cout<<"Could not read "<<path<<" , trying the next one."<<endl;
            continue;
        }
        if(verbose)
        cout << "For component " << s << " ,loading file : " << path<<endl;
        parser.Load(fin);
        parser.GetNextDocument(doc);
        fin.close();
        fin.clear();
	if(doc_root_path!=NULL){
		*doc_root_path = root_path;
	}
        if(find_str.empty()) {
            if(verbose)
            cout << " ...OK" << endl;
            return true;
        } else {
           if(!doc.FindValue(find_str)){
                   if(verbose)
                   cout <<" but key \""<< find_str<<"\" NOT FOUND, trying with the next one." << endl;
                   continue;
           }else{
                   if(verbose)
                   cout <<" and key \""<< find_str<<"\" FOUND, returning true." << endl;
                   return true;
           }
           
        }
    }
    //M3_ERR("Error while trying to load %s\n",s.c_str());
    return false;
}

/*void GetYamlDoc2(const char *filename, YAML::Node &doc)
{
    string s(filename);
    string path;
    vector<string> vpath; 
    GetFileConfigPath(filename,vpath);
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
}*/

}
