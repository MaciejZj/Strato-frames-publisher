#include "config_reader.hpp"
#include "publisher_socket.hpp"
#include <iostream>
#include <string>

template <typename frame_type> class Publisher {
  public:
	Publisher(std::string config_path, std::string port_field_group,
	          std::string port_field_name, std::string frame_topic);
	void send(frame_type &frame);

  private:
	std::unique_ptr<Publisher_socket> socket;
};

template <typename frame_type>
Publisher<frame_type>::Publisher(std::string config_path,
                                 std::string port_field_group,
                                 std::string port_field_name,
                                 std::string frame_topic) {

	Config_reader config_reader(config_path);
	int port = config_reader.read<int>(port_field_group, port_field_name);
	spdlog::info("Config parsed.");
	socket = std::make_unique<Publisher_socket>(port, frame_topic);
	spdlog::info("Socket opened at: {}", port);
}

template <typename frame_type>
void Publisher<frame_type>::send(frame_type &frame) {
	socket->send<frame_type>(frame);
}
