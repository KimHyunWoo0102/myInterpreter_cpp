#include "Evaluator.h"
const std::shared_ptr<BooleanObject> _TRUE=std::make_shared<BooleanObject>(true);
const std::shared_ptr<BooleanObject> _FALSE = std::make_shared<BooleanObject>(false);
const std::shared_ptr<Null>_NULL = std::make_shared<Null>();

const std::shared_ptr<CObject> TRUE_OBJ = std::static_pointer_cast<CObject>(_TRUE);
const std::shared_ptr<CObject> FALSE_OBJ = std::static_pointer_cast<CObject>(_FALSE);
const std::shared_ptr<CObject> NULL_OBJECT = std::static_pointer_cast<CObject>(_NULL);

bool isTruthy(const std::shared_ptr<CObject>& object)
{
    if (object == NULL_OBJECT)
        return false;
    else if (object == TRUE_OBJ)
        return true;
    else if (object == FALSE_OBJ)
        return false;

    return true; // Integer�� �ٸ� ��ü�� truthy�� ����
}

bool isError(const std::shared_ptr<CObject>& object)
{
    if (object == nullptr)
        return object->Type() == ERROR_OBJ;
    return false;
}


std::shared_ptr<CObject> eval(Node* node)
{
    if (typeid(*node) == typeid(Program)) {
        // dynamic_cast ���, Program*�� ĳ����
        auto programNode = dynamic_cast<Program*>(node);
        if (programNode) {
            // Program�� �´� ó��
            return evalProgram(programNode);
        }
    }
    else if (typeid(*node) == typeid(BlockStatement)) {
        auto tmp = dynamic_cast<BlockStatement*>(node);
        return evalBlockStatement(tmp);
    }
    else if (typeid(*node) == typeid(ExpressionStatement)) {
        auto expressionStmt = dynamic_cast<ExpressionStatement*>(node);
        if (expressionStmt) {
            // const_cast�� const Expression*�� Expression*���� ��ȯ
            return eval(const_cast<Expression*>(expressionStmt->getExpression()));
        }
    }
    else if (typeid(*node) == typeid(IntegerLiteral)) {
        auto integerLiteral = dynamic_cast<IntegerLiteral*>(node);
        return std::make_shared<Integer>(integerLiteral->getValue());
    }
    else if (typeid(*node) == typeid(Boolean)) {
        auto boolean = dynamic_cast<Boolean*>(node);
        return nativeBoolToBooleanObject(boolean->getValue());
    }
    else if (typeid(*node) == typeid(PrefixExpression)) {
        auto tmp = dynamic_cast<PrefixExpression*>(node);
        auto right = eval(const_cast<Expression*>(tmp->getRight()));
        if (isError(right))
            return right;
        return evalPrefixExpression(tmp->getOperator(), right);
    }
    else if (typeid(*node) == typeid(InfixExpression)) {
        auto tmp = dynamic_cast<InfixExpression*>(node);

        if (!tmp)
            return nullptr;

        auto left = eval(const_cast<Expression*>(tmp->getLeft()));
        if (isError(left))
            return left;

        auto right = eval(const_cast<Expression*>(tmp->getRight()));
        
        if (isError(right))
            return right;

        return evalInfixExpression(tmp->getOperator(), left, right);
    }
    else if (typeid(*node) == typeid(BlockStatement)) {
        auto tmp = dynamic_cast<BlockStatement*>(node);
        return evalStatements(tmp->getStatements());
    }
    else if (typeid(*node) == typeid(IfExpression)) {
        auto tmp = dynamic_cast<IfExpression*>(node);
        return evalIfExpression(tmp);
    }
    else if (typeid(*node) == typeid(ReturnStatement)) {
        auto tmp = dynamic_cast<ReturnStatement*>(node);
        auto val = eval(const_cast<Expression*>(tmp->getReturnValue()));

        if (isError(val))
            return val;
        return std::make_shared<ReturnValue>(val);
    }
    
    return nullptr;  // ���н� nullptr ��ȯ
}

std::shared_ptr<CObject> evalStatements(const std::vector<std::shared_ptr<Statement>>& stmts)
{
    std::shared_ptr<CObject> result;

    for (const auto& stmt : stmts) {
        result = eval(stmt.get()); // �� ������ �������� ������ ����� ����

        auto return_value = dynamic_cast<ReturnValue*>(result.get());
        if (return_value)
            return return_value->getValue();
    }

    return result; // ������ �͸� ��ȯ
}

