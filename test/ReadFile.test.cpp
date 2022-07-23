#include <asx-png/ReadFile.h>
#include <gtest/gtest.h>

TEST(ReadFile, test)
{
	const auto png = asx::png::ReadFile(TEST_IMAGE);
}
