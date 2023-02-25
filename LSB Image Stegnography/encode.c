#include <stdio.h>    //heder files
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"


Status read_and_validate_encode_args( char *argv[],EncodeInfo *encInfo) //validating cla
{

    if(strstr(argv[2],".") == NULL)                //checking extension is present
    {
	printf("No extension in Bmp file\n");
	return e_failure;
    }
    if( strcmp( strstr(argv[2],"."),".bmp" ) == 0 )  //checking extension is bmp or not
    {
	printf("1) Bmp File is Present\n");
	encInfo->Src_Img_Name = argv[2];
    }
    else
    {
	printf("ERROR:In Bmp file\n");
	return e_failure;
    }

    if( argv[3] != NULL )                          //validating secret file
    { 
	if(strstr(argv[3],".") == NULL)
	{
	    printf("No extension in secret file\n");
	    return e_failure;
	}
	strcpy( encInfo->Scrt_File_Extn,strstr(argv[3],".") );
	printf("2) Secret File is Present (%s)\n",encInfo->Scrt_File_Extn);
	encInfo -> Scrt_File_Name = argv[3];
    }
    else
    {
	return e_failure;
    }

    if( argv[4] == NULL )            //validating output file
    {
	encInfo->Dest_Img_Name = "stego.bmp";
    }
    else
    {
        if(strstr(argv[4],".") == NULL)
	{
	    encInfo->Dest_Img_Name = argv[4];    
	    strcat(encInfo->Dest_Img_Name,".bmp");
	}
	else if(strcmp(strstr(argv[4],"."),".bmp" ) == 0)
	{
    	    encInfo->Dest_Img_Name = argv[4];
	}
	else
	{
	    encInfo->Dest_Img_Name = argv[4];
	    strcat(encInfo->Dest_Img_Name,".bmp");
    	}
    }	
     
    return e_success;
} 



Status open_files(EncodeInfo *encInfo) //opening files
{
  encInfo->Src_Img_Pointer = fopen(encInfo->Src_Img_Name,"r");

  if( encInfo->Src_Img_Pointer == NULL )
  {
      perror("fopen");
      fprintf(stderr,"ERROR : Unable to open file %s\n",encInfo->Src_Img_Name);

      return e_failure;
  } 

  encInfo->Scrt_File_Pointer = fopen(encInfo->Scrt_File_Name,"r");

  if( encInfo->Scrt_File_Pointer == NULL )
  {
      perror("fopen");
      fprintf(stderr,"ERROR : Unable to open file %s\n",encInfo->Scrt_File_Name);

      return e_failure;
  }
  
  encInfo->Dest_Img_Pointer = fopen(encInfo->Dest_Img_Name,"w");

  if( encInfo->Dest_Img_Pointer == NULL )
  {
      perror("fopen");
      fprintf(stderr,"ERR : Unable to open file %s\n",encInfo->Dest_Img_Name);

      return e_failure;
  }

  return e_success;
}


Status check_capacity(EncodeInfo *encInfo)      //to check capacity
{
 fseek(encInfo->Scrt_File_Pointer,0,SEEK_END);
 encInfo->Scrt_File_Size = ftell(encInfo->Scrt_File_Pointer);
 rewind(encInfo->Scrt_File_Pointer);

 if( get_image_size_for_bmp( encInfo->Src_Img_Pointer ) > (2+4+sizeof(encInfo->Scrt_File_Extn)+4+encInfo->Scrt_File_Size)*8+54 )
 {
     return e_success;
 }
 
 return e_failure;
}

uint get_image_size_for_bmp(FILE *fptr_image)  //to get the image size
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET);

    fread(&width, sizeof(int), 1, fptr_image);
    printf("5) Image Size ->");
    printf("width = %u ", width);

    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    return width * height * 3;
}

Status copy_bmp_header(FILE *Src_Img_Pointer, FILE *Dst_Img_Pointer)  //to copy the bmp header
{
    rewind(Src_Img_Pointer);
    char Buff[54];
    fread(Buff,54,1,Src_Img_Pointer);
    fwrite(Buff,54,1,Dst_Img_Pointer);
    return e_success;
}

Status encode_magic_string( char *Magic_String, EncodeInfo *encInfo) //to encode the magic string
{ 
  if( encode_data_to_image(Magic_String,2,encInfo->Src_Img_Pointer,encInfo->Dest_Img_Pointer) == e_success )
      return e_success;
}

Status encode_data_to_image(char *Data, int Size, FILE *Src_Img_Pointer, FILE *Dest_Img_Pointer) //to encode data into image
{
    int i;
    char Img_Data[8];
    for( i=0 ; i<Size ; i++ )
    {
	fread(Img_Data,8,1,Src_Img_Pointer);   //performing encode
	encode_byte_to_lsb(Data[i],Img_Data);
	fwrite(Img_Data,8,1,Dest_Img_Pointer);
     }
    return e_success;
}

Status encode_byte_to_lsb(char Data,char *Img_Data) //to encode byte to lsb
{   
    int i;
    for( i=0 ; i<8 ; i++ )
    {
       Img_Data[i]=Img_Data[i] & 0xFE;
       Img_Data[i] = ((Data & (1<<i)) >> i) | Img_Data[i];
    }

    return e_success;
}

Status encode_secret_file_extn_size( int Size,FILE *Src_Img_Pointer,FILE *Dest_Img_Pointer) //to encode secret file extn size
{
    int i;
    char Img_Data[32];
    fread(Img_Data,32,1, Src_Img_Pointer); 
   
    for( i=0 ; i<32 ; i++ )
    {
         Img_Data[i] = Img_Data[i] & 0xFE;
	 Img_Data[i] = ((Size & (1<<i)) >> i) | Img_Data[i];
    }
    fwrite(Img_Data,32,1,Dest_Img_Pointer);

    return e_success;
}
Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo) //to encode secret file extn
{
    if( encode_data_to_image(file_extn,strlen(file_extn),encInfo->Src_Img_Pointer,encInfo->Dest_Img_Pointer) == e_success);
        return e_success;
}
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo) //to encode secret file size
{
    if ( encode_secret_file_extn_size( file_size-1,encInfo->Src_Img_Pointer,encInfo->Dest_Img_Pointer)  == e_success )
    return e_success;

}

Status encode_secret_file_data(EncodeInfo *encInfo) //to encode secret file data
{
    int i;
    char string[encInfo->Scrt_File_Size];
    fread(string,encInfo->Scrt_File_Size,1,encInfo->Scrt_File_Pointer);
    for ( i=0 ; i<encInfo->Scrt_File_Size ;i++ )
    {
  	 fread(encInfo->Scrt_File_Data,8,1,encInfo->Src_Img_Pointer);
         encode_byte_to_lsb( string[i],encInfo->Scrt_File_Data);
         fwrite(encInfo->Scrt_File_Data,8,1,encInfo->Dest_Img_Pointer);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest) //to copy remaining image data
{
  int num;
  while( fread(&num,1,1,fptr_src) > 0)
  {
  	fwrite(&num,1,1,fptr_dest);
  }
  return e_success;
}


