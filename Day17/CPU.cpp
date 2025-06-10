#include "CPU.h"
#include "OP.h"
#include <cassert>

CPU::CPU()
{
    m_opcodes[0] = new OpCode_ADV();
    m_opcodes[1] = new OpCode_BXL();
    m_opcodes[2] = new OpCode_BST();
    m_opcodes[3] = new OpCode_JNZ();
    m_opcodes[4] = new OpCode_BXC();
    m_opcodes[5] = new OpCode_OUT();
    m_opcodes[6] = new OpCode_BDV();
    m_opcodes[7] = new OpCode_CDV();
}

void CPU::SetRegisterA(int value)
{
    m_register_a = value;
}

void CPU::SetRegisterB(int value)
{
    m_register_b = value;
}

void CPU::SetRegisterC(int value)
{
    m_register_c = value;
}

void CPU::IncrementProgramCounter()
{
    m_program_counter += 2;
}

void CPU::SetProgramCounter(int value)
{
    m_program_counter = value;
}

int CPU::GetRegisterA() const
{
    return m_register_a;
}

int CPU::GetRegisterB() const
{
    return m_register_b;
}

int CPU::GetRegisterC() const
{
    return m_register_c;
}

void CPU::LoadProgram(const std::vector<int>& program)
{
    m_program = program;
}

void CPU::RunProgram()
{
    while (true)
    {
        CheckValidProgramIndex(m_program_counter);
        const int op_code = m_program[m_program_counter];
        assert(op_code <= 7);
        OpCode* opcode = m_opcodes[op_code];
        opcode->Execute(*this);
        if (opcode->IncrementsProgramCounter())
        {
            IncrementProgramCounter();
        }
    }
}

int CPU::GetOperator()
{
    CheckValidProgramIndex(m_program_counter);
    return m_program[m_program_counter];
}

int CPU::GetOperand()
{
    CheckValidProgramIndex(m_program_counter + 1);
    return m_program[m_program_counter + 1];
}

void CPU::SetHaltCallback(std::function<void()> callback)
{
    m_halt_callback = std::move(callback);
}

void CPU::HaltAndCatchFire()
{
    if (m_halt_callback)
    {
        m_halt_callback();
    }
    else
    {
        assert(false && "CPU halted unexpectedly");
    }
}

void CPU::CheckValidProgramIndex(int program_index)
{
    if (program_index < 0 || program_index >= static_cast<int>(m_program.size()))
	{
        HaltAndCatchFire();
	}
}
