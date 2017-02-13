#define private public
#include "StringTable.h"
#include <gtest/gtest.h>

TEST(StringTable,  GetIndexByName_GetNameByIndex)
{
	Wll::StringTable st;
	EXPECT_TRUE(st.GetIndexByName("test1")==0);
	EXPECT_TRUE(st.GetIndexByName("test2")==1);
	EXPECT_TRUE(st.GetIndexByName("test1")==0);
	EXPECT_TRUE(st.GetNameByIndex(1)=="test2");
}



GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
