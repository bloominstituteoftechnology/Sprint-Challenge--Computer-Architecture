<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:

	The CPU provides concurrency by sharing time among processes. The operating system does the job of timing when processes get sent to the CPU and when they get stopped to run other processes. So I guess wih multiple cores you are getting some concurrency, but most of the "concurrency" is perceived because of the speed and efficiency of process switching. Multiple cores also helps handle more processes.

2. Describe assembly language and machine language:

	Assembly language is a low level respresentation of machine language. It can be very similar accross languages, but assembly languages are specific to each computwer architecture.
	
	Machine language is the lowest level of code readable by developers. It is the actual code in hex or binary that the cpu process.

3. Why is 3D performance so much higher with a graphics card than without?

	GPUs are similar in archicture to CPUs except they operate concurrently on a much larger scale. Gpus have tons of cores that are able to process data simultaneously.

4. Suggest the role that graphics cards play in machine learning:

	Because GPUs are optimized for concurrency with thousands of computational units they are able to process data in a similar way to how neural networks are programmed to work.

	GPUs allow much larger data sets to be processed in a shorter amount of time with less infrastructure. Huge gains are had from processing with GPUs over CPUs.