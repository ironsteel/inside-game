#ifndef APP_STATE_H
#define APP_STATE_H

#include <string>
#include <vector>

class AppState;

class AppStateListener
{
public:
	AppStateListener() {};
	virtual ~AppStateListener() {};

	virtual void manageAppState(std::string stateName, AppState* state) = 0;
	virtual AppState* findByName(std::string stateName) = 0;

	virtual void changeAppState(AppState *state) = 0;
	virtual bool pushAppState(AppState* state) = 0;
	virtual void popAppState() = 0;
	virtual void pauseAppState() = 0;
	virtual void shutdown() = 0;
	virtual void popAllAndPushAppState(AppState* state) = 0;
};

class AppState
{
public:
	static void create(AppStateListener* parent, const std::string name) {}

	virtual ~AppState() {}

	void destroy() {
		delete this;
	}

	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual bool pause() {
		return true;
	}
	virtual void resume() {}
	virtual void update(double timeSinceLastFrame) {}
	virtual void draw(double timeSinceLastFrame) {}
	virtual void reshape(int width, int height) {}
	
	
	virtual void onKeyPressed(int key) {}
	
	virtual void onPointerDown(int button, double x, double y) {}
	virtual void onPointerUp(int button, double cursorX, double cursorY) {}
	
	virtual void onPointerMoved(double x, double y) {}
	

protected:
	AppState() {};


	AppState* findByName(std::string stateName) {
		return m_pParent->findByName(stateName);
	}

	void changeAppState(AppState* state) {
		m_pParent->changeAppState(state);
	}
	bool pushAppState(AppState* state) {
		return m_pParent->pushAppState(state);
	}
	void popAppState() {
		m_pParent->popAppState();
	}
	void shutdown() {
		m_pParent->shutdown();
	}
	void popAllAndPushAppState(AppState* state) {
		m_pParent->popAllAndPushAppState(state);
	}

	AppStateListener* m_pParent;
};

// Define makro, the backslashes are important for the compiler!
#define DECLARE_APPSTATE_CLASS(T) \
static void create(AppStateListener* parent, const std::string name) \
{ \
  T* myAppState = new T(); \
  myAppState->m_pParent = parent; \
  parent->manageAppState(name, myAppState); \
}

#endif // APP_STATE_H
