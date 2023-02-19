class IExpr {
};

class Expr : IExpr {
public:
    int add_one(int x);
    void add_ntimes(int num_times, int num);
};

int Expr::add_one(int x)
{
    return x + 1;
}

void Expr::add_ntimes(int num_times, int num)
{
    for (int i = 0; i < num_times; i++) {
        if (i > 0) {
            num = add_one(num);
        }
        num = add_one(num);
    }
}

void not_member()
{
}

int main()
{
    Expr* expr = new Expr();
    expr->add_ntimes(5, 5);
    return 0;
}