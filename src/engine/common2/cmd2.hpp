#ifndef CMD_HPP
#define CMD_HPP

#include "xash_types.hpp"

class CCmd
{
public:
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
	SCmdAlias *aliasGetList();
	SCmd *getFirstFunctionHandle();
	SCmd *getNextFunctionHandle( struct SCmd *cmd );
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
	bool cmd_wait;
	QString cmd_text;
	QString cmd_text_buf;
	SCmdAlias *cmd_alias;

	QString cmd_tokenized; // will have 0 bytes inserted
	QList<SCmd> cmd_functions; // possible commands to execute
	ECmdSource SCmdource;
};

#endif // CMD_HPP
