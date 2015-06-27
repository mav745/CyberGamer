#ifndef CMD_HPP
#define CMD_HPP

#include "xash_types.hpp"

class CCmd
{
public:
	CCmd();

	void cbuf_init();
	void cbuf_clear();
	void *cbuf_getSpace( SCmdBuf *buf, int length );
	void cbuf_addText(const QString &text );
	void cbuf_insertText(const QString &text );
	void cbuf_execute();
	void stuffCmds_f();
	void wait_f();
	void echo_f();
	void alias_f();

	uint argc();
	char *argv( int arg );
	char *args();
	SCmdAlias *aliasGetList();
	SCmd *getFirstFunctionHandle();
	SCmd *getNextFunctionHandle( struct SCmd *cmd );
	QString getName( SCmd *cmd );
	void tokenizeString(QString text );
	void addCommand(const QString &cmd_name, xcommand_t function, const QString &cmd_desc );
	void addGameCommand(const QString &cmd_name, xcommand_t function );
	void addClientCommand( const QString &cmd_name, xcommand_t function );
	void removeCommand( const QString &cmd_name );
	void lookupCmds( char *buffer, void *ptr, setpair_t callback );
	bool exists( const QString &cmd_name );
	void executeString( QString &text, ECmdSource src );
	void forwardToServer();
	void list_f();
	void unlink( int group );
	void null_f();

private:
	bool cmd_wait;
	QString cmd_text;
	QString cmd_text_buf;
	SCmdAlias *cmd_alias;

	int cmd_argc;
	QString cmd_args;
	QStringList cmd_argv;
	QString cmd_tokenized; // will have 0 bytes inserted
	QList<SCmd> cmd_functions; // possible commands to execute
	ECmdSource SCmdource;
};

#endif // CMD_HPP
