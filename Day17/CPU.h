#pragma once
#include <vector>
#include <functional>
class CPU
{
public:
    CPU();
    void SetRegisterA(int64_t value);
    void SetRegisterB(int64_t value);
    void SetRegisterC(int64_t value);
    void IncrementProgramCounter();
    void SetProgramCounter(int64_t value);
    int64_t GetRegisterA() const;
    int64_t GetRegisterB() const;
    int64_t GetRegisterC() const;
    int64_t GetProgramCounter() const { return m_program_counter; }

    void LoadProgram(const std::vector<int>& program);
    void RunProgram();
    void RunProgram2();
    int GetOperator();
    int GetOperand();
    void SetHaltCallback(std::function<void()> callback);
    void PackValue(int64_t value);
    int64_t GetPackedValue() const;
    int64_t GetPackedProgram() const;
    void SetRegisterAAttempt(int value) { m_register_a_attempt = value; }
private:
    void HaltAndCatchFire();
    void CheckValidProgramIndex(int64_t program_index);
    int64_t m_register_a = 0;
    int64_t m_register_b = 0;
    int64_t m_register_c = 0;
    int64_t m_program_counter = 0;
    bool m_halted = false;
    std::vector<int> m_program;
    uint64_t m_packed_program = 0;
    uint64_t m_packed_program_result = 0;
    int64_t m_register_a_attempt = 4240000000;
    int m_output_count = 0;
    std::function<void()> m_halt_callback;
    class OpCode* m_opcodes[8];

};