#include "src/common/file/io.h"

#include "gtest/gtest.h"

#include "src/common/interface/proto/io_test.pb.h"

TEST(IoTest, ReadAndWriteProtoInTextFormat) {
  const std::string temp_filename = "/tmp/io_test_temp_file";
  interface::IoTest io_test;
  const int test_int = 10;
  const double test_double = 5.20;
  const std::string test_string = "Pony.ai is a company which aims at autonomous mobility everywhere.";
  const std::vector<double> test_repeated_double{2.0, 3.1, 5.4, 0.0};
  io_test.set_test_int(test_int);
  io_test.set_test_double(test_double);
  io_test.set_test_string(test_string);
  io_test.mutable_test_repeated_double()->Add(test_repeated_double.begin(), test_repeated_double.end());
  ASSERT_TRUE(io::WriteToProtoInTextFormat(io_test, temp_filename));

  interface::IoTest io_test_to_read;
  ASSERT_TRUE(io::ReadFromProtoInTextFormat(temp_filename, &io_test_to_read));
  EXPECT_EQ(io_test_to_read.test_int(), test_int);
  EXPECT_EQ(io_test_to_read.test_double(), test_double);
  EXPECT_EQ(io_test_to_read.test_string(), test_string);
  ASSERT_EQ(io_test_to_read.test_repeated_double_size(), test_repeated_double.size());
  for (int i = 0; i < test_repeated_double.size(); ++i) {
    EXPECT_DOUBLE_EQ(io_test_to_read.test_repeated_double(i), test_repeated_double[i]);
  }
}
