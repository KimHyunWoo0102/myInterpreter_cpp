#pragma once
#include <iostream>
#include <string>

// 스레드마다 독립된 depth 추적
extern thread_local int g_trace_depth;

class CTracer {
public:
    CTracer(const std::string& funcName) : name(funcName) {
        indent();
        std::cout << "Enter: " << name << std::endl;
        ++g_trace_depth;
    }

    ~CTracer() {
        --g_trace_depth;
        indent();
        std::cout << "Exit : " << name << std::endl;
    }

private:
    std::string name;

    void indent() const {
        for (int i = 0; i < g_trace_depth; ++i) {
            std::cout << "  ";  // 2칸 들여쓰기
        }
    }
};

#define TRACE_FUNCTION() CTracer tracer(__FUNCTION__)


  