#include <asx-png/ReadFile.h>
#include <array>
#include <fstream>

namespace
{
	constexpr auto PngCode(char a, char b, char c, char d)
	{
		std::uint32_t type = (a << 24);
		type += (b << 16);
		type += (c << 8);
		type += d;
		return type;
	}

	bool TestPNGHeader(std::vector<char>::iterator& x)
	{
		constexpr std::array<char, 8> pngSignature{137, 80, 78, 71, 13, 10, 26, 10};

		for(const auto c : pngSignature)
		{
			if(c != *x)
			{
				return false;
			}

			x++;
		}

		return true;
	}

	struct ChunkHeader
	{
		std::uint32_t length{};
		std::uint32_t code{};
	};

	std::uint8_t Get8(std::vector<char>::iterator& x)
	{
		return *(x++);
	}

	std::uint16_t GetBigEndian16(std::vector<char>::iterator& x)
	{
		auto c = Get8(x);
		return (c << 8) + Get8(x);
	}

	std::uint32_t GetBigEndian32(std::vector<char>::iterator& x)
	{
		auto c = GetBigEndian16(x);
		return (c << 16) + GetBigEndian16(x);
	}

	ChunkHeader GetChunkHeader(std::vector<char>::iterator& x)
	{
		ChunkHeader header;
		header.length = GetBigEndian32(x);
		header.code = GetBigEndian32(x);
		return header;
	}
}

asx::png::PNG asx::png::ReadFile(const std::filesystem::path& x)
{
	std::vector<char> binary;

	{
		std::ifstream in{x, std::ios::binary};

		if(in.is_open() == true)
		{
			in.seekg(0, std::ios::end);
			binary.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(binary.data(), binary.size());
		}
	}

	if(binary.empty() == false)
	{
		asx::png::PNG png{};

		auto it = std::begin(binary);

		if(TestPNGHeader(it) == false)
		{
			return {};
		}

		auto ihdrFirst = false;

		std::vector<std::byte> compressed;
		auto offset = 0;

		while(it != std::end(binary))
		{
			const auto chunk = GetChunkHeader(it);

			switch(chunk.code)
			{
				// The IHDR chunk must appear FIRST. It contains:
				// Width: 4 bytes - pixels, 0 is invalid, 2^31 - 1 max
				// Height : 4 bytes - pixels, 0 is invalid, 2^31 - 1 max
				// Bit depth : 1 byte - bits per sample or per palette index (not per pixel), valid 1, 2, 4, 8, 16
				// Color type : 1 byte - bitmask of following -> 1: palette used, 2: color used, 4, alpha channel used, valid 0, 2, 4, 6
				//		0: 1, 2, 4, 8, 16 - Each pixel is sa grayscale sample.
				//		2: 8, 16 - Each pixel is an R,G,B triple.
				//		3: 1, 2, 4, 8 - Each pixel is a palette index; a PLTE chunk must appear
				//		4: 8, 16 - Each pixel is a grayscale sample, followed by an alpha sample.
				//		6: 8, 16 - Each pixel is an R,G,B triple, followed by an alpha sample
				// Compression method : 1 byte - 0 is deflate/inflate method with a sliding window of at most 32768 bytes.
				// Filter method : 1 byte - 0 is adaptive filtering with five basic filter types
				// Interlace method : 1 byte - 0 is no interlace, 1 is Adam7 interlace
				case PngCode('I', 'H', 'D', 'R'):
				{
					if(ihdrFirst == true || chunk.length != 13)
					{
						return {};
					}

					enum ColorType : uint8_t
					{
						Palette = 0b0001,
						Color = 0b0010,
						Alpha = 0b0100
					};

					constexpr auto channelCountGrayScale{1};
					constexpr auto channelCountRGB{3};
					constexpr auto channelCountAlpha{1};

					png.width = GetBigEndian32(it);
					png.height = GetBigEndian32(it);

					const auto depth = Get8(it);
					const auto color = Get8(it);

					png.channels = (color & Color ? channelCountRGB : channelCountGrayScale) + (color & Alpha ? channelCountAlpha : 0);

					const auto compression = Get8(it);
					const auto filter = Get8(it);
					const auto interlace = Get8(it);

					ihdrFirst = true;
					break;
				}

				case PngCode('I', 'D', 'A', 'T'):
				{
					if(ihdrFirst == false)
					{
						return {};
					}

					const auto start = compressed.size();
					const auto end = compressed.size() + chunk.length;
					compressed.resize(end);
					std::memcpy(&compressed[start], &(*it), chunk.length);
					it += chunk.length;
					break;
				}

				case PngCode('I', 'E', 'N', 'D'):
				{
					if(ihdrFirst == false)
					{
					}

					const auto rawSize = png.height * png.width * png.channels;
					png.data.resize(rawSize);
					// Perform decompression.
					auto compressedSize = compressed.size();

					return png;
					break;
				}

				default:
					auto count = chunk.length;
					while(count > 0)
					{
						count--;
						Get8(it);
					}
					break;
			}

			const auto crc = GetBigEndian32(it);
		}
	}

	return {};
}