#pragma once
#include <libconfig.h++>
#include <stdexcept>
#include <string>
#include <zmq.hpp>

class Publisher_socket {
  public:
	Publisher_socket(int port, std::string topic);
	~Publisher_socket();
	template <typename frame_type> void send(frame_type &frame);

  private:
	zmq::socket_t *socket = nullptr;
	zmq::context_t context;
	std::string topic;
};

Publisher_socket::Publisher_socket(int port, std::string topic) : topic(topic) {
	context = zmq::context_t(1);
	socket = new zmq::socket_t(context, ZMQ_PUB);
	if (socket != nullptr) {
		socket->bind("tcp://*:" + std::to_string(port));
	} else {
		throw std::runtime_error("Error while creating socket.");
	}
}

Publisher_socket::~Publisher_socket() {
	socket->close();
	delete socket;
	context.close();
}

template <typename frame_type> void Publisher_socket::send(frame_type &frame) {
	socket->send(topic.begin(), topic.end(), ZMQ_SNDMORE);
	socket->send(zmq::const_buffer(&frame, sizeof(frame)));
}
