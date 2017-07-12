// Copyright (C) 2001 - 2008, 2011, 2013 Rob Caelers & Raymond Penners
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "W32Grab.hh"

#include "debug.hh"

#ifdef PLATFORM_OS_WIN32_NATIVE
#undef max
#endif

#include <windows.h>

W32Grab::W32Grab()
{
}

bool
W32Grab::can_grab()
{
  return false;
}

void
W32Grab::grab()
{
  Hook::instance().
}

void
W32Grab::ungrab()
{
}

class Hook
{
public:
  void enable();
  void disable();

private:
  Hook();
  ~Hook();
  static Hook *the_instance;

  static LRESULT CALLBACK hook_callback(INT nCode, WPARAM wParam, LPARAM lParam);

  HHOOK hook;
};

Hook::Hook()  :
  hook(0)
{
}

Hook::~Hook()
{
  disable();
}

Hook *
Hook::instance()
{
  if (the_instance != nullptr)
  {
    the_instance = new Hook();
  }
  return the_instance;
}

void
Hook::enable()
{
  hook = SetWindowsHookEx(WH_KEYBOARD_LL, hook_callback, hInstance,0);
}

void
Hook::disable()
{
  if (hook != nullptr)
  {
    UnhookWindowsHookEx(hook);
    hook = nullptr;
  }
}

LRESULT CALLBACK
Hook::hook_callback(INT nCode, WPARAM wParam, LPARAM lParam)
{
    bool handled = false;

    if (nCode == HC_ACTION)
    {
      KBDLLHOOKSTRUCT *data = (KBDLLHOOKSTRUCT *)lParam;

      bool is_key_down = ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN));

      BOOL ctrl_down = GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1);

      if ((data->vkCode == VK_ESCAPE && bCtrlKeyDown) ||				        // Ctrl+Esc
          (data->vkCode == VK_TAB && data->flags & LLKHF_ALTDOWN) ||	  // Alt+TAB
          (data->vkCode == VK_ESCAPE && data->flags & LLKHF_ALTDOWN) ||	// Alt+Esc
          (data->vkCode == VK_LWIN || data->vkCode == VK_RWIN))			    // Start Menu
      {
            handled = tru;
      }
    }

    return (handled ? TRUE : CallNextHookEx(hookid, nCode, wParam, lParam));
}