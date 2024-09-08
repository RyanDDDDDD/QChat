#include "ConfigMgr.h"

ConfigMgr::ConfigMgr() {
	boost::filesystem::path currentPath = boost::filesystem::current_path();
	boost::filesystem::path configPath = currentPath / "config.ini";
	std::cout << "Config Path: " << configPath << std::endl;

	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(configPath.string(), pt);
	
	for (const auto& sectionPair : pt) {
		const std::string& sectionName = sectionPair.first;
		const boost::property_tree::ptree& section_tree = sectionPair.second;
		std::map<std::string, std::string> sectionConfig;
		
		for (const auto& key_value_pair : section_tree) {
			const std::string& key = key_value_pair.first;
			const std::string& value = key_value_pair.second.get_value<std::string>();

			sectionConfig[key] = value;
		}

		SectionInfo sectionInfo;
		sectionInfo._section_data = sectionConfig;
		_config_map[sectionName] = sectionInfo;
	}

	for (const auto& section_entry : _config_map) {
		const std::string& section_name = section_entry.first;
		SectionInfo section_config = section_entry.second;
		std::cout << "[" << section_name << "]" << std::endl;
		for (const auto& key_value_pair : section_config._section_data) {
			std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
		}
	}
}