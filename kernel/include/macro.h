#ifndef _MACRO_H
#define _MACRO_H

#define LOG2_1 0
#define LOG2_2 1
#define LOG2_4 2
#define LOG2_8 3
#define LOG2_16 4
#define LOG2_32 5
#define LOG2_64 6
#define LOG2_128 7
#define LOG2_256 8
#define LOG2_512 9
#define LOG2_1024 10
#define LOG2_2048 11
#define LOG2_4096 12
#define LOG2_8192 13
#define LOG2_16384 14
#define LOG2_32768 15
#define LOG2_65536 16
#define LOG2_131072 17
#define LOG2_262144 18
#define LOG2_524288 19
#define LOG2_1048576 20
#define LOG2_2097152 21
#define LOG2_4194304 22
#define LOG2_8388608 23
#define LOG2_16777216 24
#define LOG2_33554432 25
#define LOG2_67108864 26
#define LOG2_134217728 27
#define LOG2_268435456 28
#define LOG2_536870912 29
#define LOG2_1073741824 30
#define LOG2_2147483648 31
#define _LOG2(x) LOG2_##x
#define LOG2(x) _LOG2(x)

#endif //_MACRO_H