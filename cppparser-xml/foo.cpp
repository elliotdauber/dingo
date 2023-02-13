class IVisitor;

class IExpr {
    virtual void accept(IVisitor* v) = 0;
};

class AddExpr : IExpr {
    void accept(IVisitor* v) override;
};

class IVisitor {
    virtual void visit_add_expr(AddExpr* expr) { }
};

class PrintingVisitor : public IVisitor {
    void visit_add_expr(AddExpr* expr) override;
};

class Config {
public:
    void get_server();
};