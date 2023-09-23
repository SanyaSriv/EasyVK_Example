#include <vector>
#include <iostream>
#include <easyvk.h>
#include <cassert>
#include <vector>

const int size = 1024 * 16;

int main(int argc, char** argv) {

	// Initialize instance.
	auto instance = easyvk::Instance(true);
	// Get list of available physical devices.
	auto physicalDevices = instance.physicalDevices();
	// Create device from first physical device.
	auto device = easyvk::Device(instance, physicalDevices.at(3));
	std::cout << "Using device: " << device.properties.deviceName << "\n";
	
	auto numIters = 1;
	// Create GPU buffers.

	auto flag_array = easyvk::Buffer(device, 2 * sizeof(int));

	flag_array.store(0, 0);
	flag_array.store(1, 0);

	std::vector<easyvk::Buffer> bufs = {flag_array};

	const char* testFile = "build/vect-add.spv";
	std::vector<uint32_t> spvCode =
		#include "build/vect-add.cinit"
		;
	auto program = easyvk::Program(device, spvCode, bufs);
	program.setWorkgroups(size);
	program.setWorkgroupSize(1);


	// Run the kernel.
	program.initialize("SampleKernel");
	program.run();

	flag_array.store(1, 1); // set the flag
	while (flag_array.load(0) == 0); // spin

	program.teardown();
	flag_array.teardown();

	device.teardown();
	instance.teardown();
	return 0;
}

