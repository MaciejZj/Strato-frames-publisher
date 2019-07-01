#pragma once
#include "strato-frames/cpu_temp_frame.hpp"
#include <zmq.hpp>
#include <libconfig.h++>
#include <string>

class config_error : public std::runtime_error {
	public:
		config_error(const char* what_arg) : std::runtime_error(what_arg) {}
	};

class network_error : public std::runtime_error {
	public:
		network_error(const char* what_arg) : std::runtime_error(what_arg) {}
	};

template <typename frame_type>
class Socket_handler {
	public:
		Socket_handler(int port);
		Socket_handler(
			std::string cfg_path,
			std::string config_section,
			std::string topic);
		~Socket_handler();
		void publish(frame_type& frame);
		
	private:
		zmq::socket_t* socket;
		zmq::context_t context;
		std::string topic;
		
		int parse_port_config(
			std::string config_path,
			std::string config_section);
		void init_socket(int port);
		
		void send_data(frame_type& frame);
};

template <typename frame_type>
int Socket_handler<frame_type>::parse_port_config(std::string cfg_path, std::string section) {
	libconfig::Config cfg;
	try {
		cfg.readFile(cfg_path.c_str());
	} catch (const libconfig::FileIOException &fioex) {
		throw config_error("I/O error while reading config file.");
	} catch (const libconfig::ParseException &pex) {
		std::stringstream ss;
		ss << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
		throw config_error(ss.str().c_str());
	}
	
	const libconfig::Setting& root = cfg.getRoot();
	int port;
	if(!root[section.c_str()].lookupValue(section.append("_port").c_str(), port)) {
		throw config_error("CPU temperature file not defined in config.");
	}
	spdlog::info("Config parsed.");
	return port;
}

template <typename frame_type>
void Socket_handler<frame_type>::init_socket(int port) {
	try {
		context = zmq::context_t(1);
		socket = new zmq::socket_t(context, ZMQ_PUB);
		if (socket != nullptr) {
			socket->bind("tcp://*:" + std::to_string(port));
		} else {
			throw network_error("Error while creating socket.");
		}
	} catch (const zmq::error_t &e) {
		throw network_error(e.what());
	}
	spdlog::info("Socket opened at: {}", port);
}

template <typename frame_type>
Socket_handler<frame_type>::Socket_handler(
	std::string config_path, std::string config_section, std::string topic):
	topic(topic) {
	
	int port = parse_port_config(config_path, config_section);
	init_socket(port);
}

template <typename frame_type>
Socket_handler<frame_type>::Socket_handler(int port) {
	init_socket(port);
}

template <typename frame_type>
Socket_handler<frame_type>::~Socket_handler() {
	socket->close();
	delete socket;
	context.close();
}

template <typename frame_type>
void Socket_handler<frame_type>::send_data(frame_type& frame) {
	try {
		socket->send(topic.begin(), topic.end(), ZMQ_SNDMORE);
		socket->send(zmq::const_buffer(&frame, sizeof(frame)));
	} catch (const zmq::error_t& e) {
		throw network_error(e.what());
	}
}

template <typename frame_type>
void Socket_handler<frame_type>::publish(frame_type& frame) {
	try {
		send_data(frame);
	} catch (const network_error& e) {
		spdlog::error(e.what());
	}
}
