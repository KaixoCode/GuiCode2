#pragma once
#include "GuiCode2/pch.hpp"

namespace GuiCode
{

	class Clipboard
	{
	public:
		static inline Clipboard& Get()
		{
			static Clipboard instance;
			return instance;
		}

		void Copy(const std::string& data)
		{
			if (!OpenClipboard(nullptr))
				return;

			EmptyClipboard();

			HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE,
				(data.size() + 1) * sizeof(TCHAR));
			if (hglbCopy == NULL)
			{
				CloseClipboard();
				return;
			}

			LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hglbCopy);
			if (!lptstrCopy)
			{
				CloseClipboard();
				return;
			}
			memcpy(lptstrCopy, data.data(),
				data.size() * sizeof(TCHAR));
			lptstrCopy[data.size()] = (TCHAR)0; // null character 
			GlobalUnlock(hglbCopy);

			// Place the handle on the clipboard. 
			SetClipboardData(CF_TEXT, hglbCopy);
			CloseClipboard();
		}

		std::string Paste()
		{

			if (!IsClipboardFormatAvailable(CF_TEXT))
				return "";
			if (!OpenClipboard(nullptr))
				return "";
			std::string text = "";
			HGLOBAL hglb = GetClipboardData(CF_TEXT);
			if (hglb != NULL)
			{

				char* lptstr = static_cast<char*>(GlobalLock(hglb));
				if (lptstr != NULL)
				{
					// Call the application-defined ReplaceSelection 
					// function to insert the text and repaint the 
					// window. 
					text = lptstr;
					GlobalUnlock(hglb);
				}
			}
			CloseClipboard();
			return text;
		}
	};
}