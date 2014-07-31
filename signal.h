#define PI 3.1415926536
#include <vector>

using namespace std;

class sinusoid {

protected:
	   vector<int> x;	//дискретные значения синусоиды
       double f_discret;     //частота дискретизации
public:
       sinusoid(){ f_discret=1; }
       sinusoid(vector<int>& values ,double f_disc);
       sinusoid(const sinusoid &sine);                                      //конструктор копий

       int get_count(){return x.size();}
       double get_fdisc(){return f_discret;}
       void make_sinus(double f_sign, double f_discr,int amplitude, int n); //строит синусоиду
       void set_signal(vector<int>& values ,double f_disc);           //задаем новые значения дискретных отчетов синусоиды

       int operator[](int i){return x[i];}   //доступ к отдельным значениям дискретных отсчетов сигнала (ТОЛЬКО ДЛЯ ЧТЕНИЯ)
       sinusoid &operator=(sinusoid sine);
       sinusoid operator+(sinusoid sine);     //смешивает две отдельные синусоиды, выдавая третью результирующую(ob1.x[i]+ob2.x[i])
       };
