#ifndef CVAR_HPP
#define CVAR_HPP

#include "xash_types.hpp"

class CCvar
{
public:
	CCvar();

	QList<SConVar *> getList();
	QString getName( SCvar *cvar );
	bool validateString(const QString &s, bool isvalue );
	SConVar *findVar(const QString &var_name );
	float variableValue(const QString &var_name );
	int variableInteger(const QString &var_name );
	QString &variableString(const QString &var_name );
	void lookupVars( int checkbit, void *buffer, void *ptr, setpair_t callback );
	SConVar *get( const QString &var_name, const QString &var_value, int flags, const QString &var_desc );
	void registerVariable( SCvar *var );
	SConVar *set2( const QString &var_name, const QString &value, bool force );
	void set( const QString &var_name, const QString &value );
	void setLatched( const QString &var_name, const QString &value );
	void fullSet( const QString &var_name, const QString &value, int flags );
	void directSet( SCvar *var, const QString &value );
	void setFloat( const QString &var_name, float value );
	void reset( const QString &var_name );
	void setCheatState();
	bool command();
	void toggle_f();
	void set_f();
	void setU_f();
	void setP_f();
	void setS_f();
	void setA_f();
	void setR_f();
	void setGL_f();
	void reset_f();
	void list_f();
	void restart_f();
	void latched_f();
	void latchedVideo_f();
	void unlink_f();
	void unlink();

private:
	QList<SConVar*> cvar_vars; // head of list
	SConVar	*userinfo, *physinfo, *serverinfo, *renderinfo;
};

#endif // CVAR_HPP
