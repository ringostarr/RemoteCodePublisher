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

#include "Window.h"
//#include "../MsgClient/MsgClient.h"
using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo()
{
	// set up channel
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
	pChann_->start();
	delete pObjFact;
	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
	// set event handlers
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hSendButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendMessage);
	hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	LazyDownloadBttn->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::LazyDownloadC);
	hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
	hShowItemsButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getItemsFromList);
	PublishButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::PublishButtonPush);
	IndependentButton->Click+= gcnew RoutedEventHandler(this, &WPFCppCliDemo::getIndFL);
	del->Click+= gcnew RoutedEventHandler(this, &WPFCppCliDemo::deletebtn);
	IISpbtn->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::setupIIS);
	// set Window properties
	this->Title = "WPF C++/CLI Demo";
	this->Width = 800;
	this->Height = 600;
	// attach dock panel to Window
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	// setup Window controls and views
	setUpTabControl();
	setUpStatusBar();
	setUpSendMessageView();
	setUpFileListView();
	setUpConnectionView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}
//btn function
void WPFCppCliDemo::setupIIS(Object^ sender, RoutedEventArgs^ args)
{
	String^ str = InputTextBox->Text;
	std::string cstr  = "IIS/";
	cstr += toStdString(str);
	pSendr_->postMessage(cstr);
	hStatus->Text = "Setting up IIS";

}
//btn function
void WPFCppCliDemo::getIndFL(Object^ sender, RoutedEventArgs^ args)
{
	std::string sfiles;

	sfiles += "GIFL/";

	if (cat1y->IsChecked)
		sfiles += "CategoryOne/";
	else if (cat2y->IsChecked)
		sfiles += "CategoryTwo/";
	else if (cat3y->IsChecked)
		sfiles += "CategoryThree/";
	else
		sfiles += "DefaultCategory/";
	pSendr_->postMessage(sfiles);
	hStatus->Text = "Fetching Independent File List";

}
//btn function
void WPFCppCliDemo::PublishButtonPush(Object^ sender, RoutedEventArgs^ args)
{
	
	std::string sfiles;

	sfiles += "Publish/";
	
	if (cat1x->IsChecked)
		sfiles += "CategoryOne/";
	else if (cat2x->IsChecked)
		sfiles += "CategoryTwo/";
	else if (cat3x->IsChecked)
		sfiles += "CategoryThree/";
	else
		sfiles += "DefaultCategory/";
	pSendr_->postMessage(sfiles);
	hStatus->Text = "Publishing";

}
//btn function
void WPFCppCliDemo::LazyDownloadC(Object^ sender , RoutedEventArgs^ args)
{
	std::string sfiles;
	sfiles += "Lazy/";
	if (cat1y->IsChecked)
		sfiles += "CategoryOne/";
	else if (cat2y->IsChecked)
		sfiles += "CategoryTwo/";
	else if (cat3y->IsChecked)
		sfiles += "CategoryThree/";
	else
		sfiles += "DefaultCategory/";
	for each (String^ item in hListBox->SelectedItems)
	{
		sfiles += toStdString(item);
		sfiles += ",";
	}
	pSendr_->postMessage(sfiles);
	hStatus->Text = "Publishing";
}
//control function setup
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hSendMessageTab->Header = "FileUpload";
	hFileListTab->Header = "FileDownload";
	hConnectTab->Header = "PublisTab";
	hTabControl->Items->Add(hSendMessageTab);
	hTabControl->Items->Add(hFileListTab);
	hTabControl->Items->Add(hConnectTab);
	hFolderBrowseButton->Content = "GetFiles From Server";
	hFolderBrowseButton->Height = 30;
	hFolderBrowseButton->Width = 120;
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hFolderBrowseButton, 1);
	hFileListGrid->Children->Add(hFolderBrowseButton);

}

