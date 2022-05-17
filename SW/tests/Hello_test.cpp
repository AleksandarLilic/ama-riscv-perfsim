#include <gtest/gtest.h>
#include "../include/logic_t.h"

// Demonstrate some basic assertions.
TEST(HelloTest, TestGet) {

	logic_t ff = logic_t(50u);

	// Expect equality.
	EXPECT_EQ(ff.get(), 50);
}

TEST(HelloTest, TestEquals) {

	logic_t ff = logic_t(50u);

	ff = 100u;
	// Expect equality.
	EXPECT_EQ(ff.get(), 50);

	ff.clk_update();

	EXPECT_EQ(ff.get(), 100);

}

