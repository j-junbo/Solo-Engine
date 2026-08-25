#include <iostream>
#include <engine_main.h>
#include <string>
#include <fmod.hpp>
//#include <rttr/property.h>
//#include <rttr/type.h>

int engine_main(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {


	// FMOD_RESULT result{};
	// FMOD::System* system = NULL;

	//result = FMOD::System_Create(&system);      // Create the main system object.
	//if (result != FMOD_OK)
	//{
	//	std::cout << "oh no! miku does not talk to british people!\n";
	//}

	// rttr test
	//rttr::property prop = rttr::type::get_global_property("NameComponent");
	//std::cout << "typename is: " << prop.get_type().get_name() << '\n';




	return 0;
}

EngineAPI getEngineAPI() {
	return { {[](int i, int j) { return i * j; }} };
}