class K
{
    class B(alias a)
    {

    }

    class D(alias a, T) : T
    {

    }
}

void main()
{
    int a;
    auto k = new K;
    auto d = k.new K.D!(1, K.B!a);

    auto fun()
    {
        int b;
        auto o = k.new K.D!(b, K.B!a);
    }
}
