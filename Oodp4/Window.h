#ifndef WINDOW_H
#define WINDOW_H
/////////////////////////////////////////////////////////////////////
// Window.h - Server Part of Project 4                        //
//  Akshay,
//  708,S beech Street #1 ,Syracuse .
//  Akshay@syr.edu
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   CSE687 Pr1, Spring 2017
//	Reference: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015  //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
GUI FOR CLIENT1 , enqueues messages to be sent to native part

Public Interface:
=================
ref class window - Managed c++ window (GUI)
-many buttons , handlers etc to control GUI
-tosystemstring()-convert to systemstring
-tostdstring() - convert to std string
-settextblockproperties() - sets GUI properties , brushes etc
-setbuttonproperties() - sets GUI properties , brushes etc
Build Process:
==============
* Required Files:
Window.cpp , window.h, mockchannel.h , blockingqueue.h 
Build commands
- devenv oodp4.sln


Maintenance History:
====================
*  Maintenance History:
*  --------------------
*  ver 3.0 : 22 Apr 2016
*  - added support for multi selection of Listbox items.  Implemented by
*    Saurabh Patel.  Thanks Saurabh.
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
**/


using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>

namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		// MockChannel references

		ISendr* pSendr_;
		IRecvr* pRecvr_;
		IMockChannel* pChann_;

		// Controls for Window

		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hSendMessageTab = gcnew TabItem();
		TabItem^ hFileListTab = gcnew TabItem();
		TabItem^ hConnectTab = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();
		
		// Controls for Upload View
		Button^ IISpbtn = gcnew Button();
		Grid^ hSendMessageGrid = gcnew Grid();
		Label^ input1 = gcnew Label();
		Button^ hSendButton = gcnew Button();
		ListBox^ uploadLB = gcnew ListBox();
		Button^ hClearButton = gcnew Button();
		TextBlock^ hTextBlock1 = gcnew TextBlock();
		ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ hStackPanel1 = gcnew StackPanel();
		StackPanel^ hStackPanel2 = gcnew StackPanel();
		TextBox^ InputTextBox = gcnew TextBox();
		RadioButton^ cat1 = gcnew RadioButton();
		RadioButton^ cat2 = gcnew RadioButton();
		RadioButton^ cat3 = gcnew RadioButton();
		TextBlock^ label = gcnew TextBlock();
		RadioButton^ defcat = gcnew RadioButton();
		String^ msgText
			= "Command:ShowMessage\n"   // command
			+ "Sendr:localhost@8080\n"  // send address
			+ "Recvr:localhost@8090\n"  // receive address
			+ "Content-length:44\n"     // body length attribute
			+ "\n"                      // end header
			+ "Hello World\nCSE687 - Object Oriented Design";  // message body
								   // Controls for Download View
		Grid^ hFileListGrid = gcnew Grid();
		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		ListBox^ hListBox = gcnew ListBox();
		Button^ hFolderBrowseButton = gcnew Button();
		Button^ hShowItemsButton = gcnew Button();
		Button^ IndependentButton = gcnew Button();
		StackPanel^ downloadstck = gcnew StackPanel();
		Grid^ hGrid2 = gcnew Grid();
		Button^ del = gcnew Button();
		//Controls for PublishTabView
		Button^ PublishButton = gcnew Button();
		RadioButton^ cat1y = gcnew RadioButton();
		RadioButton^ cat2y= gcnew RadioButton();
		RadioButton^ cat3y = gcnew RadioButton();
		Button^ LazyDownloadBttn = gcnew Button();
		RadioButton^ cat1x = gcnew RadioButton();
		RadioButton^ cat2x = gcnew RadioButton();
		RadioButton^ cat3x = gcnew RadioButton();
		TextBlock^ publishTextblock = gcnew TextBlock();
		StackPanel^ PTStckPanel = gcnew StackPanel();
		StackPanel^ PTStckPanel2 = gcnew StackPanel();
		Grid^ PublishGrid = gcnew Grid();
		
		// receive thread

		Thread^ recvThread;

	public:
		WPFCppCliDemo();
		~WPFCppCliDemo();

		void setUpStatusBar();
		void setUpTabControl();
		void setUpSendMessageView();
		void setUpFileListView();
		void setUpConnectionView();
		void getIndFL(Object^ sender,RoutedEventArgs^ args);
		void LazyDownloadC(Object^ sender, RoutedEventArgs^ args);
		void sendMessage(Object^ obj, RoutedEventArgs^ args);
		void addText(String^ msg);
		void getMessage();
		void clear(Object^ sender, RoutedEventArgs^ args);
		void getItemsFromList(Object^ sender, RoutedEventArgs^ args);
		void browseForFolder(Object^ sender, RoutedEventArgs^ args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
		void PublishButtonPush(Object^ sender, RoutedEventArgs^ args);
		void addfilenames(String^ msg);
		void deletebtn(Object^ sender, RoutedEventArgs^ args);
		void setupIIS(Object^ sender, RoutedEventArgs^ args);
	private:
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		void setTextBlockProperties();
		void setButtonsProperties();
		
	};
}


#endif
