__kernel void SampleKernel(__global volatile int* flag_array) {
	int tid = (int)get_global_id(0);
	if (tid == 0) {
		flag_array[0] = 1;
		while (flag_array[1] == 0);
	}
}

