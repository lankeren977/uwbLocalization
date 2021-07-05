#ifndef LOAD_CONFIG_H
#define LOAD_CONFIG_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
using namespace std;

extern  map<int,double*> globalAnchors;
extern  map<string,double> globalParams;

static string anchors_path = "/home/linux/zhangqr/uwbLocalize/config/anchors.conf";
static string sys_params_path = "/home/linux/zhangqr/uwbLocalize/config/sys_params.conf";

double getParam(string key);
void loadConfig();

#endif