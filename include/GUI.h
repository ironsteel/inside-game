/*
 * Inside Game
 * Copyright (C) 2014  <Rangel Ivanov> <rangelivanov88[at]gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef GUI_H
#define GUI_H

#include "Singleton.h"
#include <Rocket/Core.h>

class RenderInterfaceOpenGLES;

class GUI : public Singleton<GUI>
{
public:
	void init(const std::string& baseResourcePath);

	void updateGUI();

	void setViewPortSize(int width, int height);

	bool processMouseDown(int button, int flags);
	bool processMouseUp(int button, int flags);
	void processMouseMove(double x, double y);

	Rocket::Core::Context* getContext();

	virtual ~GUI();

private:
	void loadFonts(const std::string& fontDir);


private:

	std::string mRootResourcesDir;

	Rocket::Core::FileInterface* mRocketFileInterface;
	Rocket::Core::SystemInterface* mRocketSystemInterface;
	Rocket::Core::Context* mRocketContext;
	RenderInterfaceOpenGLES *mRocketGLESRenderer;

};

#endif // GUI_H
