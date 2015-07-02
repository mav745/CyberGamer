#ifndef CMD_HPP
#define CMD_HPP

#include "xash_types.hpp"

class CHost;

class CCmd
{
public:
	class CStuffCmd: public CCmdCode { public: void exec(const QStringList &args); };
	class CWaitCmd : public CCmdCode { public: void exec(const QStringList &args); };
	class CEchoCmd : public CCmdCode { public: void exec(const QStringList &args); };
	class CAliasCmd: public CCmdCode { public: void exec(const QStringList &args); };
	class CListCmd : public CCmdCode { public: void exec(const QStringList &args); };
	class CNullCmd : public CCmdCode { public: void exec(const QStringList &args); };
	class CCmdCmd  : public CCmdCode {
		CCmd *cmd;
	public:
		CCmdCmd(CCmd *_cmd): cmd(_cmd) { }
		void exec(const QStringList &args);
	};

	CCmd();


	void cbuf_clear();
	void cbuf_addText(const QString &text );
	void cbuf_insertText(const QString &text );
	void cbuf_execute();


	void stuffCmds_f();
	void wait_f();
	void echo_f();
	void alias_f();
	void list_f();
	void null_f();

//	uint argc();
//	char *argv( int arg );
//	char *args();
	QList<SCmdAlias> aliasGetList();
//	SCmd *getFirstFunctionHandle();
//	SCmd *getNextFunctionHandle( SCmd *cmd );
	QString getName( SCmd *cmd );
	QStringList tokenizeString(const QString &text );
	void addCommand(const QString &cmdName, CCmdCode &cmdCode, const QString &cmdDesc );
	void addGameCommand(const QString &cmdName, CCmdCode &cmdCode );
	void addClientCommand( const QString &cmdName, CCmdCode &cmdCode );
	void removeCommand( const QString &cmdName );
	void lookupCmds( char *buffer, void *ptr, setpair_t callback );
	bool exists( const QString &cmdName );

	void executeString( QString &text, ECmdSource src );

	void forwardToServer();

	void unlink( int group );


private:
	CHost *host;

	bool wait;
	//QString cmd_text;
	QStringList queue;
	QList<SCmdAlias> aliases;

	//QString cmd_tokenized; // will have 0 bytes inserted
	QList<SCmd> commands; // possible commands to execute
	//ECmdSource CmdSource;
};

#endif // CMD_HPP
