// MPSCoreDemo.cpp : 
/*
Function:
	Demonstration the mission plan DLL
Author:
	godog
Email:
	708979286@qq.com
*/

#include <stdio.h>
#include <iostream>
#include <windows.h>


using namespace std;

/*
Interface Function Definition
*/

/*
Function: Establish Task Planning
*/
typedef int (*lpCreateMissionPlan)();

/*
Function：Get the Number of Task Planning Results‌
*/
typedef int (*lpGetMissionCount)();

/*
Interface Function Definition: Get Task Planning Results‌。
Input：
index- Result Index
bufsize- Size of the return buffer, not less than 1024 bytes‌。
输出：
strmsn-Returned planning results‌
*/
typedef int (*lpGetMissionContent)(int index, char* strmsn, int bufsize);

/*
Function：Initialize the scene
Input：
starttime[] - Planning start time, 6 elements representing year, month, day, hour, minute, second respectively‌。
endtime[]-Planning end time, 6 elements representing year, month, day, hour, minute, second respectively‌。
tlefile-TLE file name
enabletlefile-whether enable load TLE file，normally should be true
targetfile-file name of targets
enabletargetfile-whether enable load target file
stationfile-file name of stations
enablestationfile-whether enable load station file
*/
typedef int (*lpInitScene)(int starttime[], int endtime[], char* tlefile, bool enabletlefile, char* targetfile, bool enabletargetfile, char* stationfile, bool enablestationfile);

string GetModulePath()
{
	char szDir[2048] = { 0 };
	::GetModuleFileNameA(NULL, szDir, sizeof(szDir));

	string strResult = szDir;
	strResult = strResult.substr(0, strResult.find_last_of("\\"));
	return strResult;
}

void DemoCMission()
{
	HINSTANCE hDll;//handle

	hDll = LoadLibrary("MPS.dll");//Dynamically Load DLL Module Handle

	if (hDll)
	{
		string strpath = GetModulePath();

		string strPathFile = strpath + "\\UserData\\TLE\\satellite.txt";//tle file
		string targetfile = strpath + "\\UserData\\Targets\\Targets.csv";//target file
		string stationfile = strpath + "\\UserData\\Stations\\Stations.csv";//station file

		int result = 0;

		int starttime[] = { 2025, 10, 1, 0, 0, 0 };//start time of mission plan period
		int endtime[] = { 2025, 10, 2, 0, 0, 0 };//end time of mission plan period

		//initialize the scene
		lpInitScene InitScene = (lpInitScene)GetProcAddress(hDll, "InitScene");
		if (InitScene)
		{
			result = InitScene(starttime, endtime, (char*)strPathFile.c_str(), true, (char*)targetfile.c_str(), true, (char*)stationfile.c_str(), true);
		}

		//set the parameters of satellite, sensor,targets,stations


		//Create the mission plan
		if (result == 0)//if successful，return 0
		{
			cout << "Init scene successfully" << endl;
			cout << "Begin create mission plan......" << endl;
			lpCreateMissionPlan CreateMissionPlan = (lpCreateMissionPlan)GetProcAddress(hDll, "CreateMissionPlan");//create the mission plan
			CreateMissionPlan();

		}

		//get the results of mission plan
		//get the number of results
		int missioncount = 0;
		lpGetMissionCount GetMissionCount = (lpGetMissionCount)GetProcAddress(hDll, "GetMissionCount");//get the numbers
		if (GetMissionCount)
		{
			missioncount = GetMissionCount();
			cout << "Mission Count=" << missioncount << endl;
		}

		//Retrieve Planning Results Item by Item, Return in String Format Separated by ","‌
		char strmsn[1024] = { '\0' };
		lpGetMissionContent GetMissionContent = (lpGetMissionContent)GetProcAddress(hDll, "GetMissionContent");//Get Result Content‌
		if (GetMissionContent)
		{

			for (int i = 0; i < missioncount; i++)//Retrieve Result Content Item by Item‌
			{
				GetMissionContent(i, strmsn, 1024);
				cout << strmsn << endl;
			}
		}

		FreeLibrary(hDll);//Release the Loaded DLL Module

	}
}

int main(int argc, char* argv[])
{
	DemoCMission();

	system("pause");

	return 0;
}