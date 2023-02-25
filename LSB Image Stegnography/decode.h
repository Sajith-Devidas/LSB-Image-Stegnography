#include "types.h"   // Contains user defined types

/*
 * Structure to store information required for
 * decoding source Image to secret file
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *Src_Img_Name;
    FILE *Src_Img_Pointer;
    char Image_Data[8];
 
    /* Secret File Info */
    char Scrt_File_Name[30];
    FILE *Scrt_File_Pointer;
    int  Scrt_File_Extn_Size;
    char *Scrt_File_Extn;
    int  Scrt_File_Size;
   
    /* Output File Info */
    char Output_File_Name[30];

} DecodeInfo;

/* to do decoding */
Status Do_Decoding(char *argv[],DecodeInfo *DecInfo);

/* decoding function prototype */

/* Read and validate decode args from argv */
Status Read_And_Validate_Decode_Args(char *argv[], DecodeInfo *DecInfo);

/* Perform the decoding */

/* Get File pointers for i/p and o/p files */
Status Open_Files(DecodeInfo *DecInfo);

/* decode Magic String */
Status Decode_Magic_String(DecodeInfo *DecInfo);

/* decode image to data */
Status Decode_Image_To_Data ( char Magicdata[] ,int size , DecodeInfo *DecInfo);

/*decode byte fro lsb */
Status Decode_Byte_From_Lsb(char *data, char* Image_Data);

/*decoding secret file extension*/
Status Decode_Scrt_File_Extn_Size( int *Sizedata , DecodeInfo *DecInfo);

/*decoding secret file extension*/
Status Decode_Scrt_File_Extn(DecodeInfo *DecInfo);

/*decoding secret file size*/
Status Decode_Scrt_File_Size(DecodeInfo *DecInfo);

/*decoding secret file data*/
Status Decode_Scrt_File_Data(DecodeInfo *DecInfo); 

