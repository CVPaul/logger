#include <thread>
#include <chrono>
#include <stdio.h>
#include <iostream>
#include "log/logging.h"

int main(void){
    printf("logger examples:");
    // 最先初始化log
    if (!logger::init("logs","test")) { // 初始化，设置日志保存路径为logs, log文件的名字为test
		printf("[ERROR]logger init failed!\n");
		return 1;
	}
	else
		printf("[INFO]logger init succeed!\n");

    while(1){ // 死循环里，1比true更高效
        logger_debug << "\tHello World!";
        logger_trace << "\tHello World!";
        logger_info << "\tHello World!";
        logger_warning << "\tHello World!";
        logger_fatal << "\tHello World!";
        logger_error << "\tHello World!";
        logger::update_rotation_time(); // 更新日志切割时间（这里的切割逻辑是20点之后算新的一天），如果不想切割不调用即可
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    // 后处理
    logger::stop();
    return 0;
}