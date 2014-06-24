#include "MenuState.h"
#include "GUI.h"
#include <GLES2/gl2.h>
#include <iostream>

using namespace std;

MenuState::MenuState()
{
	mMenuStateGui = GUI::getInstance().getContext()->LoadDocument("../resources/layouts/main_menu.rml");
	mMenuStateGui->GetElementById("start")->AddEventListener("click", this);
	mMenuStateGui->GetElementById("exit")->AddEventListener("click", this);
}

MenuState::~MenuState()
{
	if (mMenuStateGui != NULL) {
		mMenuStateGui->RemoveReference();
	}
}

void MenuState::enter()
{
	mMenuStateGui->Show();
}

void MenuState::exit()
{
	mMenuStateGui->Hide();
}

bool MenuState::pause()
{
	mMenuStateGui->Hide();
	return true;
}

void MenuState::resume()
{
	mMenuStateGui->Show();
}

void MenuState::draw(double timeSinceLastFrame)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4, 0.4, 0.4, 1);
}

void MenuState::reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void MenuState::ProcessEvent(Event &event)
{
	if(event.GetCurrentElement()->GetId() == "exit") {
		m_pParent->shutdown();
	} else {
		pushAppState(m_pParent->findByName("GameState"));
	}
}
