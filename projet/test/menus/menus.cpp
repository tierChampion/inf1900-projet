#include "menu.h"

int main()
{
	Menu::initialiseMenu();

	while(true) {
		Menu::updateStep();
		Menu::executeStep();
	}
}