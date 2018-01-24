// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Copyright Drew Noakes 2013-2016

#include <joystick.h>
#include <unistd.h>
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int _argc, char** _argv) {
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Produce help message")
		("device,d",po::value< std::string >(), "Device file, /dev/input/jsX")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(_argc, _argv, desc), vm);
	po::notify(vm);  

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}

	std::string deviceStr = "/dev/input/js1";
	if (vm.count("device")) {
		deviceStr = vm["device"].as<std::string>();
	}

	// Create an instance of Joystick
	Joystick joystick(deviceStr);

	// Ensure that it was found and that we can use it
	if (!joystick.isFound()) {
		std::cout << "Failed opening " << deviceStr <<"\n";
		return -1;
	}

	while (true) {
		// Restrict rate
		usleep(1000);

		// Attempt to sample an event from the joystick
		JoystickEvent event;
		if (joystick.sample(&event)) {
			if (event.isButton()) {
				printf("Button %u is %s\n", event.number, event.value == 0 ? "up" : "down");
			}
			else if (event.isAxis()) {
				printf("Axis %u is at position %d\n", event.number, event.value);
			}
		}
	}
}
