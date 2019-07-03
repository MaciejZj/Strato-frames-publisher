#pragma once
#include "spdlog/spdlog.h"
#include <libconfig.h++>
#include <sstream>
#include <string>

class config_error : public std::runtime_error {
	public:
		config_error(const char* what_arg) : std::runtime_error(what_arg) {}
};

class Config_reader {
public:
	Config_reader(std::string config_path);
	
	template <typename T>
	T read(std::string group_name, std::string field_name);
	
private:
	libconfig::Config cfg;
};

Config_reader::Config_reader(std::string config_path) {
	try {
		cfg.readFile(config_path.c_str());
	} catch (const libconfig::FileIOException &fioex) {
		throw config_error("I/O error while reading config file.");
	} catch (const libconfig::ParseException &pex) {
		std::stringstream err_msg;
		err_msg << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
		throw config_error(err_msg.str().c_str());
	}
}

template <typename T>
T Config_reader::read(std::string group_name, std::string field_name) {
	const libconfig::Setting& root = cfg.getRoot();
	int field_value;
	
	if(!root[group_name.c_str()].lookupValue(field_name.c_str(), field_value)) {
		std::string err_msg;
		err_msg = "Field '" + field_name + "' in group '" + group_name
			+ "' not defined in config.";
		throw config_error(err_msg.c_str());
	}
	return field_value;
}
