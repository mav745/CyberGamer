#ifndef GAME_HPP
#define GAME_HPP

#include <QObject>
#include <QBasicTimer>


class Game: public QObject
{
	Q_OBJECT

public:
	Game();
	~Game();

private:
	QBasicTimer theTimer;
	void timerEvent(QTimerEvent *te); // game frame
};


#endif // GAME_HPP

