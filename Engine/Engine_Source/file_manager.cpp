#include "file_manager.h"
#include <filesystem>
#include <sstream>

const std::string FileManager::gameAssetFolder{ "Assets/" GAME_NAME "_Assets/"};
const std::string FileManager::engineAssetFolder{ "Assets/Engine_Assets/" };
std::string FileManager::writeFilePath{};
std::string FileManager::topLevelFilePath{};

void FileManager::init() {
	// writefilepath will be done next time

	std::string filepath{ "../" };
	while (!std::filesystem::exists(filepath + "Assets/")) {
		filepath = "../" + filepath;
	}

	topLevelFilePath = filepath;
}

void FileManager::writeFile(const std::string& filename, const std::string& data, std::ios_base::openmode flags) {
	// std::ifstream ifs()
}

// This function assumes the user also uses the provided asset folder static functions 
void FileManager::writeEditorFile(const std::string& filename, const std::string& data, std::ios_base::openmode flags) {
	std::ofstream ofs(topLevelFilePath + filename, flags);
	ofs << data;
}

void FileManager::readFile(const std::string& filename, std::string& data, std::ios_base::openmode flags) {
	std::ifstream ifs(filename, flags);
	std::stringstream stream(flags); 
	
	stream << ifs.rdbuf();
	data = stream.str();
}

std::string FileManager::readFile(const std::string& filename, std::ios_base::openmode flags) {
	std::ifstream ifs(filename, flags);
	std::stringstream stream(flags);

	stream << ifs.rdbuf();
	return stream.str();
}