void WPFCppCliDemo::setTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	PublishGrid->RowDefinitions->Add(hRow1Def);
	RowDefinition^ hRow5Def = gcnew RowDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow5Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = publishTextblock;
	publishTextblock->Padding = Thickness(15);
	publishTextblock->Text = "";
	publishTextblock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	publishTextblock->FontWeight = FontWeights::Bold;
	publishTextblock->FontSize = 16;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;
	PublishGrid->SetRow(hScrollViewer1, 0);
	PublishGrid->Children->Add(hScrollViewer1);
	LazyDownloadBttn->Content = "LazyDownload";
	LazyDownloadBttn->Height = 30;
	LazyDownloadBttn->Width = 120;

}

void WPFCppCliDemo::setButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(75);
	hSendMessageGrid->RowDefinitions->Add(hRow2Def);
	hSendMessageGrid->RowDefinitions->Add(hRow3Def);
	hSendButton->Content = "UploadFiles";
	Border^ hBorder15 = gcnew Border();
	hBorder15->Width = 120;
	hBorder15->Height = 30;
	hBorder15->BorderThickness = Thickness(1);
	hBorder15->BorderBrush = Brushes::Black;
	hBorder15->Child = InputTextBox;
	hBorder15->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hBorder15->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	Border^ hBorder2 = gcnew Border();
	InputTextBox->Text = "IIS PATH";
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hClearButton->Content = "Browse";
	hBorder2->Child = hSendButton;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hClearButton;
	hStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 5;
	hStackPanel1->Children->Add(hSpacer);
	hStackPanel1->Children->Add(hBorder3);
	cat1->Content = "Category1";
	cat2->Content = "Category2";
	cat3->Content = "Category3";
	defcat->Content = "Default Category";
	hSendMessageGrid->SetRow(hBorder15,2);
	hSendMessageGrid->SetRow(label, 2);
	hSendMessageGrid->Children->Add(hBorder15);
}

void WPFCppCliDemo::setUpSendMessageView()
{
	Console::Write("\n  setting up FileUpload view");
	Border^ hBorder10 = gcnew Border();
	hBorder10->Width = 120;
	hBorder10->Height = 30;
	hBorder10->BorderThickness = Thickness(1);
	hBorder10->BorderBrush = Brushes::Black;
	hBorder10->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hBorder10->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hBorder10->Child = IISpbtn;
	IISpbtn->Content = "Setup IIS Dir";
	hStackPanel1->Children->Add(hBorder10);
	hStackPanel1->Orientation = Orientation::Horizontal;
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hSendMessageGrid->SetRow(hStackPanel1, 3);
	hSendMessageGrid->Margin = Thickness(80);
	hSendMessageTab->Content = hSendMessageGrid;
	hSendMessageGrid->SetRow(hStackPanel2, 1);
	LazyDownloadBttn->BorderThickness = Thickness(2);
	IndependentButton->Content = "Get Independent Files";
	IndependentButton->Height = 30;
	IndependentButton->Width = 150;
	IndependentButton->BorderThickness = Thickness(2);
	del->Content = "Delete";
	del->Height = 30;
	del->Width = 120;
	del->BorderThickness = Thickness(2);
	del->BorderBrush = Brushes::Black;
	del->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hShowItemsButton->Content = "Download";
	hShowItemsButton->Height = 30;
	hSendMessageGrid->Children->Add(hStackPanel1);
	hSendMessageGrid->Children->Add(hStackPanel2);
	hSendButton->Content = InputTextBox;
	Border^ hBorder8 = gcnew Border();
	hBorder8->BorderThickness = Thickness(1);
	hBorder8->BorderBrush = Brushes::Black;
	uploadLB->SelectionMode = SelectionMode::Multiple;
	hBorder8->Child = uploadLB;
	hSendMessageGrid->SetRow(hBorder8, 0);
	hSendMessageGrid->Children->Add(hBorder8);
	InputTextBox->FontSize = 14;
	setTextBlockProperties();
	setButtonsProperties();
	hStackPanel2->Children->Add(cat1);
	hStackPanel2->Children->Add(cat2);
	hStackPanel2->Children->Add(cat3);
	hStackPanel2->Children->Add(defcat);
}

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

