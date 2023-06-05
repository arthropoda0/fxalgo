#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Wav Header
typedef struct wavfile_header_s
{
    char    ChunkID[4];     /*  4   */
    int32_t ChunkSize;      /*  4   */
    char    Format[4];      /*  4   */

    char    Subchunk1ID[4]; /*  4   */
    int32_t Subchunk1Size;  /*  4   */
    int16_t AudioFormat;    /*  2   */
    int16_t NumChannels;    /*  2   */
    int32_t SampleRate;     /*  4   */
    int32_t ByteRate;       /*  4   */
    int16_t BlockAlign;     /*  2   */
    int16_t BitsPerSample;  /*  2   */

    char    Subchunk2ID[4];
    int32_t Subchunk2Size;
} wavfile_header_t;

// Delay amount
const int delay_ms = 40000;

// İnt to float variable
const float ONEOVERSHORTMAX = 3.0517578125e-5f; // 1/32768

int main(int argc, char *argv[])
{

    // Check command-line arguments
    if (argc != 3)
    {
        printf("Usage: ./volume input.wav output.wav\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    wavfile_header_t wav_hdr;

    // TODO: Copy header from input file to output file
    fread(&wav_hdr, sizeof(wavfile_header_t), 1, input);
    fwrite(&wav_hdr, sizeof(wavfile_header_t), 1, output);


    // Getting information from header
    int32_t sample_rate = wav_hdr.SampleRate;
    int16_t bitspersample = wav_hdr.BitsPerSample;
    int16_t Number_of_channels = wav_hdr.NumChannels;
    int32_t data_size = wav_hdr.Subchunk2Size;
    int32_t NumSamples = data_size / (Number_of_channels * bitspersample / 8);
    int32_t byte_rate = wav_hdr.ByteRate;
    int32_t t_audio = NumSamples / sample_rate;


   // TODO: Read samples from input file and write updated data to output file
   int16_t buffer;
   int16_t* int_data = malloc(data_size + sizeof(int16_t) * 2 * delay_ms);
   int16_t* delay_data = malloc(data_size + sizeof(int16_t) * 2 * delay_ms);
   int16_t* delay_2_data = malloc(data_size + sizeof(int16_t) * 2 * delay_ms);
   int16_t* mix_Data = malloc(data_size + sizeof(int16_t) * delay_ms);

   // Adding zero value bytes in array.
   for (int k = 0; k < delay_ms; k++)
        {
            delay_data[k] = 0x0000;
        }

   for (int k = 0; k < 2 * delay_ms; k++)
        {
            delay_2_data[k] = 0x0000;
        }

   // Read samples from input file and write updated data to output file
   int count = 0;
   while(fread(&buffer, sizeof(int16_t), 1, input))
   {

    int_data[count] = buffer;
    delay_data[count + delay_ms] = buffer;
    delay_2_data[count + 2*delay_ms] = buffer;


   count = count + 1;
   }
        // Adding zeros to arrays
        for (int k = count; k < count + 2 * delay_ms; k++)
        {
            int_data[k] = 0x0000;
        }
        for (int k = count + delay_ms; k < count + 2 * delay_ms; k++)
        {
            delay_data[k] = 0x0000;
        }

        // Writing delayed samples into file
        for(int i = 0; i < count + 2 * delay_ms; i++)
        {
            mix_Data[i] = round((1*int_data[i]/2.0) + (delay_data[i]/3.0) + delay_2_data[i]/6.0);
        }

        fwrite(mix_Data,data_size + sizeof(int16_t) * delay_ms,1,output);

    // Close files
    fclose(input);
    fclose(output);
    free(int_data);
    free(delay_data);
    free(mix_Data);

}


