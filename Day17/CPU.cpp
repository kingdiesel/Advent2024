#include "CPU.h"
#include "OP.h"
#include <cassert>
#include <iostream>

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

void CPU::SetRegisterA(int64_t value)
{
    m_register_a = value;
}

void CPU::SetRegisterB(int64_t value)
{
    m_register_b = value;
}

void CPU::SetRegisterC(int64_t value)
{
    m_register_c = value;
}

void CPU::IncrementProgramCounter()
{
    m_program_counter += 2;
}

void CPU::SetProgramCounter(int64_t value)
{
    m_program_counter = value;
}

int64_t CPU::GetRegisterA() const
{
    return m_register_a;
}

int64_t CPU::GetRegisterB() const
{
    return m_register_b;
}

int64_t CPU::GetRegisterC() const
{
    return m_register_c;
}

void CPU::LoadProgram(const std::vector<int>& program)
{
    m_program = program;
    for (size_t i = 0; i < m_program.size(); ++i)
    {
        uint64_t v = static_cast<uint64_t>(m_program[i]) & 0x7u;
        m_packed_program |= (v << (i * 3));
    }
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

void CPU::RunProgram2()
{
    m_register_a = m_register_a_attempt;
    while (true)
    {
        CheckValidProgramIndex(m_program_counter);
        if (m_halted)
        {
            m_halted = false;
            continue;
        }
        const int op_code = m_program[m_program_counter];
        assert(op_code <= 7);
        static OpCode_ADV adv_opcode;
        static OpCode_BXL bxl_opcode;
        static OpCode_BDV bdv_opcode;
        static OpCode_BST bst_opcode;
        static OpCode_JNZ jnz_opcode;
        static OpCode_BXC bxc_opcode;
        static OpCode_OUT out_opcode;
        static OpCode_CDV cdv_opcode;
        /*
            m_opcodes[0] = new OpCode_ADV();
            m_opcodes[1] = new OpCode_BXL();
            m_opcodes[2] = new OpCode_BST();
            m_opcodes[3] = new OpCode_JNZ();
            m_opcodes[4] = new OpCode_BXC();
            m_opcodes[5] = new OpCode_OUT();
            m_opcodes[6] = new OpCode_BDV();
            m_opcodes[7] = new OpCode_CDV();
        */
        bool increments = false;
        switch (op_code)
        {
            case 0: // ADV
				adv_opcode.Execute(*this);
                increments = adv_opcode.IncrementsProgramCounter();
				break;
            case 1: // BXL
                bxl_opcode.Execute(*this);
                increments = bxl_opcode.IncrementsProgramCounter();
                break;
			case 2: // BST
                bst_opcode.Execute(*this);
                increments = bst_opcode.IncrementsProgramCounter();
                break;
            case 3: // JNZ
                jnz_opcode.Execute(*this);
                increments = jnz_opcode.IncrementsProgramCounter();
                break;
            case 4: // BXC
				bxc_opcode.Execute(*this);
                increments = bxc_opcode.IncrementsProgramCounter();
				break;
            case 5:
                out_opcode.Execute(*this);
                increments = out_opcode.IncrementsProgramCounter();
                break;
            case 6: // BDV
                bdv_opcode.Execute(*this);
                increments = bdv_opcode.IncrementsProgramCounter();
                break;
            case 7: // CDV
                cdv_opcode.Execute(*this);
                increments = cdv_opcode.IncrementsProgramCounter();
				break;
            default:
                assert(false);
        }



        //OpCode* opcode = m_opcodes[op_code];
        //opcode->Execute(*this);
        
        if (m_halted)
        {
            m_halted = false;
            continue;
        }
        if (increments)
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

void CPU::PackValue(int64_t value)
{
    static int64_t target_program[16] = { 2,4,1,3,7,5,4,7,0,3,1,5,5,5,3,0 };
    //static int64_t target_program[6] = { 0,3,5,4,3,0 };
    assert(value >= 0 && value < 8 && "Value must be in the range [0, 7]");
    // Pack the value into the packed program result using 3 bits
    if (value != target_program[m_output_count])
    {
        m_packed_program_result = -1;
        HaltAndCatchFire();
        return;
    }
    m_packed_program_result |= (static_cast<uint64_t>(value) & 0x7u) << (m_output_count * 3);
    m_output_count++;
}

int64_t CPU::GetPackedValue() const
{
    return m_packed_program_result;
}

int64_t CPU::GetPackedProgram() const
{
    return m_packed_program;
}

void CPU::HaltAndCatchFire()
{
    if (m_halt_callback)
    {
        m_halt_callback();
    }
    else
    {
        if (m_packed_program == m_packed_program_result)
        {
            std::cout << "Result: " << m_register_a_attempt << std::endl;
            exit(0);
        }
        else
        {
            m_register_a_attempt++;
            m_register_a = m_register_a_attempt;
            m_register_b = 0;
            m_register_c = 0;
            m_program_counter = 0;
            m_packed_program_result = 0;
            m_halted = true;
            m_output_count = 0;
            if (m_register_a_attempt % 10000000 == 0)
            {
                std::cout << m_register_a_attempt << std::endl;
            }
        }
    }
}

void CPU::CheckValidProgramIndex(int64_t program_index)
{
    if (program_index < 0 || program_index >= static_cast<int64_t>(m_program.size()))
	{
        HaltAndCatchFire();
	}
}
