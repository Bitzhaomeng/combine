#include "adq214.h"
//#include "ADQAPI.h"
#include <QMessageBox>
unsigned int success = 1;
const unsigned int adq_num = 1;
ADQ214::ADQ214(QObject *parent) : QObject(parent)
{
  adq_cu = CreateADQControlUnit();
  int apirev = ADQAPI_GetRevision();     //2?  获取API版本
  qDebug() << IS_VALID_DLL_REVISION(apirev);
}

void ADQ214::connectADQDevice()
{
    int num_of_devices,num_of_failed,num_of_ADQ214;
    num_of_devices = ADQControlUnit_FindDevices(adq_cu);			//找到所有与电脑连接的ADQ，并创建一个指针列表，返回找到设备的总数
    num_of_failed = ADQControlUnit_GetFailedDeviceCount(adq_cu);
    num_of_ADQ214 = ADQControlUnit_NofADQ214(adq_cu);				//返回找到ADQ214设备的数量
    if((num_of_failed > 0)||(num_of_devices == 0))
    {
        qDebug()<<"采集卡未连接";
        isADQ214Connected = false;
//        QMessageBox::critical(this, QString::fromStdString("采集卡未连接！！"), QString::fromStdString("采集卡未连接"));
    }
    else if (num_of_ADQ214 != 0)
    {
        qDebug()<<"采集卡已连接";
        isADQ214Connected = true;
      // beginADQ();
    }
}

void ADQ214::beginADQ()
{
//    int num_buffers = 8;
//    int size_buffers = 1024;
//    success = success && ADQ214_SetTransferBuffers(adq_cu, adq_num, num_buffers, size_buffers);

    ADQ214_SetDataFormat(adq_cu, adq_num,ADQ214_DATA_FORMAT_UNPACKED_14BIT);   //设置数据形式
    int num_sample_skip=128;
    if(ADQ214_SetSampleSkip(adq_cu, adq_num, num_sample_skip) == 0)
    {
        qDebug() << "Error";
        DeleteADQControlUnit(adq_cu);
    }

    int stream_ch = 0;
    int trig_mode=1;

    ADQ_SetTriggerMode(adq_cu, adq_num,trig_mode);
    stream_ch |= 0x8;
    stream_ch = ADQ214_STREAM_ENABLED_A;


    int clock_source = 0;           //0 = Internal clock   //设置时钟
    success = success && ADQ214_SetClockSource(adq_cu, adq_num, clock_source);

    int pll_divider = 2;            //在Internal clock=0时，设置，f_clk = 800MHz/divider
    success = success && ADQ214_SetPllFreqDivider(adq_cu, adq_num, pll_divider);

    int num_samples_collect =512;  //设置采样点数
    signed short* data_stream_target = new qint16[num_samples_collect];
    memset(data_stream_target, 0, num_samples_collect);  //初始化 缓存区

    success = ADQ214_DisarmTrigger(adq_cu, adq_num);
    success = success && ADQ214_SetStreamStatus(adq_cu, adq_num,stream_ch);
    success = success && ADQ214_ArmTrigger(adq_cu, adq_num);

    unsigned int samples_to_collect=num_samples_collect;
    int tmpval = 0;

    ADQ214_SWTrig(adq_cu, adq_num);

    unsigned int buffers_filled;
    unsigned int collect_result;

    while (samples_to_collect > 0)
    {
        tmpval = tmpval + 1;
        qDebug() << "Loops:" << tmpval;
//        ADQ214_SWTrig(adq_cu, adq_num);
        do
        {
            collect_result = ADQ214_GetTransferBufferStatus(adq_cu, adq_num, &buffers_filled);
            qDebug() << ("Filled: ") << buffers_filled;
        } while ((buffers_filled == 0) && (collect_result));

        collect_result = ADQ214_CollectDataNextPage(adq_cu, adq_num);

        int samples_in_buffer;
        if(ADQ214_GetSamplesPerPage(adq_cu, adq_num) > samples_to_collect)
        { samples_in_buffer = samples_to_collect;
            qDebug() << "samples= " << samples_in_buffer;
        }
        else
           { samples_in_buffer = ADQ214_GetSamplesPerPage(adq_cu, adq_num);
        qDebug() << "samples_in_buffer = " << samples_in_buffer;}

        if (ADQ214_GetStreamOverflow(adq_cu, adq_num))
        {
            qDebug() << ("Warning: Streaming Overflow!");
            collect_result = 0;
        }

        if (collect_result)
        {
            // Buffer all data in RAM before writing to disk, if streaming to disk is need a high performance
            // procedure could be implemented here.
            // Data format is set to 16 bits, so buffer size is Samples*2 bytes
            memcpy((void*)&data_stream_target[num_samples_collect - samples_to_collect],
                    ADQ214_GetPtrStream(adq_cu, adq_num),
                    samples_in_buffer*sizeof(signed short));
            samples_to_collect -= samples_in_buffer;
        }
        else
        {
            qDebug() << ("Collect next data page failed!");
            samples_to_collect = 0;
        }
    }
    emit this->collectFinish();

    success = success && ADQ_DisarmTrigger(adq_cu, adq_num);

    success = success && ADQ214_SetStreamStatus(adq_cu, adq_num,0);

    // Write to data to file after streaming to RAM, because ASCII output is too slow for realtime.
    qDebug() << "Writing stream data in RAM to disk" ;

    stream_ch &= 0x07;
    outfileA = fopen("data_A.out", "w");
    outfileB = fopen("data_B.out", "w");
    while (samples_to_collect > 0)
    {
        for (int i=0; i<num_samples_collect; i++)
        {
            fprintf(outfileA, "%hi\n", (int)data_stream_target[i]);
        }
        fclose(outfileA);
        fclose(outfileB);
        qDebug() << ("Collect finished!");
        delete data_stream_target;
        if(success == 0)
        {
            qDebug() << "Error!";
            DeleteADQControlUnit(adq_cu);
        }
    }

}
