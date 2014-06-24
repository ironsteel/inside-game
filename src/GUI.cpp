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
#include "GUI.h"
#include "ShellFileInterface.h"
#include "ShellSystemInterface.h"
#include "RenderInterfaceOpenGLES.h"
#include <Rocket/Debugger/Debugger.h>

void GUI::init(const std::string &baseResourcePath)
{
	mRootResourcesDir = baseResourcePath;
	mRocketSystemInterface = new ShellSystemInterface();
	Rocket::Core::SetSystemInterface(mRocketSystemInterface);

	// Rocket initialisation.
	mRocketFileInterface = new ShellFileInterface(baseResourcePath.c_str());
	Rocket::Core::SetFileInterface(mRocketFileInterface);


	mRocketGLESRenderer = new RenderInterfaceOpenGLES();
	Rocket::Core::SetRenderInterface(mRocketGLESRenderer);


	Rocket::Core::Initialise();

	loadFonts(baseResourcePath + "/fonts/");

	mRocketContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(0, 0));
	if (mRocketContext == NULL)
	{
		Rocket::Core::Shutdown();

	}

	Rocket::Debugger::Initialise(mRocketContext);

}

GUI::~GUI()
{
	// Shutdown Rocket.
	mRocketContext->RemoveReference();
	Rocket::Core::Shutdown();
}

void GUI::updateGUI()
{
	glDisable(GL_DEPTH_TEST);

	mRocketContext->Update();
	mRocketContext->Render();
}

void GUI::setViewPortSize(int width, int height)
{
	if(mRocketGLESRenderer) {
		mRocketGLESRenderer->SetViewport(width, height);
		mRocketContext->SetDimensions(Rocket::Core::Vector2i(width, height));
	}
}

bool GUI::processMouseDown(int button, int flags)
{
	if (mRocketContext->GetHoverElement() != mRocketContext->GetRootElement()) {
		mRocketContext->ProcessMouseButtonDown(button, 0);
		return true;
	}
	return false;
}

bool GUI::processMouseUp(int button, int flags)
{
	if (mRocketContext->GetHoverElement() != mRocketContext->GetRootElement()) {
		mRocketContext->ProcessMouseButtonUp(button, 0);
		return true;
	}
	return false;
}

void GUI::processMouseMove(double x, double y)
{
	mRocketContext->ProcessMouseMove((int) x,(int) y, 0);
}


Rocket::Core::Context* GUI::getContext()
{
	return mRocketContext;
}

void GUI::loadFonts(const std::string &fontDir)
{

	Rocket::Core::String font_names[4];
	font_names[0] = "Delicious-Roman.otf";
	font_names[1] = "Delicious-Italic.otf";
	font_names[2] = "Delicious-Bold.otf";
	font_names[3] = "Delicious-BoldItalic.otf";

	for (int i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++)
	{
		Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String(fontDir.c_str()) + font_names[i]);
	}
}
