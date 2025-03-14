// Explorer.cpp

#include "Explorer.h"

// Global variables
FolderTreeViewWindow g_folderTreeViewWindow;
FileListViewWindow g_fileListViewWindow;
StatusBarWindow g_statusBarWindow;

int CALLBACK FileListViewWindowCompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	int nResult = 0;

	// Allocate string memory
	LPTSTR lpszItemText1 = new char[ STRING_LENGTH + sizeof( char ) ];
	LPTSTR lpszItemText2 = new char[ STRING_LENGTH + sizeof( char ) ];

	// Get first item text
	g_fileListViewWindow.GetItemText( lParam1, lParamSort, lpszItemText1 );

	// Get second item text
	g_fileListViewWindow.GetItemText( lParam2, lParamSort, lpszItemText2 );

	// Compare item texts
	nResult = lstrcmpi( lpszItemText1, lpszItemText2 );

	// See if the modified column is being sorted
	if( lParamSort == FILE_LIST_VIEW_WINDOW_CLASS_MODIFIED_COLUMN_ID )
	{
		// The modified column is being sorted

		// Invert result
		nResult = ( 0 - nResult );
		// To show most recent files first

	} // End of the modified column is being sorted

	// Free string memory
	delete [] lpszItemText1;
	delete [] lpszItemText2;

	return nResult;

} // End of function FileListViewWindowCompareFunction

void FileListViewWindowSelectionChangedFunction( LPCTSTR lpszItemPath )
{
	// Show item path on status bar window
	g_statusBarWindow.SetText( lpszItemPath );

} // End of function FileListViewWindowSelectionChangedFunction

