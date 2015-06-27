#ifndef CONSOLE2_HPP
#define CONSOLE2_HPP

#include "xash_types.hpp"

//#include "common.h"
//#include "client.h"
//#include "keydefs.h"
//#include "protocol.h"		// get the protocol version
//#include "con_nprint.h"
//#include "gl_local.h"
//#include "qfont.h"

class CConsole
{
public:
	CConsole();

	void charEvent(SField *edit, int ch);
	void clear_f();
	void setColor_f();
	void clearNotify();
	void clearField(SField *edit);
	void clearTyping();
	int stringLength(const char *string);
	void messageMode_f();
	void messageMode2_f();
	void toggleConsole_f();
	void checkResize();
	void pageUp(int lines);
	void pageDown(int lines);
	void top();
	void bottom();
	bool visible();
	//void loadConsoleFont(int fontNumber, cl_font_t *font);
	void loadConchars();
	int drawGenericChar(int x, int y, int number, quint8 color[4]);
	void setFont(int fontNum);
	void restoreFont();
	int drawCharacter(int x, int y, int number, quint8 color[4]);
	void drawCharacterLen(int number, int *width, int *height);
	void drawStringLen(const char *pText, int *length, int *height);
	int drawGenericString(int x, int y, const char *string, quint8 setColor[4], bool forceColor, int hideChar);
	int drawString(int x, int y, const char *string, quint8 setColor[4]);

	void linefeed();
	void print(const char *txt);
	void nPrintf(int idx, char *fmt, ...);
	void nxPrintf(SConNprint *info, char *fmt, ...);
	void ui_NPrintf(int idx, char *fmt, ...);
	void ui_NXPrintf(SConNprint *info, char *fmt, ...);
	bool checkName(const char *name);
	void addCommandToList(const char *s, const char *unused1, const char *unused2, void *unused3);
	int sortCmds(const char **arg1, const char **arg2);
	void printMatches(const char *s, const char *unused1, const char *m, void *unused2);
	void concatRemaining(const char *src, const char *start);
	void completeCommand(SField *field);
	void field_Paste(SField *edit);
	void field_KeyDownEvent(SField *edit, int key);
	void field_CharEvent(SField *edit, int ch);
	void field_DrawInputLine(int x, int y, SField *edit);
	void key_Console(int key);
	void key_Message(int key);
	void drawInput();
	int drawDebugLines();
	void drawDebug();
	void drawNotify();
	void drawSolidConsole(float frac);
	void drawConsole();
	void drawVersion();
	void runConsole();
	void charEvent(int key);
	void vidInit();
	void invalidateFonts();
	void autoComplete(char *complete_string);
	void close();
	void defaultColor(int r, int g, int b);

private:
	SConsole con;
	SConVar	*con_notifytime;
	SConVar	*scr_conspeed;
	SConVar	*con_fontsize;

	static quint8 g_color_table[8][4];
};

#endif // CONSOLE2_HPP

