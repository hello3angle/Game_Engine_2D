#include "Editor.h"
#include "resource.h"
#include "Player.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_syswm.h>

Editor::Editor(const std::wstring& window_name)
{
	_window = FindWindow(nullptr, window_name.c_str());
	if (_window == nullptr)
	{
		std::wcout << "Failed to find window. Window Name: " << window_name.c_str() << std::endl;
		exit(1);
	}

	_menu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
	if (_menu == nullptr)
	{
		std::cout << "Failed to create menu. " << std::endl;
		exit(1);
	}

	BOOL set_menu_result = SetMenu(_window, _menu);
	if (set_menu_result == 0)
	{
		std::cout << "Failed to set menu. " << std::endl;
		exit(1);
	}

	Uint8 event_state_result	= SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
	Uint8 event_state_success	= SDL_DISABLE;
	if (event_state_result != event_state_success)
	{
		std::cout << "Failed to enable Windows event state." << std::endl;
		exit(1);
	}
}

Editor::~Editor()
{
}

INT_PTR CALLBACK Dialog_Configuration_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		//int length = ::GetDlgItemTextA(hWnd, IDC_EDIT_TITLE, const_cast<char*>(tmp.c_str()), 255);
		
		// Update dialog items
		{
			std::string tmp = Configuration::get_instance()->get_title();
			::SetDlgItemTextA(hWnd, IDC_EDIT_TITLE, tmp.c_str());
		}
		{
			std::string tmp = Configuration::get_instance()->get_version();
			::SetDlgItemTextA(hWnd, IDC_EDIT_VERSION, tmp.c_str());
		}
		{
			std::string tmp = std::to_string(Configuration::get_instance()->get_window_position_x());
			::SetDlgItemTextA(hWnd, IDC_EDIT_WIN_POS_X, tmp.c_str());
		}
		{
			std::string tmp = std::to_string(Configuration::get_instance()->get_window_position_y());
			::SetDlgItemTextA(hWnd, IDC_EDIT_WIN_POS_Y, tmp.c_str());
		}
		{
			std::string tmp = std::to_string(Configuration::get_instance()->get_resolution_width());
			::SetDlgItemTextA(hWnd, IDC_EDIT_RESOLUTION_W, tmp.c_str());
		}
		{
			std::string tmp = std::to_string(Configuration::get_instance()->get_resolution_height());
			::SetDlgItemTextA(hWnd, IDC_EDIT_RESOLUTION_H, tmp.c_str());
		}
		{
			std::string tmp = std::to_string(Configuration::get_instance()->get_refresh_rate());
			::SetDlgItemTextA(hWnd, IDC_EDIT_REFRESH_RATE, tmp.c_str());
		}

		return true;
	}
		
		
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			// controls message
		case IDOK:
			EndDialog(hWnd, IDOK);
			break;
		}
		break;
	}
		

	default:
		return false;
	}

	return true;
}

void Editor::update(Input* input, Scene*)
{
	if (input->is_button_state(Input::Button::FILE_PAUSE, Input::Button_State::PRESSED))
	{
		Configuration::get_instance()->_should_pause = !Configuration::get_instance()->_should_pause;
	}

	if (input->is_button_state(Input::Button::DIALOG_CONFIGURATION, Input::Button_State::PRESSED))
	{
		DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG_CONFIGURATION), _window, Dialog_Configuration_Proc);
	}

	if (input->is_button_state(Input::Button::DISPLAY_GAME_OBJECTS_ID, Input::Button_State::PRESSED))
	{
		Configuration::get_instance()->_should_display_ids = !Configuration::get_instance()->_should_display_ids;
	}

	if (input->is_button_state(Input::Button::DISPLAY_GAME_OBJECTS_POSITION, Input::Button_State::PRESSED))
	{
		Configuration::get_instance()->_should_display_positions = !Configuration::get_instance()->_should_display_positions;
	}

	if (input->is_button_state(Input::Button::DISPLAY_COLLIDERS, Input::Button_State::PRESSED))
	{
		Configuration::get_instance()->_should_display_colliders = !Configuration::get_instance()->_should_display_colliders;
	}
}
