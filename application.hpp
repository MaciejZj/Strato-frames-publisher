#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "cpu_temp_reader.hpp"
#include "strato-frames/cpu_temp_frame.hpp"
#include "publisher.hpp"
#include <fstream>
#include <thread>
#include <string>
#include <csignal>

class Application {
private:
	inline static bool running = false;
	std::unique_ptr<Cpu_temp_reader> cpu_temp_reader;
	std::unique_ptr<Publisher<cpu_temp_frame_t>> cpu_temp_publisher;

	static void termination_handler(int) {
		spdlog::info("Termination requested");
		running = false;
	}
	
public:
	void init_log() {
		auto file_logger = spdlog::basic_logger_mt("cpu_temp", "cpu_temp.log");
		spdlog::set_default_logger(file_logger);
		spdlog::default_logger()->flush_on(spdlog::level::level_enum::info);
	}

	void init_signals() {
		std::signal(SIGTERM, Application::termination_handler);
	}
	
	int exec() {
		cpu_temp_frame_t cpu_temp_frame;
		
		init_log();
		init_signals();
		spdlog::info("Starting app...");
		
		try {					
			cpu_temp_reader = std::make_unique<Cpu_temp_reader>("dummy");
			cpu_temp_publisher = 
				std::make_unique<Publisher<cpu_temp_frame_t>>
				("sp.cfg", "cpu_temp", "cpu_temp_port", "cpu_temp");
			running = true;		
		} catch (const std::exception& e) {
			spdlog::error(e.what());
		}
		
		while (running) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			try {
				cpu_temp_frame = cpu_temp_reader->read();
				cpu_temp_publisher->send(cpu_temp_frame);
			} catch (const std::exception& e) {
				spdlog::error(e.what());
				running = false;
			}
		}
		
		return 0;
	}
};
