#pragma once
#include <fstream>
#include <string>

// Mainly for editor vs game write file differences
class FileManager {
public:
	static void init();
	static void writeFile(const std::string& filename, const std::string& data, std::ios_base::openmode flags = std::ios_base::out);
	static void writeEditorFile(const std::string& filename, const std::string& data, std::ios_base::openmode flags = std::ios_base::out);
	static void readFile(const std::string& filename, std::string& data, std::ios_base::openmode flags = std::ios_base::in);
	static std::string readFile(const std::string& filename, std::ios_base::openmode flags = std::ios_base::in);

	static const std::string gameAssetFolder;
	static const std::string engineAssetFolder;

private:
	static std::string writeFilePath;
	static std::string topLevelFilePath;
};