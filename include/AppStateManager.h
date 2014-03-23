#ifndef APP_STATE_MANAGER_H
#define APP_STATE_MANAGER_H

#include "AppState.h"

class AppStateManager : public AppStateListener
{
public:
	typedef struct
	{
		std::string name;
		AppState* state;
	} state_info;

	AppStateManager();
	~AppStateManager();

	void manageAppState(std::string stateName, AppState* state);

	AppState* findByName(std::string stateName);

	void start(AppState* state);
	void changeAppState(AppState* state);
	bool pushAppState(AppState* state);
	void popAppState();
	void pauseAppState();
	void shutdown();
	void popAllAndPushAppState(AppState* state);

protected:
	void init(AppState *state);

	std::vector<AppState*> m_ActiveStateStack;
	std::vector<state_info> m_States;
	bool m_bShutdown;
};

#endif // APP_STATE_MANAGER_H
