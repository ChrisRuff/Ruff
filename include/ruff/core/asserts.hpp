#include <string>
#include <cassert>

#include <ruff/core/logger.hpp>

namespace ruff
{
	constexpr inline void ruff_assert(const bool error, const std::string& msg)
	{
		if(!error)
		{
			logError(msg);
			assert(error);
		}
	}
};
