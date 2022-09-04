#include "GameManager.h"
#include "UserInterface.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
//#ifdef _DEBUG
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif

int main(int argc, char* args[]) { /// Standard C-style entry point, you need to use it
	//interface
	UserInterface inferface;
	inferface.runDisplay();
	//Game Manager 
	GameManager* ptr = new GameManager(inferface);
	bool status  = ptr->OnCreate();
	if (status == true) {
		ptr->Run();
	} else if (status == false) { 
		std::cerr << "Fatal error occured. Cannot start this program" << std::endl;
	}
	ptr->OnDestroy();
	delete ptr;
	_CrtDumpMemoryLeaks();
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}