// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include <glog/logging.h>

void bar(int x) {
  CHECK_EQ(x, 1);
}

void foo(int x) {
  bar(x + 1);
}

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  foo(1);
}
