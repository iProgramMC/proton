#include "WiiUtils.h"
#include "PlatformSetup.h"
#include "BaseApp.h"

// Wii stuff
#include <fat.h>
#include <unistd.h>
#include <dirent.h>
#include <ogc/conf.h>

#include "util/MiscUtils.h"

ePlatformID GetPlatformID()
{
	return PLATFORM_ID_WII;
}

const char* GetAppName();

std::string GetBaseAppPath()
{
	return "./" + std::string(GetAppName()) + "/"; // TODO
}

std::string GetSavePath()
{
	return GetBaseAppPath();
}

std::string g_CachePath;

std::string GetAppCachePath()
{
	if (g_CachePath.empty())
		g_CachePath = GetBaseAppPath() + "cache";
	
	return g_CachePath;
}

void SetAppCachePath(std::string path)
{
	g_CachePath = path;
}

void CreateAppCacheDirIfNeeded()
{
	std::string path;

	if (g_CachePath != "")
		path = g_CachePath;
	else
		path = GetBaseAppPath() + "cache";

	mkdir(path.c_str(), 0777);
}

void LogMsg(const char *traceStr, ...)
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	memset ((void*)buffer, 0, logSize);

	va_start(argsVA, traceStr);
	vsnprintf(buffer, logSize, traceStr, argsVA);
	va_end(argsVA);

	std::string data = std::string(buffer) + "\n";
	
	FILE* f = fopen((GetBaseAppPath()+"log.txt").c_str(), "a");
	if (!f) return;
	
	fwrite(data.c_str(), 1, data.size(), f);
	fclose(f);
}

bool LaterThanNow(const int year, const int month, const int day)
{
	struct timeval  nowSecs;
	gettimeofday(&nowSecs, NULL);
	time_t  nowtime = nowSecs.tv_sec;
	struct tm *now = localtime(&nowtime);
	if (now->tm_year < 1900)
	{
		now->tm_year += 1900;
	}
	now->tm_mon++;
	LogMsg("Comparing against date year %d, month %d, day %d", now->tm_year, now->tm_mon, now->tm_mday);

	if (now->tm_year< year )
	{
		return false;
	}
	if (now->tm_year> year )
	{
		return true;
	}
	// year must be equal
	if (now->tm_mon < month )
	{
		return false;
	}
	if (now->tm_mon > month )
	{
		return true;
	}
	// month must be equal
	if (now->tm_mday < day )
	{
		return false;
	}
	if (now->tm_mday > day )
	{
		return true;
	}
	return false;
}

bool CheckDay(const int year, const int month, const int day)
{
	struct timeval  nowSecs;
	gettimeofday(&nowSecs, NULL);
	time_t  nowtime = nowSecs.tv_sec;
	struct tm *now = localtime(&nowtime);
	if (now->tm_year < 1900)
	{
		now->tm_year += 1900;
	}
	now->tm_mon++;
	LogMsg("Comparing against date year %d, month %d, day %d", now->tm_year, now->tm_mon, now->tm_mday);
	if ((now->tm_mday == day) && (now->tm_mon == month) && (now->tm_year == year))
	{
		return true;
	}
	return false;
}

unsigned int GetSystemTimeTick()
{

	/*
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_usec/1000 + tv.tv_sec*1000;
	*/

	//More resistent to playing with the date to change timing in games

	static unsigned int incrementingTimer = 0;
	static double buildUp = 0;
	static double lastTime = 0;

	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	double timeDouble = time.tv_sec*1000 + time.tv_nsec/1000000;

	double change = timeDouble -lastTime;
	if (change > 0 && change < 500)
	{
		incrementingTimer += change;
	}
	lastTime = timeDouble;

	return incrementingTimer;

}

double GetSystemTimeAccurate()
{
	return GetSystemTimeTick();
}

unsigned int GetFreeMemory()
{
	return 0;
}

void LaunchURL(std::string url) {}

void FireAchievement(std::string achievement) {}

