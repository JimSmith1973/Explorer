// Explorer.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Resource.h"

#include "..\Common\Ascii.h"
#include "..\Common\Common.h"

#include "..\Classes\ArgumentListClass.h"
#include "..\Classes\DroppedFilesClass.h"
#include "..\Classes\FileListViewWindowClass.h"
#include "..\Classes\FolderTreeViewWindowClass.h"
#include "..\Classes\FontClass.h"
#include "..\Classes\MessageClass.h"
#include "..\Classes\MenuClass.h"
#include "..\Classes\StatusBarWindowClass.h"
#include "..\Classes\WindowClass.h"
#include "..\Classes\WindowClassClass.h"

#define MAIN_WINDOW_CLASS_NAME													"Main Explorer Window Class"

#define MAIN_WINDOW_CLASS_MENU_NAME												MAKEINTRESOURCE( IDR_MAIN_MENU )

#define MAIN_WINDOW_TEXT														"Explorer"

#define FOLDER_TREE_VIEW_WINDOW_CLASS_WIDTH										300

#define FOLDERS_FILE_NAME														"Folders.txt"

#define ABOUT_MESSAGE_TEXT														"Explorer\r\n"						\
																				"\r\n"								\
																				"Written by Jim Smith\r\n"			\
																				"\r\n"								\
																				"February 2025"

#define UNABLE_TO_SAVE_FOLDERS_WARNING_MESSAGE									"Unable to Save Folders.\r\n\r\nDo You Want to Exit?"
