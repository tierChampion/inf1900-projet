#include "button.h"
#include "avr/interrupt.h"

uint8_t mode = 0;
uint8_t line = 0;
uint8_t column = 0;
uint8_t choice = 0;

Button gMode = Button(GeneralInterruptType::INT_2, true);
Button gSelection = Button(GeneralInterruptType::INT_0, false);
Button gValidation = Button(GeneralInterruptType::INT_1, false);

ISR(INT0_vect)
{
	gSelection.update();
}

ISR(INT1_vect)
{
	gValidation.update();
}

ISR(INT2_vect)
{
	gMode.update();
}
int main()
{
	gMode.setSenseControl(SenseControl::ANY_EDGE);
	gSelection.setSenseControl(SenseControl::ANY_EDGE);
	gValidation.setSenseControl(SenseControl::ANY_EDGE);

	gMode.enable();
	gSelection.enable();
	gValidation.enable();

	// add debounces (need release, etc.)

	while (true)
	{
		if (gMode.getIsPressed()) {
			mode = 1; // coin
			PRINT("COIN...");
		}	
		else if (gSelection.getIsPressed()) {
			if (mode < 2) {
			mode = 2; // trajet
			line = 0;
			column = 0;
			PRINT("TRAJET");
			}
			else if (mode < 4) {
				mode++;
				PRINT("MODE CHANGE");
			}
			else if (mode == 4) {
				choice = (choice + 1) % 2;
				PRINT("OUI/NON CHANGE");
			}
		}
		else if (gValidation.getIsPressed()) {
			// line choice
			if (mode == 2) {
				line++;
				line %= 4;
				PRINT("LINE INCREASE");
			}
			// column choice
			else if (mode == 3) {
				column++;
				column %= 7;
				PRINT("COLUMN INCREASE");
			}
			// confirmation
			else if (mode == 4) {
				if (choice == 0) {
					mode = 2;
					line = 0;
					column = 0;
					PRINT("RESET TRAJET");
				}
				else {
					mode = 5;
					PRINT("READY FOR TRAJET");
				}
			}
		}
	}
}