std::string g_string;

void SetLastStringInput(std::string s)
{
	g_string = s;
}

std::string GetLastStringInput()
{
	return g_string;
}

bool GetLastWriteDateOf (int *monthOut, int *dayOut, int *yearOut, int *hourOut, int *minOut, int *secOut, std::string fileName, bool bAddSavePath) { return false; }

void RemoveFile(std::string fileName, bool bAddSavePath)
{
	if (bAddSavePath)
		fileName = GetSavePath() + fileName;
	
	remove(fileName.c_str());
}

void CreateDirectoryRecursively(std::string basePath, std::string path)
{
    std::vector<std::string> token = StringTokenize(path, "/");

	if (!basePath.empty())
	{
		if (basePath[basePath.size() - 1] != '/')
        {
            basePath += "/";
        }
	}

	path = "";
	for (unsigned int i = 0; i < token.size(); i++)
	{
		path += token[i].c_str();
        std::string directory = basePath + path;
        mkdir(directory.c_str(), 0777);
		path += "/";
	}
}

bool RemoveDirectoryRecursively(string path)
{
	//LogMsg(" RemoveDirectoryRecursively: %s", path.c_str());
	
	dirent * buf, * ent;
	DIR *dp;

	dp = opendir(path.c_str());
	if (!dp)
	{
		LogError("RemoveDirectoryRecursively: opendir failed");
		return false;
	}

	buf = (dirent*) malloc(sizeof(dirent)+512);
	while (readdir_r(dp, buf, &ent) == 0 && ent)
	{
		
		if (ent->d_name[0] == '.' && ent->d_name[1] == 0) continue;
		if (ent->d_name[0] == '.' && ent->d_name[1] == '.' && ent->d_name[2] == 0) continue;

		//LogMsg("Got %s. type %d", ent->d_name, int(ent->d_type));
		if (ent->d_type == DT_REG) //regular file
		{
			string fName = path+string("/")+ent->d_name;
			//LogMsg("Deleting %s", fName.c_str());
			unlink( fName.c_str());
		}

		if (ent->d_type == DT_DIR) //regular file
		{
			string fName = path+string("/")+ent->d_name;
			//LogMsg("Entering DIR %s",fName.c_str());
			if (!RemoveDirectoryRecursively(fName.c_str()))
			{
				LogError("Error removing dir %s", fName.c_str());
				break;
			}
		}
	}

	free (buf);
	closedir(dp);

	//delete the final dir as well
	rmdir( path.c_str());
	return true; //success
}

vector<string> GetDirectoriesAtPath(string path)
{
	vector<string> v;
	
#ifdef _DEBUG
	//LogMsg("GetDirectoriesAtPath: %s", path.c_str());
#endif

	dirent * buf, * ent;
	DIR *dp;

	dp = opendir(path.c_str());
	if (!dp)
	{
		LogError("GetDirectoriesAtPath: opendir failed (%s)", path.c_str());
		return v;
	}

	buf = (dirent*) malloc(sizeof(dirent)+512);
	while (readdir_r(dp, buf, &ent) == 0 && ent)
	{
		if (ent->d_name[0] == '.' && ent->d_name[1] == 0) continue;
		if (ent->d_name[0] == '.' && ent->d_name[1] == '.' && ent->d_name[2] == 0) continue;

		//LogMsg("Got %s. type %d", ent->d_name, int(ent->d_type));
		
		if (ent->d_type == DT_DIR)
		{
			v.push_back(ent->d_name);
		}
	}

	free (buf);
	closedir(dp);
	return v;
}

