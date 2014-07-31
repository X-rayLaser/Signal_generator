#include "wave.h"
#include "exceptions.h"

owav::owav(string file_name,int channel_num,int bit_depth,int f_discr)
{

	fname=file_name;

	ofstream f(fname.c_str() );

	if (!f.is_open())
		throw bad_fstate("File can't be created because some other program uses it.");
	f.close();

	is_open=true;
	chan_count=channel_num;
	bt_dpth=bit_depth;
	samp_rate=f_discr;
	byte_pos=HEAD_SZ;

}

void owav::open(string file_name,int num_chanells,int bit_depth,int f_discr)
{
	if (is_open==true)
		throw bad_fstate("File was already open");

	fname=file_name;
	ofstream f(fname.c_str(), ios::out | ios::binary);
	if (!f.is_open())
		throw bad_fstate("File can't be created because some other program uses it.");
	f.close();


	is_open=true;
	chan_count=num_chanells;
	bt_dpth=bit_depth;
	samp_rate=f_discr;
	byte_pos=HEAD_SZ;

}


/*
    Будем считать, что размер заголовка может быть только 58 байт (константа HEAD_SZ)
*/
void owav::seek_samp(long samp_number)
{
     int byte_per_samp = bt_dpth/8*chan_count; //кол-во байт в сэмпле
     byte_pos=HEAD_SZ+samp_number*byte_per_samp;		//переходит к заданному сэмплу samp_number

}

/*
    Будем считать, что размер заголовка может быть только 58 байт (константа HEAD_SZ)
*/
long owav::tell_smp()
{
     int byte_per_samp = bt_dpth/8*chan_count; //кол-во байт в сэмпле

     return ((byte_pos-HEAD_SZ)/byte_per_samp);

     /*
     сначала находим позицию указателя считывания в файле, затем отнимаем от
     нее размер заголовка (равный data_offset),получая тем самым кол-во байт
     от первого байта после заголовка, до байта c позицией byte_pos.
     далее определяем число байт в сэмпле и получаем, таким образом, номер
     текущего сэмпла
     */
}

void owav::closewf()
{
    if (is_open==false)
    	throw bad_fstate("File must be open before closing it");

    struct wav_header wh;

    wh=fill_head();

    write_wavheader(wh);

    is_open=false;
}

struct wav_header owav::fill_head()
{
       long file_sz;
       struct wav_header wh;

        file_sz=byte_pos;

        strcpy(wh.riff,"RIFF");
        wh.file_sz=file_sz-8;
        strcpy(wh.wave,"WAVE");

        strcpy(wh.fmt,"fmt");
        wh.fmt_sz=18;
        wh.format=1;
        wh.chan_count=chan_count;
        wh.sample_rate=samp_rate;
        wh.byte_rate=samp_rate*chan_count*bt_dpth/8;
        wh.block_align=chan_count*bt_dpth/8;
        wh.bits_per_sample=bt_dpth; //фактически bits per sample =числу битов на каждый канал
        wh.extra_bytes=0;
        //cout<<samp_rate<<' '<<wh.bits_per_sample<<' '<<wh.chan_count<<'\n';
        strcpy(wh.fact,"fact");
        wh.fact_sz=4;
        wh.total_samples=(file_sz-HEAD_SZ)/(chan_count*bt_dpth/8);

        strcpy(wh.data,"data");
        wh.data_sz=file_sz-HEAD_SZ;

        return wh;
}

void owav::write_wavheader(struct wav_header wh)
{
	fstream out(fname.c_str(), ios::out | ios::in | ios::binary);
	if (!out.is_open())
		throw bad_write("Failed to add a wav header");

	 out.seekp(0);

     out.write(wh.riff,sizeof(char)*4);
     out.write((char *) &wh.file_sz,sizeof(char)*4);
     out.write(wh.wave,sizeof(char)*4);

     out.write(wh.fmt,sizeof(char)*3);
     char ch=32;
     out.write((char *) &ch,sizeof(char));

     out.write((char *) &wh.fmt_sz,sizeof(char)*4);
     out.write((char *) &wh.format,sizeof(char)*2);
     out.write((char *) &wh.chan_count,sizeof(char)*2);
     out.write((char *) &wh.sample_rate,sizeof(char)*4);
     out.write((char *) &wh.byte_rate,sizeof(char)*4);
     out.write((char *) &wh.block_align,sizeof(char)*2);
     out.write((char *) &wh.bits_per_sample,sizeof(char)*2);
     out.write((char *) &wh.extra_bytes,sizeof(char)*2);
     if (wh.fact[0]=='f')
        {
         out.write(wh.fact,sizeof(char)*4);
         out.write((char *) &wh.fact_sz,sizeof(char)*4);
         out.write((char *) &wh.total_samples,sizeof(char)*wh.fact_sz);
        }

     out.write(wh.data,sizeof(char)*4);
     out.write((char *) &wh.data_sz,sizeof(char)*4);

     out.close();
}


