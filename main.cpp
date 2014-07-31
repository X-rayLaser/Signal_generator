
#include <iostream>
#include <cstring>
#include "wave.h"	  //используется для создания и записи wav файла
#include "signal.h"   //используется для создания сигнала
#include <string>
#include "exceptions.h"
using namespace std;

#define MAX_SBUF 255

/* обеспечивает пользовательский ввод имени файла, добавляя расширение wav в конце*/
string get_fname( )
{
	string s;

	cout<<"Please set the name of a file by using latin characters\n";

	cin>>s;

	return s+".wav";
}

/*проверяет, является ли строка целым числом,
  считывает строку до символа конца строки или до первого символа,
  не являющегося цифрой.
  */
int is_intnumb(const char *s)
{
	int len=strlen(s);
	int i;

	for (i=0; i<len;i++)
		if (!isdigit(s[i])) return 0;

	return 1;
}

/* считывает амплитуду из потока ввода (клавиатура),
  отказоустойчива при неверном вводе */

int get_am()
{
	bool isok;
	int am;
	do{
	   char s[MAX_SBUF]="";
 	   cout<<"Amplitude (must be an integer value): ";
 	   cin>>s;

 	   if (is_intnumb(s))  {
 		   isok=true;
 		   am = atoi(s);
 	   }
 	   else {
 		  isok=false;
 		  cout<<"Invalid value\n";
 	   }
	}while (isok==false);

	return am;
}

/*считывает частоту синусоиды из потока ввода (клавиатура),
  отказоустойчива при неверном вводе */

int get_f()
{
	bool isok;
	int freq;
	do{
	   char s[MAX_SBUF];
	   cout<<"Frequency (must be an integer value): ";
	   cin>>s;
	   if (is_intnumb(s))  {
		   isok=true;
		   freq = atoi(s);
	   }
	   else {
		  isok=false;
		  cout<<"Invalid value\n";
	   }
	}while (isok==false);

	 return freq;
}

/* функция "безопасного" считывания символа из потока ввода (клав-ра)
    */
char getch_sf()
{
	char s[MAX_SBUF]="";


	do{
		cout<<"Press key 'q' to finish setting a signal or any other key to add more frequencies\n";
		cin>>s;
		if (strlen(s)!=1)
			cout<<"Invalid character\n";
	} while (strlen(s)!=1);

	return s[0];
}

/* ф-я обеспечивает ввод пользователем параметров синусоид/ы */
void get_snl(sinusoid &snl)
{
	    int a;
		int f;
		char ch;

		snl.make_sinus(10.1,44100,0,44100);  //создаем 44100 нулевых значений прозв. синусоиды

		do{
			cout<<"New sinusoid:\n";
			a=get_am( );
			f=get_f( );
			sinusoid snlbuf;
			snlbuf.make_sinus(f,44100,a,44100);  //создаем новую синусоиду с заданной частотой и амплитудой
			snl=snl+snlbuf;				         //добавляем новую частот. составляющую к сигналу

			ch=getch_sf();
		}while(ch!='q');						 //заканчиваем ввод при наборе символа 'q'
}

int get_channum()
{
	int x;
	bool isok;
			do{
		       char s[MAX_SBUF]="";
		       cout<<"Please set an actual number of channels (must be equal 1 or 2): ";
		 	   cin>>s;
		 	   if (is_intnumb(s) && ( (x=atoi(s))==1 || x==2) )
		 		   isok=true;
		 	   else {
		 		  isok=false;
		 		  cout<<"Invalid value\n";
		 	   }
			}while (isok==false);

	return x;
}

/* осуществляет задание сигнала пользователем и его запись в файл (ДЛЯ 1 КАНАЛА НА СЭМПЛ)*/
 void generate_1ch(string file_nm, int length)
 {
	  if (length<=0 || length>1000){
		  cout<<"The size of the recording sound is too big\n";
		  throw ;
	  }
 	  owav ow(file_nm,1,16,44100);       //создаем объект ow класса owav, предназначенного для записи wav-файлов

 	  sinusoid snl;				         //создаем объект snl (синусоиду/сигнал)
      get_snl(snl);				        //пользовательский ввод параметров создаваемых составляющих сигнала

      signed short int *x=new signed short int[44100];  //буфер для хранения дискрет. значений созданного сигнала
      	  	  	  	  	  	  	  	  	  //число элементов должно быть точно равно значению частоты дискрет-ии
      	  	  	  	  	  	  	  	  	  //сигнала
      int i;

	  for (i=0; i<44100;i++)
			x[i]=snl[i];				 //запись в буфер дискр. значений сигнала

	  for (i=0; i<length; i++)				//запись сигнала длительностью length секунд
	        ow.write_samp(x,44100);

	  ow.closewf();				     //заканчиваем запись, создав заголовок wav файла и закрыв его

	  delete[]x;		    	     //освобождаем память

 }

 /* осуществляет задание сигнала пользователем и его запись в файл (ДЛЯ 2 КАНАЛОВ НА СЭМПЛ)*/
 void generate_2ch(string file_nm, int length)
{

	  if (length<=0 || length>1000){
		  cout<<"The size of the recording sound is too big\n";
		  throw length;
	  }
      owav ow(file_nm,2,16,44100);


	  sinusoid snl1,snl2;

	  cout<<"\nChannel 1:\n";
	  get_snl(snl1);					   //пользовательский ввод параметров создаваемых составляющих сигнала

	  cout<<"\nChannel 2:\n";
	  get_snl(snl2);					   //пользовательский ввод параметров создаваемых составляющих сигнала

	  struct sample_16bits *x=new struct sample_16bits[44100];
	  //двуканальный буфер для хранения дискрет. значений созданного сигнала
	  //число элементов должно быть точно равно значению частоты дискрет-ии сигнала

	  int i;

	  for (i=0; i<44100;i++){
		  x[i].chan1=snl1[i];  //запись дискретных значений (амплитуды) левого канала сигнала
		  x[i].chan2=snl2[i];  //запись дискретных значений (амплитуды) правого канала сигнала
	      }

	  for (i=0; i<length; i++)
		  ow.write_samp(x,44100);   //запись сигнала длительностью length секунд

	  ow.closewf();					//заканчиваем запись, создав заголовок wav файла и закрыв его

	  delete[]x;			       //освобождаем память

}

int get_length()
{
	bool isok;
	int length;
	do{
	   char s[MAX_SBUF]="";
	   cout<<"Length of an audio, in seconds: ";
	   cin>>s;

	   if (is_intnumb(s))  {
		   isok=true;
		   length = atoi(s);
	   }
	   else {
		  isok=false;
		  cout<<"Invalid value\n";
	   }
	}while (isok==false);

	return length ;
}

int main() {

    try{

	string file_nm; //размер имени файла
    int channum;

    file_nm=get_fname();        //приглашает пользователя ввести имя создаваемого wav-файла

	channum=get_channum();	   //запрашивается кол-во каналов в запис-мом wav-файле

	int length = get_length();
	//в зависимости от channum вызывается та или иная функция, доделывающая всю работу по генерирования wav-файла
    if (channum==1)
    	 generate_1ch(file_nm, length );
	else if (channum==2)
		 generate_2ch(file_nm, length );
	else throw 1;

    }
    catch (base_except & ob){
    	cout<<ob.get_msg()<<endl;
    }
    catch (...)
    {
    	cout<<"Unexpected error occurred\n";
    }

    system("Pause");
	return 0;

}