vector<string> GetFilesAtPath(string path)
{
#ifdef _DEBUG
	//LogMsg("GetFilesAtPath: %s", path.c_str());
#endif

	vector<string> v;
	dirent * buf, * ent;
	DIR *dp;

	dp = opendir(path.c_str());
	if (!dp)
	{
		LogError("GetDirectoriesAtPath: opendir failed");
		return v;
	}

	buf = (dirent*) malloc(sizeof(dirent)+512);
	while (readdir_r(dp, buf, &ent) == 0 && ent)
	{
		if (ent->d_name[0] == '.' && ent->d_name[1] == 0) continue;
		if (ent->d_name[0] == '.' && ent->d_name[1] == '.' && ent->d_name[2] == 0) continue;

		//LogMsg("Got %s. type %d", ent->d_name, int(ent->d_type));
		if (ent->d_type == DT_REG) //regular file
		{
			v.push_back(ent->d_name);
		}
	}

	free (buf);
	closedir(dp);
	return v;
}

string GetRegionString()
{
	uint32_t lang = CONF_GetLanguage();
	
	switch (lang)
	{
		case CONF_LANG_ENGLISH: {
			// Resolve Ambiguity
			if (CONF_GetRegion() == CONF_REGION_US)
				return "en_US";
			return "en_UK";
		}
		
		case CONF_LANG_JAPANESE:     return "ja_JP";
		case CONF_LANG_GERMAN:       return "de_DE";
		case CONF_LANG_FRENCH:       return "fr_FR";
		case CONF_LANG_SPANISH:      return "es_ES";
		case CONF_LANG_ITALIAN:      return "it_IT";
		case CONF_LANG_DUTCH:        return "nl_NL";
		case CONF_LANG_SIMP_CHINESE: return "zh_CN";
		case CONF_LANG_TRAD_CHINESE: return "zh_TW";
		case CONF_LANG_KOREAN:       return "ko_KR";
		default:                     return "en_US";
	}
}

bool IsIphone()
{
    return false;
}

bool IsIPAD()
{
    return false;
}

bool IsIphoneOriPad()
{
    return false;
}

bool IsIPhone3GS()
{
    return false;
}

bool IsIphone4()
{
    return false;
}

int GetSystemData()
{
    return C_PIRATED_NO;
}

eNetworkType IsNetReachable(std::string url)
{
    return C_NETWORK_NONE; // TODO
}

eDeviceMemoryClass GetDeviceMemoryClass()
{
    return C_DEVICE_MEMORY_CLASS_1;   
}

bool IsIPodTouchThirdGen()
{
    return false;
}

float GetDeviceOSVersion()
{
    return 0.0f; // unimplemented
}

std::string GetMacAddress()
{
    return ""; // TODO
}

bool IsDesktop()
{
    if (GetEmulatedPlatformID() == PLATFORM_ID_WINDOWS)
        return true;
    return false;
}

bool HasVibration()
{
	return false; // TODO
}

std::string GetDeviceID()
{
	return "Ya ain't seein' that!";
}

void SystemSleep(int sleepMS)
{
	usleep(sleepMS * 1000);
}

string GetDateAndTimeAsString()
{
	time_t ltime;
	time( &ltime );

	tm today = *localtime( &ltime );

	char stTemp[128];
	sprintf(stTemp, "%d/%d %d:%d:%d", today.tm_mday, today.tm_mon+1, today.tm_hour, today.tm_min, today.tm_sec);
	return string(stTemp);
}

void GetDateAndTime(int *monthOut, int *dayOut, int *yearOut, int *hourOut, int *minOut, int *secOut)
{
	time_t ltime;
	time( &ltime );
	tm today = *localtime( &ltime );

	*monthOut = today.tm_mon+1;
	*dayOut = today.tm_mday;
	*yearOut = today.tm_year+1900;
	*hourOut = today.tm_hour;
	*minOut = today.tm_min;
	*secOut = today.tm_sec;
}

void NotifyOSOfOrientationPreference(eOrientationMode orientation) {}

std::string GetNetworkType()
{
	// TODO
	return "none";
}

bool IsStillLoadingPersistentData()
{
	return false;
}
bool IsStillSavingPersistentData()
{
	return false;
}

void SyncPersistentData() {}

std::string GetClipboardText()
{
    return "";
}