// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "glog/logging.h"
#include "gtest/gtest.h"

void FailureWriter(const char *data, size_t size) {
  fprintf(stderr, "%.*s", (int) size, data);
}

int main(int argc, char **argv) {
  google::InstallFailureSignalHandler();
  google::InstallFailureWriter(FailureWriter);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
