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
    void RunProgram2();
    int GetOperator();
    int GetOperand();
    void SetHaltCallback(std::function<void()> callback);
    void PackValue(int value);
    int64_t GetPackedValue() const;
    int64_t GetPackedProgram() const;
    void SetRegisterAAttempt(int value) { m_register_a_attempt = value; }
private:
    void HaltAndCatchFire();
    void CheckValidProgramIndex(int program_index);
    int m_register_a = 0;
    int m_register_b = 0;
    int m_register_c = 0;
    int m_program_counter = 0;
    bool m_halted = false;
    std::vector<int> m_program;
    uint64_t m_packed_program = 0;
    uint64_t m_packed_program_result = 0;
    int m_register_a_attempt = 94000000;
    int m_output_count = 0;
    std::function<void()> m_halt_callback;
    class OpCode* m_opcodes[8];
};