void FileListViewWindowDoubleClickFunction( LPCTSTR lpszItemPath )
{
	// Open item
	if( ( INT_PTR )ShellExecute( NULL, SHELL_EXECUTE_OPEN_COMMAND, lpszItemPath, NULL, NULL, SW_SHOWDEFAULT ) <= SHELL_EXECUTE_MINIMUM_SUCCESS_RETURN_VALUE )
	{
		// Unable to open item

		// Allocate string memory
		LPTSTR lpszErrorMessage = new char[ STRING_LENGTH + sizeof( char ) ];

		// Format error message
		wsprintf( lpszErrorMessage, UNABLE_TO_OPEN_FILE_ERROR_MESSAGE_FORMAT_STRING, lpszItemPath );

		// Display error message
		MessageBox( NULL, lpszErrorMessage, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		// Free string memory
		delete [] lpszErrorMessage;

	} // End of unable to open item

} // End of function FileListViewWindowDoubleClickFunction

void FolderTreeViewWindowSelectionChangedFunction( LPTSTR lpszItemText )
{
	int nFileCount;

	// Allocate string memory
	LPTSTR lpszStatusMessage = new char[ STRING_LENGTH + sizeof( char ) ];

	// Populate file list view window
	nFileCount = g_fileListViewWindow.Populate( lpszItemText, ALL_FILES_FILTER );

	// Format status message
	wsprintf( lpszStatusMessage, FILE_LIST_VIEW_WINDOW_CLASS_ADD_FILES_STATUS_MESSAGE_FORMAT_STRING, lpszItemText, nFileCount );

	// Show status text on status bar window
	g_statusBarWindow.SetText( lpszStatusMessage );

	// Free string memory
	delete [] lpszStatusMessage;

} // End of function FolderTreeViewWindowSelectionChangedFunction

void FolderTreeViewWindowDoubleClickFunction( LPTSTR lpszItemText )
{
	// Display item text
	MessageBox( NULL, lpszItemText, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

} // End of function FolderTreeViewWindowDoubleClickFunction

void OpenFileFunction( LPCTSTR lpszFilePath )
{
	// Display file path
	MessageBox( NULL, lpszFilePath, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

} // End of function OpenFileFunction

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= NULL;
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= WINDOW_CLASS_CLASS_DEFAULT_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMessage )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Create folder tree view window
			if( g_folderTreeViewWindow.Create( hWndMain, hInstance ) )
			{
				// Successfully created folder tree view window
				Font font;

				// Get font
				font = DEFAULT_GUI_FONT;

				// Set folder tree view window font
				g_folderTreeViewWindow.SetFont( font );

				// Create file list view window
				if( g_fileListViewWindow.Create( hWndMain, hInstance ) )
				{
					// Successfully created file list view window

					// Set file list view window font
					g_fileListViewWindow.SetFont( font );

					// Create status bar window
					if( g_statusBarWindow.Create( hWndMain, hInstance ) )
					{
						// Successfully created status bar window

						// Set status bar window font
						g_statusBarWindow.SetFont( font );

					} // End of successfully created status bar window

				} // End of successfully created file list view window

			} // End of successfully created folder tree view window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatus;
			int nStatusWindowHeight;
			int nControlWindowHeight;
			int nFileListViewWindowWidth;
			int nFileListViewWindowLeft;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			g_statusBarWindow.Size();

			// Get status window size
			g_statusBarWindow.GetWindowRect( &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight			= ( rcStatus.bottom - rcStatus.top );
			nControlWindowHeight		= ( nClientHeight - nStatusWindowHeight );
			nFileListViewWindowWidth	= ( ( nClientWidth - FOLDER_TREE_VIEW_WINDOW_CLASS_WIDTH ) + WINDOW_BORDER_WIDTH );

			// Calculate window positions
			nFileListViewWindowLeft		= ( FOLDER_TREE_VIEW_WINDOW_CLASS_WIDTH - WINDOW_BORDER_WIDTH );

			// Move folder tree view window
			g_folderTreeViewWindow.Move( 0, 0, FOLDER_TREE_VIEW_WINDOW_CLASS_WIDTH, nControlWindowHeight, TRUE );
			g_fileListViewWindow.Move( nFileListViewWindowLeft, 0, nFileListViewWindowWidth, nControlWindowHeight, TRUE );

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on folder tree view window
			g_folderTreeViewWindow.SetFocus();

			// Break out of switch
			break;

		} // End of an activate message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
			lpMinMaxInfo->ptMinTrackSize.x = WINDOW_CLASS_DEFAULT_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = WINDOW_CLASS_DEFAULT_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_DROPFILES:
		{
			// A drop files message
			DroppedFiles droppedFiles;

			// Get dropped files
			if( droppedFiles.Get( wParam ) )
			{
				// Successfully got dropped files

				// Process dropped files
				droppedFiles.Process( &OpenFileFunction );

			} // End of successfully got dropped files

			// Break out of switch
			break;

		} // End of a drop files message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about system command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message
			LPNMHDR lpNmHdr;

			// Get notify message information
			lpNmHdr = ( LPNMHDR )lParam;

			// See if notify message is from folder tree view window
			if( lpNmHdr->hwndFrom == g_folderTreeViewWindow )
			{
				// Notify message is from folder tree view window

				// Handle notify message from folder tree view window
				if( !( g_folderTreeViewWindow.HandleNotifyMessage( wParam, lParam, &FolderTreeViewWindowSelectionChangedFunction ) ) )
				{
					// Notify message was not handled from folder tree view window

					// Call default handler
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

				} // End of notify message was not handled from folder tree view window

			} // End of notify message is from folder tree view window
			else if( lpNmHdr->hwndFrom == g_fileListViewWindow )
			{
				// Notify message is from file list view window

				// Handle notify message from file list view window
				if( !( g_fileListViewWindow.HandleNotifyMessage( wParam, lParam, &FileListViewWindowSelectionChangedFunction, &FileListViewWindowDoubleClickFunction, &FileListViewWindowCompareFunction ) ) )
				{
					// Notify message was not handled from file list view window

					// Call default handler
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

				} // End of notify message was not handled from file list view window

			} // End of notify message is from file list view window
			else
			{
				// Notify message is not from control window

				// Call default handler
				lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			} // End of notify message is not from control window

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			Menu popupMenu;

			// Load popup menu
			popupMenu.Load( MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show popup menu
			popupMenu.TrackPopupMenu( 0, lParam, hWndMain );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Save folder tree view window
			if( g_folderTreeViewWindow.Save( FOLDERS_FILE_NAME ) )
			{
				// Successfully saved folder tree view window

				// Destroy main window
				DestroyWindow( hWndMain );

			} // End of successfully saved folder tree view window
			else
			{
				// Unable to save folder tree view window

				// Ensure that user is ok to continue
				if( MessageBox( hWndMain, UNABLE_TO_SAVE_FOLDERS_WARNING_MESSAGE, WARNING_MESSAGE_CAPTION, ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2 ) ) == IDYES )
				{
					// User is ok to continue

					// Destroy main window
					DestroyWindow( hWndMain );

				} // End of user is ok to continue

			} // End of unable to save folder tree view window

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default window procedure
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	Message message;

	WindowClass mainWindowClass;

	// Initialise main window class
	mainWindowClass.Initialise( MAIN_WINDOW_CLASS_NAME, hInstance, MainWindowProcedure, MAIN_WINDOW_CLASS_MENU_NAME );

	// Register main window class
	if( mainWindowClass.Register() )
	{
		// Successfully registered main window class
		Window mainWindow;

		// Create main window
		if( mainWindow.Create( MAIN_WINDOW_CLASS_NAME, NULL, hInstance, MAIN_WINDOW_TEXT ) )
		{
			// Successfully created main window
			Menu systemMenu;
			ArgumentList argumentList;

			// Get system menu
			systemMenu.GetSystem( mainWindow );

			// Add separator item to system menu
			systemMenu.InsertSeparator( MENU_CLASS_SYSTEM_MENU_SEPARATOR_ITEM_POSITION );

			// Add about item to system menu
			systemMenu.InsertItem( MENU_CLASS_SYSTEM_MENU_ABOUT_ITEM_POSITION, IDM_HELP_ABOUT, MENU_CLASS_SYSTEM_MENU_ABOUT_ITEM_TEXT );

			// Get argument list
			if( argumentList.Get() )
			{
				// Successfully got argument list

				// Process arguments
				argumentList.ProcessArguments( &OpenFileFunction );

			} // End of successfully got argument list

			// Show main window
			mainWindow.Show( nCmdShow );

			// Update main window
			mainWindow.Update();

			// Load folder tree view window
			if( !( g_folderTreeViewWindow.Load( FOLDERS_FILE_NAME ) ) )
			{
				// Unable to load folder tree view window

				// Allocate strinng memory
				LPTSTR lpszFolderPath = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get current folder path
				if( GetCurrentDirectory( STRING_LENGTH, lpszFolderPath ) )
				{
					// Successfully got current folder path
					HTREEITEM htiFolder;

					// Add folder to folder tree view window
					htiFolder = g_folderTreeViewWindow.InsertItem( lpszFolderPath );

					// Ensure that folder was added to folder tree view window
					if( htiFolder )
					{
						// Successfully added folder to folder tree view window

						// Add dummy sub-item to folder tree view window
						if( g_folderTreeViewWindow.InsertItem( lpszFolderPath, htiFolder ) )
						{
							// Successfully added dummy sub-item to folder tree view window

							// Expand folder
							g_folderTreeViewWindow.ExpandItem( htiFolder );

							// Select folder
							g_folderTreeViewWindow.SelectItem( htiFolder );

						} // End of successfully added dummy sub-item to folder tree view window

					} // End of successfully added folder to folder tree view window

				} // End of successfully got current folder path

				// Free string memory
				delete [] lpszFolderPath;

			} // End of unable to load folder tree view window

			// Message loop
			while( message.Get() > 0 )
			{
				// Translate message
				message.Translate();

				// Dispatch message
				message.Dispatch();

			}; // End of message loop

		} // End of successfully created main window
		else
		{
			// Unable to create main window

			// Display error message
			MessageBox( NULL, WINDOW_CLASS_UNABLE_TO_CREATE_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to create main window

	} // End of successfully registered main window class
	else
	{
		// Unable to register main window class

		// Display error message
		MessageBox( NULL, WINDOW_CLASS_CLASS_UNABLE_TO_REGISTER_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to register main window class

	return message;

} // End of function WinMain