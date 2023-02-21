#include <string.h>

class Config {
public:
    static Config GetInstance();
};

Config Config::GetInstance()
{
    static Config instance;
    return instance;
}

class IVisitor;

class Num {
public:
    Num(int num)
        : num(num)
    {
    }
    int num;
};

class IExpr {
public:
    virtual void accept(IVisitor* v) = 0;
};

class AddExpr : IExpr {
public:
    void accept(IVisitor* v) override;
    IExpr* lhs;
    IExpr* rhs;
};

class NumExpr : IExpr {
public:
    void accept(IVisitor* v) override;
    Num num;
};

class IVisitor {
public:
    virtual void visit_add(AddExpr* add) { }
};

class ExecutingVisitor : IVisitor {
public:
    void visit_add(AddExpr* add) override;
};

void ExecutingVisitor::visit_add(AddExpr* add)
{
    Config config = Config::GetInstance();
}

void AddExpr::accept(IVisitor* v)
{
    v->visit_add(this);
};

int main()
{
    return 0;
}