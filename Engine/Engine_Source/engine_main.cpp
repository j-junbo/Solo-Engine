#include <iostream>
#include <engine_main.h>
#include <string>
#include <fmod.hpp>
#include <rttr/property.h>
#include <rttr/type.h>
#include <registry.h>
#include "components.h"

int engine_main(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {


	// FMOD_RESULT result{};
	// FMOD::System* system = NULL;

	//result = FMOD::System_Create(&system);      // Create the main system object.
	//if (result != FMOD_OK)
	//{
	//	std::cout << "oh no! miku does not talk to british people!\n";
	//}

	// rttr test
	rttr::property prop = rttr::type::get_by_name("NameComponent").get_property("name");
	std::cout << "typename is: " << prop.get_type().get_name().data() << '\n';

	ComponentStorage comp{ NameComponent{} };
	auto [sparse, dense] = comp.getStorage<NameComponent>();
	auto& index = comp.getIndex();

	comp.addComponent(5, NameComponent{ "Player" });
	comp.addComponent(4, NameComponent{ "Enemy" });
	comp.addComponent(10, NameComponent{ "Camera" });
	comp.addComponent(7, NameComponent{ "Light" });
	comp.addComponent(15, NameComponent{ "Ground" });

	std::cout << "sparse set: ";
	for (size_t i : sparse) {
		std::cout << (i == -1 ? std::string(" ") : std::to_string(i)) << ", ";
	}
	std::cout << "\n";
	std::cout << "dense set: ";
	for (const NameComponent& c : dense) {
		std::cout << c.name << ' ';
	}
	std::cout << "\n";
	std::cout << "index set: ";
	for (size_t i : index) {
		std::cout << i << ' ';
	}
	std::cout << "\n";

	comp.removeComponent(3);
	std::cout << "after nonexistent remove: \n";
	std::cout << "sparse set: ";
	for (size_t i : sparse) {
		std::cout << (i == -1 ? std::string(" ") : std::to_string(i)) << ", ";
	}
	std::cout << "\n";
	std::cout << "dense set: ";
	for (const NameComponent& c : dense) {
		std::cout << c.name << ' ';
	}
	std::cout << "\n";
	std::cout << "index set: ";
	for (size_t i : index) {
		std::cout << i << ' ';
	}
	std::cout << "\n";

	comp.removeComponent(4);
	std::cout << "after remove: \n";
	std::cout << "sparse set: ";
	for (size_t i : sparse) {
		std::cout << (i == -1 ? std::string(" ") : std::to_string(i)) << ", ";
	}
	std::cout << "\n";
	std::cout << "dense set: ";
	for (const NameComponent& c : dense) {
		std::cout << c.name << ' ';
	}
	std::cout << "\n";
	std::cout << "index set: ";
	for (size_t i : index) {
		std::cout << i << ' ';
	}
	std::cout << "\n";


	return 0;
}

EngineAPI getEngineAPI() {
	return { {[](int i, int j) { return i * j; }} };
}