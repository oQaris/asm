#include <iostream>
#include <iomanip>
#include <cmath>
#define PI 3.141592653589793238462643383279

double Cos(double x, double epsilon = 1e-18)
{
   //double result = 1, delta = 1;
    //for (double n = 0; fabs(delta) > epsilon; n++){
    //    delta = -(delta * x * x) / ((2 * n + 2) * (2 * n + 1));
    //    result += delta;
    //}
    //return result;
    
    int two = 2, four = 4, six = 6, n = 0;
    double delta = 1, pi2 = PI * 2;
    x = fmod(x, pi2);
    __asm {
        lea     eax, x
        push    eax
        lea     eax, pi2
        push    eax
        call    fmod
        add     esp, 16
        fstp    x

        finit
        fld1
FOR:
        fld     delta
        fld     x
        fld     st(0)
        fmulp   st(1), st(0)
        fmulp   st(1), st(0)
        fild    four
        fild    n
        fmulp   st(1), st(0)
        fild    six
        faddp   st(1), st(0)
        fild    n
        fmulp   st(1), st(0)
        fild    two
        faddp   st(1), st(0)
        fdivp   st(1), st(0)
        fchs
        fadd    st(1), st(0)
        fst     delta

        fabs
        fld     epsilon
        fcomi   st(0), st(1)
        fstp    st(0)
        fstp    st(0)
        inc     n
        jna     FOR
    }
}

int main()
{
    int numargs = 5;
    double args[] = { -1.0, 0.0, PI / 2, -PI * 387, DBL_MAX };

    for (int i = 0; i < numargs; i++)
    {
        std::cout << std::setprecision(12) << "Cos(" << args[i] << ")" << std::endl <<
            "original: " << Cos(args[i]) << std::endl <<
            "ideal:    " << cos(args[i]) << std::endl <<
            "error:    " << 1 - Cos(args[i]) / cos(args[i]) << std::endl << std::endl;
    }
    return 0;
}