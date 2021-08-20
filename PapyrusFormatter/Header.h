#pragma once

#ifdef EXPORT_TEST_FUNCTIONS

#define MY_CPP_UNITTESTAPP_EXPORT __declspec(dllexport)
#else
#define MY_CPP_UNITTESTAPP_EXPORT
#endif

//#define DEBUG
//#define MYFILES

extern const char* outname;
int main_impl(int argc, char** argv);
int main_impl_dbg();
MY_CPP_UNITTESTAPP_EXPORT int format_papyrus(const char* inp, const char* out);
