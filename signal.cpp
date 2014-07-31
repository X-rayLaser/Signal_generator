
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "signal.h"
#include "exceptions.h"

using namespace std;


//////Класс sinusoid

sinusoid::sinusoid(const sinusoid &sine)
{

     f_discret=sine.f_discret;

     x=sine.x;
}

sinusoid::sinusoid(vector<int>& values ,double f_disc)
{
	  if (f_disc==0)
		  throw bad_signal("Frequency must not be equal to zero");
      f_discret=f_disc;
      x=values;
}


void sinusoid::make_sinus(double f_sign, double f_discr, int amplitude, int n)
{
    if (n<=0)
    	 throw bad_signal("Signal must have at least one discrete amplitude value");
    if (f_discr==0)
    	throw bad_signal("Frequency must not be equal to zero");

    double dt,w;
    dt=1.0/f_discr;
    w=2*PI*f_sign;

    f_discret=f_discr;

    x.clear();

    double t=0;
    for ( int i=0; i<n; i++,t+=dt)
        x.push_back( (int) amplitude*sin(w*t) )  ;

}

void sinusoid::set_signal(vector<int>& values ,double f_disc)           //задаем новые значения дискретных отчетов синусоиды
{
	if (f_disc==0)
		  throw bad_signal("Frequency must not be equal to zero");
	f_discret=f_disc;

	x=values;
}

sinusoid &sinusoid::operator=(sinusoid sine)
{
	f_discret=sine.f_discret;

	x=sine.x;

	return *this;
}


sinusoid sinusoid::operator+(sinusoid sine)
{

       if (f_discret!=sine.f_discret)         //частоты дискр-ии не совпадают
    	   throw bad_signal("Both operands must have the exact same frequency\n");

       double temp_f;      //временные переменные, которыми позднее будет инициал-ся
       int temp_count;     //временный объект temp

       temp_f=f_discret;

       if (x.size() > sine.x.size())        //если длина синусоид, разная, то берем кол-во точек меньшей из них
          temp_count=sine.x.size();
       else
           temp_count=x.size();

       vector<int> v(temp_count); //выделяем память под времен. синусоиду temp


       for (int i=0; i<temp_count; i++)              //складываем почленно значения синусоид,
           v[i]=x[i]+sine.x[i];          //обрезая при этом размер новой синусоиды до меньшей из складываемых

       sinusoid temp(v,temp_f); //получили все необходимое для инициализации временного объекта temp,
                                                   //объявляем этот объект

       return temp;        //передаем объект temp в место вызова (здесь должен сработать конструктор копий)
}
