#include <iostream>
#include "driver.hpp"

int main() 
{
	while (!driver.init()) {
		
		std::cout << "Driver is not loaded\n";
	}
	driver.process_id = driver.get_process_id(L"notepad.exe");
	driver.base_address = driver.get_base_address();
	std::cout << " [ base_address ] -> " << driver.base_address << std::endl;
	Sleep(-1);
}