void WPFCppCliDemo::sendMessage(Object^ obj, RoutedEventArgs^ args)
{
	//Message msg;
	std::string sfiles;
	sfiles += "Upload/";
	if (cat1->IsChecked)
		sfiles += "CategoryOne/";
	else if (cat2->IsChecked)
		sfiles += "CategoryTwo/";
	else if (cat3->IsChecked)
		sfiles += "CategoryThree/";
	else
		sfiles += "DefaultCategory/";
	for each (String^ item in uploadLB->SelectedItems)
	{
		sfiles += toStdString(item);
		sfiles +=",";
	}
	pSendr_->postMessage(sfiles);
	Console::Write("\n  Upload Message Sent");
	hStatus->Text = "Upload Request Sent";
}

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

void WPFCppCliDemo::addText(String^ msg)
{
	hTextBlock1->Text += msg + "\n\n";
}
void WPFCppCliDemo::addfilenames(String^ msg)
{
	hListBox->Items->Clear();
	array<String^>^ arrayx = msg->Split(',');
	for each(String^ temp in arrayx)
	{
		hListBox->Items->Add(temp);
	}
}
void sendtonativeclient(std::string msg)
{

}
void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function

	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		std::string msg = pRecvr_->getMessage();
		std::string nmes;
		size_t n;
		if (msg.find("::gtf") != std::string::npos)
		{
			nmes = msg.substr(0, msg.find("::gtf"));
			 n= std::count(nmes.begin(), nmes.end(), ',');
			 
		}
		else if (msg.find("::gifl") != std::string::npos)
		{
			nmes = msg.substr(0, msg.find("::gifl"));
			n = std::count(nmes.begin(), nmes.end(), ',');

		}
		String^ lbfn = toSystemString(nmes);
		if (n > 0)
		{

			/*int i = 0;
			size_t pos;
			while ((pos = nmes.find(",")) != std::string::npos)
			{
				std::string tok = nmes.substr(0, pos);
				lbfn[i] = toSystemString(tok);
				nmes.erase(0, pos + 1);
				i++;
			}*/
		}

		String^ sMsg = toSystemString(msg);
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;
		if (n > 0) {
			Action<String^>^ act2 = gcnew Action<String^>(this, &WPFCppCliDemo::addfilenames);
			Dispatcher->Invoke(act2, lbfn);
			
		}
		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
		Dispatcher->Invoke(act, sMsg);  // must call addText on main UI thread
	}
}

void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	uploadLB->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			uploadLB->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			uploadLB->Items->Add(L"(Dir) " + dirs[i]);
	}
}
void WPFCppCliDemo::deletebtn(Object^ sender, RoutedEventArgs^ args)
{
	std::string sfiles;
	sfiles += "Delete/";
	if (cat1y->IsChecked)
		sfiles += "CategoryOne/";
	else if (cat2y->IsChecked)
		sfiles += "CategoryTwo/";
	else if (cat3y->IsChecked)
		sfiles += "CategoryThree/";
	else
		sfiles += "DefaultCategory/";
	if (hListBox->SelectedItems->Count <= 0)
		hStatus->Text = "Select File please!";
	else
	{
		for each (String^ item in hListBox->SelectedItems)
		{
			sfiles += toStdString(item);
			sfiles += ",";
		}

		pSendr_->postMessage(sfiles);
		Console::Write("\n  sent message");
		hStatus->Text = "Sent message";
	}
}


void WPFCppCliDemo::getItemsFromList(Object^ sender, RoutedEventArgs^ args)
{
	std::string sfiles;
	sfiles += "Download/";
	if (cat1y->IsChecked)
		sfiles += "CategoryOne/";
	else if (cat2y->IsChecked)
		sfiles += "CategoryTwo/";
	else if (cat3y->IsChecked)
		sfiles += "CategoryThree/";
	else
		sfiles += "DefaultCategory/";
	if (hListBox->SelectedItems->Count <= 0)
		hStatus->Text = "Select File please!";
	else
	{
		for each (String^ item in hListBox->SelectedItems)
		{
			sfiles += toStdString(item);
			sfiles += ",";
		}

		pSendr_->postMessage(sfiles);
		//pSendr_->postMessage(toStdString(InputTextBox->Text));
		Console::Write("\n  sent message");
		hStatus->Text = "Sent message";
	}
}



