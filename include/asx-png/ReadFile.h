#pragma once

#include <asx-png/export.hxx>
#include <cstddef>
#include <filesystem>
#include <vector>

namespace asx::png
{
	struct PNG
	{
		std::vector<std::byte> data;
		int width{};
		int height{};
		int channels{};
	};

	ASX_PNG_EXPORT PNG ReadFile(const std::filesystem::path& x);
}
