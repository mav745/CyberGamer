#ifndef XASH_HPP
#define XASH_HPP

#include "xash_types.hpp"

class CConsole;
class CCmd;
class CCvar;

class XASH_EXP CHost
{
public:
	static void changeGame(const char *progname);

	int  main(const char *progname, int bChangeGame);
	void shutdown();
	void setServerState(int state);
	int serverState();
	static void newInstance(const char *name, const char *finalmsg);
	static void changeGame_f();
	static void exec_f();
	static void error_f();
	static void crash_f();
	static void memStats_f();
	static void minimize_f();

	void initCommon(const QString &progname, bool bChangeGame);
	void freeCommon();

	int compareFileTime(long ft1, long ft2);
	bool newGame(const char *mapName, bool loadGame);
	void endGame(const char *message, ...);
	static void abortCurrentFrame();
	void restartAmbientSounds();
	void restartDecals();
	bool registerDecal(const char *name);
	bool changeGame(const char *gamefolder, bool bReset);
	void writeServerConfig(const char *name);
	void writeOpenGLConfig();
	void writeVideoConfig();
	void writeConfig();
	bool isLocalGame();
	void shutdownServer();
	void getConsoleCommands(void);
	bool filterTime(float time);
	void frame(float time);
	void print(const char *txt);
	static void error(const char *error, ...);

	void printEngineFeatures();
	void initDecals();
	void credits();

	void inputFrame();
	void serverFrame();
	void clientFrame(double frametime);


	double sys_DoubleTime();
	char *sys_GetClipboardData();
	void sys_SetClipboardData(const quint8 *buffer, size_t size);
	void sys_Sleep(int msec);
	char *sys_GetCurrentUser();
	void sys_ShellExecute(const char *path, const char *parms, bool exit);
	int sys_CheckParm(const QString &parm);
	bool _sys_GetParmFromCmdLine(char *parm, char *out, size_t size);
	void sys_SendKeyEvents();
	bool sys_LoadLibrary(SDllInfo *dll);
	void* sys_GetProcAddress(SDllInfo *dll, const char* name);
	bool sys_FreeLibrary(SDllInfo *dll);
	void sys_WaitForQuit();
	void sys_Error(const char *error, ...);
	void sys_Break(const char *error, ...);
	void sys_Quit();
	void sys_Print(const char *pMsg);
	void msg(const char *pMsg, ...);
	void msgDev(int level, const char *pMsg, ...);

private:
	CConsole *console;
	CCmd *cmd;
	CCvar *cvar;

	SHostParams host; // host parms
	SSysInfo SI;

	bool error_on_exit;	// arg for exit();

	SConVar	*host_serverstate;
	SConVar	*host_gameloaded;
	SConVar	*host_clientloaded;
	SConVar	*host_limitlocal;
	SConVar	*host_cheats;
	SConVar	*host_maxfps;
	SConVar	*host_framerate;
	SConVar	*con_gamemaps;
	SConVar	*build, *ver;

	int num_decals;

	double	oldtime, newtime;
};

#endif // XASH_HPP
