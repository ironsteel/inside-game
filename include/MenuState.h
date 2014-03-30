#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "AppState.h"
#include <Rocket/Core.h>

using namespace Rocket::Core;

class MenuState : public AppState, public EventListener
{
public:

	MenuState();
	virtual ~MenuState();
	DECLARE_APPSTATE_CLASS(MenuState)

	void enter();
	void exit();
	bool pause();
	void resume();

	virtual void draw(double timeSinceLastFrame);
	virtual void reshape(int width, int height);


	virtual void ProcessEvent(Event &event);

	Rocket::Core::ElementDocument* mMenuStateGui;

};

#endif // MENUSTATE_H