void WPFCppCliDemo::setUpFileListView()
{
	Console::Write("\n  setting up Download view");
	hFileListGrid->Margin = Thickness(20);
	hFileListTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hListBox->SelectionMode = SelectionMode::Multiple;
	hBorder1->Child = hListBox;
	hFileListGrid->SetRow(hBorder1, 0);
	hFileListGrid->Children->Add(hBorder1);
	RowDefinition^ hRow2Def4 = gcnew RowDefinition();
	hRow2Def4->Height = GridLength(75);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	RowDefinition^ hRow2Def2 = gcnew RowDefinition();
	hRow2Def2->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hFileListGrid->RowDefinitions->Add(hRow2Def2);
	hFileListGrid->RowDefinitions->Add(hRow2Def4);
	hShowItemsButton->Width = 120;
	hShowItemsButton->BorderThickness = Thickness(2);
	hShowItemsButton->BorderBrush = Brushes::Black;
	IndependentButton->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hFileListGrid->SetRow(hShowItemsButton, 2);
	hFileListGrid->SetRow(LazyDownloadBttn,3 );
	hFileListGrid->SetRow(IndependentButton, 3);
	hFileListGrid->SetRow(del, 2);
	hFileListGrid->Children->Add(del);
	hFileListGrid->Children->Add(hShowItemsButton);
	hFileListGrid->Children->Add(LazyDownloadBttn);
	hFileListGrid->Children->Add(IndependentButton);
	cat1y->Content = "CategoryOne";
	cat2y->Content = "CategoryTwo";
	cat3y->Content = "CategoryThree";
	downloadstck->Children->Add(cat1y);
	downloadstck->Children->Add(cat2y);
	downloadstck->Children->Add(cat3y);
	RowDefinition^ hRow2Def3= gcnew RowDefinition();
	hRow2Def2->Height = GridLength(75);
	hFileListGrid->SetRow(downloadstck, 2);
	hFileListGrid->Children->Add(downloadstck);
	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	hListBox->Items->Clear();
	std::cout << "\n  Fetching Files for Server";
	std::string sfiles;
	sfiles += "GetFiles/";
	if (cat1y->IsChecked)
		sfiles += "CategoryOne/";
	else if (cat2y->IsChecked)
		sfiles += "CategoryTwo/";
	else if (cat3y->IsChecked)
		sfiles += "CategoryThree/";
	else
		sfiles += "DefaultCategory/";
	pSendr_->postMessage(sfiles);
	sfiles.clear();
	hStatus->Text = "Requested Server for Files";

}
void WPFCppCliDemo::setUpConnectionView()
{
	Console::Write("\n  setting up PublishView");
	PublishGrid->Margin = Thickness(20);
	hConnectTab->Content = PublishGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(75);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	PublishButton->Content = "Publish";
	PublishButton->Height = 30;
	PublishButton->Width = 120;
	PTStckPanel->Orientation = Orientation::Horizontal;
	PTStckPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	PTStckPanel2->Orientation = Orientation::Vertical;
	PTStckPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	PublishGrid->RowDefinitions->Add(hRow1Def);
	PublishGrid->RowDefinitions->Add(hRow2Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Height = 30;
	hBorder1->Width = 120;
	hBorder1->Child = PublishButton;
	PublishGrid->SetRow(hBorder1, 0);
	PTStckPanel->Children->Add(hBorder1);
	cat1x->Content = "Category 1";
	cat2x->Content = "Category 2";
	cat3x->Content = "Category 3";
	PTStckPanel2->Children->Add(cat1x);
	PTStckPanel2->Children->Add(cat2x);
	PTStckPanel2->Children->Add(cat3x);
	PublishGrid->SetRow(PTStckPanel, 2);
	PublishGrid->SetRow(PTStckPanel2, 3);
	PublishGrid->Children->Add(PTStckPanel);
	PublishGrid->Children->Add(PTStckPanel2);
}

void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");

	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo());
	Console::WriteLine(L"\n\n");
}