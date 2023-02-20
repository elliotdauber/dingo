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

class IExpr {
public:
    virtual void accept(IVisitor* v) = 0;
};

class AddExpr : IExpr {
public:
    void accept(IVisitor* v) override;
};

class IVisitor {
public:
    virtual void visit_add(AddExpr* add) { }
};

class PrintingVisitor : IVisitor {
public:
    void visit_add(AddExpr* add) override;
};

void PrintingVisitor::visit_add(AddExpr* add)
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