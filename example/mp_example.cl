__kernel void SampleKernel(__global volatile atomic_int* flag_array) {
	int tid = (int)get_global_id(0);
	if (tid == 0) {
		atomic_store(&flag_array[0], 1);
		while (atomic_load_explicit(&flag_array[1], memory_order_seq_cst, memory_scope_all_svm_devices) == 0);
	}
}

