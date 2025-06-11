#pragma once
class OpCode
{
public:
	OpCode() = default;
	virtual void Execute(class CPU& cpu) = 0;
	virtual bool IncrementsProgramCounter() const = 0;
	virtual bool IsComboOperand() const = 0;
	int GetOperand(class CPU& cpu) const;
	virtual void ResetInternals(){};
};

class OpCode_ADV : public OpCode
{
public:
	void Execute(class CPU& cpu) override;
	bool IncrementsProgramCounter() const override;
	bool IsComboOperand() const override;
};

class OpCode_BXL : public OpCode
{
public:
	void Execute(CPU& cpu) override;
	bool IncrementsProgramCounter() const override;
	bool IsComboOperand() const override;
};

class OpCode_BST : public OpCode
{
public:
	void Execute(CPU& cpu) override;
	bool IncrementsProgramCounter() const override;
	bool IsComboOperand() const override;
};

class OpCode_JNZ : public OpCode
{
public:
	void Execute(CPU& cpu) override;
	bool IncrementsProgramCounter() const override;
	bool IsComboOperand() const override;
private:
	bool m_performed_jump = false;
};

class OpCode_BXC : public OpCode
{
public:
	void Execute(CPU& cpu) override;
	bool IncrementsProgramCounter() const override;
	bool IsComboOperand() const override;
};

class OpCode_OUT : public OpCode
{
public:
	void Execute(CPU& cpu) override;
	bool IncrementsProgramCounter() const override;
	bool IsComboOperand() const override;
	void ResetInternals() override
	{
		m_output_counter = 0;
	}
private:
	int m_output_counter = 0;
};

class OpCode_BDV : public OpCode
{
public:
	void Execute(CPU& cpu) override;
	bool IncrementsProgramCounter() const override;
	bool IsComboOperand() const override;
};

class OpCode_CDV : public OpCode
{
public:
	void Execute(CPU& cpu) override;
	bool IncrementsProgramCounter() const override;
	bool IsComboOperand() const override;
};