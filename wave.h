
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include "exceptions.h"
using namespace std;

#define HEAD_SZ 58
#define BUF_SZ 100000   //размер буфера при чтении/записи из файла



struct wav_header{
       char riff[5];
       int file_sz;
       char wave[5];

       char fmt[5];
       int fmt_sz;
       short int format;
       unsigned short int chan_count;
       int sample_rate;
       int byte_rate;
       short int block_align;
       short int bits_per_sample;     //на самом деле это не число бит на сэмпл, а число бит на канал
       short int extra_bytes;

       char fact[5];
       int fact_sz;
       unsigned int total_samples;

       char data[5];
       int data_sz;

       };

#pragma pack(push,1)
struct sample_16bits        //структура, хранящая значения отдельных каналов сэмпла (для глубины звучания 16 бит
       {
       signed short int  chan1;
       signed short int  chan2;
       };

struct sample_8bits        //структура, хранящая значения отдельных каналов сэмпла (для глубины звучания 8 бит)
{
 unsigned char chan1;
 unsigned char chan2;
};
#pragma pack(pop)


//Класс owav
class owav{
      //закрытые члены класса
      int chan_count;              //используется при записи в wav файл для определения числа каналов на сэмпл
      int bt_dpth;                 //глубина звучания (=числу бит на канал), используется при записи
      int samp_rate;                       //частота дискретизации
      string fname;          //имя файла
      bool is_open;                //открыт файл или нет
      int byte_pos;				   //указывает текущую позицию для записи

      struct wav_header fill_head();  //служебная функция, заполняет заголовок файла
      void write_wavheader(struct wav_header wh);         //записывает заголовок в файл
public:
       //закрытые члены
       owav(){is_open=false;}                                           //конструктор по умолчанию
       owav(string file_name,int channel_num,int bit_depth,int f_discr); //перегруженный конструктор с 4 пар-ми
       //filename-имя wav файла, channel_num-число каналов в сэмпле,
       //bit_depth-число бит на канал,f_discr- частота дискретизации
       void open(string file_name,int channel_num,int bit_depth,int f_discr);     //открывает файл для записи
       //параметры имеют тот же смысл, что и в конструкторе


       template<class SmpType>
       long int write_samp(SmpType *sampls, int sample_count){
    	   //записывает заданное число сэмплов
    	   //возвращает позицию в файле до этой процедуры чтения

    	   	if (is_open==false )
    	   		throw bad_fstate("File must be open before reading it");

    	   	if (sample_count<=0 || sample_count>=100000000 )
    	   		throw bad_write("sample_count must lie in a range from 1 to 10000000");

    	   	fstream f(fname.c_str(),ios::in | ios::out | ios::binary);

    	   	if (!f.is_open())
    	   		throw bad_write("Failed to write samples in file");

    	   	f.seekp(byte_pos);

    	   	int step=BUF_SZ/sizeof(*sampls); // шаг (в числе элементов массива samples)

    	   	int i;
    	   	for (  i=0; i<sample_count-step; i+=step )
    	   		f.write((char *) &sampls[i], sizeof(char) * BUF_SZ) ;

    	   	int n=sample_count-i; 		    //оставшиеся недописанные сэмплы
    	   	f.write((char *) &sampls[i],n*sizeof(*sampls)); //дописываем необходимое число байтов

    	   	byte_pos+=sample_count*sizeof(sampls[0]);

    	   	f.close();

    	   	return 1;
       }


       //long int write_samp(struct sample_16bits *sampls, int sample_count);
       void seek_samp(long samp_number);               //переставляет указатель в файле на заданный сэмпл
       long tell_smp();                                //возвращает номер текущего сэмпла

       void closewf();                                  //записывает заголовок и закрывает файл
};