const std::shared_ptr<CObject> evalProgram(const Program* program)
{
    std::shared_ptr<CObject> result;

    for (const auto& stmt : program->getStatement()) {
        result = eval(stmt.get());

        // ReturnValue Ÿ������ Ȯ���ϰ� ���� ���� ��ȯ
        if (auto returnValue = dynamic_cast<ReturnValue*>(result.get())) {
            return returnValue->getValue();
        }
        // Error Ÿ������ Ȯ��
        else if (auto error = dynamic_cast<Error*>(result.get())) {
            // Error Ÿ���� ó���ϴ� ������ ���⿡ �ۼ�
            return result;  // ���÷� error �޽����� ��ȯ
        }
        // �� ���� �ٸ� ��� (�ʿ��ϴٸ� �߰� ó��)
    }


    return result;
}
const std::shared_ptr<CObject> evalPrefixExpression(const std::string& op, std::shared_ptr<CObject>& right)
{
    if (op == "!")
        return evalBangOperatorExpression(right);
    else if (op == "-")
        return evalMinusPrefixOperatorExpression(right);
    else
        return newError("unknown operator: {}{}",op,right->Type());
}



const std::shared_ptr<CObject>& evalBangOperatorExpression(const std::shared_ptr<CObject>& right)
{
    if (right == TRUE_OBJ)
        return FALSE_OBJ;
    else if (right == FALSE_OBJ)
        return TRUE_OBJ;
    else if (right == NULL_OBJECT)
        return TRUE_OBJ;
    else
        return FALSE_OBJ;
}

const std::shared_ptr<CObject> evalMinusPrefixOperatorExpression(const std::shared_ptr<CObject>& right)
{
    if (right->Type() != INTEGER_OBJ)
        return newError("unknown operator: -{}", right->Type());
    auto value = dynamic_cast<const Integer*>(right.get())->getValue();
    return std::make_shared<Integer>(-value);
    // TODO: ���⿡ return ���� �����մϴ�.
}

const std::shared_ptr<CObject> evalInfixExpression(const std::string& op, const std::shared_ptr<CObject>& left, const std::shared_ptr<CObject>& right)
{
    if (left->Type() == INTEGER_OBJ && right->Type() == INTEGER_OBJ)
        return evalIntegerInfixExpression(op, left, right);
    else if (op == "==")
        return nativeBoolToBooleanObject(left == right);
    else if (op == "!=")
        return nativeBoolToBooleanObject(left != right);
    else if (left->Type() != right->Type())
        return newError("type mismatch: {} {} {}", left->Type(), op, right->Type());
    return newError("unknown operator: {} {} {}", left->Type(), op, right->Type());
}

const std::shared_ptr<CObject> evalIntegerInfixExpression(const std::string& op, const std::shared_ptr<CObject>& left, const std::shared_ptr<CObject>& right)
{
    auto left_val = dynamic_cast<Integer*>(left.get())->getValue();
    auto right_val = dynamic_cast<Integer*>(right.get())->getValue();

    if (op == "+")
        return std::make_shared<Integer>(left_val + right_val);
    else if (op == "-")
        return std::make_shared<Integer>(left_val - right_val);
    else if (op == "*")
        return std::make_shared<Integer>(left_val * right_val);
    else if (op == "/")
        return std::make_shared<Integer>(left_val / right_val);
    else if (op == "<")
        return nativeBoolToBooleanObject(left_val < right_val);
    else if (op == ">")
        return nativeBoolToBooleanObject(left_val > right_val);
    else if (op == "==")
        return nativeBoolToBooleanObject(left_val == right_val);
    else if (op == "!=")
        return nativeBoolToBooleanObject(left_val != right_val);
    return nullptr;
}

const std::shared_ptr<CObject> evalIfExpression(const IfExpression* ie)
{
    auto condition = eval(const_cast<Expression*>(ie->getCondition()));

    if (isError(condition))
        return condition;

    if (isTruthy(condition))
        return eval(const_cast<BlockStatement*>(ie->getConsequence()));
    else if (ie->getAlternative() != nullptr)
        return eval(const_cast<BlockStatement*>(ie->getAlternative()));
    else
        return NULL_OBJECT;
}

const std::shared_ptr<CObject> evalBlockStatement(const BlockStatement* block)
{
    std::shared_ptr<CObject> result;

    for (auto stmt : block->getStatements()) {
        result = eval(stmt.get());

        if (result != nullptr) {
            auto rt = result->Type();
            if (rt == RETURN_VALUE_OBJ || rt == ERROR_OBJ)
                return result;
        }
    }
    return result;
}


// ��ĳ���õ� ������ �̸� �غ�

// ���� ��ȯ
const std::shared_ptr<CObject>& nativeBoolToBooleanObject(bool value) {
    return value ? TRUE_OBJ : FALSE_OBJ;
}

template<typename... Args>
std::shared_ptr<Error> newError(const char* formatStr, Args&&... args) {
    std::string message = std::vformat(formatStr, std::make_format_args(std::forward<Args>(args)...));
    return std::make_shared<Error>(message);
}

