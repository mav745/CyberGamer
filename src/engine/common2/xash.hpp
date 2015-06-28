#ifndef XASH_HPP
#define XASH_HPP

#include "xash_types.hpp"

class CConsole;
class CCmd;
class CCvar;

class XASH_EXP CHost
{
public:
	void changeGame(const QString &progname);

	int  main(const QString &progname, int bChangeGame);
	void shutdown();
	void setServerState(int state);
	int serverState();
	void newInstance(const QString &name, const QString &finalmsg);

	class CChangeGameCmd: public CCmdCode { public: void exec(const QStringList &args); };
	class CExecCmd      : public CCmdCode { public: void exec(const QStringList &args); };
	class CErrorCmd     : public CCmdCode { public: void exec(const QStringList &args); };
	class CNetErrorCmd  : public CCmdCode { public: void exec(const QStringList &args); };
	class CCrashCmd     : public CCmdCode { public: void exec(const QStringList &args); };
	class CMemStatsCmd  : public CCmdCode { public: void exec(const QStringList &args); };
	class CMinimizeCmd  : public CCmdCode { public: void exec(const QStringList &args); };

//	void changeGame_f();
//	void exec_f();
//	void error_f();
//	void crash_f();
//	void memStats_f();
//	void minimize_f();

	void initCommon(const QString &progname, bool bChangeGame);
	void freeCommon();

	int compareFileTime(long ft1, long ft2);
	bool newGame(const QString &mapName, bool loadGame);
	void endGame(const QString &message, ...);
	void abortCurrentFrame();
	void restartAmbientSounds();
	void restartDecals();
	bool registerDecal(const QString &name);
	bool changeGame(const QString &gamefolder, bool bReset);
	void writeServerConfig(const QString &name);
	void writeOpenGLConfig();
	void writeVideoConfig();
	void writeConfig();
	bool isLocalGame();
	void shutdownServer();
	void getConsoleCommands();
	bool filterTime(float time);
	void frame(float time);
	void print(const QString &txt);
	void error(const QString &error, ...);

	void printEngineFeatures();
	void initDecals();
	void credits();

	void inputFrame();
	void serverFrame();
	void clientFrame(double frametime);


	double sys_DoubleTime();
	QString sys_GetClipboardData();
	void sys_SetClipboardData(const QString &text);

	static QString currentUser();

	int sys_CheckParm(const QString &parm);
	bool _sys_GetParmFromCmdLine(QString &parm, QString &out, size_t size);
	void sys_SendKeyEvents();
	bool sys_LoadLibrary(SDllInfo *dll);
	void* sys_GetProcAddress(SDllInfo *dll, const QString &name);
	bool sys_FreeLibrary(SDllInfo *dll);
	void sys_WaitForQuit();
	void sys_Error(const QString &error, ...);
	void sys_Break(const QString &error, ...);
	void sys_Quit();
	void sys_Print(const QString pMsg);
	void msg(const QString &pMsg, ...);
	void msgDev(int level, const QString &pMsg, ...);

private:
	CConsole *console;
	CCmd *cmd;
	CCvar *cvar;

	SHostParams host; // host parms
	SSysInfo SI;

	bool error_on_exit;	// arg for exit();

	SConVar *host_serverstate;
	SConVar *host_gameloaded;
	SConVar *host_clientloaded;
	SConVar *host_limitlocal;
	SConVar *host_cheats;
	SConVar *host_maxfps;
	SConVar *host_framerate;
	SConVar *con_gamemaps;
	SConVar *build, *ver;

	int num_decals;

	double oldtime, newtime;
};

#endif // XASH_HPP
