#include "fence.hpp"

#include <iostream>
#include <chrono>

typedef std::chrono::high_resolution_clock waitClock;

const double timeWarningNano = 2000000;

void Fence::wait()
{
	if (!sync) return;

	auto t1 = waitClock::now();
	GLenum ret = glClientWaitSync(sync, 0, 0);

	if (ret == GL_ALREADY_SIGNALED |
		  ret == GL_CONDITION_SATISFIED)
	{
		auto t2 = waitClock::now();
		if (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() > timeWarningNano)
		{
			std::cout << "Waiting too long on fence " << (intptr_t)sync << std::endl;
		}
	}
	else if (ret == GL_WAIT_FAILED)
	{
		throw std::runtime_error("Fence " + std::to_string((intptr_t)sync) + " wait failed");
	}
}

void Fence::lock()
{
	glDeleteSync(sync);
	sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}