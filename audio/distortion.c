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

// İnt to float variable
const float ONEOVERSHORTMAX = 3.0517578125e-5f; // 1/32768

int main(int argc, char *argv[])
{

    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
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

    // Distortion amount
    float factor = atof(argv[3]);

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

   int16_t buffer;
   int16_t* int_data = malloc(data_size);
   float* wet_data = malloc(data_size);

   // Read samples from input file and write updated data to output file

   int count = 0;
   while(fread(&buffer, sizeof(int16_t), 1, input))
   {
    int_data[count] = buffer;
    // İnteger to float
    wet_data[count] = int_data[count] * ONEOVERSHORTMAX * factor;
    // Soft Clipping Algorithm
    wet_data[count] = tanh(5*wet_data[count]) / factor;
    // Back to 16bit integer
    int_data[count] =  ((int_data[count]/5) + (4*wet_data[count]/5) / ONEOVERSHORTMAX);
    fwrite(&int_data[count], sizeof(int16_t), 1, output);

   count = count + 1;
   }

    // Close files
    fclose(input);
    fclose(output);
    free(wet_data);


}


