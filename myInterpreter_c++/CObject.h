#pragma once
#include<string>
#include <memory>

using ObjectType = std::string;

extern const ObjectType INTEGER_OBJ;
extern const ObjectType BOOLEAN_OBJ;
extern const ObjectType NULL_OBJ;
extern const ObjectType RETURN_VALUE_OBJ;
extern const ObjectType ERROR_OBJ;

class CObject
{
public:
	CObject() = default;
	virtual ~CObject() = default;
	
	virtual const ObjectType Type() const = 0;
	virtual const std::string Inspect() const= 0;
private:
};

class Integer :public CObject {
public:
	Integer() = default;
	Integer(int64_t value):_value(value){}
	Integer(int value):_value(static_cast<int64_t>(value)){}
	~Integer() = default;

	virtual const ObjectType Type() const override { return INTEGER_OBJ; }
	virtual const std::string Inspect() const override { return std::to_string(this->_value); }

	const int64_t getValue()const { return _value; }

	void setValue(int value) { _value = static_cast<int64_t>(value); }
	void setValue(int64_t value) { _value = value; }
private:
	int64_t _value;
};

class BooleanObject :public CObject {
public:
	BooleanObject() = default;
	BooleanObject(bool value):_value(value){}
	~BooleanObject() = default;

	virtual const ObjectType Type() const override { return BOOLEAN_OBJ; }
	virtual const std::string Inspect() const override { return this->_value?"true":"false"; }

	const bool getValue() const{ return _value; }
private:
	bool _value;
};

class Null :public CObject {
public:
	Null() = default;
	~Null() = default;

	virtual const ObjectType Type() const override { return NULL_OBJ; }
	virtual const std::string Inspect() const override { return "null"; }
};

class ReturnValue : public CObject {
public:
	ReturnValue() = default;
	ReturnValue(const std::shared_ptr<CObject>& object) : _object(object) {}
	~ReturnValue() = default;

	virtual const ObjectType Type() const override { return RETURN_VALUE_OBJ; }
	virtual const std::string Inspect() const override { return _object->Inspect(); }

	const std::shared_ptr<CObject>& getValue() const { return _object; }

private:
	std::shared_ptr<CObject> _object;
};

class Error :public CObject {
public:
	Error() = default;
	Error(const std::string&message):_message(message){}
	~Error() = default;

	virtual const ObjectType Type() const override { return ERROR_OBJ; }
	virtual const std::string Inspect() const override { return "ERROR: " + _message; }

	const std::string& getMessage() const { return _message; }
private:
	std::string _message;
};