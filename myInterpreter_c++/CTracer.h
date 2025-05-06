#pragma once
#include <iostream>
#include <string>

// �����帶�� ������ depth ����
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
            std::cout << "  ";  // 2ĭ �鿩����
        }
    }
};

#define TRACE_FUNCTION() CTracer tracer(__FUNCTION__)


  