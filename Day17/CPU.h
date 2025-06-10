#pragma once
#include <vector>
#include <functional>
class CPU
{
public:
    CPU();
    void SetRegisterA(int value);
    void SetRegisterB(int value);
    void SetRegisterC(int value);
    void IncrementProgramCounter();
    void SetProgramCounter(int value);
    int GetRegisterA() const;
    int GetRegisterB() const;
    int GetRegisterC() const;
    int GetProgramCounter() const { return m_program_counter; }

    void LoadProgram(const std::vector<int>& program);
    void RunProgram();
    int GetOperator();
    int GetOperand();
    void SetHaltCallback(std::function<void()> callback);
private:
    void HaltAndCatchFire();
    void CheckValidProgramIndex(int program_index);
    int m_register_a = 0;
    int m_register_b = 0;
    int m_register_c = 0;
    int m_program_counter = 0;
    std::vector<int> m_program;
    std::function<void()> m_halt_callback;
    class OpCode* m_opcodes[8];
};