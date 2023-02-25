#ifndef ENCODE_H

#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 32)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *Src_Img_Name;
    FILE *Src_Img_Pointer;
    char Img_Data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *Scrt_File_Name;
    FILE *Scrt_File_Pointer;
    char Scrt_File_Extn[MAX_FILE_SUFFIX];
    char Scrt_File_Data[MAX_SECRET_BUF_SIZE];
    int Scrt_File_Size;

    /* Stego Image Info */
    char *Dest_Img_Name;
    FILE *Dest_Img_Pointer;

} EncodeInfo;
 
/* to do Encoding */
Status Do_Encoding(char *argv[],EncodeInfo *encInfo);

/* Encoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
//uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *Src_Img_Pointer, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string( char *magic_string, EncodeInfo *encInfo);

Status encode_secret_file_extn_size( int Size,FILE *Src_Img_Pointer,FILE *Dest_Img_Pointer);
/* Encode secret file extenstion */
Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *Src_Img_Pointer, FILE *Dest_Img_Pointer );

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
